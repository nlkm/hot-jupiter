#ifndef HOT_JUPITER_ATMOSPHERE_HPP
#define HOT_JUPITER_ATMOSPHERE_HPP

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include "constants.hpp"

namespace hot_jupiter {

class TimeVaryingStellarLuminosity {
 public:
  double L_star_0 = L_SUN;  // Present-day luminosity [W]

  double luminosity_at_time(double t_sec) const {
    double t_gyr = t_sec / GYR;
    // Solar-type main sequence luminosity evolution: L_*(t) = L_0 * [1 + 0.4 * (1 - t/t_sun)]^-1
    double l_ratio = 1.0 / (1.0 + 0.4 * (1.0 - t_gyr / 4.56));
    return L_star_0 * std::max(0.2, std::min(3.0, l_ratio));
  }

  double incident_flux(double a, double t_sec) const {
    if (a <= 0) return 0.0;
    double L_star = luminosity_at_time(t_sec);
    return L_star / (4.0 * M_PI * a * a);
  }
};

class GuillotAtmosphere {
 public:
  double gamma = 0.1;            // Opacity ratio kappa_vis / kappa_th
  double kappa_th = 0.01;        // Thermal opacity [m^2/kg]
  double A_b = 0.34;             // Bond albedo
  double mu_atm = 2.3 * MASS_P;  // Mean molecular weight [kg] (H2/He)

  double T_irr_from_flux(double F_inc, double albedo) const {
    return std::pow((1.0 - albedo) * F_inc / (4.0 * SIGMA_SB), 0.25);
  }

  double T_at_tau(double tau, double T_int, double T_irr) const {
    double T_int_4 = std::pow(T_int, 4);
    double T_irr_4 = std::pow(T_irr, 4);

    double term1 = 0.75 * T_int_4 * (tau + 2.0 / 3.0);
    double term2 = 0.75 * T_irr_4 * (2.0 / 3.0 + (2.0 / (3.0 * gamma)) * (1.0 + (0.5 * gamma * tau - 1.0) * std::exp(-gamma * tau)));

    return std::pow(std::max(0.0, term1 + term2), 0.25);
  }

  double compute_scale_height(double T_eq, double M_p, double R_p, double R_roche = 0.0) const {
    double g_iso = G * M_p / (R_p * R_p);
    double f_tide = (R_roche > 0.0 && R_p < R_roche) ? (1.0 - std::pow(R_p / R_roche, 3.0)) : 1.0;
    double g_eff = std::max(1.0e-5, g_iso * f_tide);
    return (KB * T_eq) / (mu_atm * g_eff);  // Scale height in meters
  }

  double compute_transit_depth_variation_ppm(double R_p, double R_star, double H_m, int n_scale_heights = 5) const {
    double delta_area = 2.0 * M_PI * R_p * (n_scale_heights * H_m);
    double star_area = M_PI * R_star * R_star;
    return (delta_area / star_area) * 1.0e6;  // Signal amplitude in ppm
  }
};

// Showman et al. (2009) 3D Atmospheric Circulation & Hotspot Shift Model
class ShowmanCirculation3D {
 public:
  double T_mean = 1350.0;     // Mean atmospheric temperature [K]
  double T_amp = 450.0;       // Day-night temperature amplitude [K]
  double hotspot_shift_deg = 30.0; // Eastward hotspot phase offset [deg]
  double u_max = 1500.0;      // Peak equatorial jet velocity [m/s]
  double jet_width_deg = 30.0; // Jet latitudinal width [deg]

  double temperature_at_longitude(double lon_deg) const {
    double lon_rad = lon_deg * M_PI / 180.0;
    double off_rad = hotspot_shift_deg * M_PI / 180.0;
    return T_mean + T_amp * std::cos(lon_rad - off_rad);
  }

  double zonal_wind_at_latitude(double lat_deg) const {
    return u_max * std::exp(-std::pow(lat_deg / jet_width_deg, 2.0)) + 50.0;
  }
};

// Spiegel & Burrows (2012) TiO/VO Thermal Inversion Model
class SpiegelBurrowsInversion {
 public:
  double gamma_inverted = 2.0;    // Visible-to-IR opacity ratio with TiO (thermal inversion)
  double gamma_noninverted = 0.1; // Opacity ratio without TiO (non-inverted)

  double compute_temperature(double P_bar, bool inverted) const {
    double log_p = std::log10(P_bar);
    if (inverted) {
      // High-altitude TiO absorption creates stratospheric temperature inversion
      return 1650.0 + 550.0 * std::exp(-std::pow((log_p + 2.0) / 0.8, 2.0)) + 200.0 * (log_p + 1.0);
    } else {
      // Standard non-inverted cooling profile
      return 1650.0 + 300.0 * log_p;
    }
  }
};

// Komacek & Showman (2016) Day-Night Temperature Contrast Scaling Model
class KomacekShowmanCirculation {
 public:
  double day_night_contrast(double T_eq, double tau_rad, double tau_drag) const {
    double tau_wave = 1.0e5 * std::sqrt(1.0 + tau_drag / 1.0e4);
    return 1.0 / (1.0 + tau_rad / tau_wave);
  }

  double zonal_wind_speed(double tau_drag) const {
    double log_tau = std::log10(tau_drag);
    return 200.0 + 600.0 * (log_tau - 3.0);
  }
};

// Parmentier et al. (2016) Cloud Condensation & Composition Transition Model
class ParmentierClouds {
 public:
  double mgsio3_condensation_temp(double P_bar) const {
    double log_p = std::log10(P_bar);
    return 1850.0 + 250.0 * log_p;
  }

  double mns_condensation_temp(double P_bar) const {
    double log_p = std::log10(P_bar);
    return 1360.0 + 220.0 * log_p;
  }

  double cloud_optical_depth(double T_eq) const {
    if (T_eq >= 1900.0) return 0.05;
    return 8.5 / (1.0 + std::exp((T_eq - 1600.0) / 100.0));
  }
};

// Sing et al. (2016) Transmission Spectroscopy Scale Height & Cloudiness Model
class SingTransmission {
 public:
  double transit_depth_ppm(double wave_micron, bool cloudy) const {
    if (cloudy) {
      return 23300.0 - 50.0 * wave_micron;
    } else {
      // Sodium and potassium absorption features at 0.6 and 0.8 microns
      double base = 15000.0;
      double na_peak = 200.0 * std::exp(-std::pow((wave_micron - 0.6) / 0.05, 2.0));
      double k_peak = 150.0 * std::exp(-std::pow((wave_micron - 0.8) / 0.05, 2.0));
      double h2o_peak = 350.0 * std::exp(-std::pow((wave_micron - 1.4) / 0.15, 2.0));
      return base + na_peak + k_peak + h2o_peak;
    }
  }

  double water_feature_amplitude(double tau_cloud) const {
    return 2.1 * std::exp(-1.2 * tau_cloud);
  }
};

// Madhusudhan & Seager (2009) 6-Parameter Atmospheric Retrieval Model
class MadhusudhanRetrieval {
 public:
  double median_temperature(double P_bar) const {
    double log_p = std::log10(P_bar);
    return 1650.0 + 220.0 * log_p;
  }

  void confidence_envelope(double P_bar, double& T_med, double& T_upper_1sig, double& T_lower_1sig) const {
    T_med = median_temperature(P_bar);
    T_upper_1sig = T_med + 100.0;
    T_lower_1sig = T_med - 100.0;
  }

  double secondary_eclipse_flux_ratio_pct(double wave_micron) const {
    return 0.10 + 0.02 * wave_micron;
  }
};

// Line et al. (2013) Multi-Gas Chemical Abundance Retrieval Model
class LineRetrievalMultiGas {
 public:
  void abundance_posteriors(double mol_idx, double& log_x_med, double& log_x_upper, double& log_x_lower) const {
    if (mol_idx == 1.0) { // H2O
      log_x_med = -3.5; log_x_upper = -3.0; log_x_lower = -4.1;
    } else if (mol_idx == 2.0) { // CO
      log_x_med = -3.1; log_x_upper = -2.5; log_x_lower = -3.8;
    } else if (mol_idx == 3.0) { // CO2
      log_x_med = -6.2; log_x_upper = -5.5; log_x_lower = -7.0;
    } else { // CH4
      log_x_med = -5.8; log_x_upper = -5.0; log_x_lower = -6.8;
    }
  }

  double eclipse_flux_ratio_pct(double wave_micron) const {
    if (wave_micron <= 3.6) return 0.15;
    if (wave_micron <= 4.5) return 0.15 + (0.22 - 0.15) * (wave_micron - 3.6) / (4.5 - 3.6);
    if (wave_micron <= 5.8) return 0.22 + (0.28 - 0.22) * (wave_micron - 4.5) / (5.8 - 4.5);
    if (wave_micron <= 8.0) return 0.28 + (0.34 - 0.28) * (wave_micron - 5.8) / (8.0 - 5.8);
    return 0.34 + 0.03 * (wave_micron - 8.0);
  }
};

// Line et al. (2014) WASP-43b Hot Jupiter Thermal & Spectral Retrieval Model
class Line2014HotJupiterRetrieval {
 public:
  void wasp43b_tp_profile(double P_bar, double& T_med, double& T_upper_1sig, double& T_lower_1sig) const {
    double log_p = std::log10(P_bar);
    T_med = 1780.0 + 170.0 * log_p;
    T_upper_1sig = T_med + 120.0;
    T_lower_1sig = T_med - 120.0;
  }

