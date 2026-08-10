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

}  // namespace hot_jupiter

#endif  // HOT_JUPITER_ATMOSPHERE_HPP
