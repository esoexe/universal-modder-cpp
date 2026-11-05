#!/usr/bin/env bash
# Runs every suite against build/um (or T=...). Needs bash, jq and node; ffmpeg is optional (video checks skip).
cd "$(dirname "$0")/.." || exit 1
rc=0
for t in tests/modder_*.sh; do bash "$t" || rc=1; done
exit $rc
