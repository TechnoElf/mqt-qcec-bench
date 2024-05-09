{
  inputs = {
    nixpkgs.url = github:nixos/nixpkgs/nixos-23.11;
    flake-utils.url = github:numtide/flake-utils;
    rust-overlay.url = github:oxalica/rust-overlay;
  };

  outputs = { self, nixpkgs, flake-utils, rust-overlay, ... }: flake-utils.lib.eachDefaultSystem (currentSystem:
    let
      pkgs = import nixpkgs {
        system = currentSystem;
        overlays = [ rust-overlay.overlays.default ];
      };
    in with pkgs; {
      devShell = mkShell.override {
        stdenv = pkgs.llvmPackages_17.libcxxStdenv;
      } rec {
        nativeBuildInputs = [
          cmake
          gnumake
          (python3.withPackages (ps: with ps; [
            pybind11
            matplotlib
            numpy
          ]))
          (rust-bin.selectLatestNightlyWith (toolchain: toolchain.default.override {
            extensions = [ "rust-src" ];
          }))
          pkg-config
          fontconfig
        ];

        LD_LIBRARY_PATH = "${lib.makeLibraryPath nativeBuildInputs}";
      };

      packages.default = mkDerivation {};
    }
  );
}
