// Copyright 2026 Antigravity Solar System Dynamics Replication Campaign
// First-principles replication of McCord et al. (1998, 1999)
// "Non-Ice Constituents on Europa's Surface"
// Galileo NIMS near-infrared reflectance spectroscopy, hydrated salt mineral identification,
// ocean brine freezing concentration, and vacuum sublimation lag mantle formation.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "constants.hpp"
#include "solar_system.hpp"

// Observational NIMS spectral calibration points for comparison
struct NIMSReferencePoint {
  double wavelength_um;
  double leading_plains_refl;
  double conamara_chaos_refl;
  double minos_linea_refl;
};

// Galileo NIMS observational datasets digitized from McCord et al. (1998, Fig. 2 & Fig. 3)
const std::vector<NIMSReferencePoint> kGalileoNimsData = {
    {0.80, 0.720, 0.380, 0.310},
    {0.90, 0.745, 0.388, 0.318},
    {1.00, 0.750, 0.385, 0.312},
    {1.04, 0.730, 0.375, 0.300},
    {1.10, 0.742, 0.380, 0.308},
    {1.20, 0.725, 0.365, 0.288},
    {1.25, 0.680, 0.345, 0.270},
    {1.30, 0.705, 0.355, 0.280},
    {1.40, 0.640, 0.330, 0.255},
    {1.48, 0.420, 0.280, 0.210},
    {1.50, 0.360, 0.270, 0.198},
    {1.53, 0.385, 0.265, 0.192},
    {1.60, 0.460, 0.285, 0.212},
    {1.65, 0.545, 0.300, 0.225},
    {1.70, 0.585, 0.320, 0.245},
    {1.80, 0.615, 0.350, 0.270},
    {1.90, 0.520, 0.310, 0.230},
    {1.98, 0.310, 0.240, 0.170},
    {2.02, 0.220, 0.210, 0.145},
    {2.08, 0.280, 0.190, 0.130},
    {2.15, 0.420, 0.230, 0.165},
    {2.20, 0.510, 0.275, 0.205},
    {2.30, 0.580, 0.325, 0.250},
    {2.40, 0.595, 0.330, 0.255},
    {2.50, 0.570, 0.315, 0.240}
};

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << " Paper #223: McCord et al. (1998) - Non-Ice Constituents on Europa's Surface   " << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::EuropaSaltHydrationModel model;

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "Europa Mass:                  " << hot_jupiter::EuropaSaltHydrationModel::M_EUROPA_KG << " kg" << std::endl;
  std::cout << "Europa Mean Radius:           " << hot_jupiter::EuropaSaltHydrationModel::R_EUROPA_M / 1000.0 << " km" << std::endl;
  std::cout << "Surface Gravity:              " << hot_jupiter::EuropaSaltHydrationModel::G_SURF << " m/s^2" << std::endl;
  std::cout << "Mean Surface Temperature:     " << hot_jupiter::EuropaSaltHydrationModel::T_SURF_MEAN_K << " K" << std::endl;
  std::cout << "Equatorial Peak Temperature:  " << hot_jupiter::EuropaSaltHydrationModel::T_EQUATOR_K << " K" << std::endl;
  std::cout << "MgSO4 Eutectic Temperature:   " << hot_jupiter::EuropaSaltHydrationModel::EUTECTIC_T_MGSO4_K << " K" << std::endl;
  std::cout << "MgSO4 Eutectic Salinity:      " << hot_jupiter::EuropaSaltHydrationModel::EUTECTIC_S_MGSO4_G_KG << " g/kg (28.2 wt%)" << std::endl;
  std::cout << "Na2SO4 Eutectic Temperature:  " << hot_jupiter::EuropaSaltHydrationModel::EUTECTIC_T_NA2SO4_K << " K" << std::endl;
  std::cout << "Na2SO4 Eutectic Salinity:     " << hot_jupiter::EuropaSaltHydrationModel::EUTECTIC_S_NA2SO4_G_KG << " g/kg (16.3 wt%)" << std::endl;

  // 1. Export High-Resolution Spectra Comparison
  std::ofstream out_spec("replications_ss/paper_223/nims_spectra_comparison.csv");
  out_spec << "wavelength_um,pure_ice,leading_plains_model,trailing_terrain_model,conamara_chaos_model,minos_linea_model,pure_hexahydrite,pure_epsomite,pure_mirabilite,pure_h2so4_hyd\n";

  for (double l = 0.80; l <= 2.505; l += 0.01) {
    double r_ice = model.bidirectional_reflectance(l, 0.0, 300.0, "hexahydrite");
    double r_lead = model.bidirectional_reflectance(l, 0.08, 250.0, "hexahydrite");
    double r_trail = model.bidirectional_reflectance(l, 0.55, 80.0, "hexahydrite");
    double r_conam = model.bidirectional_reflectance(l, 0.72, 60.0, "hexahydrite");
    double r_minos = model.bidirectional_reflectance(l, 0.85, 50.0, "hexahydrite");
    double r_hexa = model.bidirectional_reflectance(l, 1.0, 50.0, "hexahydrite");
    double r_epso = model.bidirectional_reflectance(l, 1.0, 50.0, "epsomite");
    double r_mira = model.bidirectional_reflectance(l, 1.0, 50.0, "mirabilite");
    double r_h2so4 = model.bidirectional_reflectance(l, 1.0, 50.0, "sulfuric_acid_hydrate");

    out_spec << std::fixed << std::setprecision(4)
             << l << "," << r_ice << "," << r_lead << "," << r_trail << ","
             << r_conam << "," << r_minos << "," << r_hexa << "," << r_epso << ","
             << r_mira << "," << r_h2so4 << "\n";
  }
  out_spec.close();
  std::cout << "✅ Wrote nims_spectra_comparison.csv" << std::endl;

  // 2. Export Observational Data & Compute R^2 Metrics
  std::ofstream out_nims("replications_ss/paper_223/galileo_nims_obs.csv");
  out_nims << "wavelength_um,leading_plains_obs,conamara_chaos_obs,minos_linea_obs,leading_model,conamara_model,minos_model\n";

  double ss_tot_lead = 0.0, ss_res_lead = 0.0, mean_lead = 0.0;
  double ss_tot_conam = 0.0, ss_res_conam = 0.0, mean_conam = 0.0;
  double ss_tot_minos = 0.0, ss_res_minos = 0.0, mean_minos = 0.0;
  int n_obs = kGalileoNimsData.size();

  for (const auto& pt : kGalileoNimsData) {
    mean_lead += pt.leading_plains_refl;
    mean_conam += pt.conamara_chaos_refl;
    mean_minos += pt.minos_linea_refl;
  }
  mean_lead /= n_obs;
  mean_conam /= n_obs;
  mean_minos /= n_obs;

  for (const auto& pt : kGalileoNimsData) {
    double m_lead = model.bidirectional_reflectance(pt.wavelength_um, 0.08, 250.0, "hexahydrite");
    double m_conam = model.bidirectional_reflectance(pt.wavelength_um, 0.72, 60.0, "hexahydrite");
    double m_minos = model.bidirectional_reflectance(pt.wavelength_um, 0.85, 50.0, "hexahydrite");

    ss_tot_lead += std::pow(pt.leading_plains_refl - mean_lead, 2.0);
    ss_res_lead += std::pow(pt.leading_plains_refl - m_lead, 2.0);

    ss_tot_conam += std::pow(pt.conamara_chaos_refl - mean_conam, 2.0);
    ss_res_conam += std::pow(pt.conamara_chaos_refl - m_conam, 2.0);

    ss_tot_minos += std::pow(pt.minos_linea_refl - mean_minos, 2.0);
    ss_res_minos += std::pow(pt.minos_linea_refl - m_minos, 2.0);

    out_nims << std::fixed << std::setprecision(4)
             << pt.wavelength_um << "," << pt.leading_plains_refl << ","
             << pt.conamara_chaos_refl << "," << pt.minos_linea_refl << ","
             << m_lead << "," << m_conam << "," << m_minos << "\n";
  }
  out_nims.close();
  std::cout << "✅ Wrote galileo_nims_obs.csv" << std::endl;

  double r2_lead = 1.0 - (ss_res_lead / ss_tot_lead);
  double r2_conam = 1.0 - (ss_res_conam / ss_tot_conam);
  double r2_minos = 1.0 - (ss_res_minos / ss_tot_minos);

  std::cout << "\n--- Quantitative Goodness-of-Fit (R^2) vs Galileo NIMS ---" << std::endl;
  std::cout << "Leading Plains Model R^2:     " << r2_lead << " (Target >= 0.98)" << std::endl;
  std::cout << "Conamara Chaos Model R^2:     " << r2_conam << " (Target >= 0.98)" << std::endl;
  std::cout << "Minos Linea Model R^2:        " << r2_minos << " (Target >= 0.98)" << std::endl;

  // 3. Export Ocean Brine Freezing & Fractional Crystallization
  std::ofstream out_brine("replications_ss/paper_223/brine_freezing_evolution.csv");
  out_brine << "temperature_k,salinity_s35_g_kg,liquid_frac_s35,salinity_s70_g_kg,liquid_frac_s70,salinity_s100_g_kg,liquid_frac_s100\n";

  for (double t = 273.15; t >= 248.0; t -= 0.5) {
    double s35 = model.brine_salinity_at_temperature(t, 35.0);
    double f35 = model.brine_liquid_fraction(t, 35.0);
    double s70 = model.brine_salinity_at_temperature(t, 70.0);
    double f70 = model.brine_liquid_fraction(t, 70.0);
    double s100 = model.brine_salinity_at_temperature(t, 100.0);
    double f100 = model.brine_liquid_fraction(t, 100.0);

    out_brine << std::fixed << std::setprecision(2)
              << t << "," << s35 << "," << f35 << ","
              << s70 << "," << f70 << ","
              << s100 << "," << f100 << "\n";
  }
  out_brine.close();
  std::cout << "✅ Wrote brine_freezing_evolution.csv" << std::endl;

  // 4. Export Sublimation Lag Formation Dynamics
  std::ofstream out_sub("replications_ss/paper_223/sublimation_lag_evolution.csv");
  out_sub << "log10_time_yr,salt_vol_frac_90K,salt_vol_frac_100K,salt_vol_frac_110K,salt_vol_frac_120K,salt_vol_frac_130K\n";

  for (double log_t = 0.0; log_t <= 7.0; log_t += 0.1) {
    double time_yr = std::pow(10.0, log_t);
    double vf_90 = model.salt_lag_volume_fraction(0.18, time_yr, 90.0);
    double vf_100 = model.salt_lag_volume_fraction(0.18, time_yr, 100.0);
    double vf_110 = model.salt_lag_volume_fraction(0.18, time_yr, 110.0);
    double vf_120 = model.salt_lag_volume_fraction(0.18, time_yr, 120.0);
    double vf_130 = model.salt_lag_volume_fraction(0.18, time_yr, 130.0);

    out_sub << std::fixed << std::setprecision(3)
            << log_t << "," << vf_90 << "," << vf_100 << ","
            << vf_110 << "," << vf_120 << "," << vf_130 << "\n";
  }
  out_sub.close();
  std::cout << "✅ Wrote sublimation_lag_evolution.csv" << std::endl;

  // 5. Export Spectral Metrics vs Salt Fraction
  std::ofstream out_metrics("replications_ss/paper_223/spectral_metrics_vs_salt_fraction.csv");
  out_metrics << "salt_fraction,band_depth_1_65um,band_center_2_0um,band_fwhm_1_5um,refl_1_0um,refl_1_8um\n";

  for (double fs = 0.0; fs <= 1.001; fs += 0.02) {
    double d165 = model.crystalline_band_depth_1_65um(fs, 100.0);
    double c20 = model.band_minimum_2_0um(fs, "hexahydrite");
    double fwhm15 = model.band_fwhm_1_5um(fs, "hexahydrite");
    double r10 = model.bidirectional_reflectance(1.00, fs, 100.0, "hexahydrite");
    double r18 = model.bidirectional_reflectance(1.80, fs, 100.0, "hexahydrite");

    out_metrics << std::fixed << std::setprecision(4)
                << fs << "," << d165 << "," << c20 << ","
                << fwhm15 << "," << r10 << "," << r18 << "\n";
  }
  out_metrics.close();
  std::cout << "✅ Wrote spectral_metrics_vs_salt_fraction.csv" << std::endl;

  std::cout << "\n✅ All calculations and CSV exports completed successfully." << std::endl;
  return 0;
}
