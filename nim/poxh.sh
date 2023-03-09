rm -f /tmp/nimpx/nmpx
mkdir -p /tmp/nimpox
nim c -d:release  -o:nmpx --outdir:/tmp/nimpox nim/example.nim
/tmp/nimpox/nmpx $@