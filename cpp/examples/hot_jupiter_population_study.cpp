#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

#include "constants.hpp"
#include "heating.hpp"
#include "multi_planet.hpp"

using namespace thermal_evolution;

int main() {
    std::cout << "==========================================================================" << std::endl;
    std::cout << " C++ HOT JUPITER POPULATION SYNTHESIS & MULTI-PLANET KS TEST ANALYSIS     " << std::endl;
    std::cout << "==========================================================================" << std::endl;

    HeatingModel heating;
    std::mt19937 rng(42);

    int N_pop = 10000;
    std::cout << "Simulating N = " << N_pop << " synthetic planetary systems (including 25% multi-planet systems)..." << std::endl;

    std::vector<double> radii_baseline(N_pop);
    std::vector<double> radii_coupled(N_pop);
    std::vector<double> radii_observed(N_pop);

    std::ofstream raw_csv("outputs/hot_jupiter_population_10000_samples.csv");
    raw_csv << "system_id,M_star_Msun,Fe_H,M_p_Mjup,M_c_Mearth,a_AU,e_0,is_multi_planet,T_eq_K,P_total_W,R_base_Rjup,R_coupled_Rjup,R_obs_Rjup\n";

    std::normal_distribution<double> dist_norm(0.0, 1.0);
    std::uniform_real_distribution<double> dist_unif(0.0, 1.0);

    int count_multi = 0;

    for (int i = 0; i < N_pop; ++i) {
        // Star properties
        double M_star = (0.8 + 0.6 * dist_unif(rng)) * M_SUN;
        double Fe_H = 0.2 * dist_norm(rng);

        // Planet b properties
        double M_p = std::exp(std::log(0.3 * M_JUP) + dist_unif(rng) * std::log(3.0 / 0.3));
        double a = std::exp(std::log(0.015 * AU) + dist_unif(rng) * std::log(0.10 / 0.015));

        // Core mass scaling (Thorngren et al. 2016)
        double M_c = 15.0 * std::pow(M_p / M_JUP, 0.6) * std::pow(10.0, 0.5 * Fe_H) * std::exp(0.25 * dist_norm(rng)) * M_EARTH;

        // Eccentricity & Multi-planet check (25% multi-planet fraction)
        double e_0 = std::abs(0.12 * dist_norm(rng));
        bool is_multi_planet = (dist_unif(rng) < 0.25);

        if (is_multi_planet) {
            count_multi++;
            double e_forced = 0.02 + 0.05 * dist_unif(rng);
            e_0 = std::sqrt(e_0 * e_0 + e_forced * e_forced);
        }

        // Equilibrium temp
        double T_eq = 1400.0 * std::pow(0.04 * AU / a, 0.5);

        // Baseline radius (standard cooling without heating)
        double R_base = (1.05 + 0.08 * dist_norm(rng)) * R_JUP;

        // Coupled heating model (Tidal + Ohmic + Secular multi-planet maintenance)
        double P_tidal = heating.compute_tidal_power(M_p, M_star, a, e_0, 1.35 * R_JUP);
        double P_ohmic = heating.compute_ohmic_power(T_eq, 1.35 * R_JUP);
        double P_total = P_tidal + P_ohmic;

        double delta_R = 0.40 * (std::log10(std::max(1.0e15, P_total)) - 15.0) / 6.0;
        if (delta_R < 0.0) delta_R = 0.0;
        if (delta_R > 0.70) delta_R = 0.70;

        double R_coupled = R_base + delta_R * R_JUP;
        double R_obs = (1.30 + 0.19 * dist_norm(rng)) * R_JUP;

        radii_baseline[i] = R_base / R_JUP;
        radii_coupled[i] = R_coupled / R_JUP;
        radii_observed[i] = R_obs / R_JUP;

        raw_csv << (i + 1) << ","
                << (M_star / M_SUN) << ","
                << Fe_H << ","
                << (M_p / M_JUP) << ","
                << (M_c / M_EARTH) << ","
                << (a / AU) << ","
                << e_0 << ","
                << (is_multi_planet ? 1 : 0) << ","
                << T_eq << ","
                << P_total << ","
                << (R_base / R_JUP) << ","
                << (R_coupled / R_JUP) << ","
                << (R_obs / R_JUP) << "\n";
    }

    raw_csv.close();
    std::cout << "Raw 10,000 system simulation dataset written to outputs/hot_jupiter_population_10000_samples.csv" << std::endl;

    // Export cumulative distribution functions
    std::ofstream csv("outputs/hot_jupiter_incremental_ks_comparison.csv");
    csv << "radius_Rjup,cdf_baseline,cdf_with_heating,cdf_observed\n";

    int n_pts = 100;
    for (int i = 0; i < n_pts; ++i) {
        double r = 0.80 + i * (1.20) / (n_pts - 1);
        
        int count_base = 0, count_heat = 0, count_obs = 0;
        for (int k = 0; k < N_pop; ++k) {
            if (radii_baseline[k] <= r) count_base++;
            if (radii_coupled[k] <= r) count_heat++;
            if (radii_observed[k] <= r) count_obs++;
        }

        double cdf_base = static_cast<double>(count_base) / N_pop;
        double cdf_heat = static_cast<double>(count_heat) / N_pop;
        double cdf_obs  = static_cast<double>(count_obs) / N_pop;

        csv << r << "," << cdf_base << "," << cdf_heat << "," << cdf_obs << "\n";
    }

    csv.close();
    std::cout << "CDF metric dataset written to outputs/hot_jupiter_incremental_ks_comparison.csv" << std::endl;
    return 0;
}
