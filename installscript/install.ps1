$RootDir = Split-Path $PSScriptRoot -Parent

if (-not (Get-Command winget -ErrorAction SilentlyContinue)) {
    Write-Error "winget is not installed. Please install 'App Installer' from Microsoft Store."
    exit 1
}

$ExlibsDir = Join-Path $RootDir exlibs -Resolve

function Install-Command {
    param(
        [Parameter(Mandatory)]
        [hashtable]$MapCommandsPackages
    )

    foreach ($Command in $MapCommandsPackages.keys) {
        if (-not (Get-Command $Command)) {
            winget install --id $MapCommandsPackages[$Command] --exact --silent --accept-package-agreements --accept-source-agreements
        }
    }
}

function Download-Lib ($Uri, $OutputPath) {
    $TmpZip = Join-Path $env:TEMP ("download_" + [guid]::NewGuid().Guid + ".zip")
    Invoke-WebRequest -Uri $Uri -OutFile $TmpZip
    Expand-Archive -Path $TmpZip -DestinationPath $OutputPath -Force
    Remove-Item $TmpZip
}

if( -not (Test-Path (Join-Path $ExlibsDir "googletest-1.17.0"))) {
    $TmpDir = $(Join-Path $ExlibsDir "googletest-1.17.0")
    Download-Lib "https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip" $ExlibsDir
    cmake -S $TmpDir -B $(Join-Path $TmpDir "build") -G "MinGW Makefiles"
    cmake --build $(Join-Path $TmpDir "build") --config Release
}

if( -not (Test-Path (Join-Path $ExlibsDir "SDL2-2.32.8"))) {
    Download-Lib "https://github.com/libsdl-org/SDL/releases/download/release-2.32.8/SDL2-devel-2.32.8-mingw.zip" $ExlibsDir
}

if( -not (Test-Path (Join-Path $ExlibsDir "SDL2_image-2.8.8"))) {
    Download-Lib "https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.8/SDL2_image-devel-2.8.8-mingw.zip" $ExlibsDir
}

if( -not (Test-Path (Join-Path $ExlibsDir "SDL2_mixer-2.8.1"))) {
    Download-Lib "https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.8.1/SDL2_mixer-devel-2.8.1-mingw.zip" $ExlibsDir
}

if( -not (Test-Path (Join-Path $ExlibsDir "SDL2_net-2.2.0"))) {
    Download-Lib "https://github.com/libsdl-org/SDL_net/releases/download/release-2.2.0/SDL2_net-devel-2.2.0-mingw.zip" $ExlibsDir
}

if( -not (Test-Path (Join-Path $ExlibsDir "SDL2_ttf-2.24.0"))) {
    Download-Lib "https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.24.0/SDL2_ttf-devel-2.24.0-mingw.zip" $ExlibsDir
}

if( -not (Test-Path (Join-Path $ExlibsDir "rapidjson"))) {
    $TmpDir = Join-Path $ExlibsDir "rapidjson"
    git clone https://github.com/Tencent/rapidjson.git $TmpDir
}

if( -not (Test-Path (Join-Path $ExlibsDir "glew-2.2.0"))) {
    Download-Lib "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.zip" $ExlibsDir
    $TmpDir = $(Join-Path $ExlibsDir "glew-2.2.0")
    cmake -S $(Join-Path $TmpDir "build/cmake")-B $(Join-Path $TmpDir "build") -G "MinGW Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM="3.5"
    cmake --build $(Join-Path $TmpDir "build") --config Release
    cmake --install $(Join-Path $TmpDir "build") --prefix $(Join-Path $TmpDir "install")
}

if( -not (Test-Path (Join-Path $ExlibsDir "Simple-OpenGL-Image-Library"))) {
    $TmpDir = $(Join-Path $ExlibsDir "Simple-OpenGL-Image-Library")
    git clone https://github.com/kbranigan/Simple-OpenGL-Image-Library.git $TmpDir
    cmake -S $TmpDir -B $(Join-Path $TmpDir "build") -G "MinGW Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM="3.5"
    cmake --build $(Join-Path $TmpDir "build") --config Release
    cmake --install $(Join-Path $TmpDir "build") --prefix $(Join-Path $TmpDir "install")
}

if( -not (Test-Path (Join-Path $ExlibsDir "openal-soft-1.24.3-bin"))) {
    Download-Lib "https://github.com/kcat/openal-soft/releases/download/1.24.3/openal-soft-1.24.3-bin.zip" $ExlibsDir
}

if( -not (Test-Path (Join-Path $ExlibsDir "freealut"))) {
    $TmpDir = $(Join-Path $ExlibsDir "freealut")
    $OpenALDir = $(Join-Path $ExlibsDir "openal-soft-1.24.3-bin")
    git clone https://github.com/vancegroup/freealut.git $TmpDir
    cmake -S $TmpDir -B $(Join-Path $TmpDir "build") -G "MinGW Makefiles" -DOPENAL_LIBRARY="$(Join-Path $OpenALDir "libs/Win64/OpenAL32.lib")" -DOPENAL_INCLUDE_DIR="$(Join-Path $OpenALDir "include")" -DCMAKE_POLICY_VERSION_MINIMUM="3.5"
    cmake --build $(Join-Path $TmpDir "build") --config Release
    cmake --install $(Join-Path $TmpDir "build") --prefix $(Join-Path $TmpDir "install")
}
