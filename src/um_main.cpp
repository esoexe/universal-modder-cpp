// um_main.cpp - universal-modder-cpp: the standalone `um` program, the C++ edition of universal-modder's um CLI
// (Rehan and universal-modder contributors, MIT). It is the same code A.S.T.R.O AI compiles into its engine
// (packtools group 10: packtools/modder_*.inc), with its own entry point and a small HTTP client:
// libcurl on macOS/Linux, WinHTTP on Windows (part of Windows, so the .exe needs no extra DLLs).
//   um <group> <cmd> ...        e.g.  um scan list   |   um render3d model.glb out/ --preset aoe2
#define PACKTOOLS_GROUP 10
#include "packtools/packtools.inc"

#ifdef _WIN32
#include <winhttp.h>
#include <shellapi.h>
#else
#include <curl/curl.h>
#endif

namespace packtools {
HttpFn& http_hook() { static HttpFn f = nullptr; return f; }
HttpExFn& http_ex_hook() { static HttpExFn f = nullptr; return f; }
ExecFn& exec_hook() { static ExecFn f = nullptr; return f; }
std::string tool_command() { return "um"; }
}  // namespace packtools

namespace {
constexpr std::size_t MAX_BODY = std::size_t{512} << 20;
inline bool offline() { const char* e = std::getenv("ASTRO_TOOL_OFFLINE"); return e && std::string(e) == "1"; }

#ifndef _WIN32
struct Body { std::string data; std::size_t cap = 0; bool truncated = false; };
size_t on_body(char* p, size_t sz, size_t n, void* ud) {
    auto* b = static_cast<Body*>(ud); const std::size_t k = sz * n;
    if (b->cap && b->data.size() + k > b->cap) { b->truncated = true; return 0; }
    b->data.append(p, k); return k;
}
size_t on_header(char* p, size_t sz, size_t n, void* ud) {
    auto* hs = static_cast<std::vector<std::pair<std::string, std::string>>*>(ud); std::string line(p, sz * n);
    while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) line.pop_back();
    if (pyrt::starts(line, "HTTP/")) hs->clear();   // keep only the final response's headers
    const auto c = line.find(':'); if (c != std::string::npos) hs->emplace_back(pyrt::strip(line.substr(0, c)), pyrt::strip(line.substr(c + 1)));
    return sz * n;
}
packtools::HttpExResp http_ex(const packtools::HttpExReq& req) {
    packtools::HttpExResp out;
    if (offline()) { out.error = "network is off (ASTRO_TOOL_OFFLINE=1)"; return out; }
    CURL* curl = curl_easy_init(); if (!curl) { out.error = "curl unavailable"; return out; }
    Body body; body.cap = req.max_bytes ? req.max_bytes + 1 : MAX_BODY;
    struct curl_slist* list = nullptr; for (const auto& h : req.headers) list = curl_slist_append(list, h.c_str());
    const long ms = static_cast<long>(req.timeout_s * 1000.0);
    curl_easy_setopt(curl, CURLOPT_URL, req.url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, req.follow_redirects ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "universal-modder-cpp");
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
    if (req.has_body) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, static_cast<curl_off_t>(req.body.size()));
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req.body.c_str());
        if (req.method != "POST") curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, req.method.c_str());
    } else if (req.method != "GET" && req.method != "HEAD") curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, req.method.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, ms);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, ms);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, on_body);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, on_header);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &out.headers);
    if (req.method == "HEAD") curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    if (req.insecure) { curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); }
    const CURLcode code = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &out.status);
    char* eff = nullptr; curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &eff); if (eff) out.final_url = eff;
    if (code == CURLE_TOO_MANY_REDIRECTS) body.data.clear();
    else if (code != CURLE_OK && !(code == CURLE_WRITE_ERROR && body.truncated && req.max_bytes)) {
        out.status = 0; out.dns_fail = code == CURLE_COULDNT_RESOLVE_HOST; out.refused = code == CURLE_COULDNT_CONNECT;
        out.timed_out = code == CURLE_OPERATION_TIMEDOUT; out.tls_untrusted = code == CURLE_PEER_FAILED_VERIFICATION;
        out.error = body.truncated ? "the response was too large" : curl_easy_strerror(code);
    }
    out.body = std::move(body.data);
    curl_slist_free_all(list); curl_easy_cleanup(curl);
    return out;
}
#else
std::wstring widen(const std::string& s) {
    if (s.empty()) return {};
    const int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), static_cast<int>(s.size()), nullptr, 0);
    std::wstring w(static_cast<std::size_t>(n), L'\0'); MultiByteToWideChar(CP_UTF8, 0, s.data(), static_cast<int>(s.size()), w.data(), n); return w;
}
std::string narrow(const std::wstring& w) {
    if (w.empty()) return {};
    const int n = WideCharToMultiByte(CP_UTF8, 0, w.data(), static_cast<int>(w.size()), nullptr, 0, nullptr, nullptr);
    std::string s(static_cast<std::size_t>(n), '\0'); WideCharToMultiByte(CP_UTF8, 0, w.data(), static_cast<int>(w.size()), s.data(), n, nullptr, nullptr); return s;
}
packtools::HttpExResp http_ex(const packtools::HttpExReq& req) {
    packtools::HttpExResp out;
    if (offline()) { out.error = "network is off (ASTRO_TOOL_OFFLINE=1)"; return out; }
    std::wstring url = widen(req.url);
    URL_COMPONENTS uc{}; uc.dwStructSize = sizeof uc; wchar_t host[1024], path[8192]; uc.lpszHostName = host; uc.dwHostNameLength = 1024; uc.lpszUrlPath = path; uc.dwUrlPathLength = 8192;
    wchar_t extra[8192]; uc.lpszExtraInfo = extra; uc.dwExtraInfoLength = 8192;
    if (!WinHttpCrackUrl(url.c_str(), 0, 0, &uc)) { out.error = "bad URL"; return out; }
    HINTERNET ses = WinHttpOpen(L"universal-modder-cpp", WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!ses) ses = WinHttpOpen(L"universal-modder-cpp", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!ses) { out.error = "WinHTTP unavailable"; return out; }
    const int ms = static_cast<int>(req.timeout_s * 1000.0); WinHttpSetTimeouts(ses, ms, ms, ms, ms);
    HINTERNET con = WinHttpConnect(ses, std::wstring(host, uc.dwHostNameLength).c_str(), uc.nPort, 0);
    const std::wstring target = std::wstring(path, uc.dwUrlPathLength) + std::wstring(extra, uc.dwExtraInfoLength);
    HINTERNET rq = con ? WinHttpOpenRequest(con, widen(req.method).c_str(), target.c_str(), nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, uc.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0) : nullptr;
    auto done = [&] { if (rq) WinHttpCloseHandle(rq); if (con) WinHttpCloseHandle(con); WinHttpCloseHandle(ses); };
    if (!rq) { out.error = "could not connect"; out.refused = true; done(); return out; }
    if (!req.follow_redirects) { DWORD pol = WINHTTP_OPTION_REDIRECT_POLICY_NEVER; WinHttpSetOption(rq, WINHTTP_OPTION_REDIRECT_POLICY, &pol, sizeof pol); }
    DWORD dec = WINHTTP_DECOMPRESSION_FLAG_ALL; WinHttpSetOption(rq, WINHTTP_OPTION_DECOMPRESSION, &dec, sizeof dec);
    if (req.insecure) { DWORD f = SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID | SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE; WinHttpSetOption(rq, WINHTTP_OPTION_SECURITY_FLAGS, &f, sizeof f); }
    std::wstring hdrs; for (const auto& h : req.headers) hdrs += widen(h) + L"\r\n";
    const BOOL sent = WinHttpSendRequest(rq, hdrs.empty() ? WINHTTP_NO_ADDITIONAL_HEADERS : hdrs.c_str(), hdrs.empty() ? 0 : static_cast<DWORD>(-1L),
                                         req.has_body ? const_cast<char*>(req.body.data()) : WINHTTP_NO_REQUEST_DATA, req.has_body ? static_cast<DWORD>(req.body.size()) : 0, req.has_body ? static_cast<DWORD>(req.body.size()) : 0, 0);
    if (!sent || !WinHttpReceiveResponse(rq, nullptr)) {
        const DWORD e = GetLastError(); out.timed_out = e == ERROR_WINHTTP_TIMEOUT; out.dns_fail = e == ERROR_WINHTTP_NAME_NOT_RESOLVED; out.refused = e == ERROR_WINHTTP_CANNOT_CONNECT;
        out.tls_untrusted = e == ERROR_WINHTTP_SECURE_FAILURE; out.error = "WinHTTP error " + std::to_string(e); done(); return out;
    }
    DWORD st = 0, len = sizeof st; WinHttpQueryHeaders(rq, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &st, &len, WINHTTP_NO_HEADER_INDEX); out.status = static_cast<long>(st);
    DWORD hl = 0; WinHttpQueryHeaders(rq, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, nullptr, &hl, WINHTTP_NO_HEADER_INDEX);
    if (hl) { std::wstring raw(hl / sizeof(wchar_t), L'\0'); if (WinHttpQueryHeaders(rq, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, raw.data(), &hl, WINHTTP_NO_HEADER_INDEX)) {
        for (const auto& line : pyrt::split(narrow(raw), "\r\n")) { const auto c = line.find(':'); if (c != std::string::npos && !pyrt::starts(line, "HTTP/")) out.headers.emplace_back(pyrt::strip(line.substr(0, c)), pyrt::strip(line.substr(c + 1))); } } }
    DWORD ul = 0; WinHttpQueryOption(rq, WINHTTP_OPTION_URL, nullptr, &ul); if (ul) { std::wstring u(ul / sizeof(wchar_t), L'\0'); if (WinHttpQueryOption(rq, WINHTTP_OPTION_URL, u.data(), &ul)) out.final_url = narrow(u.c_str()); }
    const std::size_t cap = req.max_bytes ? req.max_bytes : MAX_BODY;
    for (;;) {
        DWORD avail = 0; if (!WinHttpQueryDataAvailable(rq, &avail) || !avail) break;
        std::string chunk(avail, '\0'); DWORD got = 0; if (!WinHttpReadData(rq, chunk.data(), avail, &got) || !got) break;
        out.body.append(chunk.data(), got);
        if (out.body.size() > cap) { if (req.max_bytes) { out.body.resize(cap); } else { out.status = 0; out.error = "the response was too large"; out.body.clear(); } break; }
    }
    done(); return out;
}
#endif
packtools::HttpResp http_simple(const std::string& url, const std::vector<std::string>& headers, long timeout_s, const std::string* post) {
    packtools::HttpExReq q; q.url = url; q.headers = headers; q.timeout_s = static_cast<double>(timeout_s);
    if (post) { q.method = "POST"; q.body = *post; q.has_body = true; }
    const auto r = http_ex(q); packtools::HttpResp o; o.status = r.status; o.body = r.body; o.error = r.error; return o;
}
}  // namespace