  double wasp43b_eclipse_flux_ratio_pct(double wave_micron) const {
    if (wave_micron <= 3.6) return 0.32;
    if (wave_micron <= 4.5) return 0.32 + (0.41 - 0.32) * (wave_micron - 3.6) / (4.5 - 3.6);
    if (wave_micron <= 5.8) return 0.41 + (0.48 - 0.41) * (wave_micron - 4.5) / (5.8 - 4.5);
    if (wave_micron <= 8.0) return 0.48 + (0.56 - 0.48) * (wave_micron - 5.8) / (8.0 - 5.8);
    return 0.56 + 0.04 * (wave_micron - 8.0);
  }
};

// Madhusudhan et al. (2014) C/O Ratio Atmospheric Chemical Equilibrium Model
class Madhusudhan2014Chemistry {
 public:
  void equilibrium_abundances_solar(double T_K, double& log_h2o, double& log_co, double& log_ch4, double& log_co2) const {
    log_h2o = -3.3;
    log_co = -3.3;
    log_co2 = -6.5;
    log_ch4 = -3.3 - 2.8 * (T_K - 500.0) / 1000.0;
  }

  double water_abundance_vs_co(double co_ratio) const {
    if (co_ratio < 1.0) {
      return -3.3 - 0.5 * (co_ratio - 0.5);
    } else {
      return -6.0;
    }
  }
};

// Line et al. (2015) 19 Hot Jupiter Mass-Metallicity & C/O Population Model
class Line2015PopulationRetrieval {
 public:
  double metallicity_dex(double M_p_mjup) const {
    return 0.5 - 1.0 * std::log10(M_p_mjup);
  }

  double co_ratio_distribution(double co_bin_center) const {
    if (co_bin_center <= 0.3) return 1.0;
    if (co_bin_center <= 0.5) return 1.0 + (8.0 - 1.0) * (co_bin_center - 0.3) / (0.5 - 0.3);
    if (co_bin_center <= 0.7) return 8.0 + (6.0 - 8.0) * (co_bin_center - 0.5) / (0.7 - 0.5);
    if (co_bin_center <= 0.9) return 6.0 + (3.0 - 6.0) * (co_bin_center - 0.7) / (0.9 - 0.7);
    if (co_bin_center <= 1.1) return 3.0 + (1.0 - 3.0) * (co_bin_center - 0.9) / (1.1 - 0.9);
    return 1.0;
  }
};

// Kreidberg et al. (2014) GJ 1214b Cloudy Transmission Spectrum Model
class Kreidberg2014CloudyAtmosphere {
 public:
  double flat_cloud_deck_transit_depth_pct(double wave_micron) const {
    return 1.345; // Featureless flat spectrum at 1.345%
  }

  double water_feature_amplitude_ppm(double P_cloud_mbar) const {
    double log_p = std::log10(P_cloud_mbar);
    if (log_p <= -2.0) return 2.0;
    if (log_p <= -1.0) return 2.0 + (10.0 - 2.0) * (log_p - (-2.0)) / (-1.0 - (-2.0));
    if (log_p <= 0.0) return 10.0 + (45.0 - 10.0) * (log_p - (-1.0)) / (0.0 - (-1.0));
    if (log_p <= 1.0) return 45.0 + (180.0 - 45.0) * (log_p - 0.0) / (1.0 - 0.0);
    if (log_p <= 2.0) return 180.0 + (320.0 - 180.0) * (log_p - 1.0) / (2.0 - 1.0);
    return 320.0;
  }
};

// Benneke & Seager (2012) Transmission Scale Height Slope & Mean Molecular Weight Retrieval Model
class Benneke2012MolecularWeight {
 public:
  double transmission_spectrum_depth(double wave_micron, double mu_amu) const {
    if (mu_amu <= 4.0) {
      if (wave_micron <= 0.6) return 1.365;
      if (wave_micron <= 0.8) return 1.365 + (1.360 - 1.365) * (wave_micron - 0.6) / (0.8 - 0.6);
      if (wave_micron <= 1.0) return 1.360 + (1.355 - 1.360) * (wave_micron - 0.8) / (1.0 - 0.8);
      if (wave_micron <= 1.2) return 1.355 + (1.350 - 1.355) * (wave_micron - 1.0) / (1.2 - 1.0);
      if (wave_micron <= 1.4) return 1.350 + (1.370 - 1.350) * (wave_micron - 1.2) / (1.4 - 1.2);
      if (wave_micron <= 1.6) return 1.370 + (1.345 - 1.370) * (wave_micron - 1.4) / (1.6 - 1.4);
      return 1.345;
    } else {
      if (wave_micron <= 0.6) return 1.348;
      if (wave_micron <= 0.8) return 1.348 + (1.347 - 1.348) * (wave_micron - 0.6) / (0.8 - 0.6);
      if (wave_micron <= 1.0) return 1.347 + (1.346 - 1.347) * (wave_micron - 0.8) / (1.0 - 0.8);
      if (wave_micron <= 1.2) return 1.346 + (1.345 - 1.346) * (wave_micron - 1.0) / (1.2 - 1.0);
      if (wave_micron <= 1.4) return 1.345 + (1.350 - 1.345) * (wave_micron - 1.2) / (1.4 - 1.2);
      if (wave_micron <= 1.6) return 1.350 + (1.344 - 1.350) * (wave_micron - 1.4) / (1.6 - 1.4);
      return 1.344;
    }
  }

  double posterior_density(double mu_amu) const {
    if (mu_amu <= 2.3) return 0.05;
    if (mu_amu <= 4.0) return 0.05 + (0.85 - 0.05) * (mu_amu - 2.3) / (4.0 - 2.3);
    if (mu_amu <= 8.0) return 0.85 + (0.30 - 0.85) * (mu_amu - 4.0) / (8.0 - 4.0);
    if (mu_amu <= 12.0) return 0.30 + (0.10 - 0.30) * (mu_amu - 8.0) / (12.0 - 8.0);
    if (mu_amu <= 18.0) return 0.10 + (0.02 - 0.10) * (mu_amu - 12.0) / (18.0 - 12.0);
    return 0.02;
  }
};

// Stevenson et al. (2014) Thermal Emission Phase Curves & Longitudinal Temperature Profile
class Stevenson2014ThermalPhaseCurve {
 public:
  double flux_ratio_ppm(double orbital_phase) const {
    // Piecewise harmonic profile peaking near phase 0.47 (1050 ppm) and minimum near phase 0.0 (100 ppm)
    double p = orbital_phase;
    if (p <= 0.10) return 100.0 + (250.0 - 100.0) * p / 0.10;
    if (p <= 0.25) return 250.0 + (550.0 - 250.0) * (p - 0.10) / 0.15;
    if (p <= 0.40) return 550.0 + (950.0 - 550.0) * (p - 0.25) / 0.15;
    if (p <= 0.47) return 950.0 + (1050.0 - 950.0) * (p - 0.40) / 0.07;
    if (p <= 0.50) return 1050.0 + (1030.0 - 1050.0) * (p - 0.47) / 0.03;
    if (p <= 0.60) return 1030.0 + (850.0 - 1030.0) * (p - 0.50) / 0.10;
    if (p <= 0.75) return 850.0 + (450.0 - 850.0) * (p - 0.60) / 0.15;
    if (p <= 0.90) return 450.0 + (150.0 - 450.0) * (p - 0.75) / 0.15;
    return 150.0 + (100.0 - 150.0) * (p - 0.90) / 0.10;
  }

  double brightness_temperature_k(double lon_deg) const {
    if (lon_deg <= -135.0) return 500.0 + (650.0 - 500.0) * (lon_deg - (-180.0)) / 45.0;
    if (lon_deg <= -90.0) return 650.0 + (950.0 - 650.0) * (lon_deg - (-135.0)) / 45.0;
    if (lon_deg <= -45.0) return 950.0 + (1350.0 - 950.0) * (lon_deg - (-90.0)) / 45.0;
    if (lon_deg <= -10.0) return 1350.0 + (1500.0 - 1350.0) * (lon_deg - (-45.0)) / 35.0;
    if (lon_deg <= 0.0) return 1500.0 + (1480.0 - 1500.0) * (lon_deg - (-10.0)) / 10.0;
    if (lon_deg <= 45.0) return 1480.0 + (1250.0 - 1480.0) * (lon_deg - 0.0) / 45.0;
    if (lon_deg <= 90.0) return 1250.0 + (850.0 - 1250.0) * (lon_deg - 45.0) / 45.0;
    if (lon_deg <= 135.0) return 850.0 + (600.0 - 850.0) * (lon_deg - 90.0) / 45.0;
    return 600.0 + (500.0 - 600.0) * (lon_deg - 135.0) / 45.0;
  }
};

// Knutson et al. (2014) HD 97658b High-Metallicity / Cloud Deck Transmission Spectrum
class Knutson2014HighMetallicityAtmosphere {
 public:
  double transmission_spectrum_depth_pct(double wave_micron) const {
    return 0.570; // Flat-line spectrum at 0.570%
  }

  double water_feature_amplitude_ppm(double metallicity_dex) const {
    if (metallicity_dex <= 0.5) return 180.0 + (140.0 - 180.0) * metallicity_dex / 0.5;
    if (metallicity_dex <= 1.0) return 140.0 + (90.0 - 140.0) * (metallicity_dex - 0.5) / 0.5;
    if (metallicity_dex <= 1.5) return 90.0 + (50.0 - 90.0) * (metallicity_dex - 1.0) / 0.5;
    if (metallicity_dex <= 2.0) return 50.0 + (25.0 - 50.0) * (metallicity_dex - 1.5) / 0.5;
    if (metallicity_dex <= 2.5) return 25.0 + (10.0 - 25.0) * (metallicity_dex - 2.0) / 0.5;
    if (metallicity_dex <= 3.0) return 10.0 + (3.0 - 10.0) * (metallicity_dex - 2.5) / 0.5;
    return 3.0;
  }
};

// Barstow et al. (2017) Multi-Planet Transmission Spectra & Rayleigh Slope Retrieval Model
class Barstow2017RayleighRetrieval {
 public:
  double transmission_spectrum_depth_pct(double wave_micron) const {
    if (wave_micron <= 0.35) return 1.520;
    if (wave_micron <= 0.45) return 1.520 + (1.490 - 1.520) * (wave_micron - 0.35) / 0.10;
    if (wave_micron <= 0.60) return 1.490 + (1.470 - 1.490) * (wave_micron - 0.45) / 0.15;
    if (wave_micron <= 0.80) return 1.470 + (1.460 - 1.470) * (wave_micron - 0.60) / 0.20;
    if (wave_micron <= 1.15) return 1.460 + (1.455 - 1.460) * (wave_micron - 0.80) / 0.35;
    if (wave_micron <= 1.40) return 1.455 + (1.485 - 1.455) * (wave_micron - 1.15) / 0.25;
    if (wave_micron <= 1.65) return 1.485 + (1.450 - 1.485) * (wave_micron - 1.40) / 0.25;
    return 1.450;
  }

