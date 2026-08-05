#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "constants.hpp"
#include "orbital.hpp"
#include "heating.hpp"

using namespace hot_jupiter;

int main() {
    std::cout << "==========================================================================" << std::endl;
    std::cout << " C++ STELLAR SPIN-ORBIT MISALIGNMENT (ROSSITER-MCLAUGHLIN) BENCHMARK      " << std::endl;
    std::cout << "==========================================================================" << std::endl;

    StellarTidalRates star_rates;
    HeatingModel heating;

    double M_p = 1.0 * M_JUP;
    double M_star = 1.0 * M_SUN;
    double R_p = 1.35 * R_JUP;

    std::ofstream csv("outputs/stellar_misaligned_orbit_evolution.csv");
    csv << "t_gyr,a_aligned,a_polar,a_retrograde,P_tidal_retro_W\n";

    double a_aligned = 0.040 * AU;
    double a_polar = 0.040 * AU;
    double a_retro = 0.040 * AU;

    double dt = 2.0e6 * YEAR;
    int steps = 500;

    for (int i = 0; i < steps; ++i) {
        double t_gyr = (i * dt) / GYR;

        auto [da_aligned, _1] = star_rates.evaluate_stellar_rates(M_p, M_star, a_aligned, 2.9e-6, 0.0);
        auto [da_polar, _2]   = star_rates.evaluate_stellar_rates(M_p, M_star, a_polar, 2.9e-6, 80.0 * M_PI / 180.0);
        auto [da_retro, _3]   = star_rates.evaluate_stellar_rates(M_p, M_star, a_retro, 2.9e-6, 135.0 * M_PI / 180.0);

        a_aligned += da_aligned * dt;
        a_polar   += da_polar * dt;
        a_retro   += da_retro * dt;

        double P_tidal_retro = heating.compute_tidal_power(M_p, M_star, a_retro, 0.15, R_p);

        csv << t_gyr << "," << (a_aligned / AU) << "," << (a_polar / AU) << "," << (a_retro / AU) << "," << P_tidal_retro << "\n";
    }

    csv.close();
    std::cout << "CSV data written to outputs/stellar_misaligned_orbit_evolution.csv" << std::endl;
    return 0;
}
