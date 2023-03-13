mkdir -p /tmp/poxgo /tmp/poxgo/libpoxh
cp go/libpoxh.go /tmp/poxgo/libpoxh/libpoxh.go
cp go/runner.go /tmp/poxgo/main.go
rm -f /tmp/poxgo/go-poxhash-runner
rm -f /tmp/poxgo/go.mod
cd /tmp/poxgo  && go mod init pox &> /dev/null && go mod tidy && go build -ldflags "-s -w" -o /tmp/poxgo/go-poxhash-runner main.go 
/tmp/poxgo/go-poxhash-runner $@
