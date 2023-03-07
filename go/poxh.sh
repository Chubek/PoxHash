mkdir -p /tmp/poxgo /tmp/poxgo/libpoxh
cp go/libpoxh.go /tmp/poxgo/libpoxh/libpoxh.go
cp go/example.go /tmp/poxgo/main.go
rm -f /tmp/poxgo/go.mod
cd /tmp/poxgo  && go mod init pox &> /dev/null && go mod tidy && go run main.go $@
