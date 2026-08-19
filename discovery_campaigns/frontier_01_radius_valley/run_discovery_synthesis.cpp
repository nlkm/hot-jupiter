// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Frontier 1 Execution Driver: Forward Population Synthesis of the Exoplanet Radius Valley

#include <iostream>
#include <iomanip>
#include <fstream>
#include "cpp/include/radius_valley_discovery.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   FRONTIER 1 DISCOVERY: SUB-NEPTUNE RADIUS VALLEY POPULATION SYNTHESIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::RadiusValleyDiscoveryEngine engine;
  int n_synthetic = 50000;

  std::cout << "Synthesizing N = " << n_synthetic << " exoplanet systems under Photoevaporation..." << std::endl;
  auto pop_photo = engine.GeneratePopulation(n_synthetic, hot_jupiter::ValleyMechanism::PHOTOEVAPORATION);

  std::cout << "Synthesizing N = " << n_synthetic << " exoplanet systems under Core-Powered Mass Loss..." << std::endl;
  auto pop_core = engine.GeneratePopulation(n_synthetic, hot_jupiter::ValleyMechanism::CORE_POWERED_MASS_LOSS);

  std::cout << "Synthesizing N = " << n_synthetic << " exoplanet systems under Primordial Water Worlds..." << std::endl;
  auto pop_water = engine.GeneratePopulation(n_synthetic, hot_jupiter::ValleyMechanism::PRIMORDIAL_WATER_WORLDS);

  std::cout << "Synthesizing N = " << n_synthetic << " exoplanet systems under Unified Hybrid Model..." << std::endl;
  auto pop_hybrid = engine.GeneratePopulation(n_synthetic, hot_jupiter::ValleyMechanism::HYBRID_UNIFIED);

  // Write synthesis results to CSV for publication plotting
  std::ofstream out_csv("discovery_campaigns/frontier_01_radius_valley/population_synthesis_results.csv");
  out_csv << "mechanism,period_days,radius_rearth,m_star_msun,core_mass_mearth,f_env_final,is_stripped\n";

  for (const auto& p : pop_photo) {
    out_csv << "photoevaporation," << p.orbital_period_days << "," << p.final_radius_rearth << ","
            << p.star_mass_msun << "," << p.core_mass_mearth << "," << p.envelope_mass_fraction_final << ","
            << (p.stripped_to_bare_core ? 1 : 0) << "\n";
  }
  for (const auto& p : pop_core) {
    out_csv << "core_powered," << p.orbital_period_days << "," << p.final_radius_rearth << ","
            << p.star_mass_msun << "," << p.core_mass_mearth << "," << p.envelope_mass_fraction_final << ","
            << (p.stripped_to_bare_core ? 1 : 0) << "\n";
  }
  for (const auto& p : pop_water) {
    out_csv << "water_worlds," << p.orbital_period_days << "," << p.final_radius_rearth << ","
            << p.star_mass_msun << "," << p.core_mass_mearth << "," << p.envelope_mass_fraction_final << ","
            << (p.stripped_to_bare_core ? 1 : 0) << "\n";
  }
  for (const auto& p : pop_hybrid) {
    out_csv << "hybrid_unified," << p.orbital_period_days << "," << p.final_radius_rearth << ","
            << p.star_mass_msun << "," << p.core_mass_mearth << "," << p.envelope_mass_fraction_final << ","
            << (p.stripped_to_bare_core ? 1 : 0) << "\n";
  }
  out_csv.close();

  std::cout << "Successfully exported 200,000 synthetic exoplanets to population_synthesis_results.csv!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