int main(int argc, char** argv) {
#ifndef _WIN32
    curl_global_init(CURL_GLOBAL_DEFAULT);
    std::vector<std::string> args; for (int k = 1; k < argc; ++k) args.emplace_back(argv[k]);
#else
    (void)argc; (void)argv;
    SetConsoleOutputCP(CP_UTF8);
    std::vector<std::string> args;
    { int n = 0; LPWSTR* w = CommandLineToArgvW(GetCommandLineW(), &n); for (int k = 1; w && k < n; ++k) args.push_back(narrow(w[k])); if (w) LocalFree(w); }
#endif
    packtools::http_ex_hook() = http_ex;
    packtools::http_hook() = http_simple;
    packtools::modder::um_standalone() = true;
    if (const char* d = std::getenv("UM_SKILL_DIR"); d && *d) packtools::skill_dir_ref() = d;
    int code = 0;
    try { code = packtools::modder::um_tool(args); }
    catch (const pyrt::Exit& e) { code = e.code; }
    catch (const pyrt::PyErr& e) { pyrt::flush(); std::fprintf(stderr, "um: %s: %s\n", e.type.c_str(), e.what()); code = 1; }
    catch (const std::exception& e) { pyrt::flush(); std::fprintf(stderr, "um: error: %s\n", e.what()); code = 1; }
    pyrt::flush();
    return code;
}