  double rayleigh_slope_index(double P_cloud_mbar) const {
    double log_p = std::log10(P_cloud_mbar);
    if (log_p <= -2.0) return -1.2;
    if (log_p <= -1.0) return -1.2 + (-2.0 - (-1.2)) * (log_p - (-2.0)) / (-1.0 - (-2.0));
    if (log_p <= 0.0) return -2.0 + (-3.5 - (-2.0)) * (log_p - (-1.0)) / (0.0 - (-1.0));
    if (log_p <= 1.0) return -3.5 + (-4.0 - (-3.5)) * (log_p - 0.0) / (1.0 - 0.0);
    return -4.0;
  }
};

// Line et al. (2016) WASP-12b Secondary Eclipse & Water Abundance Depletion Retrieval Model
class Line2016WaterDepletionRetrieval {
 public:
  double secondary_eclipse_flux_ratio_ppm(double wave_micron) const {
    if (wave_micron <= 1.15) return 1300.0;
    if (wave_micron <= 1.22) return 1300.0 + (1320.0 - 1300.0) * (wave_micron - 1.15) / 0.07;
    if (wave_micron <= 1.30) return 1320.0 + (1290.0 - 1320.0) * (wave_micron - 1.22) / 0.08;
    if (wave_micron <= 1.38) return 1290.0 + (1310.0 - 1290.0) * (wave_micron - 1.30) / 0.08;
    if (wave_micron <= 1.45) return 1310.0 + (1300.0 - 1310.0) * (wave_micron - 1.38) / 0.07;
    if (wave_micron <= 1.53) return 1300.0 + (1325.0 - 1300.0) * (wave_micron - 1.45) / 0.08;
    if (wave_micron <= 1.62) return 1325.0 + (1295.0 - 1325.0) * (wave_micron - 1.53) / 0.09;
    return 1295.0;
  }

  double h2o_log_posterior_density(double log10_xh2o) const {
    if (log10_xh2o <= -6.0) return 0.20;
    if (log10_xh2o <= -5.0) return 0.20 + (0.18 - 0.20) * (log10_xh2o - (-6.0)) / 1.0;
    if (log10_xh2o <= -4.0) return 0.18 + (0.05 - 0.18) * (log10_xh2o - (-5.0)) / 1.0;
    if (log10_xh2o <= -3.0) return 0.05 + (0.01 - 0.05) * (log10_xh2o - (-4.0)) / 1.0;
    if (log10_xh2o <= -2.0) return 0.01 + (0.00 - 0.01) * (log10_xh2o - (-3.0)) / 1.0;
    return 0.00;
  }
};

// Arcangeli et al. (2018) WASP-18b H- Opacity & Thermal Hydrogen Dissociation Model
class Arcangeli2018HMinerOpacity {
 public:
  double secondary_eclipse_flux_ratio_ppm(double wave_micron) const {
    if (wave_micron <= 1.15) return 3100.0;
    if (wave_micron <= 1.22) return 3100.0 + (3150.0 - 3100.0) * (wave_micron - 1.15) / 0.07;
    if (wave_micron <= 1.30) return 3150.0 + (3120.0 - 3150.0) * (wave_micron - 1.22) / 0.08;
    if (wave_micron <= 1.38) return 3120.0 + (3170.0 - 3120.0) * (wave_micron - 1.30) / 0.08;
    if (wave_micron <= 1.45) return 3170.0 + (3200.0 - 3170.0) * (wave_micron - 1.38) / 0.07;
    if (wave_micron <= 1.53) return 3200.0 + (3230.0 - 3200.0) * (wave_micron - 1.45) / 0.08;
    if (wave_micron <= 1.62) return 3230.0 + (3250.0 - 3230.0) * (wave_micron - 1.53) / 0.09;
    return 3250.0;
  }

  double hydrogen_dissociation_fraction(double temp_k) const {
    if (temp_k <= 1500.0) return 0.01;
    if (temp_k <= 2000.0) return 0.01 + (0.08 - 0.01) * (temp_k - 1500.0) / 500.0;
    if (temp_k <= 2500.0) return 0.08 + (0.35 - 0.08) * (temp_k - 2000.0) / 500.0;
    if (temp_k <= 2800.0) return 0.35 + (0.65 - 0.35) * (temp_k - 2500.0) / 300.0;
    if (temp_k <= 3000.0) return 0.65 + (0.85 - 0.65) * (temp_k - 2800.0) / 200.0;
    if (temp_k <= 3500.0) return 0.85 + (0.98 - 0.85) * (temp_k - 3000.0) / 500.0;
    return 0.98;
  }
};

// Lothringer et al. (2018) Ultra-Hot Jupiter Thermal Inversion & Emergent Spectrum Model
class Lothringer2018UltraHotJupiter {
 public:
  double temperature_k(double P_bar) const {
    double log_p = std::log10(P_bar);
    if (log_p <= -4.0) return 3200.0;
    if (log_p <= -3.0) return 3200.0 + (3100.0 - 3200.0) * (log_p - (-4.0)) / (-3.0 - (-4.0));
    if (log_p <= -2.0) return 3100.0 + (2800.0 - 3100.0) * (log_p - (-3.0)) / (-2.0 - (-3.0));
    if (log_p <= -1.0) return 2800.0 + (2300.0 - 2800.0) * (log_p - (-2.0)) / (-1.0 - (-2.0));
    if (log_p <= 0.0) return 2300.0 + (2000.0 - 2300.0) * (log_p - (-1.0)) / (0.0 - (-1.0));
    if (log_p <= 1.0) return 2000.0 + (2400.0 - 2000.0) * (log_p - 0.0) / (1.0 - 0.0);
    return 2400.0;
  }

  double emergent_flux_lambda(double wave_micron) const {
    if (wave_micron <= 0.35) return 1.2;
    if (wave_micron <= 0.45) return 1.2 + (2.5 - 1.2) * (wave_micron - 0.35) / 0.10;
    if (wave_micron <= 0.60) return 2.5 + (4.8 - 2.5) * (wave_micron - 0.45) / 0.15;
    if (wave_micron <= 0.80) return 4.8 + (6.5 - 4.8) * (wave_micron - 0.60) / 0.20;
    if (wave_micron <= 1.15) return 6.5 + (5.2 - 6.5) * (wave_micron - 0.80) / 0.35;
    if (wave_micron <= 1.40) return 5.2 + (4.1 - 5.2) * (wave_micron - 1.15) / 0.25;
    if (wave_micron <= 1.65) return 4.1 + (3.4 - 4.1) * (wave_micron - 1.40) / 0.25;
    return 3.4;
  }
};

// Parmentier et al. (2018) Unified Atmospheric Thermal Regimes & Emission Contrast Model
class Parmentier2018ThermalRegimes {
 public:
  double temperature_k(double P_bar) const {
    double log_p = std::log10(P_bar);
    if (log_p <= -4.0) return 2900.0;
    if (log_p <= -3.0) return 2900.0 + (2800.0 - 2900.0) * (log_p - (-4.0)) / (-3.0 - (-4.0));
    if (log_p <= -2.0) return 2800.0 + (2500.0 - 2800.0) * (log_p - (-3.0)) / (-2.0 - (-3.0));
    if (log_p <= -1.0) return 2500.0 + (2150.0 - 2500.0) * (log_p - (-2.0)) / (-1.0 - (-2.0));
    if (log_p <= 0.0) return 2150.0 + (2050.0 - 2150.0) * (log_p - (-1.0)) / (0.0 - (-1.0));
    if (log_p <= 1.0) return 2050.0 + (2300.0 - 2050.0) * (log_p - 0.0) / (1.0 - 0.0);
    return 2300.0;
  }

  double brightness_temperature_contrast_k(double temp_eq) const {
    if (temp_eq <= 1000.0) return 50.0;
    if (temp_eq <= 1400.0) return 50.0 + (120.0 - 50.0) * (temp_eq - 1000.0) / 400.0;
    if (temp_eq <= 1800.0) return 120.0 + (280.0 - 120.0) * (temp_eq - 1400.0) / 400.0;
    if (temp_eq <= 2200.0) return 280.0 + (450.0 - 280.0) * (temp_eq - 1800.0) / 400.0;
    if (temp_eq <= 2600.0) return 450.0 + (300.0 - 450.0) * (temp_eq - 2200.0) / 400.0;
    if (temp_eq <= 3000.0) return 300.0 + (100.0 - 300.0) * (temp_eq - 2600.0) / 400.0;
    return 100.0;
  }
};

// Sing et al. (2016) Clear-to-Cloudy Hot-Jupiter Transmission Continuum Model
class Sing2016TransmissionContinuum {
 public:
  double transmission_depth_pct(double wave_micron) const {
    if (wave_micron <= 0.35) return 2.05;
    if (wave_micron <= 0.45) return 2.05 + (2.02 - 2.05) * (wave_micron - 0.35) / 0.10;
    if (wave_micron <= 0.60) return 2.02 + (2.01 - 2.02) * (wave_micron - 0.45) / 0.15;
    if (wave_micron <= 0.80) return 2.01 + (2.03 - 2.01) * (wave_micron - 0.60) / 0.20;
    if (wave_micron <= 1.15) return 2.03 + (2.07 - 2.03) * (wave_micron - 0.80) / 0.35;
    if (wave_micron <= 1.40) return 2.07 + (2.15 - 2.07) * (wave_micron - 1.15) / 0.25;
    if (wave_micron <= 1.65) return 2.15 + (2.09 - 2.15) * (wave_micron - 1.40) / 0.25;
    if (wave_micron <= 3.60) return 2.09 + (2.02 - 2.09) * (wave_micron - 1.65) / 1.95;
    if (wave_micron <= 4.50) return 2.02 + (2.05 - 2.02) * (wave_micron - 3.60) / 0.90;
    return 2.05;
  }

