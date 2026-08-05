#include <iostream>
#include <cassert>
#include "constants.hpp"

using namespace hot_jupiter;

int main() {
    std::cout << "[Unit Test] Constants..." << std::endl;
    assert(G > 6.67e-11 && G < 6.68e-11);
    assert(M_SUN > 1.98e30);
    assert(R_SUN > 6.95e8);
    assert(M_JUP > 1.89e27);
    assert(R_JUP > 7.14e7);
    assert(M_EARTH > 5.97e24);
    assert(AU > 1.49e11);
    assert(KB > 1.38e-23);
    assert(HBAR > 1.05e-34);
    assert(SIGMA_SB > 5.67e-8);
    std::cout << "  -> PASSED." << std::endl;
    return 0;
}
