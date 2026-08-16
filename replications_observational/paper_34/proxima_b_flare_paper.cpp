// Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
// Observational Paper #34: Proxima Centauri b Stellar Flare Irradiation & Atmospheric Stripping

#include <iostream>
#include <iomanip>
#include <cmath>
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "   OBSERVATIONAL PAPER #34: PROXIMA CENTAURI b FLARE & HABITABILITY ANALYSIS" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::ProximaCentauribFlareHabitabilityModel model;
  double a_au = model.semimajor_axis_au();
  double flux_rel = model.stellar_flux_relative();
  double f_xuv = model.superflare_xuv_fluence_erg_cm2_s();
  double tau_loss = model.atmosphere_loss_timescale_myr();
  double t_eq = model.equilibrium_temp_k();

  // ESPRESSO & ALMA/MOST flare observations (Anglada-Escude et al. 2016, Howard et al. 2018)
  double obs_a = 0.0485;    // AU (11.2 day orbit)
  double obs_flux = 0.65;   // S_Earth (Habitable zone incident flux)
  double obs_f_xuv = 2.5e4; // erg/cm^2/s during megaflares (Howard et al. 2018 ApJ)
  double obs_teq = 234.0;   // K (Zero albedo equilibrium temp)

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "Semi-Major Axis a (Model)           = " << a_au << " AU (Observed: " << obs_a << " AU)" << std::endl;
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Relative Incident Stellar Flux      = " << flux_rel << " S_Sun (Observed: " << obs_flux << " S_Sun)" << std::endl;
  std::cout << std::scientific << std::setprecision(2);
  std::cout << "Superflare Peak XUV Fluence         = " << f_xuv << " erg/cm^2/s (Observed: " << obs_f_xuv << " erg/cm^2/s)" << std::endl;
  std::cout << std::fixed << std::setprecision(1);
  std::cout << "Atmospheric Stripping Timescale     = " << tau_loss << " Myr" << std::endl;
  std::cout << "Equilibrium Temperature T_eq        = " << t_eq << " K (Observed: " << obs_teq << " K)" << std::endl;
  std::cout << "Relative Flux Discrepancy           = " << std::abs((flux_rel - obs_flux) / obs_flux) * 100.0 << " %" << std::endl;
  std::cout << "================================================================================" << std::endl;

  return 0;
}