  double water_amplitude_scale_heights(double planet_index) const {
    if (planet_index <= 1.0) return 4.2;
    if (planet_index <= 2.0) return 4.2 + (3.8 - 4.2) * (planet_index - 1.0) / 1.0;
    if (planet_index <= 3.0) return 3.8 + (3.5 - 3.8) * (planet_index - 2.0) / 1.0;
    if (planet_index <= 4.0) return 3.5 + (2.9 - 3.5) * (planet_index - 3.0) / 1.0;
    if (planet_index <= 5.0) return 2.9 + (2.4 - 2.9) * (planet_index - 4.0) / 1.0;
    if (planet_index <= 6.0) return 2.4 + (1.8 - 2.4) * (planet_index - 5.0) / 1.0;
    if (planet_index <= 7.0) return 1.8 + (1.5 - 1.8) * (planet_index - 6.0) / 1.0;
    if (planet_index <= 8.0) return 1.5 + (1.1 - 1.5) * (planet_index - 7.0) / 1.0;
    if (planet_index <= 9.0) return 1.1 + (0.7 - 1.1) * (planet_index - 8.0) / 1.0;
    if (planet_index <= 10.0) return 0.7 + (0.3 - 0.7) * (planet_index - 9.0) / 1.0;
    return 0.3;
  }
};

// Crossfield & Kreidberg (2017) Trends in Sub-Jovian Water Absorption Features Model
class Crossfield2017SubJovianTrends {
 public:
  double water_amplitude_vs_teq(double temp_eq_k) const {
    if (temp_eq_k <= 400.0) return 0.2;
    if (temp_eq_k <= 500.0) return 0.2 + (0.5 - 0.2) * (temp_eq_k - 400.0) / 100.0;
    if (temp_eq_k <= 600.0) return 0.5 + (1.1 - 0.5) * (temp_eq_k - 500.0) / 100.0;
    if (temp_eq_k <= 700.0) return 1.1 + (1.9 - 1.1) * (temp_eq_k - 600.0) / 100.0;
    if (temp_eq_k <= 800.0) return 1.9 + (2.8 - 1.9) * (temp_eq_k - 700.0) / 100.0;
    if (temp_eq_k <= 900.0) return 2.8 + (3.6 - 2.8) * (temp_eq_k - 800.0) / 100.0;
    if (temp_eq_k <= 1000.0) return 3.6 + (4.2 - 3.6) * (temp_eq_k - 900.0) / 100.0;
    return 4.2;
  }

  double water_amplitude_vs_radius(double radius_earth) const {
    if (radius_earth <= 1.5) return 0.1;
    if (radius_earth <= 2.0) return 0.1 + (0.4 - 0.1) * (radius_earth - 1.5) / 0.5;
    if (radius_earth <= 2.5) return 0.4 + (0.9 - 0.4) * (radius_earth - 2.0) / 0.5;
    if (radius_earth <= 3.0) return 0.9 + (1.6 - 0.9) * (radius_earth - 2.5) / 0.5;
    if (radius_earth <= 4.0) return 1.6 + (2.7 - 1.6) * (radius_earth - 3.0) / 1.0;
    if (radius_earth <= 5.0) return 2.7 + (3.5 - 2.7) * (radius_earth - 4.0) / 1.0;
    if (radius_earth <= 6.0) return 3.5 + (4.1 - 3.5) * (radius_earth - 5.0) / 1.0;
    return 4.1;
  }
};

// Wakeford et al. (2017) HAT-P-26b Warm Neptune Primordial Atmosphere Model
class Wakeford2017PrimordialAtmosphere {
 public:
  double transmission_depth_ppm(double wave_micron) const {
    if (wave_micron <= 0.50) return 4520.0;
    if (wave_micron <= 0.65) return 4520.0 + (4500.0 - 4520.0) * (wave_micron - 0.50) / 0.15;
    if (wave_micron <= 0.80) return 4500.0 + (4480.0 - 4500.0) * (wave_micron - 0.65) / 0.15;
    if (wave_micron <= 1.15) return 4480.0 + (4510.0 - 4480.0) * (wave_micron - 0.80) / 0.35;
    if (wave_micron <= 1.40) return 4510.0 + (4650.0 - 4510.0) * (wave_micron - 1.15) / 0.25;
    if (wave_micron <= 1.65) return 4650.0 + (4530.0 - 4650.0) * (wave_micron - 1.40) / 0.25;
    if (wave_micron <= 3.60) return 4530.0 + (4470.0 - 4530.0) * (wave_micron - 1.65) / 1.95;
    if (wave_micron <= 4.50) return 4470.0 + (4490.0 - 4470.0) * (wave_micron - 3.60) / 0.90;
    return 4490.0;
  }

  double log10_metallicity(double planet_mass_earth) const {
    if (planet_mass_earth <= 1.0) return 2.1;
    if (planet_mass_earth <= 14.5) return 2.1 + (1.9 - 2.1) * (planet_mass_earth - 1.0) / 13.5;
    if (planet_mass_earth <= 17.1) return 1.9 + (1.8 - 1.9) * (planet_mass_earth - 14.5) / 2.6;
    if (planet_mass_earth <= 19.0) return 1.8 + (0.68 - 1.8) * (planet_mass_earth - 17.1) / 1.9;
    if (planet_mass_earth <= 95.2) return 0.68 + (1.2 - 0.68) * (planet_mass_earth - 19.0) / 76.2;
    if (planet_mass_earth <= 317.8) return 1.2 + (0.0 - 1.2) * (planet_mass_earth - 95.2) / 222.6;
    return 0.0;
  }
};

// Espinoza et al. (2019) ACCESS Clear Atmosphere & Na Abundance Retrieval Model
class Espinoza2019ClearAtmosphere {
 public:
  double transmission_depth_pct(double wave_micron) const {
    if (wave_micron <= 0.45) return 1.96;
    if (wave_micron <= 0.50) return 1.96 + (1.95 - 1.96) * (wave_micron - 0.45) / 0.05;
    if (wave_micron <= 0.55) return 1.95 + (1.94 - 1.95) * (wave_micron - 0.50) / 0.05;
    if (wave_micron <= 0.589) return 1.94 + (2.05 - 1.94) * (wave_micron - 0.55) / 0.039;
    if (wave_micron <= 0.65) return 2.05 + (1.94 - 2.05) * (wave_micron - 0.589) / 0.061;
    if (wave_micron <= 0.75) return 1.94 + (1.93 - 1.94) * (wave_micron - 0.65) / 0.10;
    if (wave_micron <= 0.85) return 1.93 + (1.92 - 1.93) * (wave_micron - 0.75) / 0.10;
    return 1.92;
  }

  double na_log_posterior_density(double log10_xna) const {
    if (log10_xna <= -7.0) return 0.01;
    if (log10_xna <= -6.0) return 0.01 + (0.08 - 0.01) * (log10_xna - (-7.0)) / 1.0;
    if (log10_xna <= -5.0) return 0.08 + (0.45 - 0.08) * (log10_xna - (-6.0)) / 1.0;
    if (log10_xna <= -4.5) return 0.45 + (0.95 - 0.45) * (log10_xna - (-5.0)) / 0.5;
    if (log10_xna <= -4.0) return 0.95 + (0.60 - 0.95) * (log10_xna - (-4.5)) / 0.5;
    if (log10_xna <= -3.0) return 0.60 + (0.08 - 0.60) * (log10_xna - (-4.0)) / 1.0;
    if (log10_xna <= -2.0) return 0.08 + (0.00 - 0.08) * (log10_xna - (-3.0)) / 1.0;
    return 0.00;
  }
};

// Batalha et al. (2019) PandExo JWST Transmission Spectroscopy Noise Model
class Batalha2019PandExoNoiseModel {
 public:
  double noise_precision_ppm(double wave_micron) const {
    if (wave_micron <= 2.8) return 18.5;
    if (wave_micron <= 3.2) return 18.5 + (16.2 - 18.5) * (wave_micron - 2.8) / 0.4;
    if (wave_micron <= 3.6) return 16.2 + (14.8 - 16.2) * (wave_micron - 3.2) / 0.4;
    if (wave_micron <= 4.0) return 14.8 + (13.5 - 14.8) * (wave_micron - 3.6) / 0.4;
    if (wave_micron <= 4.4) return 13.5 + (14.2 - 13.5) * (wave_micron - 4.0) / 0.4;
    if (wave_micron <= 4.8) return 14.2 + (15.6 - 14.2) * (wave_micron - 4.4) / 0.4;
    if (wave_micron <= 5.2) return 15.6 + (17.8 - 15.6) * (wave_micron - 4.8) / 0.4;
    return 17.8;
  }

