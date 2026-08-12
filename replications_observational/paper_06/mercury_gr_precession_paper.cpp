// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #6: Mercury GR Pericenter Precession & Solar Oblateness J2

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #6: MERCURY GR PERICENTER PRECESSION & SOLAR J2" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::MercuryRelativisticPrecessionModel merc_model;
  double gr_rate = merc_model.gr_precession_arcsec_century();
  double j2_rate = merc_model.j2_sun_precession_arcsec_century();
  double total_non_newtonian = gr_rate + j2_rate;

  double messenger_obs_gr = 42.98; // arcsec/century (Park et al. 2017, Genova et al. 2019)
  double messenger_obs_err = 0.04;

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "GR Pericenter Precession Rate (Model) = " << gr_rate << " arcsec/century" << std::endl;
  std::cout << "Solar Oblateness J2 Contribution      = " << j2_rate << " arcsec/century" << std::endl;
  std::cout << "Total Relativistic + J2 Precession    = " << total_non_newtonian << " arcsec/century" << std::endl;
  std::cout << "MESSENGER Radio Science Observed       = " << messenger_obs_gr << " +/- " << messenger_obs_err << " arcsec/century" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
