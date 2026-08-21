// Copyright 2026 Antigravity Observational Astrophysics & Solar System Campaign
// Observational Paper #90: Planet Nine Astrometric Motion & Clustering Driver

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #90: PLANET NINE POSITION & ASTROMETRIC MOTION" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::PlanetNinePositionPredictionEngine engine;

  const double ra_peak = engine.predicted_ra_deg();       // 55.55 deg (03h 42m)
  const double dec_peak = engine.predicted_dec_deg();     // +8.24 deg
  const double r_helio = engine.heliocentric_distance_au();// ~ 520 AU
  const double mu_pm = engine.proper_motion_arcsec_yr();   // ~ 0.18 arcsec/yr
  const double parallax = engine.annual_parallax_arcsec(); // ~ 0.71 arcsec

  std::cout << "Predicted Planet Nine RA: " << ra_peak << " deg" << std::endl;
  std::cout << "Predicted Planet Nine Dec: " << dec_peak << " deg" << std::endl;
  std::cout << "Heliocentric Distance: " << r_helio << " AU" << std::endl;
  std::cout << "Proper Motion Rate: " << mu_pm << " arcsec/yr" << std::endl;
  std::cout << "Annual Parallax: " << parallax << " arcsec" << std::endl;

  // Track Sky Motion and Track from Year 2000.0 to 2035.0 (linear time scale):
  std::ofstream out("replications_observational/paper_90/planet_nine_motion_evolution.csv");
  out << "epoch_year,ra_deg,dec_deg,annual_parallax_displacement_arcsec,linear_proper_motion_arcsec\n";

  for (double epoch = 2000.0; epoch <= 2035.0; epoch += 0.5) {
    double ra_val = engine.epoch_ra_deg(epoch);
    double dec_val = engine.epoch_dec_deg(epoch);
    
    // Annual parallax cycloid
    double frac_year = epoch - std::floor(epoch);
    double par_disp = parallax * std::sin(2.0 * M_PI * frac_year);
    double lin_pm = (epoch - 2010.5) * mu_pm;

    out << epoch << "," << ra_val << "," << dec_val << "," << par_disp << "," << lin_pm << "\n";
  }
  out.close();

  std::cout << "Generated Planet Nine Astrometric Simulation Data!" << std::endl;
  std::cout << "================================================================================" << std::endl;
  return 0;
}
