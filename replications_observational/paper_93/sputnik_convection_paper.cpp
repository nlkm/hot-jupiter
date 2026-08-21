// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #93: Pluto Sputnik Planitia Nitrogen Ice Convection Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #93: PLUTO SPUTNIK PLANITIA NITROGEN CONVECTION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::PlutoSputnikPlanitiaConvectionModel model;

  const double d_cell_km = model.cell_diameter_km();             // ~ 30 km
  const double t_overturn_yr = model.overturning_timescale_years();// ~ 5.0e5 yr (500,000 yr)
  const double h_ice_km = model.nitrogen_ice_thickness_km();      // ~ 6.0 km
  const double ra_num = model.rayleigh_number();                 // ~ 1.0e7

  std::cout << "Polygonal Convective Cell Diameter: " << d_cell_km << " km" << std::endl;
  std::cout << "Overturning Timescale: " << t_overturn_yr << " years" << std::endl;
  std::cout << "Solid Nitrogen Ice Thickness: " << h_ice_km << " km" << std::endl;
  std::cout << "Convective Rayleigh Number Ra: " << ra_num << std::endl;

  // Track Surface Relief Topography across Polygonal Cell x = -20 km to +20 km (linear spatial scale):
  // Central upwelling produces ~ +50 m dome, margin downwelling troughs produce ~ -50 m depression
  std::ofstream out("replications_observational/paper_93/sputnik_topography_profile.csv");
  out << "distance_from_cell_center_km,surface_topography_relief_meters,subsurface_upwelling_velocity_cm_yr\n";

  const double r_cell = d_cell_km / 2.0; // 15 km

  for (double x_km = -20.0; x_km <= 20.0; x_km += 0.5) {
    // Thermal buoyant topography profile
    double topo_m = 50.0 * std::cos(M_PI * x_km / (2.0 * r_cell));
    if (std::abs(x_km) > r_cell) {
      topo_m = -50.0 * std::exp(-std::pow((std::abs(x_km) - r_cell) / 3.0, 2.0));
    }

    // Vertical convective velocity profile v_z [cm/year]
    double v_z_cm_yr = (h_ice_km * 1.0e5 / t_overturn_yr) * std::cos(M_PI * x_km / (2.0 * r_cell)); // ~ 1.2 cm/yr

    out << x_km << "," << topo_m << "," << v_z_cm_yr << "\n";
  }
  out.close();

  std::cout << "Generated Sputnik Planitia Convection Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
