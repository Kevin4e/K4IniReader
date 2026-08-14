# K4IniReader

A lightweight cross-platform INI reader in C++

## Overview

K4IniReader is a **header-only** library designed for fast reading of `.ini` configuration files.

**C++17+** is required.

### Features

- Supports section headers: `[Section]`.
- Inline comment support: `//`, `;`, `#`.
- Case-insensitive section and key names.
- Reads numeric, string, or boolean values via templates (`int`, `float`, `std::string`, `bool` etc.).
- Boolean parsing recognizes:
  - `true`, `1`, `on`, `yes` → `true`
  - `false`, `0`, `off`, `no` → `false`
- Fallback to **default values** for missing keys, sections, or invalid conversions.
- Lightweight memory optimization via optional pre-allocation of sections/keys.

## Usage

### Example `.ini` file

```
// Config.ini

[Resolution]
ResX = 1920 // Horizontal resolution
ResY = 1080 ; Vertical resolution

[HUD]
Scale = 1.00 # HUD's scale

[Graphics]
gpu = Vulkan // Sets the GPU API
v-sync = off ; Toggle v-sync
```

### Initialization of the INI reader

```cpp
#include "K4IniReader.hpp"

K4IniReader iniReader("Config.ini", 20, 10);
/*
 *  Parameters:
 *  - "Config.ini" → Path to the INI file
 *  - 20 → Optional: pre-allocate 20 sections (default = 0, no pre-allocation)
 *  - 10 → Optional: pre-allocate 10 keys per section (default = 0, no pre-allocation)
 */
```

### Checking whether the INI file was loaded successfully

```cpp
if (iniReader) {
    // File loaded correctly
} else {
    // Failed to load file
}
```

### Reading values from the `.ini` file

```cpp
// Resolution
int resX = iniReader.read<int>("Resolution", "ResX", 0);
int resY = iniReader.read<int>("Resolution", "ResY", 0);

// HUD
float scale = iniReader.read<float>("HUD", "Scale", 1.00f);

// Graphics
std::string gpu = iniReader.read<std::string>("Graphics", "gpu", "any");
bool vsync = iniReader.read<bool>("Graphics", "v-sync", false);
```

## Notes

- Section and key names are **case-insensitive**.
- Boolean parsing **always** lowers the boolean strings.
- The `toLowerString` parameter in `read<std::string>` allows forcing the returned string to lowercase:
```cpp
std::string gpuLower = iniReader.read<std::string>("Graphics", "gpu", "any", true);
```
- Conditions where the **default value** is returned:
  - Unhandled types (e.g. `struct`, `class`, **inheritance**/**wrappers** of the supported types).
  - Unrecognized boolean strings.
  - Numeric conversion failures.

## License

- See [LICENSE](LICENSE)

## Credits

- **[Kevin4e](https://github.com/Kevin4e/)** - Author of the library