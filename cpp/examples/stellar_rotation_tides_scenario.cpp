#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "constants.hpp"
#include "orbital.hpp"

using namespace hot_jupiter;

int main() {
    std::cout << "==========================================================================" << std::endl;
    std::cout << " C++ STELLAR ROTATION TIDAL MIGRATION BENCHMARK                           " << std::endl;
    std::cout << "==========================================================================" << std::endl;

    StellarTidalRates star_rates;

    double M_p = 1.0 * M_JUP;
    double M_star = 1.0 * M_SUN;

    std::ofstream csv("outputs/stellar_rotation_tidal_migration.csv");
    csv << "t_gyr,a_sub_AU,a_super_AU\n";

    double a_sub = 0.030 * AU;
    double a_super = 0.040 * AU;

    // Sub-synchronous: P_* = 25 days (Omega_* = 2.9e-6 rad/s)
    double Omega_sub = (2.0 * M_PI) / (25.0 * DAY);
    // Super-synchronous: P_* = 1.5 days (Omega_* = 4.8e-5 rad/s)
    double Omega_super = (2.0 * M_PI) / (1.5 * DAY);

    double dt = 2.0e6 * YEAR;
    int steps = 500;

    for (int i = 0; i < steps; ++i) {
        double t_gyr = (i * dt) / GYR;

        auto [da_sub, _1]   = star_rates.evaluate_stellar_rates(M_p, M_star, a_sub, Omega_sub);
        auto [da_super, _2] = star_rates.evaluate_stellar_rates(M_p, M_star, a_super, Omega_super);

        a_sub   = std::max(0.005 * AU, a_sub + da_sub * dt);
        a_super += da_super * dt;

        csv << t_gyr << "," << (a_sub / AU) << "," << (a_super / AU) << "\n";
    }

    csv.close();
    std::cout << "CSV data written to outputs/stellar_rotation_tidal_migration.csv" << std::endl;
    return 0;
}
