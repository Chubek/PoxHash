rm -f /tmp/nimpox/nimpoxhash
mkdir -p /tmp/nimpox
nim c -d:release  -o:nimpoxhash --outdir:/tmp/nimpox nim/runner.nim &> /dev/null
/tmp/nimpox/nimpoxhash $@