# EIC-2020

Run the tests using Make
```bash
make test_generation

make test_storage

make test_exploration

make test_final

make clean
```

Run the tests using Rust (```clang``` is required)
```bash
cargo test test_generation

cargo test test_storage

cargo test test_exploration

cargo test test_final

cargo test # for all tests

cargo clean
```

