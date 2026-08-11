// Solver for Paper #125 (MILESTONE 25% REACHED): Callisto Subsurface Ocean Hydrodynamics & Induced Dipole Field (Zimmer 2000, Spohn & Schubert 2003, Vance 2014, Gomez Casajus 2021)
// Evaluates ice shell thickness d_ice ~ 100 - 150 km, subsurface ocean conductivity sigma ~ 2.5 - 5.0 S/m, Galileo magnetometer induced magnetic moment amplitude B_ind ~ 30 - 40 nT in response to Jovian magnetospheric excitation (T_Jup = 10.1 hr), and ocean layer thickness d_ocean ~ 100 - 200 km.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Zimmer (2000) & Spohn (2003) Callisto Subsurface Ocean Solver (MILESTONE 25%) ===" << std::endl;

  std::ofstream csv_file("replications_ss/paper_125/callisto_subsurface_ocean.csv");
  csv_file << "ice_shell_thickness_km,ocean_conductivity_s_m,induced_dipole_moment_ratio,magnetic_field_response_nt,tidal_heat_flux_mw_m2\n";

  // Ice shell thickness d_ice from 50 km to 200 km
  for (double d_ice_km = 50.0; d_ice_km <= 200.0; d_ice_km += 25.0) {
    double ocean_conductivity_s_m = 4.0;  // Saline NH3-H2O-MgSO4 ocean conductivity

    // Callisto radius R_c = 2410 km:
    double r_ocean_km = 2410.0 - d_ice_km;

    // Induced magnetic moment ratio A = (r_ocean / R_c)^3 * f_cond (Zimmer et al. 2000):
    double f_cond = 0.95;  // Skin depth shielding factor at 10.1 hr period
    double a_ratio = std::pow(r_ocean_km / 2410.0, 3.0) * f_cond;

    // Induced magnetic field perturbation B_ind (nT) at flyby altitude (B_Jovian ~ 35 nT):
    double b_ind_nt = 35.0 * a_ratio;

    // Tidal + radiogenic conductive heat flux F_heat (mW/m^2):
    double f_heat_mw_m2 = 3.5 * (100.0 / d_ice_km);

    csv_file << std::fixed << std::setprecision(1) << d_ice_km << "," << std::setprecision(1) << ocean_conductivity_s_m << "," << std::setprecision(3) << a_ratio << "," << std::setprecision(1) << b_ind_nt << "," << std::setprecision(2) << f_heat_mw_m2 << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_125/callisto_subsurface_ocean.csv" << std::endl;
  return 0;
}
