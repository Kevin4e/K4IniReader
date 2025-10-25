# K4IniReader
A lightweight cross-platform INI reader in C++

## Overview
K4IniReader is a **header-only** library designed for fast reading of .ini configuration files.

**C++20+** is required.

### Features:
- Supports section headers (`[Section]`);
- Handles inline comments (`//`, `;`, `#`);
- Can read integral or non-integral data type (`int`, `float`, `std::string`, etc.) via templates;
- Uses highly efficient **hash maps** for fast lookup.

## Usage

### Example .ini file
```
// Config.ini

[Resolution]
ResX = 1920 // Horizontal resolution
ResY = 1080 ;  Vertical resolution

[HUD]
Scale = 1.00 # HUD's scale
```

### Defining the INI reader object:

```cpp
K4INIReader iniReader("Config.ini", 20, 10);
/*
 *  'Config.ini' is the .ini file to read the values from.
 *  '20' defines the number of sections inside of 'Config.ini' (optional, 32 by default).
 *  '10' defines the minimum number of keys inside of any section (optional, 8 by default).
 *
 *   These optional parameters are used for pre-allocating hash map memory, which is a small optimization.
 */
```

### Reading values from the .ini file
```cpp
// Resolution
int resX = iniReader.read<int>("Resolution", "ResX", 0);
int resY = iniReader.read<int>("Resolution", "ResY", 0);

// HUD
float scale = iniReader.read<float>("HUD", "Scale", 1.00);

/*
 *  Here 'data_type' can be an integral or non-integral data type.
 *  If the reading process failed, returns 'default_value', and it must match 'data_type'.
 */
```

## Credits
- **Kevin4e** - Author of the library.
