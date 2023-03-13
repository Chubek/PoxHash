rm -f /tmp/nimpox/nim-poxhash-runner
mkdir -p /tmp/nimpox
nim c -d:release  -o:nim-poxhash-runner --outdir:/tmp/nimpox nim/runner.nim &> /dev/null
/tmp/nimpox/nim-poxhash-runner $@