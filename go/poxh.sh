mkdir -p /tmp/poxgo /tmp/poxgo/libpoxh
cp go/libpoxh.go /tmp/poxgo/libpoxh/libpoxh.go
cp go/runner.go /tmp/poxgo/main.go
rm -f /tmp/poxgo/gopoxhash
rm -f /tmp/poxgo/go.mod
cd /tmp/poxgo  && go mod init pox &> /dev/null && go mod tidy && go build -o /tmp/poxgo/gopoxhash main.go 
/tmp/poxgo/gopoxhash $@
