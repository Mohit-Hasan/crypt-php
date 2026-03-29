# CryptPHP - Obfuscator for Protect PHP Code

**CryptPHP** is a high-performance, cross-platform PHP obfuscator and encoder designed to secure your source code. It combines granular file selection with multi-layer protection to safeguard your intellectual property.

![tab-1](https://raw.githubusercontent.com/Mohit-Hasan/crypt-php/refs/heads/main/assets/tab-1.png)

---

## Key Features

*   **Granular Control**: Recursive file tree with synchronized selection logic.
*   **Obfuscation (Phase 1)**: Advanced variable/function/class renaming and statement shuffling via YakPro.
*   **Layer-2 Encoding (Phase 2)**: Secondary `gzdeflate` + `Base64` protection for core logic.
*   **Smart Packaging**: Automatically generates **Full Install** and **Update Patch** ZIP archives.
*   **Seed Injection**: Inject custom files like licenses or decoy assets directly into your builds. These act as traps for crackers, wasting their time and effort while keeping your real code safe.
*   **Cross-Platform**: Native binaries for macOS, Linux, and Windows.

---

## Quick Start

### 1. Build from Source
Ensure you have **C++17**, **CMake**, and **PHP 7.4+** installed.

```bash
# Setup resources (Downloads PHP-Parser and YakPro)
chmod +x setup_resources.sh
./setup_resources.sh

# Build the application
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)
```

### 2. Run the Application
```bash
./CryptPHP
```

---

## How to Use

1.  **Initialize**: Set your Project Path and verify the Global Ignore List.
2.  **Select**: Use the **YakPro** tab to pick folders or files for obfuscation.
3.  **Encode**: Use the **Layer-2** tab for additional encoding on sensitive files.
4.  **Package**: Configure your **Seed Folder** and click **Build & Orchestrate**.
5.  **Deploy**: Find your protected ZIP files in the `dist/` directory.

![tab-2](https://raw.githubusercontent.com/Mohit-Hasan/crypt-php/refs/heads/main/assets/tab-2.png)

---

## Credits & Licensing

*   **Engine**: Based on [YakPro-PO](https://github.com/pk-fr/yakpro-po).
*   **Parser**: [PHP-Parser](https://github.com/nikic/PHP-Parser) 4.x.
*   **UI**: [Dear ImGui](https://github.com/ocornut/imgui).

---
*Developed by Mohit Hasan*
