rustc rust/libpoxh.rs --crate-type lib --crate-name libpoxh -o /tmp/libpoxhtmp.rlib
rustc rust/example.rs --extern libpoxh=/tmp/libpoxhtmp.rlib -o /tmp/rspox
/tmp/rspox $@