# Multi-Platform Support

**CryptPHP** is fully compatible with **macOS**, **Linux**, and **Windows**. You can either download the pre-built binaries from our automated pipeline or build them yourself from source.

---

## Option 1: Download Pre-built Binaries (Fastest)

Every time code is pushed to the repository, **GitHub Actions** automatically generates native binaries for all three platforms.

1.  Navigate to the **[Actions](https://github.com/mohit-hasan/crypt-php/actions)** tab of this repository.
2.  Click on the most recent workflow run (usually titled "CI Build" or your last commit message).
3.  Scroll down to the **Artifacts** section at the bottom.
4.  Download the **CryptPHP-Binaries** ZIP file.
5.  This ZIP contains:
    - **macOS**: `CryptPHP` (Universal binary for Intel & Apple Silicon)
    - **Linux**: `CryptPHP` (x86_64 / Ubuntu-compatible)
    - **Windows**: `CryptPHP.exe` (x64)

---

## Option 2: Build Manually (Developer Mode)

If you'd like to compile the latest features directly on your machine, follow the instructions for your Operating System.

### macOS
1.  **Prerequisites**: Install Xcode Command Line Tools and CMake.
2.  **Commands**:
    ```bash
    chmod +x setup_resources.sh
    ./setup_resources.sh
    mkdir build && cd build
    cmake ..
    make -j$(sysctl -n hw.ncpu)
    ```

### Linux (Ubuntu/Debian)
1.  **Prerequisites**: Install `build-essential`, `cmake`, `libglfw3-dev`, and `libglew-dev`.
2.  **Commands**:
    ```bash
    chmod +x setup_resources.sh
    ./setup_resources.sh
    mkdir build && cd build
    cmake ..
    make -j$(nproc)
    ```

### Windows
1.  **Prerequisites**: Install **Visual Studio 2022** (with C++ Desktop development) and **CMake**. Ensure `php.exe` is in your System PATH.
2.  **Commands (PowerShell)**:
    ```powershell
    ./setup_resources.sh
    mkdir build; cd build
    cmake ..
    cmake --build . --config Release
    ```

---
*Note: Ensure PHP 7.4+ is installed on the host system to run the obfuscation engine.*
