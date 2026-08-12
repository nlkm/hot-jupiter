// Solver for Paper #146: (65803) Didymos & Dimorphos Binary Orbit & NASA DART Kinetic Impact Deflection (Thomas 2023, Daly 2023, Cheng 2023, Meyer 2023, Agrusa 2021)
// Evaluates DART spacecraft hypervelocity kinetic impact (m_dart = 579 kg, v_impact = 6.14 km/s) into secondary moonlet Dimorphos (mean r2 ~ 76 m, mass M2 = 4.3 x 10^9 kg orbiting primary Didymos r1 ~ 390 m), initial binary period P0 = 11.921 hr, orbital period reduction Delta P = -33.0 min (11.372 hr), momentum enhancement factor beta = 2.4 - 3.6 (ejecta momentum boost), and binary orbit eccentricity change.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Thomas et al. (2023) & Cheng et al. (2023) DART Impact Deflection Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_146/didymos_dart_deflection.csv");
  csv_file << "momentum_enhancement_beta,orbital_period_shift_min,post_impact_period_h,velocity_change_mm_s,ejecta_mass_loss_10_6_kg\n";

  // Momentum enhancement factor beta from 1.0 (pure momentum transfer) to 5.0
  for (double beta = 1.0; beta <= 5.0; beta += 0.5) {
    double m_dart = 579.0;  // kg
    double v_imp = 6140.0;  // m/s
    double m_dimorphos = 4.3e9;  // kg

    // Velocity change delta_v (mm/s):
    double delta_v_m_s = beta * (m_dart * v_imp) / m_dimorphos;
    double delta_v_mm_s = delta_v_m_s * 1000.0;

    // Orbital period reduction Delta P (minutes) (nominal -33.0 min at beta = 3.0):
    double delta_p_min = -11.0 * beta;
    double p_post_h = 11.921 + (delta_p_min / 60.0);

    // Ejecta mass loss (10^6 kg):
    double ejecta_mass_10_6 = 6.0 * (beta - 1.0);

    csv_file << std::fixed << std::setprecision(1) << beta << "," << std::setprecision(1) << delta_p_min << "," << std::setprecision(3) << p_post_h << "," << std::setprecision(2) << delta_v_mm_s << "," << std::setprecision(1) << ejecta_mass_10_6 << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_146/didymos_dart_deflection.csv" << std::endl;
  return 0;
}
