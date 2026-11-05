# lib.sh - tiny assertion helpers for the suites (bash + jq + node). T runs `um`: the suites call `$T um <group> ...`
# (the same files run inside A.S.T.R.O AI, where `um` is a tool of the engine), so tests/um-tool drops that word.
T="${T:-$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/um-tool}"
export ASTRO_TOOL_OFFLINE=1   # never call live services from the suites
FIX="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/fixtures"
PASS=0; FAILN=0
ok()   { PASS=$((PASS+1)); }
bad()  { FAILN=$((FAILN+1)); echo "  FAIL: $*"; }
eq()   { [ "$1" == "$2" ] && ok || bad "$3: expected [$2], got [$1]"; }
has()  { [[ "$1" == *"$2"* ]] && ok || bad "$3: [$2] not in output"; }
hasnt(){ [[ "$1" != *"$2"* ]] && ok || bad "$3: [$2] should not be in output"; }
run()  { OUT=$($T "$@" 2>"${TMPDIR:-/tmp}/um_err"); CODE=$?; ERR=$(cat "${TMPDIR:-/tmp}/um_err"); }
done_() { echo "$1: $PASS passed, $FAILN failed"; [ "$FAILN" -eq 0 ]; }
