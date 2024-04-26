{
  inputs = {
    nixpkgs.url = github:nixos/nixpkgs/nixos-23.11;
    flake-utils.url = github:numtide/flake-utils;
  };

  outputs = { self, nixpkgs, flake-utils, ... }: flake-utils.lib.eachDefaultSystem (currentSystem:
    let
      pkgs = import nixpkgs {
        system = currentSystem;
      };
    in with pkgs; {
      devShell = let
        py-pkgs = ps: with ps; [ ];
      in mkShell rec {
        nativeBuildInputs = [
          cmake
          gnumake
          (python3.withPackages (ps: with ps; [
            pybind11
          ]))
        ];

        LD_LIBRARY_PATH = "${lib.makeLibraryPath nativeBuildInputs}";
      };

      packages.default = mkDerivation {};
    }
  );
}
