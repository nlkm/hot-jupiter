#include <iostream>
#include <cassert>
#include "mass_loss.hpp"

using namespace hot_jupiter;

int main() {
    std::cout << "[Unit Test] Mass Loss (RLOF & Photoevaporation)..." << std::endl;
    RocheLobeMassLoss rlof;
    double r_roche = RocheLobeMassLoss::roche_lobe_radius(0.02 * AU, 1.0 * M_JUP, 1.0 * M_SUN);
    assert(r_roche > 0.0 && r_roche < 0.02 * AU);

    double fill_under = rlof.roche_lobe_filling_factor(0.8 * r_roche, 0.02 * AU, 1.0 * M_JUP, 1.0 * M_SUN);
    double fill_over = rlof.roche_lobe_filling_factor(1.1 * r_roche, 0.02 * AU, 1.0 * M_JUP, 1.0 * M_SUN);
    assert(fill_under < 1.0);
    assert(fill_over > 1.0);

    double dM_dt_xuv = rlof.compute_photoevaporative_mdot(10.0, 1.4 * R_JUP, 1.0 * M_JUP);
    assert(dM_dt_xuv < 0.0);

    auto [dM_dt, da_dt] = rlof.evaluate_mass_loss_rate(1.05 * r_roche, 0.018 * AU, 1.0 * M_JUP, 1.0 * M_SUN, 5.0);
    assert(dM_dt < 0.0);
    assert(da_dt != 0.0);

    std::cout << "  -> PASSED." << std::endl;
    return 0;
}
