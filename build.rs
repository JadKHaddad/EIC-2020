extern crate bindgen;
extern crate cc;

fn main() {

    cc::Build::new()
        .file("atf.c")
        .file("test_generation_.c")
        .file("test_storage_.c")
        .file("test_exploration_.c")
        .file("test_final_.c")
        .warnings(false)
        .compile("eic_2020");

    let bindings = bindgen::Builder::default()
        .header("wrapper.h")
        .generate()
        .expect("Couldn't generate bindings!");

    let out_path = std::path::PathBuf::from(std::env::var("OUT_DIR").unwrap());

    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}
