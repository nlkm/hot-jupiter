// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #35: Triton Retrograde Capture & Extreme Tidal Circularization

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #35: TRITON RETROGRADE CAPTURE & TIDAL DYNAMICS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::TritonRetrogradeCaptureModel model;
  double inc = model.retrograde_inclination_deg();
  double e0 = model.post_capture_eccentricity();
  double tau_circ = model.circularization_timescale_myr();
  double f_tide = model.peak_tidal_circularization_flux_w_m2();
  double a_now = model.present_orbital_radius_km();

  // Voyager 2 & ground-based astrometric observations (Agnor & Hamilton 2006, Goldreich et al. 1989)
  double obs_inc = 156.8;    // deg (Retrograde inclination to Neptune's equator)
  double obs_e0 = 0.99;      // High eccentricity post-exchange capture
  double obs_tau = 100.0;    // Myr tidal circularization timescale
  double obs_anow = 354760.0;// km (14.3 Neptune radii)

  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Retrograde Inclination i (Model)    = " << inc << " deg (Observed: " << obs_inc << " deg)" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Post-Capture Eccentricity e_0       = " << e0 << " (Theoretical: " << obs_e0 << ")" << std::endl;
  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Tidal Circularization Timescale     = " << tau_circ << " Myr (Inferred: " << obs_tau << " Myr)" << std::endl;
  std::cout << std::scientific << std::setprecision(2);
  std::cout << "Peak Tidal Circularization Flux     = " << f_tide << " W/m^2 (Global melting pulse!)" << std::endl;
  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Present Orbital Radius a_now        = " << a_now << " km (Observed: " << obs_anow << " km)" << std::endl;
  std::cout << "Relative Inclination Discrepancy    = " << std::abs((inc - obs_inc) / obs_inc) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
