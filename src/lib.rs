#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

#[test]
fn test_generation_test() {
    assert_eq!(unsafe { test_generation() }, 0);
}
#[test]
fn test_storage_test() {
    assert_eq!(unsafe { test_storage() }, 0);
}

#[test]
fn test_exploration_test() {
    assert_eq!(unsafe { test_exploration() }, 0);
}

#[test]
fn test_final_test() {
    assert_eq!(unsafe { test_final() }, 0);
}