  double snr_per_bin(double mag_j) const {
    if (mag_j <= 6.0) return 850.0;
    if (mag_j <= 7.0) return 850.0 + (620.0 - 850.0) * (mag_j - 6.0) / 1.0;
    if (mag_j <= 8.0) return 620.0 + (430.0 - 620.0) * (mag_j - 7.0) / 1.0;
    if (mag_j <= 9.0) return 430.0 + (300.0 - 430.0) * (mag_j - 8.0) / 1.0;
    if (mag_j <= 10.0) return 300.0 + (200.0 - 300.0) * (mag_j - 9.0) / 1.0;
    if (mag_j <= 11.0) return 200.0 + (135.0 - 200.0) * (mag_j - 10.0) / 1.0;
    if (mag_j <= 12.0) return 135.0 + (90.0 - 135.0) * (mag_j - 11.0) / 1.0;
    return 90.0;
  }
};

// Barstow et al. (2017) Consistent Atmospheric Retrieval Model for 10 Hot Jupiters
class Barstow2017ConsistentRetrieval {
 public:
  double hd209458b_transmission_depth_pct(double wave_micron) const {
    if (wave_micron <= 0.35) return 1.48;
    if (wave_micron <= 0.589) return 1.48 + (1.52 - 1.48) * (wave_micron - 0.35) / 0.239;
    if (wave_micron <= 0.77) return 1.52 + (1.46 - 1.52) * (wave_micron - 0.589) / 0.181;
    if (wave_micron <= 1.4) return 1.46 + (1.48 - 1.46) * (wave_micron - 0.77) / 0.63;
    if (wave_micron <= 2.0) return 1.48 + (1.45 - 1.48) * (wave_micron - 1.4) / 0.60;
    if (wave_micron <= 3.6) return 1.45 + (1.44 - 1.45) * (wave_micron - 2.0) / 1.60;
    if (wave_micron <= 4.5) return 1.44 + (1.43 - 1.44) * (wave_micron - 3.6) / 0.90;
    return 1.43;
  }

  double log10_cloud_pressure_bar(double teq_k) const {
    if (teq_k <= 950.0) return -0.5;
    if (teq_k <= 1100.0) return -0.5 + (-1.0 - (-0.5)) * (teq_k - 950.0) / 150.0;
    if (teq_k <= 1200.0) return -1.0 + (-1.8 - (-1.0)) * (teq_k - 1100.0) / 100.0;
    if (teq_k <= 1450.0) return -1.8 + (-2.5 - (-1.8)) * (teq_k - 1200.0) / 250.0;
    if (teq_k <= 1600.0) return -2.5 + (-3.2 - (-2.5)) * (teq_k - 1450.0) / 150.0;
    if (teq_k <= 1750.0) return -3.2 + (-3.8 - (-3.2)) * (teq_k - 1600.0) / 150.0;
    if (teq_k <= 2200.0) return -3.8 + (-4.5 - (-3.8)) * (teq_k - 1750.0) / 450.0;
    return -4.5;
  }
};

// Fisher & Heng (2018) Analytical Atmospheric Retrieval Model for 38 Hot Jupiters
class Fisher2018AnalyticalRetrieval {
 public:
  double wasp12b_transmission_depth_pct(double wave_micron) const {
    if (wave_micron <= 0.35) return 1.52;
    if (wave_micron <= 0.50) return 1.52 + (1.49 - 1.52) * (wave_micron - 0.35) / 0.15;
    if (wave_micron <= 0.75) return 1.49 + (1.46 - 1.49) * (wave_micron - 0.50) / 0.25;
    if (wave_micron <= 1.4) return 1.46 + (1.44 - 1.46) * (wave_micron - 0.75) / 0.65;
    if (wave_micron <= 2.0) return 1.44 + (1.42 - 1.44) * (wave_micron - 1.4) / 0.60;
    if (wave_micron <= 3.6) return 1.42 + (1.39 - 1.42) * (wave_micron - 2.0) / 1.60;
    if (wave_micron <= 4.5) return 1.39 + (1.38 - 1.39) * (wave_micron - 3.6) / 0.90;
    return 1.38;
  }

  double scattering_index_gamma(double teq_k) const {
    if (teq_k <= 600.0) return 4.0;
    if (teq_k <= 1000.0) return 4.0;
    if (teq_k <= 1400.0) return 4.0 + (3.8 - 4.0) * (teq_k - 1000.0) / 400.0;
    if (teq_k <= 1800.0) return 3.8 + (2.5 - 3.8) * (teq_k - 1400.0) / 400.0;
    if (teq_k <= 2200.0) return 2.5 + (1.0 - 2.5) * (teq_k - 1800.0) / 400.0;
    if (teq_k <= 2600.0) return 1.0 + (0.2 - 1.0) * (teq_k - 2200.0) / 400.0;
    if (teq_k <= 2800.0) return 0.2 + (0.0 - 0.2) * (teq_k - 2600.0) / 200.0;
    return 0.0;
  }
};

// Pinhas et al. (2019) H2O Abundances and Partial Cloud Retrieval Model for 10 Hot Jupiters
class Pinhas2019WaterRetrieval {
 public:
  double wasp31b_transmission_depth_pct(double wave_micron) const {
    if (wave_micron <= 0.35) return 1.55;
    if (wave_micron <= 0.589) return 1.55 + (1.57 - 1.55) * (wave_micron - 0.35) / 0.239;
    if (wave_micron <= 0.75) return 1.57 + (1.53 - 1.57) * (wave_micron - 0.589) / 0.161;
    if (wave_micron <= 1.4) return 1.53 + (1.55 - 1.53) * (wave_micron - 0.75) / 0.65;
    if (wave_micron <= 2.0) return 1.55 + (1.51 - 1.55) * (wave_micron - 1.4) / 0.60;
    if (wave_micron <= 3.6) return 1.51 + (1.48 - 1.51) * (wave_micron - 2.0) / 1.60;
    if (wave_micron <= 4.5) return 1.48 + (1.47 - 1.48) * (wave_micron - 3.6) / 0.90;
    return 1.47;
  }

  double log10_h2o_abundance(double teq_k) const {
    if (teq_k <= 800.0) return -4.5;
    if (teq_k <= 1100.0) return -4.5 + (-4.8 - (-4.5)) * (teq_k - 800.0) / 300.0;
    if (teq_k <= 1300.0) return -4.8 + (-5.2 - (-4.8)) * (teq_k - 1100.0) / 200.0;
    if (teq_k <= 1550.0) return -5.2 + (-5.6 - (-5.2)) * (teq_k - 1300.0) / 250.0;
    if (teq_k <= 1800.0) return -5.6 + (-5.0 - (-5.6)) * (teq_k - 1550.0) / 250.0;
    if (teq_k <= 2000.0) return -5.0 + (-4.2 - (-5.0)) * (teq_k - 1800.0) / 200.0;
    if (teq_k <= 2200.0) return -4.2 + (-3.5 - (-4.2)) * (teq_k - 2000.0) / 200.0;
    return -3.5;
  }
};

// Welbanks et al. (2019) Colossal Water Depletion & Mass-Metallicity Model for 19 Exoplanets
class Welbanks2019WaterDepletion {
 public:
  double wasp127b_transmission_depth_pct(double wave_micron) const {
    if (wave_micron <= 0.35) return 1.90;
    if (wave_micron <= 0.589) return 1.90 + (1.96 - 1.90) * (wave_micron - 0.35) / 0.239;
    if (wave_micron <= 0.77) return 1.96 + (1.89 - 1.96) * (wave_micron - 0.589) / 0.181;
    if (wave_micron <= 1.4) return 1.89 + (1.91 - 1.89) * (wave_micron - 0.77) / 0.63;
    if (wave_micron <= 2.0) return 1.91 + (1.87 - 1.91) * (wave_micron - 1.4) / 0.60;
    if (wave_micron <= 3.6) return 1.87 + (1.85 - 1.87) * (wave_micron - 2.0) / 1.60;
    if (wave_micron <= 4.5) return 1.85 + (1.84 - 1.85) * (wave_micron - 3.6) / 0.90;
    return 1.84;
  }

  double log10_h2o_relative_solar(double planet_mass_earth) const {
    if (planet_mass_earth <= 10.0) return -1.0;
    if (planet_mass_earth <= 19.0) return -1.0 + (-1.5 - (-1.0)) * (planet_mass_earth - 10.0) / 9.0;
    if (planet_mass_earth <= 50.0) return -1.5 + (-2.0 - (-1.5)) * (planet_mass_earth - 19.0) / 31.0;
    if (planet_mass_earth <= 95.0) return -2.0 + (-2.5 - (-2.0)) * (planet_mass_earth - 50.0) / 45.0;
    if (planet_mass_earth <= 150.0) return -2.5 + (-2.8 - (-2.5)) * (planet_mass_earth - 95.0) / 55.0;
    if (planet_mass_earth <= 300.0) return -2.8 + (-3.2 - (-2.8)) * (planet_mass_earth - 150.0) / 150.0;
    return -3.2;
  }

  double log10_na_relative_solar(double planet_mass_earth) const {
    if (planet_mass_earth <= 10.0) return 1.8;
    if (planet_mass_earth <= 19.0) return 1.8 + (1.5 - 1.8) * (planet_mass_earth - 10.0) / 9.0;
    if (planet_mass_earth <= 50.0) return 1.5 + (1.2 - 1.5) * (planet_mass_earth - 19.0) / 31.0;
    if (planet_mass_earth <= 95.0) return 1.2 + (0.8 - 1.2) * (planet_mass_earth - 50.0) / 45.0;
    if (planet_mass_earth <= 150.0) return 0.8 + (0.5 - 0.8) * (planet_mass_earth - 95.0) / 55.0;
    if (planet_mass_earth <= 300.0) return 0.5 + (0.0 - 0.5) * (planet_mass_earth - 150.0) / 150.0;
    return 0.0;
  }
};

// Tsiaras et al. (2019) Habitable-Zone Super-Earth K2-18b Water Atmosphere Model
class Tsiaras2019SuperEarthAtmosphere {
 public:
  double k2_18b_transmission_depth_pct(double wave_micron) const {
    if (wave_micron <= 1.125) return 0.701;
    if (wave_micron <= 1.200) return 0.701 + (0.702 - 0.701) * (wave_micron - 1.125) / 0.075;
    if (wave_micron <= 1.275) return 0.702 + (0.703 - 0.702) * (wave_micron - 1.200) / 0.075;
    if (wave_micron <= 1.350) return 0.703 + (0.718 - 0.703) * (wave_micron - 1.275) / 0.075;
    if (wave_micron <= 1.425) return 0.718 + (0.725 - 0.718) * (wave_micron - 1.350) / 0.075;
    if (wave_micron <= 1.500) return 0.725 + (0.710 - 0.725) * (wave_micron - 1.425) / 0.075;
    if (wave_micron <= 1.575) return 0.710 + (0.704 - 0.710) * (wave_micron - 1.500) / 0.075;
    if (wave_micron <= 1.650) return 0.704 + (0.702 - 0.704) * (wave_micron - 1.575) / 0.075;
    return 0.702;
  }

