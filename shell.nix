# shell.nix
{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  nativeBuildInputs = [
    pkgs.gcc
    pkgs.cmake
    pkgs.gtest
  ];
  buildInputs = [
  ];
  shellHook = ''
  '';
}
