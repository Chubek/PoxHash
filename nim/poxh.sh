rm -f /tmp/nimpox/nmpx
mkdir -p /tmp/nimpox
nim c -d:release  -o:nmpx --outdir:/tmp/nimpox nim/example.nim &> /dev/null
/tmp/nimpox/nmpx $@