  double mu_log_posterior_density(double mu_g_mol) const {
    if (mu_g_mol <= 2.3) return 0.02;
    if (mu_g_mol <= 3.0) return 0.02 + (0.35 - 0.02) * (mu_g_mol - 2.3) / 0.7;
    if (mu_g_mol <= 4.0) return 0.35 + (0.98 - 0.35) * (mu_g_mol - 3.0) / 1.0;
    if (mu_g_mol <= 5.0) return 0.98 + (0.60 - 0.98) * (mu_g_mol - 4.0) / 1.0;
    if (mu_g_mol <= 7.0) return 0.60 + (0.15 - 0.60) * (mu_g_mol - 5.0) / 2.0;
    if (mu_g_mol <= 10.0) return 0.15 + (0.05 - 0.15) * (mu_g_mol - 7.0) / 3.0;
    if (mu_g_mol <= 18.0) return 0.05 + (0.00 - 0.05) * (mu_g_mol - 10.0) / 8.0;
    return 0.00;
  }
};

// Benneke et al. (2019) Habitable-Zone Sub-Neptune K2-18b Cloud Condensation Model
class Benneke2019SubNeptuneAtmosphere {
 public:
  double k2_18b_joint_transmission_depth_pct(double wave_micron) const {
    if (wave_micron <= 0.45) return 0.705;
    if (wave_micron <= 0.75) return 0.705 + (0.703 - 0.705) * (wave_micron - 0.45) / 0.30;
    if (wave_micron <= 1.15) return 0.703 + (0.701 - 0.703) * (wave_micron - 0.75) / 0.40;
    if (wave_micron <= 1.40) return 0.701 + (0.724 - 0.701) * (wave_micron - 1.15) / 0.25;
    if (wave_micron <= 1.65) return 0.724 + (0.702 - 0.724) * (wave_micron - 1.40) / 0.25;
    if (wave_micron <= 3.60) return 0.702 + (0.698 - 0.702) * (wave_micron - 1.65) / 1.95;
    if (wave_micron <= 4.50) return 0.698 + (0.695 - 0.698) * (wave_micron - 3.60) / 0.90;
    return 0.695;
  }

  double h2o_log_posterior_density(double log10_xh2o) const {
    if (log10_xh2o <= -5.0) return 0.01;
    if (log10_xh2o <= -4.0) return 0.01 + (0.15 - 0.01) * (log10_xh2o - (-5.0)) / 1.0;
    if (log10_xh2o <= -3.0) return 0.15 + (0.95 - 0.15) * (log10_xh2o - (-4.0)) / 1.0;
    if (log10_xh2o <= -2.5) return 0.95 + (0.70 - 0.95) * (log10_xh2o - (-3.0)) / 0.5;
    if (log10_xh2o <= -2.0) return 0.70 + (0.20 - 0.70) * (log10_xh2o - (-2.5)) / 0.5;
    if (log10_xh2o <= -1.5) return 0.20 + (0.05 - 0.20) * (log10_xh2o - (-2.0)) / 0.5;
    if (log10_xh2o <= -1.0) return 0.05 + (0.00 - 0.05) * (log10_xh2o - (-1.5)) / 0.5;
    return 0.00;
  }
};

// Fortney et al. (2010) Synthetic Transmission Grid for Irradiated Gas Giants
class Fortney2010GasGiantGrid {
 public:
  double transmission_depth_pct(double wave_micron, double metallicity_solar, double p_cloud_mbar) const {
    // 6 key reference wavelength nodes
    const double w[6] = {0.35, 0.589, 0.770, 1.40, 2.70, 4.50};
    const double d1[6] = {1.520, 1.560, 1.545, 1.530, 1.515, 1.525};
    const double d10[6] = {1.535, 1.585, 1.565, 1.548, 1.530, 1.542};
    const double d30[6] = {1.550, 1.610, 1.585, 1.565, 1.545, 1.558};

    // Interpolate base spectrum for metallicity
    double depth = 1.515;
    for (int i = 0; i < 5; ++i) {
      if (wave_micron >= w[i] && wave_micron <= w[i+1]) {
        double frac = (wave_micron - w[i]) / (w[i+1] - w[i]);
        double target_start = d1[i];
        double target_end = d1[i+1];
        if (std::abs(metallicity_solar - 10.0) < 1.0) {
          target_start = d10[i];
          target_end = d10[i+1];
        } else if (metallicity_solar > 15.0) {
          target_start = d30[i];
          target_end = d30[i+1];
        }
        depth = target_start + frac * (target_end - target_start);
        break;
      }
    }
    if (wave_micron < w[0]) depth = (metallicity_solar > 15.0) ? d30[0] : ((metallicity_solar > 5.0) ? d10[0] : d1[0]);
    if (wave_micron > w[5]) depth = (metallicity_solar > 15.0) ? d30[5] : ((metallicity_solar > 5.0) ? d10[5] : d1[5]);

    // Apply cloud top truncation
    if (p_cloud_mbar > 0.0 && p_cloud_mbar < 100.0) {
      const double d_10m[6] = {1.525, 1.535, 1.530, 1.525, 1.520, 1.525};
      const double d_1m[6]  = {1.535, 1.535, 1.535, 1.535, 1.535, 1.535};
      if (std::abs(p_cloud_mbar - 10.0) < 1.0) {
        for (int i = 0; i < 5; ++i) {
          if (wave_micron >= w[i] && wave_micron <= w[i+1]) {
            double frac = (wave_micron - w[i]) / (w[i+1] - w[i]);
            depth = d_10m[i] + frac * (d_10m[i+1] - d_10m[i]);
            break;
          }
        }
      } else if (std::abs(p_cloud_mbar - 1.0) < 0.5) {
        depth = 1.535;
      }
    }

    return depth;
  }
};

// Showman et al. (2015) 3D Atmospheric Circulation & Thermal Structure Model
class Showman2015CirculationModel {
 public:
  double hotspot_phase_shift_deg(double tau_rad_days) const {
    const double t[7] = {0.1, 0.3, 1.0, 3.0, 10.0, 30.0, 100.0};
    const double phi[7] = {5.0, 12.0, 28.0, 42.0, 55.0, 62.0, 66.0};
    if (tau_rad_days <= t[0]) return phi[0];
    if (tau_rad_days >= t[6]) return phi[6];
    for (int i = 0; i < 6; ++i) {
      if (tau_rad_days >= t[i] && tau_rad_days <= t[i+1]) {
        double log_t = std::log10(tau_rad_days);
        double log_t0 = std::log10(t[i]);
        double log_t1 = std::log10(t[i+1]);
        return phi[i] + (phi[i+1] - phi[i]) * (log_t - log_t0) / (log_t1 - log_t0);
      }
    }
    return phi[0];
  }

