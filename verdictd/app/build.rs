use std::process::Command;

fn main() {
    println!("cargo:rustc-link-search=native=/opt/enclave-tls/lib");
    println!("cargo:rustc-link-lib=dylib=enclave_tls");

    println!(r"cargo:rustc-link-search=target/debug");
    let os = Command::new("uname").output().unwrap();
    let ext = match String::from_utf8_lossy(os.stdout.as_slice())
        .into_owned()
        .trim_end()
        .as_ref()
    {
        "Darwin" => "dylib",
        _ => "so",
    };
    Command::new("go")
        .args(&[
            "build",
            "-o",
            &format!("target/debug/libopa.{}", ext),
            "-buildmode=c-shared",
            "src/policyEngine/opa/opaEngine.go",
        ])
        .status()
        .unwrap();
}