// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #7: Asteroid (101955) Bennu Yarkovsky Drift & Astrometry

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #7: ASTEROID (101955) BENNU YARKOVSKY DRIFT ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::BennuYarkovskyModel bennu_model;
  double drift_m_yr = bennu_model.yarkovsky_drift_m_yr();
  double drift_AU_Myr = bennu_model.yarkovsky_drift_AU_Myr();

  double osiris_rex_obs_m_yr = -284.0; // m/yr (Farnocchia et al. 2013, Lauretta et al. 2019)
  double osiris_rex_obs_err = 1.5;

  std::cout << std::fixed << std::setprecision(3);
  std::cout << "Diurnal Yarkovsky Drift Rate (Model) = " << drift_m_yr << " m/yr (" << drift_AU_Myr << " AU/Myr)" << std::endl;
  std::cout << "OSIRIS-REx & Arecibo Observed Rate   = " << osiris_rex_obs_m_yr << " +/- " << osiris_rex_obs_err << " m/yr" << std::endl;
  std::cout << "Relative Model Agreement             = " << std::abs((drift_m_yr - osiris_rex_obs_m_yr) / osiris_rex_obs_m_yr) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
