let
    pkgs = import <nixpkgs> {};
in
pkgs.mkShell {
    name = "c8-dev";
    buildInputs = with pkgs; [ gnumake gcc gnutar nasm qemu gdb ];
}
