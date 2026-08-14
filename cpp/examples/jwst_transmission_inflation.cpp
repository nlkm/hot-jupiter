#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "constants.hpp"
#include "atmosphere.hpp"
#include "heating.hpp"

using namespace hot_jupiter;

int main() {
    std::cout << "==========================================================================" << std::endl;
    std::cout << " C++ JWST TRANSMISSION SPECTRUM SCALE HEIGHT INFLATION BENCHMARK          " << std::endl;
    std::cout << "==========================================================================" << std::endl;

    GuillotAtmosphere atm;
    HeatingModel heating;

    double M_p = 1.0 * M_JUP;
    double R_star = 1.0 * R_SUN;
    double a = 0.03 * AU;

    std::ofstream csv("outputs/jwst_transmission_scale_height.csv");
    csv << "t_gyr,R_p_inflated_Rjup,R_p_base_Rjup,H_inflated_km,H_base_km,delta_ppm_inflated,delta_ppm_base\n";

    double dt = 4.56e6 * YEAR;
    int steps = 1000;

    for (int i = 0; i < steps; ++i) {
        double t_gyr = (i * dt) / GYR;

        // Un-heated baseline radius cooling
        double R_base = (1.02 + 0.63 * std::exp(-t_gyr / 0.50)) * R_JUP;
        // Inflated radius under deep interior tidal + Ohmic heating
        double R_inflated = (1.42 + 0.28 * std::exp(-t_gyr / 0.80)) * R_JUP;

        double T_eq = 1500.0 * std::pow(0.04 * AU / a, 0.5);

        double H_base_m = atm.compute_scale_height(T_eq, M_p, R_base);
        double H_inflated_m = atm.compute_scale_height(T_eq, M_p, R_inflated);

        double delta_ppm_base = atm.compute_transit_depth_variation_ppm(R_base, R_star, H_base_m, 5);
        double delta_ppm_inflated = atm.compute_transit_depth_variation_ppm(R_inflated, R_star, H_inflated_m, 5);

        csv << t_gyr << ","
            << (R_inflated / R_JUP) << ","
            << (R_base / R_JUP) << ","
            << (H_inflated_m / 1000.0) << ","
            << (H_base_m / 1000.0) << ","
            << delta_ppm_inflated << ","
            << delta_ppm_base << "\n";
    }

    csv.close();
    std::cout << "JWST scale height transmission benchmark written to outputs/jwst_transmission_scale_height.csv" << std::endl;
    return 0;
}
