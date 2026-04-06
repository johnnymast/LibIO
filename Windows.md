# Windows Build Tutorial (CLion + Visual Studio + vcpkg) — LibIO

LibIO heeft geen externe dependencies via vcpkg.  
Dit maakt het de eenvoudigste Windows-build binnen SparkleLibs.

Deze handleiding legt uit hoe je LibIO correct bouwt op Windows met:

- CLion
- Visual Studio 2022 Community
- MSVC v143
- Windows SDK (automatisch geïnstalleerd)
- Visual Studio generator (GEEN Ninja)
- Geen vcpkg dependencies nodig

------------------------------------------------------------
1. Installeer Visual Studio 2022 Community
------------------------------------------------------------

Open de Visual Studio Installer en vink slechts één workload aan:

    ✔ Desktop development with C++

Dit installeert automatisch:

- MSVC v143 compiler
- Windows SDK (10 of 11)
- CMake tools
- NMake Makefiles
- Visual Studio generator

Je hoeft geen extra componenten te selecteren.

------------------------------------------------------------
2. (Optioneel) Installeer vcpkg
------------------------------------------------------------

LibIO gebruikt geen vcpkg dependencies, maar als je vcpkg al gebruikt
voor andere SparkleLibs-projecten, kun je het gewoon laten staan.

    git clone https://github.com/microsoft/vcpkg
    cd vcpkg
    bootstrap-vcpkg.bat

Voor LibIO hoef je niets te installeren.

------------------------------------------------------------
3. Configureer CLion Toolchain
------------------------------------------------------------

Ga naar:

    File → Settings → Build, Execution, Deployment → Toolchains

Selecteer:

- Toolchain: Visual Studio
- C Compiler: cl.exe
- C++ Compiler: cl.exe
- Debugger: bundled

CLion detecteert automatisch:

- Windows SDK
- MSVC toolchain

⚠ BELANGRIJK:
Gebruik GEEN Ninja generator op Windows.
Ninja veroorzaakt timestamp- en include-path problemen.

------------------------------------------------------------
4. Configureer CMake in CLion
------------------------------------------------------------

Ga naar:

    Settings → Build, Execution, Deployment → CMake

Stel in:

- Generator: Visual Studio 17 2022
- Toolchain: Visual Studio

Omdat LibIO geen vcpkg dependencies heeft, hoef je GEEN toolchain file toe te voegen.

CMake options mag leeg blijven.

------------------------------------------------------------
5. Verwijder oude build-mappen
------------------------------------------------------------

Verwijder:

    cmake-build-debug/
    cmake-build-release/
    cmake-build-debug-visual-studio/
    build/

------------------------------------------------------------
6. Builden
------------------------------------------------------------

Klik:

- Reload CMake Project
- Build

Je krijgt:

- ✔ MSVC toolchain gevonden
- ✔ Windows SDK gevonden
- ✔ LibIO.dll gebouwd

------------------------------------------------------------
7. Install (Als Administrator)
------------------------------------------------------------

Ga naar de CLion build-map:

    cd io/cmake-build-debug-visual-studio

Voer uit:

    cmake --install . --prefix "C:/Program Files/LibIO"

Dit installeert:

- C:/Program Files/LibIO/bin/LibIO.dll
- C:/Program Files/LibIO/lib/LibIO.lib
- C:/Program Files/LibIO/include/LibIO/...
- C:/Program Files/LibIO/lib/cmake/LibIO/LibIOConfig.cmake

------------------------------------------------------------
Klaar
------------------------------------------------------------

LibIO is de eenvoudigste Windows-build binnen SparkleLibs:

- Geen vcpkg nodig
- Geen externe dependencies
- Alleen Visual Studio + CLion configureren
- Visual Studio generator gebruiken

Je bent klaar om LibIO te gebruiken in Windows-projecten.
