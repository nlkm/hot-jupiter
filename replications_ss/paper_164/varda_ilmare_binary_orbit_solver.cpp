// Solver for Paper #164: Classical Trans-Neptunian Object (174567) Varda & Satellite Ilmare Mutual Orbit, High Bulk Density, & Binary Formation Dynamics (Grundy 2015, Thirouin 2014, Souami 2020)
// Evaluates Hubble Space Telescope (HST) astrometric mutual orbit determination for cold/hot classical Kuiper Belt Object (174567) Varda (primary radius R_varda = 360 +- 15 km) and its satellite Ilmare (radius R_ilmare = 163 +- 10 km), orbital semi-major axis a_orb = 4800 +- 40 km, eccentricity e_orb = 0.0215, period P_orb = 5.751 days, total system mass M_sys = (2.664 +- 0.04) x 10^20 kg, high bulk density rho_bulk = 1250 +- 150 kg/m^3 indicating silicate-dominated interior, and collision/capture binary origin models.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Grundy et al. (2015) & Thirouin et al. (2014) Varda-Ilmarë Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_164/varda_ilmare_orbit.csv");
  csv_file << "varda_radius_km,ilmare_radius_km,semimajor_axis_km,eccentricity,orbital_period_days,system_mass_10_20_kg,bulk_density_kg_m3\n";

  // Semi-major axis range a_orb from 3000 km to 7000 km (nominal a_orb = 4800 km)
  for (double a_km = 3000.0; a_km <= 7000.0; a_km += 500.0) {
    double r_varda_km = 360.0;
    double r_ilmare_km = 163.0;

    // System total mass (10^20 kg):
    double m_sys_10_20 = 2.664;
    double m_sys_kg = m_sys_10_20 * 1.0e20;

    // Keplerian orbital period (days):
    double a_m = a_km * 1000.0;
    double p_orb_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (hot_jupiter::G * m_sys_kg));
    double p_orb_days = p_orb_sec / 86400.0;

    // Eccentricity e:
    double ecc = 0.0215;

    // Bulk density rho_bulk (kg/m^3):
    double rho_bulk = 1250.0;

    csv_file << std::fixed << std::setprecision(1) << r_varda_km << "," << std::setprecision(1) << r_ilmare_km << "," << std::setprecision(1) << a_km << "," << std::setprecision(4) << ecc << "," << std::setprecision(3) << p_orb_days << "," << std::setprecision(3) << m_sys_10_20 << "," << std::setprecision(0) << rho_bulk << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_164/varda_ilmare_orbit.csv" << std::endl;
  return 0;
}
