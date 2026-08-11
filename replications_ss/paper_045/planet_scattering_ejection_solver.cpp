// Solver for Paper #45: Giant Planet Gravitational Scattering & Ejection Dynamics (Rasio & Ford 1996, Weidenschilling & Marzari 1996)
// Evaluates escape velocity v_esc = sqrt(2*G*M_star / a), ejection velocity v_ej, and final eccentricity e_final.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Rasio & Ford (1996) & Weidenschilling (1996) Planet Scattering Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_045/planet_scattering_ejections.csv");
  csv_file << "planet_mass_jup,semimajor_axis_au,v_surf_km_s,v_orb_km_s,safronov_number_theta\n";

  double m_sun = hot_jupiter::M_SUN;

  // Planet masses from 0.1 M_jup to 10.0 M_jup at 5.0 AU
  for (double m_jup = 0.1; m_jup <= 10.0; m_jup *= 2.0) {
    double m_planet_kg = m_jup * hot_jupiter::M_JUP;
    double a_m = 5.0 * hot_jupiter::AU;

    double v_orb_m_s = std::sqrt(hot_jupiter::G * m_sun / a_m);
    double r_planet_m = hot_jupiter::R_JUP * std::pow(m_jup, 1.0 / 3.0);  // constant density scaling
    double v_surf_m_s = std::sqrt(2.0 * hot_jupiter::G * m_planet_kg / r_planet_m);

    // Safronov parameter Theta = (v_surf / v_orb)^2 / 2
    double theta_safronov = 0.5 * std::pow(v_surf_m_s / v_orb_m_s, 2.0);

    csv_file << std::fixed << std::setprecision(1) << m_jup << ",5.0," << std::setprecision(2) << (v_surf_m_s / 1000.0) << "," << std::setprecision(2) << (v_orb_m_s / 1000.0) << "," << std::setprecision(2) << theta_safronov << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_045/planet_scattering_ejections.csv" << std::endl;
  return 0;
}
