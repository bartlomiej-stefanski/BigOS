{
  description = "An operating system developed at University of Wrocław tergeting the RISC-V architecture";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/release-25.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem (
      system: let
        pkgs = nixpkgs.legacyPackages.${system};

        # riscv-none-elf toolchain
        riscvPkgs = pkgs.pkgsCross.riscv64-embedded;

        riscvGcc = riscvPkgs.buildPackages.gcc;
        riscvBinutils = riscvPkgs.buildPackages.binutils;
        riscvGdb = riscvPkgs.buildPackages.gdb;
      in {
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            cmake
            clang-tools_19
          ];

          buildInputs = [
            pkgs.qemu
            riscvGcc
            riscvBinutils
            riscvGdb
          ];

          shellHook = ''
            echo "Welcome to BigOS shell environment!"
            echo "Installed toolchain is riscv-none-elf"
          '';
        };
      }
    );
}
