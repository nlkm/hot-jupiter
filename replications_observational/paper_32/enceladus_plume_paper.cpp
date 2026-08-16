// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #32: Enceladus Plume Hydrothermal Activity & Ocean Salinity

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #32: ENCELADUS PLUME HYDROTHERMAL DYNAMICS ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::EnceladusPlumeHydrothermalModel model;
  double p_spt = model.south_polar_heat_power_gw();
  double t_vent = model.hydrothermal_vent_temp_k();
  double v_gas = model.plume_gas_velocity_m_s();
  double mdot = model.plume_mass_loss_kg_s();
  double salinity = model.ocean_salinity_ppt();

  // Cassini CIRS & INMS observations (Spencer et al. 2006, Waite et al. 2017, Postberg et al. 2018)
  double obs_p_spt = 5.8;    // GW (CIRS south polar terrain endogenic flux)
  double obs_t_vent = 363.0; // K (90 deg C, serpentinization H2 production)
  double obs_mdot = 200.0;   // kg/s (Tiger stripe vent vapor output)
  double obs_sal = 15.0;     // ppt (CDA sodium-rich E-ring ice grains)

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "South Polar Terrain Heat Output    = " << p_spt << " GW (Observed: " << obs_p_spt << " GW)" << std::endl;
  std::cout << "Hydrothermal Vent Core Temp T_vent = " << t_vent << " K (Inferred: " << obs_t_vent << " K)" << std::endl;
  std::cout << "Plume Gas Exhaust Velocity v_gas   = " << v_gas << " m/s" << std::endl;
  std::cout << "Total Plume Mass Loss Rate mdot    = " << mdot << " kg/s (Observed: " << obs_mdot << " kg/s)" << std::endl;
  std::cout << "Global Ocean Salinity [ppt]        = " << salinity << " ppt (Observed: " << obs_sal << " ppt)" << std::endl;
  std::cout << "Relative Heat Power Discrepancy    = " << std::abs((p_spt - obs_p_spt) / obs_p_spt) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
