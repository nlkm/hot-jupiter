// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Frontier 6 Execution Driver: Ocean-Freezing Pressurization & Viscoelastic Cryosphere Fracture Engine

#include <iostream>
#include <iomanip>
#include <fstream>
#include "cpp/include/cryosphere_fracture_discovery.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   FRONTIER 6 DISCOVERY: OCEAN-FREEZING VISCOELASTIC CRYOSPHERE FRACTURE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  // 1. Charon Global Chasma Benchmark (R = 606 km, g = 0.288 m/s^2, density = 1700 kg/m^3)
  hot_jupiter::CryosphereFractureDiscoveryEngine charon(606.0, 0.288, 1700.0, 3.5, 2.0);
  auto charon_track = charon.EvolveFreezingCryosphere(20.0, 100.0, 110.0, 0.12, 1000.0, 1.0);

  std::ofstream out_charon("discovery_campaigns/frontier_06_cryosphere_fracture/charon_freezing_track.csv");
  out_charon << "time_myr,h_ice_km,h_ocean_km,delta_p_mpa,hoop_stress_mpa,is_fractured\n";
  for (const auto& s : charon_track) {
    out_charon << s.time_myr << "," << s.ice_shell_thickness_km << "," << s.ocean_thickness_km << ","
               << s.ocean_overpressure_mpa << "," << s.surface_hoop_stress_mpa << "," << (s.is_fractured ? 1 : 0) << "\n";
  }
  out_charon.close();

  // 2. Enceladus Tiger Stripe Benchmark (R = 252 km, g = 0.113 m/s^2, density = 1610 kg/m^3)
  hot_jupiter::CryosphereFractureDiscoveryEngine enceladus(252.0, 0.113, 1610.0, 3.5, 1.5);
  auto enc_track = enceladus.EvolveFreezingCryosphere(10.0, 40.0, 130.0, 0.05, 800.0, 1.0);

  std::ofstream out_enc("discovery_campaigns/frontier_06_cryosphere_fracture/enceladus_freezing_track.csv");
  out_enc << "time_myr,h_ice_km,h_ocean_km,delta_p_mpa,hoop_stress_mpa,is_fractured\n";
  for (const auto& s : enc_track) {
    out_enc << s.time_myr << "," << s.ice_shell_thickness_km << "," << s.ocean_thickness_km << ","
            << s.ocean_overpressure_mpa << "," << s.surface_hoop_stress_mpa << "," << (s.is_fractured ? 1 : 0) << "\n";
  }
  out_enc.close();

  // 3. 2D Viscoelastic Phase Diagram Grid: Lid Temperature vs Freezing Rate
  std::ofstream out_grid("discovery_campaigns/frontier_06_cryosphere_fracture/fracture_phase_grid.csv");
  out_grid << "lid_temp_k,freezing_rate_km_myr,failure_mode,max_stress_mpa\n";

  for (double t_lid = 80.0; t_lid <= 260.0; t_lid += 10.0) {
    for (double f_rate = 0.01; f_rate <= 0.50; f_rate += 0.02) {
      auto hist = charon.EvolveFreezingCryosphere(20.0, 80.0, t_lid, f_rate, 400.0, 2.0);
      auto mode = charon.ClassifyFailure(hist);
      double max_sigma = 0.0;
      for (const auto& step : hist) {
        max_sigma = std::max(max_sigma, step.surface_hoop_stress_mpa);
      }
      std::string mode_str = (mode == hot_jupiter::CryosphereFailureMode::BRITTLE_TENSILE_RUPTURE) ? "brittle_rupture" :
                             ((mode == hot_jupiter::CryosphereFailureMode::DUCTILE_VISCOUS_RELAXATION) ? "ductile_relaxation" : "quiescent");
      out_grid << t_lid << "," << f_rate << "," << mode_str << "," << max_sigma << "\n";
    }
  }
  out_grid.close();

  std::cout << "Successfully generated Cryosphere Freezing Tracks and Phase Grids!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
