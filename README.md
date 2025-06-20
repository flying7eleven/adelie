![Adélie Engine](artwork/logo_readme.png)

This repository contains the source code for the Adélie game engine (formerly known as Venom Engine). At least that's
the plan for the future. Let's see how far I will get.

## Development

### Current build status

| Platform | Build status                                                                                                                                                                                                               | Addional information         |
| -------- |----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|------------------------------|
| Linux    | [![Build Adélie Game Engine (Linux)](https://github.com/flying7eleven/Adelie/actions/workflows/engine_linux.yml/badge.svg?branch=main)](https://github.com/flying7eleven/Adelie/actions/workflows/engine_linux.yml)        | primary development platform |
| Windows  | [![Build Adélie Game Engine (Windows)](https://github.com/flying7eleven/Adelie/actions/workflows/engine_windows.yml/badge.svg?branch=main)](https://github.com/flying7eleven/Adelie/actions/workflows/engine_windows.yml)  | only tested sporadically     |
| macOS    | [![Build Adélie Game Engine (macOS)](https://github.com/flying7eleven/Adelie/actions/workflows/engine_macos.yml/badge.svg?branch=main)](https://github.com/flying7eleven/Adelie/actions/workflows/engine_macos.yml)        | only tested sporadically     |


### Required external libraries (see the next sections how to install them)
- Boost (Stacktrace, Thread)
- yaml-cpp (for configuration files)
- microsoft-gsl (guidelines support library)

#### On Linux (Examples for Arch Linux)
- Clang compiler with C++20 support
- Install the dependencies using ```pacman``` via ```pacman -S boost yaml-cpp vulkan-devel microsoft-gsl freetype``` (TODO: check if they are correct)
- Optional: The AMD standalone Vulkan driver via ```pacman -S amdvlk```

#### On Windows
- Microsoft Visual C++ compiler with C++20 support
- [vcpkg](https://github.com/Microsoft/vcpkg) as a package manager for the dependencies
- Install the [VulkanSDK](https://vulkan.lunarg.com/sdk/home#windows) and set the environment variable ```VULKAN_SDK``` to the installation path 
- Install the dependencies using ```vcpkg``` via ```vcpkg install boost-stacktrace boost-thread yaml-cpp ms-gsl freetype```

#### On macOS
- Clang compiler with C++20 support
- [Homebrew](https://brew.sh/) installed
- Install the dependencies using ```brew``` via ```brew install git clang-format boost yaml-cpp cpp-gsl freetype```

### Further prerequisites

#### On Windows
- In a PowerShell instance run `Set-ExecutionPolicy -Scope CurrentUser Bypass` to allow the helper scripts to run (**WARNING:** Potential security risk!)
- Ensure that you can use the `PATH` variable for definining the DLL search dir by adding a DWORD named `SafeDllSearchMode` to `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager` with an value of `0` (**WARNING:** Potential security risk!)

#### Further information
- Be sure that you are compiling everything in 64 bit since this is the only supported architecture
- Ensure that the environment variable ```VCPKG_DEFAULT_TRIPLET``` is set to ```x64-windows```
- Ensure that the environment variable ```VCPKG_ROOT``` is set to the root directory of your ```vcpkg``` installation
- If you set or changed an environment variable, it might help to restart the computer once to solve any issues

### TODO
- RenderDoc for graphical debugging: https://renderdoc.org/
