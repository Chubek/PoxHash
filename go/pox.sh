mkdir -p /tmp/poxgo /tmp/poxgo/libpox
cp go/libpox.go /tmp/poxgo/libpox/libpox.go
cp go/example.go /tmp/poxgo/main.go
rm -f /tmp/poxgo/go.mod
cd /tmp/poxgo  && go mod init pox &> /dev/null && go mod tidy && go run main.go $@
