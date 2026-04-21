{
  perSystem =
    {
      pkgs,
      self',
      ...
    }:
    {
      packages =
        let
          src = ./..;
          buildInputs = [
            pkgs.root
            self'.packages.zzanalysis-headers
          ];
          nativeBuildInputs = with pkgs; [
            cmake
            pkg-config
          ];
          preConfigure = ''
            ln -s ${src} ../STXSCategorizer
          '';
          pname = "STXS-Categorize-BDT";
          version = "0.0.1";
          cmakeFlags = [
            "-DZZANALYSIS_INCLUDE_DIR=${self'.packages.zzanalysis-headers}/include"
            "-DBUILD_CATEGORIZEBDT=ON"
          ];
        in
        {
          trainBDT = pkgs.stdenv.mkDerivation {
            inherit
              src
              pname
              version
              buildInputs
              nativeBuildInputs
              preConfigure
              cmakeFlags
              ;
          };
        };
    };
}
