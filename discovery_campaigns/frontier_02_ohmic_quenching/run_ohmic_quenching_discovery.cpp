// Copyright 2026 Antigravity Scientific Automation & Multi-Physics Discovery Campaign
// Frontier 2 Execution Driver: Ohmic Dissipation Non-Monotonic Inflation & Dynamo Quenching

#include <iostream>
#include <iomanip>
#include <fstream>
#include "cpp/include/ohmic_quenching_discovery.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   FRONTIER 2 DISCOVERY: HOT JUPITER OHMIC DYNAMO QUENCHING ENGINE" << std::endl;
  std::cout << "================================================================================" << std::endl;

  // Evaluate across multiple magnetic field strengths (1 G, 5 G, 10 G, 20 G)
  std::vector<double> b_fields = {1.0, 5.0, 10.0, 20.0};

  std::ofstream out_csv("discovery_campaigns/frontier_02_ohmic_quenching/ohmic_quenching_results.csv");
  out_csv << "b_gauss,t_eq_k,conductivity_s_m,wind_speed_m_s,lorentz_drag_m_s2,ohmic_power_w,radius_rjup,is_quenched\n";

  for (double b : b_fields) {
    hot_jupiter::OhmicQuenchingDiscoveryEngine engine(b, 1.0);
    auto curve = engine.GenerateHeatingCurve(200);

    for (const auto& s : curve) {
      out_csv << b << "," << s.t_eq_k << "," << s.atmospheric_conductivity_s_m << ","
              << s.wind_speed_m_s << "," << s.lorentz_drag_accel_m_s2 << ","
              << s.ohmic_heating_power_watts << "," << s.equilibrium_radius_rjup << ","
              << (s.is_dynamo_quenched ? 1 : 0) << "\n";
    }
  }
  out_csv.close();

  std::cout << "Successfully generated Ohmic Dissipation Quenching Grid (800 profiles)!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
