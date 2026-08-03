#include <iostream>
#include <cassert>
#include "interior.hpp"

using namespace thermal_evolution;

int main() {
    std::cout << "[Unit Test] 1D Hydrostatic Interior Solver..." << std::endl;
    InteriorSolver solver;
    PlanetStructure st = solver.solve_structure(1.0 * M_JUP, 12.0 * M_EARTH, 1.34e5);
    assert(st.R_p > 0.5 * R_JUP && st.R_p < 2.5 * R_JUP);
    assert(st.P_c > 1.0e4);
    assert(st.T_c > 100.0);
    assert(st.rho.size() > 10);
    std::cout << "  -> PASSED." << std::endl;
    return 0;
}
