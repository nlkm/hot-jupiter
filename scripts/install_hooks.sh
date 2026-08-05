#!/usr/bin/env bash
set -e

# Install pre-commit git hook to enforce Google Python and C++ Style before every commit.
echo "--> Installing Google Style pre-commit git hook..."

HOOK_PATH=".git/hooks/pre-commit"
mkdir -p .git/hooks

cat << 'EOF' > "$HOOK_PATH"
#!/usr/bin/env bash
set -e

echo "=== Running Google Language Style Pre-Commit Checks ==="

# Find staged files
STAGED_PY=$(git diff --cached --name-only --diff-filter=ACM | grep '\.py$' || true)
STAGED_CPP=$(git diff --cached --name-only --diff-filter=ACM | grep -E '\.(cpp|hpp|h|cc|cxx)$' || true)

# Python Google Style Checks (yapf & ruff)
if [ -n "$STAGED_PY" ]; then
    echo "--> Linting Python files with Google Style (yapf & ruff)..."
    python3 -m yapf --in-place --style=google $STAGED_PY
    python3 -m ruff check $STAGED_PY --line-length=100
    git add $STAGED_PY
fi

# C++ Google Style Checks (cpplint)
if [ -n "$STAGED_CPP" ]; then
    echo "--> Linting C++ files with Google C++ Style (cpplint)..."
    python3 -m cpplint --linelength=100 --filter=-build/header_guard,-build/include_subdir,-readability/casting $STAGED_CPP
fi

echo "✅ All Google Language Style Pre-Commit Checks Passed!"
EOF

chmod +x "$HOOK_PATH"
echo "✅ Installed pre-commit hook at $HOOK_PATH"
