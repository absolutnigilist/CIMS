git config --system core.longpaths true

if (-not (Test-Path "$env:USERPROFILE\vcpkg")) {
  git clone https://github.com/microsoft/vcpkg $env:USERPROFILE\vcpkg
  & "$env:USERPROFILE\vcpkg\bootstrap-vcpkg.bat"
}
$env:VCPKG_ROOT = "$env:USERPROFILE\vcpkg"

if (-not (Test-Path ".\CIMS")) {
  git clone --recurse-submodules https://github.com/absolutnigilist/CIMS
}
Set-Location CIMS
git submodule update --init --recursive

if (Test-Path ".\external\vcpkg\bootstrap-vcpkg.bat") {
  .\external\vcpkg\bootstrap-vcpkg.bat
}

cmake --preset qt-client-vs2022-debug
cmake --build  --preset qt-client-vs2022-debug

cmake --preset server-vs2022-debug
cmake --build  --preset server-vs2022-debug

cmake --preset qt-client-vs2022-debug
cmake --build  --preset qt-client-vs2022-debug