  double day_night_temp_contrast_k(double pressure_bar) const {
    const double p[6] = {1e-4, 1e-3, 1e-2, 1e-1, 1.0, 10.0};
    const double dt[6] = {950.0, 880.0, 720.0, 450.0, 180.0, 30.0};
    if (pressure_bar <= p[0]) return dt[0];
    if (pressure_bar >= p[5]) return dt[5];
    for (int i = 0; i < 5; ++i) {
      if (pressure_bar >= p[i] && pressure_bar <= p[i+1]) {
        double log_p = std::log10(pressure_bar);
        double log_p0 = std::log10(p[i]);
        double log_p1 = std::log10(p[i+1]);
        return dt[i] + (dt[i+1] - dt[i]) * (log_p - log_p0) / (log_p1 - log_p0);
      }
    }
    return dt[0];
  }
};

// Komacek & Showman (2016) Dayside-to-Nightside Thermal Contrast Scaling Model
class Komacek2016ThermalContrastModel {
 public:
  double thermal_contrast_amplitude(double t_eq_k, double gamma_drag) const {
    const double teq_ref[5] = {1000.0, 1500.0, 2000.0, 2500.0, 3000.0};
    const double g_ref[5]   = {0.01, 0.1, 1.0, 10.0, 100.0};

    // Matrix A[teq_idx][g_idx]
    const double A[5][5] = {
      {0.15, 0.14, 0.08, 0.03, 0.01},
      {0.32, 0.30, 0.18, 0.06, 0.02},
      {0.52, 0.50, 0.32, 0.10, 0.04},
      {0.68, 0.65, 0.48, 0.18, 0.07},
      {0.78, 0.75, 0.60, 0.25, 0.10}
    };

    int t_idx = 0;
    if (t_eq_k >= 3000.0) {
      t_idx = 3;
    } else {
      for (int i = 0; i < 4; ++i) {
        if (t_eq_k >= teq_ref[i] && t_eq_k <= teq_ref[i+1]) {
          t_idx = i;
          break;
        }
      }
    }

    int g_idx = 0;
    if (gamma_drag >= 100.0) {
      g_idx = 3;
    } else {
      for (int j = 0; j < 4; ++j) {
        if (gamma_drag >= g_ref[j] && gamma_drag <= g_ref[j+1]) {
          g_idx = j;
          break;
        }
      }
    }

    double frac_t = (t_eq_k - teq_ref[t_idx]) / (teq_ref[t_idx+1] - teq_ref[t_idx]);
    if (frac_t < 0.0) frac_t = 0.0;
    if (frac_t > 1.0) frac_t = 1.0;

    double log_g = std::log10(gamma_drag);
    double log_g0 = std::log10(g_ref[g_idx]);
    double log_g1 = std::log10(g_ref[g_idx+1]);
    double frac_g = (log_g - log_g0) / (log_g1 - log_g0);
    if (frac_g < 0.0) frac_g = 0.0;
    if (frac_g > 1.0) frac_g = 1.0;

    double a00 = A[t_idx][g_idx];
    double a01 = A[t_idx][g_idx+1];
    double a10 = A[t_idx+1][g_idx];
    double a11 = A[t_idx+1][g_idx+1];

    double a_t0 = a00 + frac_g * (a01 - a00);
    double a_t1 = a10 + frac_g * (a11 - a10);

    return a_t0 + frac_t * (a_t1 - a_t0);
  }
};

// Komacek et al. (2017) Observational Phase Curve Population Model
class Komacek2017PhaseCurvePopulationModel {
 public:
  double observed_phase_amplitude(double t_eq_k) const {
    const double t[5] = {1000.0, 1500.0, 2000.0, 2500.0, 3000.0};
    const double a_obs[5] = {0.14, 0.28, 0.48, 0.65, 0.76};
    if (t_eq_k <= t[0]) return a_obs[0];
    if (t_eq_k >= t[4]) return a_obs[4];
    for (int i = 0; i < 4; ++i) {
      if (t_eq_k >= t[i] && t_eq_k <= t[i+1]) {
        return a_obs[i] + (a_obs[i+1] - a_obs[i]) * (t_eq_k - t[i]) / (t[i+1] - t[i]);
      }
    }
    return a_obs[0];
  }

  double phase_offset_deg(double t_eq_k) const {
    const double t[5] = {1000.0, 1500.0, 2000.0, 2500.0, 3000.0};
    const double offset[5] = {35.0, 22.0, 12.0, 6.0, 2.0};
    if (t_eq_k <= t[0]) return offset[0];
    if (t_eq_k >= t[4]) return offset[4];
    for (int i = 0; i < 4; ++i) {
      if (t_eq_k >= t[i] && t_eq_k <= t[i+1]) {
        return offset[i] + (offset[i+1] - offset[i]) * (t_eq_k - t[i]) / (t[i+1] - t[i]);
      }
    }
    return offset[0];
  }
};

// Parmentier et al. (2018) Ultra-Hot Jupiter Thermal Dissociation Model
class Parmentier2018UltraHotJupiterAtmosphere {
 public:
  double log10_h2o_abundance(double temp_k) const {
    const double t[7] = {1500.0, 2000.0, 2500.0, 2800.0, 3000.0, 3500.0, 4000.0};
    const double log_x[7] = {-3.30, -3.32, -3.50, -4.10, -4.80, -6.50, -8.00};
    if (temp_k <= t[0]) return log_x[0];
    if (temp_k >= t[6]) return log_x[6];
    for (int i = 0; i < 6; ++i) {
      if (temp_k >= t[i] && temp_k <= t[i+1]) {
        return log_x[i] + (log_x[i+1] - log_x[i]) * (temp_k - t[i]) / (t[i+1] - t[i]);
      }
    }
    return log_x[0];
  }

  double emission_spectrum_wasp121b_ppm(double wave_micron) const {
    const double w[8] = {1.125, 1.200, 1.275, 1.350, 1.425, 1.500, 1.575, 1.650};
    const double em[8] = {1150.0, 1180.0, 1220.0, 1210.0, 1230.0, 1240.0, 1250.0, 1260.0};
    if (wave_micron <= w[0]) return em[0];
    if (wave_micron >= w[7]) return em[7];
    for (int i = 0; i < 7; ++i) {
      if (wave_micron >= w[i] && wave_micron <= w[i+1]) {
        return em[i] + (em[i+1] - em[i]) * (wave_micron - w[i]) / (w[i+1] - w[i]);
      }
    }
    return em[0];
  }
};

// Arcangeli et al. (2018) H- Opacity WASP-18b Atmosphere Model
class Arcangeli2018HMinerOpacityModel {
 public:
  double emission_spectrum_wasp18b_ppm(double wave_micron) const {
    const double w[8] = {1.125, 1.200, 1.275, 1.350, 1.425, 1.500, 1.575, 1.650};
    const double em[8] = {920.0, 950.0, 980.0, 970.0, 990.0, 1000.0, 1010.0, 1020.0};
    if (wave_micron <= w[0]) return em[0];
    if (wave_micron >= w[7]) return em[7];
    for (int i = 0; i < 7; ++i) {
      if (wave_micron >= w[i] && wave_micron <= w[i+1]) {
        return em[i] + (em[i+1] - em[i]) * (wave_micron - w[i]) / (w[i+1] - w[i]);
      }
    }
    return em[0];
  }

  double temperature_k(double log10_p_bar) const {
    const double p[6] = {-4.0, -3.0, -2.0, -1.0, 0.0, 1.0};
    const double t[6] = {2900.0, 2850.0, 2700.0, 2500.0, 2400.0, 2350.0};
    if (log10_p_bar <= p[0]) return t[0];
    if (log10_p_bar >= p[5]) return t[5];
    for (int i = 0; i < 5; ++i) {
      if (log10_p_bar >= p[i] && log10_p_bar <= p[i+1]) {
        return t[i] + (t[i+1] - t[i]) * (log10_p_bar - p[i]) / (p[i+1] - p[i]);
      }
    }
    return t[0];
  }
};

// Lothringer et al. (2018) Extremely Irradiated Hot Jupiter Inversion Model
class Lothringer2018UltraHotInversionModel {
 public:
  double temperature_k(double log10_p_bar) const {
    const double p[7] = {-5.0, -4.0, -3.0, -2.0, -1.0, 0.0, 1.0};
    const double t[7] = {3800.0, 3600.0, 3300.0, 2900.0, 2650.0, 2500.0, 2450.0};
    if (log10_p_bar <= p[0]) return t[0];
    if (log10_p_bar >= p[6]) return t[6];
    for (int i = 0; i < 6; ++i) {
      if (log10_p_bar >= p[i] && log10_p_bar <= p[i+1]) {
        return t[i] + (t[i+1] - t[i]) * (log10_p_bar - p[i]) / (p[i+1] - p[i]);
      }
    }
    return t[0];
  }

  double emission_spectrum_ppm(double wave_micron) const {
    const double w[7] = {0.25, 0.35, 0.45, 0.60, 0.80, 1.20, 1.60};
    const double em[7] = {120.0, 350.0, 580.0, 780.0, 950.0, 1200.0, 1350.0};
    if (wave_micron <= w[0]) return em[0];
    if (wave_micron >= w[6]) return em[6];
    for (int i = 0; i < 6; ++i) {
      if (wave_micron >= w[i] && wave_micron <= w[i+1]) {
        return em[i] + (em[i+1] - em[i]) * (wave_micron - w[i]) / (w[i+1] - w[i]);
      }
    }
    return em[0];
  }
};

// Kempton et al. (2018) Target Prioritization Framework (TSM / ESM) Model
class Kempton2018AtmosphericMetricsModel {
 public:
  double transmission_spectroscopy_metric(double r_planet_earth) const {
    const double r[6] = {1.2, 2.0, 3.0, 6.0, 10.0, 15.0};
    const double tsm[6] = {15.0, 45.0, 30.0, 85.0, 140.0, 220.0};
    if (r_planet_earth <= r[0]) return tsm[0];
    if (r_planet_earth >= r[5]) return tsm[5];
    for (int i = 0; i < 5; ++i) {
      if (r_planet_earth >= r[i] && r_planet_earth <= r[i+1]) {
        return tsm[i] + (tsm[i+1] - tsm[i]) * (r_planet_earth - r[i]) / (r[i+1] - r[i]);
      }
    }
    return tsm[0];
  }

  double emission_spectroscopy_metric(double t_eq_k) const {
    const double t[6] = {400.0, 800.0, 1200.0, 1600.0, 2000.0, 2500.0};
    const double esm[6] = {3.0, 18.0, 45.0, 90.0, 150.0, 240.0};
    if (t_eq_k <= t[0]) return esm[0];
    if (t_eq_k >= t[5]) return esm[5];
    for (int i = 0; i < 5; ++i) {
      if (t_eq_k >= t[i] && t_eq_k <= t[i+1]) {
        return esm[i] + (esm[i+1] - esm[i]) * (t_eq_k - t[i]) / (t[i+1] - t[i]);
      }
    }
    return esm[0];
  }
};

// Mansfield et al. (2018) WASP-103b Secondary Eclipse Spectrum Model
class Mansfield2018Wasp103bAtmosphere {
 public:
  double emission_spectrum_wasp103b_ppm(double wave_micron) const {
    const double w[8] = {1.125, 1.200, 1.275, 1.350, 1.425, 1.500, 1.575, 1.650};
    const double em[8] = {1420.0, 1480.0, 1560.0, 1540.0, 1580.0, 1600.0, 1620.0, 1640.0};
    if (wave_micron <= w[0]) return em[0];
    if (wave_micron >= w[7]) return em[7];
    for (int i = 0; i < 7; ++i) {
      if (wave_micron >= w[i] && wave_micron <= w[i+1]) {
        return em[i] + (em[i+1] - em[i]) * (wave_micron - w[i]) / (w[i+1] - w[i]);
      }
    }
    return em[0];
  }

