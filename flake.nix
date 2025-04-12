{
  description = "simple c dev environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        devShell = pkgs.mkShell rec {
          name = "sfml";

          nativeBuildInputs = with pkgs; [
            cmake
            cmake-lint
            cmake-format
            cmake-language-server
            ninja
            pkg-config
            libgcc
          ];

          buildInputs = with pkgs; [
            freetype.dev
            libjpeg.dev
            flac.dev
            libvorbis.dev
            openal
            alsa-lib
            libGL.dev
            libdrm.dev
            systemdLibs
            libxkbcommon
            xorg.libX11.dev
            xorg.libXrandr.dev
            xorg.libXext.dev
            xorg.libXcursor.dev
            xorg.libXinerama.dev
            xorg.libXi.dev
            xorg.libXfixes.dev
            xorg.libxcb
          ];

          packages = with pkgs; [
            clang-tools
            valgrind
            zellij
          ];
          LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath buildInputs;
          shellHook = ''
            zellij
          '';
        };
      }
    );
}
