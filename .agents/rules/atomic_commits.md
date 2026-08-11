# Atomic Commit Policy & Git Workflow Guidelines

## 📌 Core Rule: Atomic Commits Mandate

All commits in this repository MUST be strictly **atomic**. A single commit must contain exactly one logical, self-contained change.

### Guidelines for Execution:

1. **Step-by-Step Commit Planning**:
   - When a task requires multiple distinct changes (e.g., adding C++ physics headers, adding C++ unit tests, updating Bazel `BUILD.bazel`, creating Python bindings, writing Python unit tests, or updating documentation), you MUST plan a sequence of fine-grained atomic commits.

2. **Sequence of Atomic Steps**:
   - **Step 1**: Add core C++ physical equations / headers.
   - **Step 2**: Add C++ unit tests & update `BUILD.bazel`.
   - **Step 3**: Add Python subpackage bindings.
   - **Step 4**: Add Python unit tests.
   - **Step 5**: Update documentation / README.

3. **No Bulk Commits**:
   - Never combine unrelated feature additions, refactoring, test additions, or documentation updates into a single giant commit.
   - Each commit must pass pre-commit checks (`yapf`, `ruff`, `cpplint`) and unit tests (`bazel test //...`, `pytest`).
