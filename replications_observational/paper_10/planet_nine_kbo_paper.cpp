// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #10: Planet Nine Secular Perturbations & eTNO Orbit Clustering

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #10: PLANET NINE SECULAR eTNO CLUSTERING ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::PlanetNineSecularModel p9_model;
  double angle_model = p9_model.secular_perihelion_clustering_deg();
  double period_model_Myr = p9_model.secular_precession_period_Myr();

  double mpc_obs_angle = 180.0; // deg (Anti-aligned longitude of perihelion offset; Batygin & Brown 2016)
  double mpc_obs_err = 15.0;

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Anti-Aligned Longitude of Perihelion Angle (Model) = " << angle_model << " deg" << std::endl;
  std::cout << "Minor Planet Center Observed eTNO Clustering       = " << mpc_obs_angle << " +/- " << mpc_obs_err << " deg" << std::endl;
  std::cout << "Secular Precession Period (Model)                  = " << period_model_Myr << " Myr" << std::endl;
  std::cout << "Relative Model Agreement                           = " << std::abs((angle_model - mpc_obs_angle) / mpc_obs_angle) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
