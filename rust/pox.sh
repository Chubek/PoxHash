rustc rust/libpox.rs --crate-type lib --crate-name libpox -o /tmp/libpoxtmp.rlib
rustc rust/example.rs --extern libpox=/tmp/libpoxtmp.rlib -o /tmp/rspox
/tmp/rspox $@