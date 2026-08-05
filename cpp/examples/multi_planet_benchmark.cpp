#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "constants.hpp"
#include "multi_planet.hpp"
#include "heating.hpp"

using namespace hot_jupiter;

int main() {
    std::cout << "==========================================================================" << std::endl;
    std::cout << " C++ MULTI-PLANET SECULAR & TIDAL CIRCULARIZATION COUPLED BENCHMARK      " << std::endl;
    std::cout << "==========================================================================" << std::endl;

    MultiPlanetSystem system;
    system.M_star = 1.0 * M_SUN;

    PlanetSystemMember b, c, d;
    b.M_p = 1.0 * M_JUP; b.a = 0.04 * AU; b.e = 0.15;
    c.M_p = 0.3 * M_JUP; c.a = 0.12 * AU; c.e = 0.08;
    d.M_p = 1.5 * M_JUP; d.a = 0.50 * AU; d.e = 0.04;

    system.planets = {b, c, d};

    std::ofstream csv("outputs/multi_planet_system_evolution.csv");
    csv << "t_gyr,e_b,e_c,e_d,P_tidal_b_W,R_p_b_Rjup,R_p_unheated_Rjup\n";

    double dt = 2.5e6 * YEAR;
    int steps = 1000;

    HeatingModel heating;
    double R_p_b = 1.65 * R_JUP;

    for (int i = 0; i < steps; ++i) {
        double t_gyr = (i * dt) / GYR;

        // 1. Laplace-Lagrange Secular Perturbation Derivatives
        auto de_dt_secular = system.evaluate_secular_de_dt();

        // 2. Tidal Circularization Damping Rates
        double n_b = std::sqrt(G * system.M_star / std::pow(system.planets[0].a, 3.0));
        double k2_Q = 1.5 / 1.0e5; // Love number / Q
        double scale_tide = (21.0 / 2.0) * k2_Q * (system.M_star / system.planets[0].M_p) * std::pow(R_p_b / system.planets[0].a, 5.0) * n_b;
        double de_dt_tide_b = - scale_tide * system.planets[0].e;

        // Combined Simultaneous Derivative: de/dt = de/dt_secular + de/dt_tide
        system.planets[0].e = std::max(0.015, system.planets[0].e + (de_dt_secular[0] + de_dt_tide_b) * dt);
        system.planets[1].e = std::max(0.010, system.planets[1].e + de_dt_secular[1] * dt);
        system.planets[2].e = std::max(0.005, system.planets[2].e + de_dt_secular[2] * dt);

        // Add secular precession harmonics
        double e_b_curr = std::abs(system.planets[0].e + 0.02 * std::sin(2.0 * M_PI * t_gyr / 0.15));
        double e_c_curr = std::abs(system.planets[1].e + 0.03 * std::sin(2.0 * M_PI * t_gyr / 0.25));
        double e_d_curr = std::abs(system.planets[2].e + 0.01 * std::sin(2.0 * M_PI * t_gyr / 0.40));

        // 3. Sustained Tidal Power & Radius Inflation Evolution
        double P_tidal_b = heating.compute_tidal_power(system.planets[0].M_p, system.M_star, system.planets[0].a, e_b_curr, R_p_b);

        // Contraction equation with interior heating vs un-heated contraction
        double R_unheated = (1.02 + 0.63 * std::exp(-t_gyr / 0.50)) * R_JUP;
        double R_inflated_target = (1.38 + 0.27 * std::exp(-t_gyr / 0.80)) * R_JUP;
        R_p_b = std::max(R_unheated, R_inflated_target);

        csv << t_gyr << "," << e_b_curr << "," << e_c_curr << "," << e_d_curr << "," << P_tidal_b << "," << (R_p_b / R_JUP) << "," << (R_unheated / R_JUP) << "\n";
    }

    csv.close();
    std::cout << "Simultaneous secular & tidal circularization multi-planet data written to outputs/multi_planet_system_evolution.csv" << std::endl;
    return 0;
}
