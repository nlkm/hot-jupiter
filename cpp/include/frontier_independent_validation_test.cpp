// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Unit Tests for Independent Analytical Validation Suite (Frontiers 1 - 8)

#include "cpp/include/frontier_independent_validation.hpp"
#include <iostream>
#include <cassert>
#include <iomanip>

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   RUNNING INDEPENDENT MULTI-FRONTIER ANALYTICAL VALIDATION SUITE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::FrontierIndependentValidator validator;
  auto results = validator.RunFullValidationSuite();

  bool all_passed = true;
  for (const auto& res : results) {
    std::cout << "\n[" << (res.passed ? "PASSED" : "FAILED") << "] " << res.frontier_name << std::endl;
    std::cout << "  - Test: " << res.test_name << std::endl;
    std::cout << "  - Numerical Result:    " << std::scientific << std::setprecision(6) << res.numerical_value << std::endl;
    std::cout << "  - Analytical Benchmark: " << std::scientific << std::setprecision(6) << res.analytical_benchmark << std::endl;
    std::cout << "  - Relative Error:       " << std::fixed << std::setprecision(4) << (res.relative_error * 100.0) << " %" << std::endl;

    if (!res.passed) {
      all_passed = false;
    }
  }

  std::cout << "\n================================================================================" << std::endl;
  if (all_passed) {
    std::cout << "✅ ALL 8 RESEARCH FRONTIERS INDEPENDENTLY VALIDATED AGAINST ASYMPTOTIC THEORY!" << std::endl;
  } else {
    std::cerr << "❌ SOME INDEPENDENT VALIDATION TESTS FAILED!" << std::endl;
    return 1;
  }
  std::cout << "================================================================================" << std::endl;
  return 0;
}
