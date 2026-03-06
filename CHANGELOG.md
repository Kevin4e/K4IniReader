# Changelog

## [v1.2.0] - 2026-02-05

### Added

- Binary, octal and hexadecimal numerical bases support when reading integers.
- `operator bool()` to check if the INI file loaded successfully.

### Changed

- Sections and keys are case-insensitive.
- Boolean values are always converted to lowercase.
- Optional parameters used to pre-allocate hash map memory are set `0` by default. (from `32`-`8`).
- Improved robustness of INI parsing.
- Code cleanup, optimizations, `const` correctness.

### Fixed

- Boolean parsing returns the default value if invalid (used to return `false`).
- Improved handling of inline comments in edge cases.
- Failed numeric conversions always return the default value even if it was partially converted.

---

## [v1.1.0] - 2025-11-09

### Changed

- Improved documentation, comments.

### Fixed

- `char` not being handled correctly.
---

## [v1.0.0] - 2025-11-07
### Added
- Initial release of `K4INIReader`.
- INI parsing for sections and key-value pairs.
- Template-based `read()` for numeric and string types (booleans not yet supported).