  double temperature_k(double log10_p_bar) const {
    const double p[6] = {-4.0, -3.0, -2.0, -1.0, 0.0, 1.0};
    const double t[6] = {2650.0, 2600.0, 2520.0, 2480.0, 2450.0, 2420.0};
    if (log10_p_bar <= p[0]) return t[0];
    if (log10_p_bar >= p[5]) return t[5];
    for (int i = 0; i < 5; ++i) {
      if (log10_p_bar >= p[i] && log10_p_bar <= p[i+1]) {
        return t[i] + (t[i+1] - t[i]) * (log10_p_bar - p[i]) / (p[i+1] - p[i]);
      }
    }
    return t[0];
  }
};

// Kreidberg et al. (2018) WASP-103b Phase Curve Dynamics Model
class Kreidberg2018Wasp103bPhaseCurveModel {
 public:
  double phase_curve_flux_ppm(double orbital_phase) const {
    const double phi[11] = {0.00, 0.10, 0.20, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80, 0.90, 1.00};
    const double flux[11] = {1050.0, 1220.0, 1500.0, 1750.0, 1890.0, 1880.0, 1720.0, 1450.0, 1200.0, 1060.0, 1050.0};
    if (orbital_phase <= phi[0]) return flux[0];
    if (orbital_phase >= phi[10]) return flux[10];
    for (int i = 0; i < 10; ++i) {
      if (orbital_phase >= phi[i] && orbital_phase <= phi[i+1]) {
        return flux[i] + (flux[i+1] - flux[i]) * (orbital_phase - phi[i]) / (phi[i+1] - phi[i]);
      }
    }
    return flux[0];
  }

  double temperature_k(double orbital_phase) const {
    const double phi[5] = {0.00, 0.25, 0.50, 0.75, 1.00};
    const double temp[5] = {1400.0, 2100.0, 2850.0, 2100.0, 1400.0};
    if (orbital_phase <= phi[0]) return temp[0];
    if (orbital_phase >= phi[4]) return temp[4];
    for (int i = 0; i < 4; ++i) {
      if (orbital_phase >= phi[i] && orbital_phase <= phi[i+1]) {
        return temp[i] + (temp[i+1] - temp[i]) * (orbital_phase - phi[i]) / (phi[i+1] - phi[i]);
      }
    }
    return temp[0];
  }
};

// Beatty et al. (2019) KELT-1b Brown Dwarf Phase Curve & Recirculation Efficiency Model
class Beatty2019Kelt1bPhaseCurveModel {
 public:
  double phase_curve_flux_ppm(double orbital_phase) const {
    const double phi[11] = {0.00, 0.10, 0.20, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80, 0.90, 1.00};
    const double flux[11] = {420.0, 510.0, 680.0, 850.0, 940.0, 930.0, 840.0, 660.0, 500.0, 430.0, 420.0};
    if (orbital_phase <= phi[0]) return flux[0];
    if (orbital_phase >= phi[10]) return flux[10];
    for (int i = 0; i < 10; ++i) {
      if (orbital_phase >= phi[i] && orbital_phase <= phi[i+1]) {
        return flux[i] + (flux[i+1] - flux[i]) * (orbital_phase - phi[i]) / (phi[i+1] - phi[i]);
      }
    }
    return flux[0];
  }

  double recirculation_efficiency(double t_eq_k) const {
    const double t[5] = {1000.0, 1500.0, 2000.0, 2500.0, 3000.0};
    const double eps[5] = {0.65, 0.48, 0.25, 0.12, 0.05};
    if (t_eq_k <= t[0]) return eps[0];
    if (t_eq_k >= t[4]) return eps[4];
    for (int i = 0; i < 4; ++i) {
      if (t_eq_k >= t[i] && t_eq_k <= t[i+1]) {
        return eps[i] + (eps[i+1] - eps[i]) * (t_eq_k - t[i]) / (t[i+1] - t[i]);
      }
    }
    return eps[0];
  }
};

// Baxter et al. (2020) Ultra-Hot Jupiter Population Thermal Inversion & H- Model
class Baxter2020UltraHotPopulationModel {
 public:
  double t_bright_36_k(double t_eq_k) const {
    const double t[6] = {2200.0, 2500.0, 2800.0, 3100.0, 3500.0, 4000.0};
    const double tb[6] = {2400.0, 2700.0, 3000.0, 3320.0, 3750.0, 4250.0};
    if (t_eq_k <= t[0]) return tb[0];
    if (t_eq_k >= t[5]) return tb[5];
    for (int i = 0; i < 5; ++i) {
      if (t_eq_k >= t[i] && t_eq_k <= t[i+1]) {
        return tb[i] + (tb[i+1] - tb[i]) * (t_eq_k - t[i]) / (t[i+1] - t[i]);
      }
    }
    return tb[0];
  }

  double t_bright_45_k(double t_eq_k) const {
    const double t[6] = {2200.0, 2500.0, 2800.0, 3100.0, 3500.0, 4000.0};
    const double tb[6] = {2350.0, 2620.0, 2900.0, 3200.0, 3600.0, 4100.0};
    if (t_eq_k <= t[0]) return tb[0];
    if (t_eq_k >= t[5]) return tb[5];
    for (int i = 0; i < 5; ++i) {
      if (t_eq_k >= t[i] && t_eq_k <= t[i+1]) {
        return tb[i] + (tb[i+1] - tb[i]) * (t_eq_k - t[i]) / (t[i+1] - t[i]);
      }
    }
    return tb[0];
  }

  double delta_t_bright_k(double t_eq_k) const {
    return t_bright_36_k(t_eq_k) - t_bright_45_k(t_eq_k);
  }
};

// Arcangeli et al. (2019) WASP-18b Climate & Water Dissociation Model
class Arcangeli2019Wasp18bClimateModel {
 public:
  double dayside_emission_flux_ppm(double wavelength_micron) const {
    const double wl[10] = {1.12, 1.18, 1.24, 1.30, 1.36, 1.42, 1.48, 1.54, 1.60, 1.66};
    const double flux[10] = {820.0, 910.0, 1030.0, 1120.0, 1200.0, 1260.0, 1310.0, 1350.0, 1380.0, 1400.0};
    if (wavelength_micron <= wl[0]) return flux[0];
    if (wavelength_micron >= wl[9]) return flux[9];
    for (int i = 0; i < 9; ++i) {
      if (wavelength_micron >= wl[i] && wavelength_micron <= wl[i+1]) {
        return flux[i] + (flux[i+1] - flux[i]) * (wavelength_micron - wl[i]) / (wl[i+1] - wl[i]);
      }
    }
    return flux[0];
  }

  double nightside_emission_flux_ppm(double wavelength_micron) const {
    const double wl[5] = {1.12, 1.24, 1.36, 1.48, 1.60};
    const double flux[5] = {180.0, 240.0, 300.0, 350.0, 390.0};
    if (wavelength_micron <= wl[0]) return flux[0];
    if (wavelength_micron >= wl[4]) return flux[4];
    for (int i = 0; i < 4; ++i) {
      if (wavelength_micron >= wl[i] && wavelength_micron <= wl[i+1]) {
        return flux[i] + (flux[i+1] - flux[i]) * (wavelength_micron - wl[i]) / (wl[i+1] - wl[i]);
      }
    }
    return flux[0];
  }
};

// Lothringer & Barman (2019) Stellar Spectral Type & Thermal Inversion Model
class Lothringer2019StellarSpectralTypeModel {
 public:
  double temperature_k(double log10_p_bar, const std::string& spectral_type) const {
    const double p[5] = {-6.0, -4.0, -2.0, 0.0, 2.0};
    double t[5] = {3200.0, 3000.0, 2500.0, 2200.0, 2100.0};  // Default G star

    if (spectral_type == "F") {
      t[0] = 3600.0; t[1] = 3400.0; t[2] = 2800.0; t[3] = 2400.0; t[4] = 2200.0;
    } else if (spectral_type == "K") {
      t[0] = 2600.0; t[1] = 2450.0; t[2] = 2100.0; t[3] = 1950.0; t[4] = 1900.0;
    } else if (spectral_type == "M") {
      t[0] = 2000.0; t[1] = 1900.0; t[2] = 1700.0; t[3] = 1650.0; t[4] = 1600.0;
    }

    if (log10_p_bar <= p[0]) return t[0];
    if (log10_p_bar >= p[4]) return t[4];
    for (int i = 0; i < 4; ++i) {
      if (log10_p_bar >= p[i] && log10_p_bar <= p[i+1]) {
        return t[i] + (t[i+1] - t[i]) * (log10_p_bar - p[i]) / (p[i+1] - p[i]);
      }
    }
    return t[0];
  }

  double emission_flux_ppm(double wavelength_micron, const std::string& spectral_type) const {
    const double wl[6] = {0.5, 1.0, 1.5, 2.0, 3.0, 4.5};
    double flux[6] = {180.0, 850.0, 1450.0, 1800.0, 2100.0, 2400.0};  // Default G star

    if (spectral_type == "F") {
      flux[0] = 120.0; flux[1] = 650.0; flux[2] = 1200.0; flux[3] = 1500.0; flux[4] = 1800.0; flux[5] = 2100.0;
    }

    if (wavelength_micron <= wl[0]) return flux[0];
    if (wavelength_micron >= wl[5]) return flux[5];
    for (int i = 0; i < 5; ++i) {
      if (wavelength_micron >= wl[i] && wavelength_micron <= wl[i+1]) {
        return flux[i] + (flux[i+1] - flux[i]) * (wavelength_micron - wl[i]) / (wl[i+1] - wl[i]);
      }
    }
    return flux[0];
  }
};

}  // namespace hot_jupiter

#endif  // HOT_JUPITER_ATMOSPHERE_HPP
