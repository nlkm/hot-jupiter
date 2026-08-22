// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #110: Enceladus CDA Salt Fractionation Driver (110-Paper Milestone)

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #110: ENCELADUS CDA SODIUM SALT FRACTIONATION" << std::endl;
  std::cout << "   *** 110-PAPER LANDMARK OBSERVATIONAL SERIES MILESTONE ***" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::EnceladusCDASaltFractionationModel model;

  const double salt_frac = model.sodium_salt_mass_fraction();      // ~ 0.015 (1.5%)
  const double m_dot = model.dust_mass_production_rate_kg_s();     // ~ 5.0 kg/s
  const double ocean_ph = model.ocean_ph_value();                  // ~ 9.5 (Alkaline ocean)
  const double v_grain = model.e_ring_grain_velocity_m_s();        // ~ 250.0 m/s

  std::cout << "Sodium Salt Mass Fraction: " << (salt_frac * 100.0) << " %" << std::endl;
  std::cout << "E-Ring Dust Mass Production Rate: " << m_dot << " kg/s" << std::endl;
  std::cout << "Subsurface Ocean pH: " << ocean_ph << " (Alkaline serpentinizing fluid)" << std::endl;
  std::cout << "Mean Plume Grain Vent Velocity: " << v_grain << " m/s" << std::endl;

  // Track CDA Time-of-Flight Mass Spectrum Intensity vs m/z from 1.0 to 100.0 Da (linear mass scale):
  // Type III Salt-Rich grains show major peaks at:
  // H3O+ (19), Na+ (23), K+ (39), NaOH+ (40), Na2OH+ (63), NaCl+ (58), Na2Cl+ (81)
  std::ofstream out("replications_observational/paper_110/enceladus_cda_spectrum.csv");
  out << "mass_to_charge_mz,relative_ion_intensity,type3_salt_peak_intensity\n";

  for (double mz = 1.0; mz <= 100.0; mz += 0.5) {
    // Baseline continuum
    double base = 0.02 * std::exp(-mz / 40.0);

    // Water clusters: H3O+ (19), (H2O)2H+ (37), (H2O)3H+ (55)
    double peak_19 = 0.85 * std::exp(-std::pow((mz - 19.0) / 0.6, 2.0));
    double peak_37 = 0.45 * std::exp(-std::pow((mz - 37.0) / 0.6, 2.0));
    double peak_55 = 0.25 * std::exp(-std::pow((mz - 55.0) / 0.6, 2.0));

    // Sodium and Potassium salt peaks:
    double peak_na = 1.00 * std::exp(-std::pow((mz - 23.0) / 0.5, 2.0)); // Na+ (23)
    double peak_k  = 0.35 * std::exp(-std::pow((mz - 39.0) / 0.5, 2.0)); // K+ (39)
    double peak_na2oh = 0.40 * std::exp(-std::pow((mz - 63.0) / 0.7, 2.0)); // Na2OH+ (63)
    double peak_na2cl = 0.30 * std::exp(-std::pow((mz - 81.0) / 0.7, 2.0)); // Na2Cl+ (81)

    double salt_tot = peak_na + peak_k + peak_na2oh + peak_na2cl;
    double ion_tot = base + peak_19 + peak_37 + peak_55 + salt_tot;

    out << mz << "," << ion_tot << "," << salt_tot << "\n";
  }
  out.close();

  std::cout << "Generated Enceladus CDA Salt Spectrum Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
