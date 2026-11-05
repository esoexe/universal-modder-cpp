# universal-modder-cpp: `make` builds build/um; `make test` runs the suites; `make windows` cross-builds build/um.exe
CXX ?= c++
CXXFLAGS ?= -std=c++20 -O2 -Wall -Wextra -Wpedantic -pthread
LDLIBS ?= -lcurl -lz
MINGW ?= x86_64-w64-mingw32-g++-posix
SOURCES := src/um_main.cpp $(wildcard src/packtools/*)

all: build/um
build/um: $(SOURCES)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) src/um_main.cpp -o $@ $(LDFLAGS) $(LDLIBS)
windows: build/um.exe
build/um.exe: $(SOURCES)
	@mkdir -p build
	$(MINGW) -std=c++20 -O2 -Wall -Wextra -Wpedantic src/um_main.cpp -o $@ -static -lwinhttp -lshell32 -lws2_32
test: build/um
	bash tests/run_all.sh
clean:
	rm -rf build
.PHONY: all windows test clean
