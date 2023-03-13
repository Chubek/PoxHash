rm -f /tmp/rust-poxhash-runner /tmp/libpoxrust.rlib
rustc rust/libpoxh.rs --crate-type lib --crate-name libpoxh -o /tmp/libpoxrust.rlib
rustc rust/runner.rs --extern libpoxh=/tmp/libpoxrust.rlib -o /tmp/rust-poxhash-runner
/tmp/rust-poxhash-runner $@