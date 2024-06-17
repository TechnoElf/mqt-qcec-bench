{
  inputs = {
    nixpkgs.url = github:nixos/nixpkgs/nixos-24.05;
    nixpkgs-unstable.url = github:nixos/nixpkgs/nixos-unstable;
    flake-utils.url = github:numtide/flake-utils;
    rust-overlay.url = github:oxalica/rust-overlay;
  };

  outputs = { self, nixpkgs, nixpkgs-unstable, flake-utils, rust-overlay, ... }: flake-utils.lib.eachDefaultSystem (currentSystem:
    let
      pkgs = import nixpkgs {
        system = currentSystem;
        overlays = [
          rust-overlay.overlays.default
        ];
      };
      pkgs-unstable = import nixpkgs-unstable {
        system = currentSystem;
      };
    in {
      devShell = (pkgs.buildFHSUserEnv.override {
        stdenv = pkgs.llvmPackages_17.libcxxStdenv;
      } rec {
        name = "mqt-qcec-bench-shell";

        targetPkgs = pkgs: with pkgs; [
          cmake
          gnumake
          (python3.withPackages (ps: with ps; []))
          (rust-bin.selectLatestNightlyWith (toolchain: toolchain.default.override {
            extensions = [ "rust-src" ];
          }))
          pkg-config
          fontconfig.dev
          freetype.dev
          clang-tools
        ];

        runScript = ''
          env LIBRARY_PATH=/usr/lib \
            C_INCLUDE_PATH=/usr/include \
            CPLUS_INCLUDE_PATH=/usr/include \
            CMAKE_LIBRARY_PATH=/usr/lib \
            CMAKE_INCLUDE_PATH=/usr/include \
            LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib \
            bash
        '';
      }).env;

      packages.default = pkgs.mkDerivation {};
    }
  );
}
