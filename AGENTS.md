# Repository Rules & Development Standards

## 1. Atomic Commits Policy
- All git commits MUST be fine-grained and atomic (one logical change per commit).
- When implementing a feature or paper replication, plan out a sequence of separate atomic commits:
  1. Core C++ physics header addition
  2. Bazel `BUILD.bazel` target & C++ unit test addition
  3. Python subpackage wrapper addition
  4. Pytest unit test addition
  5. Documentation / README update

## 2. Google Engineering Standards
- Use **Bazel** for all builds and tests (`bazel test //...`).
- Enforce **Google C++ Style Guide** (`cpplint`) and **Google Python Style Guide** (`yapf --style=google`, `ruff`).
- Pure first-principles physics (zero hardcoded lookup tables).
- SIMD vectorization & `-O3` performance optimization on low-end hardware.
