// Solver for Paper #68: Debris Disk Radiation Pressure Blowout & Dohnanyi Collisional Cascade (Burns 1979, Dohnanyi 1969)
// Evaluates radiation force parameter beta = F_rad / F_grav = 3 L_* Q_pr / (16 pi G M_* c rho s), blowout grain radius s_blow, and Dohnanyi mass spectrum q = 11/6.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Burns (1979) & Dohnanyi (1969) Radiation Pressure Blowout Solver ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_068/dust_blowout_betas.csv");
  csv_file << "grain_radius_um,beta_param,orbit_state_str,dohnanyi_dn_ds\n";

  double l_star_solar = 1.0;     // Solar luminosity L_sun
  double m_star_solar = 1.0;     // Solar mass M_sun
  double rho_dust_g_cm3 = 2.0;   // Dust density 2.0 g/cm^3 (2000 kg/m^3)
  double q_pr = 1.0;             // Radiation pressure efficiency factor ~ 1.0
  double c_speed = 2.99792458e8; // Speed of light [m/s]

  double l_star_w = l_star_solar * hot_jupiter::L_SUN;
  double m_star_kg = m_star_solar * hot_jupiter::M_SUN;
  double rho_dust_kg_m3 = rho_dust_g_cm3 * 1000.0;

  // Burns (1979) blowout grain size s_blow where beta = 0.5 (unbound parabolic orbit upon creation):
  // beta = 3 * L_* * Q_pr / (16 * pi * G * M_* * c * rho * s) = 0.57 um * (L_*/L_sun) * (M_sun/M_*) * (2000/rho)
  double s_blow_um = 0.573 * (l_star_solar / m_star_solar) * (2000.0 / rho_dust_kg_m3);
  std::cout << "Blowout Grain Radius (beta = 0.5): " << s_blow_um << " um" << std::endl;

  // Grain radii from 0.05 um to 100.0 um
  for (double s_um = 0.05; s_um <= 100.0; s_um *= 1.5) {
    double s_m = s_um * 1.0e-6;

    // Beta parameter ratio F_rad / F_grav
    double beta = 3.0 * l_star_w * q_pr / (16.0 * hot_jupiter::PI * hot_jupiter::G * m_star_kg * c_speed * rho_dust_kg_m3 * s_m);

    std::string state = "BOUND";
    if (beta >= 1.0) {
      state = "HYPERBOLIC_BLOWOUT";
    } else if (beta >= 0.5) {
      state = "UNBOUND_PARABOLIC";
    }

    // Dohnanyi (1969) steady-state collisional cascade size spectrum dN/ds ~ s^-3.5 (q = 11/6)
    double dn_ds_dohnanyi = std::pow(s_um, -3.5);

    csv_file << std::scientific << std::setprecision(4) << s_um << "," << std::fixed << std::setprecision(3) << beta << "," << state << "," << std::scientific << std::setprecision(4) << dn_ds_dohnanyi << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_068/dust_blowout_betas.csv" << std::endl;
  return 0;
}
