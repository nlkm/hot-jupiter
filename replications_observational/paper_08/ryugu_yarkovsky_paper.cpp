// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #8: Asteroid (162173) Ryugu Yarkovsky Drift & Astrometry

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #8: ASTEROID (162173) RYUGU YARKOVSKY DRIFT ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::RyuguYarkovskyModel ryugu_model;
  double drift_m_yr = ryugu_model.yarkovsky_drift_m_yr();
  double drift_AU_Myr = ryugu_model.yarkovsky_drift_AU_Myr();

  double hayabusa2_obs_m_yr = -215.0; // m/yr (Watanabe et al. 2019, Sugita et al. 2019)
  double hayabusa2_obs_err = 15.0;

  std::cout << std::fixed << std::setprecision(3);
  std::cout << "Diurnal Yarkovsky Drift Rate (Model) = " << drift_m_yr << " m/yr (" << drift_AU_Myr << " AU/Myr)" << std::endl;
  std::cout << "Hayabusa2 & Astrometry Observed Rate = " << hayabusa2_obs_m_yr << " +/- " << hayabusa2_obs_err << " m/yr" << std::endl;
  std::cout << "Relative Model Agreement             = " << std::abs((drift_m_yr - hayabusa2_obs_m_yr) / hayabusa2_obs_m_yr) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
