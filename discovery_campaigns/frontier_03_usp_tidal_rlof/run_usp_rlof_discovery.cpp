// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Frontier 3 Execution Driver: USP Tidal Plunge vs. Roche Lobe Mantle Stripping

#include <iostream>
#include <iomanip>
#include <fstream>
#include "cpp/include/usp_rlof_discovery.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   FRONTIER 3 DISCOVERY: ULTRA-SHORT-PERIOD PLANET TIDAL RLOF ENGINE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::USPRLOFDiscoveryEngine engine(1.0, 1.0, 1.0e-6);

  // 1. Grid of initial planet parameters: core mass [1, 10] M_E, mantle mass [1, 15] M_E, a_init in [0.012, 0.025] AU
  std::ofstream out_grid("discovery_campaigns/frontier_03_usp_tidal_rlof/usp_fate_grid.csv");
  out_grid << "m_core_init,m_mantle_init,a_init_au,fate,final_mass,final_iron_frac,final_period_hr,final_radius\n";

  for (double m_c = 1.0; m_c <= 10.0; m_c += 1.5) {
    for (double m_m = 1.0; m_m <= 15.0; m_m += 2.0) {
      for (double a0 = 0.012; a0 <= 0.025; a0 += 0.002) {
        auto hist = engine.EvolveSystem(m_c, m_m, a0, 5000.0, 1.0);
        auto fate = engine.ClassifyFate(hist);
        const auto& f = hist.back();
        std::string fate_str = (fate == hot_jupiter::USPFate::CATASTROPHIC_COLLISION) ? "collision" :
                               ((fate == hot_jupiter::USPFate::STABLE_ROCHE_STRIPPED_REMNANT) ? "stripped_remnant" : "parked");
        double fe_frac = f.core_mass_mearth / std::max(0.01, f.planet_mass_mearth);

        out_grid << m_c << "," << m_m << "," << a0 << "," << fate_str << ","
                 << f.planet_mass_mearth << "," << fe_frac << "," << f.orbital_period_hours << ","
                 << f.planet_radius_rearth << "\n";
      }
    }
  }
  out_grid.close();

  // 2. High-resolution time track for a landmark Super-Mercury progenitor (TOI-849b analog)
  // M_core = 8 M_E, M_mantle = 12 M_E, a_0 = 0.017 AU
  auto track = engine.EvolveSystem(8.0, 12.0, 0.017, 3000.0, 0.2);
  std::ofstream out_track("discovery_campaigns/frontier_03_usp_tidal_rlof/usp_evolution_track.csv");
  out_track << "time_myr,a_au,period_hr,mass_me,core_me,mantle_me,radius_re,a_roche_au,is_overflowing\n";
  for (const auto& s : track) {
    out_track << s.time_myr << "," << s.semimajor_axis_au << "," << s.orbital_period_hours << ","
              << s.planet_mass_mearth << "," << s.core_mass_mearth << "," << s.mantle_mass_mearth << ","
              << s.planet_radius_rearth << "," << s.roche_radius_au << "," << (s.is_overflowing_roche ? 1 : 0) << "\n";
  }
  out_track.close();

  std::cout << "Successfully generated USP RLOF Grid and Evolutionary Tracks!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
