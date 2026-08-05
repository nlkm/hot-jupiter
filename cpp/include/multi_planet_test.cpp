#include <iostream>
#include <cassert>
#include "multi_planet.hpp"

using namespace hot_jupiter;

int main() {
    std::cout << "[Unit Test] Multi-Planet Secular Dynamics..." << std::endl;
    MultiPlanetSystem system;
    PlanetSystemMember b, c, d;
    b.M_p = 1.0 * M_JUP; b.a = 0.04 * AU; b.e = 0.15;
    c.M_p = 0.3 * M_JUP; c.a = 0.12 * AU; c.e = 0.08;
    d.M_p = 1.5 * M_JUP; d.a = 0.50 * AU; d.e = 0.04;
    system.planets = {b, c, d};

    auto matrix = system.compute_secular_matrix();
    assert(matrix.size() == 3);
    assert(matrix[0].size() == 3);

    auto de_dt = system.evaluate_secular_de_dt();
    assert(de_dt.size() == 3);

    std::cout << "  -> PASSED." << std::endl;
    return 0;
}
