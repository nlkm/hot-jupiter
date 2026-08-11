import re

cpp_header = "/home/neil/hot_jupiter/cpp/include/atmosphere.hpp"

with open(cpp_header, 'r') as f:
    content = f.read()

classes = re.findall(r'class\s+([A-Za-z0-9_]+)\s*\{', content)
print(f"Total Model Classes in atmosphere.hpp: {len(classes)}")

lookup_count = 0
formula_count = 0

for cls in classes:
    # extract class body
    match = re.search(r'class\s+' + cls + r'\s*\{([^}]+)\}', content)
    if match:
        body = match.group(1)
        has_arrays = 'const double' in body or 'double' in body and '{' in body
        has_math = 'std::pow' in body or 'std::exp' in body or 'std::sqrt' in body or 'std::cos' in body or '+' in body or '*' in body
        print(f"Class: {cls} -> Arrays: {has_arrays}, Math/Physics: {has_math}")
