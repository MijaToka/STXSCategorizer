{
  description = "C++ Template";

  inputs = {
    nixpkgs.url = "nixpkgs";
    systems.url = "github:nix-systems/x86_64-linux";
    flake-utils = {
      url = "github:numtide/flake-utils";
      inputs.systems.follows = "systems";
    };
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        pname = "STXS-Categorize"; # package name
        version = "0.0.1";
        src = ./.;

        zzanalysis-headers = pkgs.stdenv.mkDerivation {
          name = "zzanalysis-headers";
          src = pkgs.fetchFromGitHub {
            owner = "CJLST";
            repo = "ZZAnalysis";
            rev = "Run3";
            hash = "sha256-uSufJ/vXWmWV09PTRtvyMDAUSgnPafBWMusKPDlHlB4=";
          };
          dontBuild = true;
          installPhase = ''
            mkdir -p $out/include/ZZAnalysis/AnalysisStep/interface
            mkdir -p $out/share/ZZAnalysis/AnalysisStep/data/cconstants

            cp AnalysisStep/interface/{cConstants,Discriminants}.h \
            $out/include/ZZAnalysis/AnalysisStep/interface

            cp AnalysisStep/data/cconstants/* \
            $out/share/ZZAnalysis/AnalysisStep/data/cconstants
          '';
        };
        buildInputs = with pkgs; [
          root
          zzanalysis-headers
          (python313.withPackages (
            ps: with ps; [
              numpy
              root
            ]
          ))
        ];
        nativeBuildInputs = with pkgs; [
          cmake
          pkg-config
        ];
      in
      {
        devShells.default = pkgs.mkShell {
          inherit buildInputs nativeBuildInputs;

          shellHook = ''
            export ROOT_RDF_SNAPSHOT_INFO=0
            export CPATH=${zzanalysis-headers}/include:$CPATH
            export ZZANALYSIS_DATA=${zzanalysis-headers}/share/ZZAnalysis/AnalysisStep
          '';
        };
        packages = {
          categorize = pkgs.stdenv.mkDerivation {
            inherit
              buildInputs
              nativeBuildInputs
              pname
              version
              src
              ;
            cmakeFlags = [
              "DZZANALYSIS_INCLUDE_DIR=${zzanalysis-headers}/include"
            ];
            preConfigure = ''
              ln -s ${src} ../STXSCategorizer
            '';
          };
          default = self.packages.${system}.categorize;
        };
      }
    );
}
