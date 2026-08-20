// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #59: Enceladus E-Ring Cryovolcanic Salt Fractionation Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #59: ENCELADUS E-RING CDA SALT FRACTIONATION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::EnceladusCDASaltFractionationModel model;

  const double bulk_salt_frac = model.sodium_salt_mass_fraction(); // 0.015 (1.5% NaCl + Na2CO3)


  // Grain size distribution and salt fractionation:
  // Large grains (r > 1 um) are flash-frozen ocean droplet spray that are salt-rich (Type III)
  // Small grains (r < 0.5 um) condense from pure vapor and are salt-poor (Type I)
  std::ofstream out("replications_observational/paper_59/enceladus_grain_salt_spectrum.csv");
  out << "grain_radius_um,salt_mass_fraction_pct,ejection_velocity_m_s,type_fraction\n";

  for (double r_um = 0.1; r_um <= 5.0; r_um += 0.1) {
    // Salt fraction increases sigmoidally with droplet radius
    double salt_pct = bulk_salt_frac * 100.0 / (1.0 + std::exp(-(r_um - 0.8) / 0.25));
    // Gas drag velocity acceleration: v(r) ~ v_gas / (1 + r / r_0)
    double v_grain = 450.0 / (1.0 + (r_um / 1.2));

    out << r_um << "," << salt_pct << "," << v_grain << "," << (salt_pct / 1.5) << "\n";
  }
  out.close();

  std::cout << "Generated Enceladus CDA Salt Fractionation simulation data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
