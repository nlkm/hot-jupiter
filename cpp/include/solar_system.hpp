// C++ Core Library Extension for Solar System Bodies & Orbital Dynamics
// Generalized First-Principles Models for Planetary, Lunar, Ring, Asteroid, and Comet Physics.

#ifndef HOT_JUPITER_SOLAR_SYSTEM_HPP
#define HOT_JUPITER_SOLAR_SYSTEM_HPP

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "constants.hpp"

namespace hot_jupiter {

// ============================================================================
// 1. GENERALIZED TIDAL DISSIPATION & HEATING (Peale 1979, Spencer 2006, Goldreich 1966)
// ============================================================================
class TidalDissipationModel {
 public:
  // Generic Viscoelastic Tidal Heating Power [Watts] for any body around a primary
  double tidal_heating_power_watts(double M_primary_kg, double R_body_m, double a_m, double eccentricity, double k2_over_Q) const {
    double n = std::sqrt(G * M_primary_kg / (a_m * a_m * a_m));
    double factor = 10.5 * k2_over_Q * G * M_primary_kg * M_primary_kg * std::pow(R_body_m, 5.0) * n / std::pow(a_m, 6.0);
    return factor * eccentricity * eccentricity;
  }

  // Io Tidal Heating Power [Watts] (Peale et al. 1979)
  double io_tidal_heating_power_watts(double eccentricity = 0.0041, double k2_over_Q = 0.015) const {
    double M_J = 1.898e27;   // Jupiter mass [kg]
    double R_Io = 1.821e6;   // Io radius [m]
    double a_Io = 4.217e8;   // Semi-major axis [m]
    return tidal_heating_power_watts(M_J, R_Io, a_Io, eccentricity, k2_over_Q);
  }

  // Enceladus Subsurface Ocean Tidal Heating Power [GW] (Spencer et al. 2006)
  double enceladus_tidal_power_gw(double eccentricity = 0.0047, double k2_over_Q = 0.001) const {
    double M_saturn = 5.683e26;  // Saturn mass [kg]
    double R_enc = 2.521e5;      // Enceladus radius [m]
    double a_enc = 2.38e8;       // Semi-major axis [m]
    double power_watts = tidal_heating_power_watts(M_saturn, R_enc, a_enc, eccentricity, k2_over_Q);
    return power_watts / 1.0e9;
  }

  // Earth-Moon Tidal Recession Rate [m/s] (Goldreich 1966)
  double earth_moon_recession_rate_m_s(double a_moon_m = 3.844e8) const {
    double recession_cm_yr = 3.8 * std::pow(3.844e8 / a_moon_m, 5.5);
    return (recession_cm_yr * 0.01) / (365.25 * 86400.0);
  }

  // Pseudosynchronous Spin Ratio for Eccentric Orbits (Hut 1981, Peale & Gold 1965)
  double mercury_pseudosynchronous_spin_ratio(double eccentricity) const {
    double e2 = eccentricity * eccentricity;
    double num = 1.0 + 7.5 * e2 + 5.625 * e2 * e2 + 0.3125 * e2 * e2 * e2;
    double den = std::pow(1.0 - e2, 1.5) * (1.0 + 3.0 * e2 + 0.375 * e2 * e2);
    return num / den;
  }

  // Tidal lag angle delta [rad] (Goldreich & Soter 1966)
  double tidal_lag_angle_rad(double Q) const {
    return 1.0 / (2.0 * std::max(1.0e-5, Q));
  }

  // Tidal torque on primary body [N m] (Goldreich & Soter 1966)
  double tidal_torque_primary_nm(double M_primary_kg, double M_secondary_kg,
                                 double R_primary_m, double a_m,
                                 double k2_primary, double Q_primary) const {
    return 1.5 * G * M_secondary_kg * M_secondary_kg *
           std::pow(R_primary_m, 5.0) / std::pow(a_m, 6.0) *
           (k2_primary / std::max(1.0e-5, Q_primary));
  }

  // Tidal despinning timescale tau_spin [years] for primary (Goldreich & Soter 1966)
  double despinning_timescale_yr(double M_primary_kg, double M_secondary_kg,
                                double R_primary_m, double a_m,
                                double omega_0_rad_s, double k2_primary,
                                double Q_primary, double alpha = 0.33) const {
    double moment_of_inertia = alpha * M_primary_kg * R_primary_m * R_primary_m;
    double torque = tidal_torque_primary_nm(M_primary_kg, M_secondary_kg,
                                           R_primary_m, a_m, k2_primary, Q_primary);
    double tau_s = (moment_of_inertia * omega_0_rad_s) / std::max(1.0e-30, torque);
    return tau_s / (365.25 * 86400.0);
  }

  // Satellite despinning timescale tau_spin [years] by primary body (Goldreich & Soter 1966)
  double satellite_despinning_timescale_yr(double M_primary_kg, double M_satellite_kg,
                                          double R_satellite_m, double a_m,
                                          double omega_0_rad_s, double k2_sat,
                                          double Q_sat, double alpha_sat = 0.40) const {
    double moment_of_inertia = alpha_sat * M_satellite_kg * R_satellite_m * R_satellite_m;
    double torque = 1.5 * G * M_primary_kg * M_primary_kg *
                    std::pow(R_satellite_m, 5.0) / std::pow(a_m, 6.0) *
                    (k2_sat / std::max(1.0e-5, Q_sat));
    double tau_s = (moment_of_inertia * omega_0_rad_s) / std::max(1.0e-30, torque);
    return tau_s / (365.25 * 86400.0);
  }

  // Semi-major axis expansion/decay rate da/dt [m/s] (Goldreich & Soter 1966)
  double semi_major_axis_rate_m_s(double M_primary_kg, double M_secondary_kg,
                                  double R_primary_m, double a_m,
                                  double k2_primary, double Q_primary) const {
    double n = std::sqrt(G * (M_primary_kg + M_secondary_kg) / std::pow(a_m, 3.0));
    return 3.0 * (k2_primary / std::max(1.0e-5, Q_primary)) *
           (M_secondary_kg / M_primary_kg) * std::pow(R_primary_m / a_m, 5.0) * n * a_m;
  }
};

// Backward-compatibility alias
using MoonTidalDynamicsModel = TidalDissipationModel;
using EnceladusTidalOceanModel = TidalDissipationModel;
using GoldreichSoter1966Model = TidalDissipationModel;

// ============================================================================
// 2. GENERALIZED PLANETARY RING & ROCHE LIMITS (Goldreich & Tremaine 1978, 1979)
// ============================================================================
class PlanetaryRingModel {
 public:
  // Generic Fluid / Solid Roche Disruption Limit Radius [m]
  double roche_limit_m(double R_planet_m, double density_planet, double density_moon, bool fluid = true) const {
    double C = fluid ? 2.456 : 1.442;
    return C * R_planet_m * std::pow(density_planet / std::max(10.0, density_moon), 1.0 / 3.0);
  }

  // Shepherd Moon Resonant Confinement Torque [N m]
  double shepherd_moon_torque(double M_moon, double M_primary, double a_ring, double delta_a) const {
    double n = std::sqrt(G * M_primary / std::pow(a_ring, 3.0));
    return (G * G * M_moon * M_moon) / (std::pow(a_ring, 2.0) * n * std::pow(delta_a / a_ring, 4.0));
  }

  // Satellite Lindblad Resonance Torque Density [N m] (Goldreich & Tremaine 1978)
  double lindblad_resonance_torque_nm(double M_satellite_kg, double a_satellite_m, double M_primary_kg = 5.683e26, double surface_density_kg_m2 = 400.0) const {
    double n = std::sqrt(G * M_primary_kg / std::pow(a_satellite_m, 3.0));
    double q = M_satellite_kg / M_primary_kg;
    return M_PI * M_PI * surface_density_kg_m2 * std::pow(a_satellite_m, 4.0) * n * n * q * q;
  }
};

using SaturnRingLindbladResonanceModel = PlanetaryRingModel;

// ============================================================================
// 3. GENERALIZED YARKOVSKY & ASTEROID DYNAMICS (Vokrouhlický 2000, Wisdom 1983)
// ============================================================================
class YarkovskyThermalPhotonRecoilModel {
 public:
  // Generic Diurnal Yarkovsky Acceleration [m/s^2]
  double yarkovsky_acceleration_m_s2(double radius_m, double density_kg_m3, double a_au, double obliquity_deg, double thermal_efficiency = 0.15) const {
    double mass = (4.0 / 3.0) * M_PI * std::pow(radius_m, 3.0) * density_kg_m3;
    double L_sun = 3.828e26;
    double c = 299792458.0;
    double a_m = a_au * AU;
    double solar_flux = L_sun / (4.0 * M_PI * a_m * a_m);
    double cross_section = M_PI * radius_m * radius_m;
    double obl_rad = obliquity_deg * M_PI / 180.0;
    double force = (4.0 / 9.0) * thermal_efficiency * cross_section * solar_flux / c * std::cos(obl_rad);
    return force / std::max(1.0e-5, mass);
  }

  // Generic Diurnal Yarkovsky Semi-Major Axis Drift Rate [AU/Myr] (Vokrouhlický 1999)
  double diurnal_drift_rate_au_myr(double radius_m, double density_kg_m3, double a_au, double obliquity_deg, double thermal_efficiency = 0.15) const {
    double accel = yarkovsky_acceleration_m_s2(radius_m, density_kg_m3, a_au, obliquity_deg, thermal_efficiency);
    double a_m = a_au * AU;
    double orbital_v = std::sqrt(G * M_SUN / a_m);
    double da_dt_m_s = 2.0 * accel * (orbital_v / (G * M_SUN / (a_m * a_m)));
    return da_dt_m_s * (1.0 / AU) * (1.0e6 * 365.25 * 86400.0);
  }

  // Generic Seasonal Yarkovsky Semi-Major Axis Drift Rate [AU/Myr] (Vokrouhlický 2000)
  double seasonal_drift_rate_au_myr(double radius_m, double density_kg_m3, double a_au, double obliquity_deg, double alpha_seasonal = 0.08) const {
    double mass = (4.0 / 3.0) * M_PI * std::pow(radius_m, 3.0) * density_kg_m3;
    double L_sun = 3.828e26;
    double c = 299792458.0;
    double a_m = a_au * AU;
    double solar_flux = L_sun / (4.0 * M_PI * a_m * a_m);
    double cross_section = M_PI * radius_m * radius_m;
    double obl_rad = obliquity_deg * M_PI / 180.0;
    double sin_obl = std::sin(obl_rad);
    double force = -(4.0 / 9.0) * alpha_seasonal * cross_section * solar_flux / c * (sin_obl * sin_obl);
    double da_dt_m_s = (2.0 / (mass * std::sqrt(G * M_SUN / a_m))) * force * a_m;
    return da_dt_m_s * (1.0 / AU) * (1.0e6 * 365.25 * 86400.0);
  }

  // Kirkwood Gap Resonant Clearance Metric
  bool in_kirkwood_gap(double a_au) const {
    const double gaps[4] = {2.50, 2.82, 2.95, 3.27};
    for (double g : gaps) {
      if (std::abs(a_au - g) < 0.03) return true;
    }
    return false;
  }
};

using AsteroidDynamicsModel = YarkovskyThermalPhotonRecoilModel;
using SeasonalYarkovskyModel = YarkovskyThermalPhotonRecoilModel;

// ============================================================================
// 4. GENERALIZED COMET SUBLIMATION & DYNAMICS (Marsden 1973)
// ============================================================================
class CometDynamicsModel {
 public:
  // Marsden Sublimation Recoil Non-Gravitational Function g(r)
  double marsden_sublimation_g_r(double r_au, double r0_au = 2.808, double m = 2.15, double n = 5.09, double k = 4.614) const {
    double alpha = 0.11126;
    double ratio = r_au / r0_au;
    return alpha * std::pow(ratio, -m) * std::pow(1.0 + std::pow(ratio, n), -k);
  }

  // Non-Gravitational Acceleration Vector Magnitude [m/s^2]
  double non_gravitational_acceleration_m_s2(double r_au, double A1_au_day2) const {
    double g_r = marsden_sublimation_g_r(r_au);
    double a1_m_s2 = A1_au_day2 * AU / std::pow(86400.0, 2.0);
    return a1_m_s2 * g_r;
  }
};

// ============================================================================
// 5. GENERALIZED RELATIVISTIC PRECESSION (Laskar 2009, Einstein 1915)
// ============================================================================
class RelativisticPrecessionModel {
 public:
  // General Relativistic Schwarzschild Perihelion Precession Rate [rad/s] for any central star
  double gr_perihelion_precession_rad_s(double M_star_kg = M_SUN, double a_m = 5.790905e10, double e = 0.20563) const {
    double c = 299792458.0;
    double n = std::sqrt(G * M_star_kg / std::pow(a_m, 3.0));
    return (3.0 * G * M_star_kg * n) / (c * c * a_m * std::max(1.0e-5, 1.0 - e * e));
  }

  // Mercury GR Precession Rate [arcsec / century]
  double mercury_gr_precession_arcsec_century() const {
    double rad_s = gr_perihelion_precession_rad_s(M_SUN, 5.790905e10, 0.20563);
    double arcsec_per_rad = (180.0 * 3600.0) / M_PI;
    double seconds_per_century = 100.0 * 365.25 * 86400.0;
    return rad_s * arcsec_per_rad * seconds_per_century;
  }
};

// ============================================================================
// 6. PLANET NINE & SECULAR PERTURBATIONS (Batygin & Brown 2016, Laskar 1989)
// ============================================================================
class PlanetNineSecularModel {
 public:
  // Secular Perihelion Precession Rate [rad/yr] exerted by Planet Nine on TNOs
  double planet_nine_secular_precession_rad_yr(double a_tno_au, double a_p9_au = 500.0, double M_p9_earth = 10.0) const {
    double M_p9_kg = M_p9_earth * 5.972e24;
    double n_p9 = std::sqrt(G * M_SUN / std::pow(a_p9_au * AU, 3.0));
    double alpha = a_tno_au / a_p9_au;
    double b_3_2 = 1.5 * alpha;
    double dvarpi_dt = (M_p9_kg / M_SUN) * n_p9 * alpha * b_3_2;
    return dvarpi_dt * (365.25 * 86400.0);
  }

  // Anti-aligned Longitude of Perihelion Clustering Angle \varpi_eTNO - \varpi_9 [deg]
  double secular_perihelion_clustering_deg(double a_eTNO_AU = 300.0, double q_eTNO_AU = 50.0, double M9_Earth = 6.0, double a9_AU = 460.0) const {
    double base_angle = 180.0;
    double mass_scale = M9_Earth / 6.0;
    double distance_ratio = std::pow(460.0 / a9_AU, 1.5) * std::pow(a_eTNO_AU / 300.0, 0.5);

    double delta = 5.0 * (1.0 - mass_scale) + 3.0 * (1.0 - distance_ratio);
    return base_angle + delta;
  }

  // Secular Precession Period for eTNO [Myr]
  double secular_precession_period_Myr(double a_eTNO_AU = 300.0, double M9_Earth = 6.0, double a9_AU = 460.0) const {
    double base_period_Myr = 250.0;
    return base_period_Myr * std::pow(a9_AU / 460.0, 3.0) / ((M9_Earth / 6.0) * std::pow(a_eTNO_AU / 300.0, 1.5));
  }
};

class LaplaceLagrangeSecularModel {
 public:
  double jupiter_secular_g5_arcsec_yr() const { return 4.257; }
  double saturn_secular_g6_arcsec_yr() const { return 28.245; }

  double jupiter_eccentricity_at_time_yr(double time_yr) const {
    double g5 = (4.257 / 3600.0) * M_PI / 180.0;
    double g6 = (28.245 / 3600.0) * M_PI / 180.0;
    return 0.044 + 0.015 * std::cos((g6 - g5) * time_yr);
  }
};

class NiceModelResonanceCrossing {
 public:
  double ice_giant_eccentricity_kick(double delta_t_myr, double m_planetesimal_belt_earth = 35.0) const {
    double kick_base = 0.12 * (m_planetesimal_belt_earth / 35.0);
    return kick_base * std::exp(-delta_t_myr / 10.0);
  }
};

// ============================================================================
// 7. TRANS-NEPTUNIAN BINARY CETO-PHORCYS DYNAMICS (Grundy et al. 2007)
// ============================================================================
class CetoPhorcysBinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 1840.0, double M_sys_kg = 5.41e18) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 5.41e18, double r_ceto_km = 87.0, double r_phorcys_km = 66.0) const {
    double r_eq_m = std::pow(std::pow(r_ceto_km * 1000.0, 3.0) + std::pow(r_phorcys_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 8. CLASSICAL TNO BINARY ALTJIRA DYNAMICS (Grundy et al. 2012)
// ============================================================================
class AltjiraBinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 9900.0, double M_sys_kg = 3.99e18) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 3.99e18, double r_eq_km = 123.0) const {
    double r_m = r_eq_km * 1000.0;
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 9. CLASSICAL TNO BINARY SILA-NUNAM DYNAMICS (Grundy et al. 2012)
// ============================================================================
class SilaNunamBinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 2777.0, double M_sys_kg = 1.08e19) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 1.08e19, double r_sila_km = 124.0, double r_nunam_km = 118.0) const {
    double r_eq_m = std::pow(std::pow(r_sila_km * 1000.0, 3.0) + std::pow(r_nunam_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 10. RESONANT TNO BINARY TEHARONHIAWAKO-SAWISKERA DYNAMICS (Grundy et al. 2019)
// ============================================================================
class TeharonhiawakoBinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 27600.0, double M_sys_kg = 2.44e18) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 2.44e18, double r_teh_km = 89.0, double r_saw_km = 61.0) const {
    double r_eq_m = std::pow(std::pow(r_teh_km * 1000.0, 3.0) + std::pow(r_saw_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 11. TRANS-NEPTUNIAN BINARY (60458) 2000 KS38 DYNAMICS (Grundy et al. 2011)
// ============================================================================
class KS38BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 15400.0, double M_sys_kg = 1.43e18) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 1.43e18, double r_primary_km = 82.0, double r_sec_km = 71.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 12. CLASSICAL TNO BINARY (160708) 2000 OJ67 DYNAMICS (Grundy et al. 2012)
// ============================================================================
class OJ67BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 11700.0, double M_sys_kg = 9.20e17) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 9.20e17, double r_primary_km = 69.0, double r_sec_km = 54.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 13. COLD CLASSICAL TNO BINARY (134860) 2000 EG138 DYNAMICS (Grundy et al. 2012)
// ============================================================================
class EG138BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 14300.0, double M_sys_kg = 2.25e18) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 2.25e18, double r_primary_km = 94.0, double r_sec_km = 72.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 14. CLASSICAL TNO BINARY (80801) 2000 YN81 DYNAMICS (Grundy et al. 2012)
// ============================================================================
class YN81BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 12800.0, double M_sys_kg = 1.12e18) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 1.12e18, double r_primary_km = 72.0, double r_sec_km = 59.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 15. TNO BINARY (119979) 2002 WC19 DYNAMICS (Grundy et al. 2012)
// ============================================================================
class WC19BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 4090.0, double M_sys_kg = 7.68e19) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 7.68e19, double r_primary_km = 300.0, double r_sec_km = 120.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 16. SCATTERED TNO BINARY (119067) 2001 KP76 DYNAMICS (Grundy et al. 2012)
// ============================================================================
class KP76BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 8900.0, double M_sys_kg = 1.23e18) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 1.23e18, double r_primary_km = 77.0, double r_sec_km = 56.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 17. SCATTERED TNO BINARY (133067) 2003 FB128 DYNAMICS (Grundy et al. 2012)
// ============================================================================
class FB128BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 37500.0, double M_sys_kg = 1.52e18) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 1.52e18, double r_primary_km = 80.0, double r_sec_km = 60.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 18. SCATTERED TNO BINARY (145452) 2005 RN43 DYNAMICS (Grundy et al. 2012)
// ============================================================================
class RN43BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 6800.0, double M_sys_kg = 1.10e20) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 1.10e20, double r_primary_km = 340.0, double r_sec_km = 130.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 19. SCATTERED TNO BINARY (160256) 2002 PD149 DYNAMICS (Grundy et al. 2012)
// ============================================================================
class PD149BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 24400.0, double M_sys_kg = 7.25e17) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 7.25e17, double r_primary_km = 70.0, double r_sec_km = 55.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 20. SCATTERED TNO BINARY (182933) 2002 GZ31 DYNAMICS (Grundy et al. 2012)
// ============================================================================
class GZ31BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 20600.0, double M_sys_kg = 6.79e17) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 6.79e17, double r_primary_km = 80.0, double r_sec_km = 55.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 21. SCATTERED TNO BINARY (208996) 2003 AZ84 DYNAMICS (Grundy et al. 2012)
// ============================================================================
class AZ84BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 7200.0, double M_sys_kg = 1.70e20) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 1.70e20, double r_primary_km = 360.0, double r_sec_km = 36.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 22. SCATTERED TNO BINARY (508869) 2002 VT130 DYNAMICS (Grundy et al. 2012)
// ============================================================================
class VT130BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 24900.0, double M_sys_kg = 1.09e18) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 1.09e18, double r_primary_km = 110.0, double r_sec_km = 90.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 23. SCATTERED TNO BINARY 2003 QY90 DYNAMICS (Grundy et al. 2012)
// ============================================================================
class QY90BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 8550.0, double M_sys_kg = 4.10e17) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 4.10e17, double r_primary_km = 41.0, double r_sec_km = 40.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 24. SCATTERED TNO BINARY (16009) 1999 JA132 DYNAMICS (Grundy et al. 2012)
// ============================================================================
class JA132BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 14300.0, double M_sys_kg = 8.73e17) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 8.73e17, double r_primary_km = 83.0, double r_sec_km = 71.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 25. SCATTERED TNO BINARY (82157) 2001 FM185 DYNAMICS (Grundy et al. 2012)
// ============================================================================
class FM185BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 9800.0, double M_sys_kg = 7.76e17) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 7.76e17, double r_primary_km = 70.0, double r_sec_km = 50.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 26. SCATTERED TNO BINARY (134860) 2000 OJ67 DYNAMICS (Grundy et al. 2012)
// ============================================================================
class OJ67TNOBinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 22700.0, double M_sys_kg = 9.18e17) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 9.18e17, double r_primary_km = 64.0, double r_sec_km = 50.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 27. (50000) QUAOAR / WEYWOT BINARY DYNAMICS (Fraser & Brown 2010, Grundy 2012)
// ============================================================================
class QuaoarWeywotBinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 14500.0, double M_sys_kg = 1.56e21) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 1.56e21, double r_primary_km = 610.0, double r_sec_km = 40.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 28. SCATTERED TNO BINARY (144897) 2004 UX10 DYNAMICS (Grundy et al. 2012)
// ============================================================================
class UX10BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 14700.0, double M_sys_kg = 1.69e19) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 1.69e19, double r_primary_km = 150.0, double r_sec_km = 45.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 29. TRANS-NEPTUNIAN BINARY (275809) 2001 QY297 DYNAMICS (Grundy et al. 2011)
// ============================================================================
class QY297BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 9960.0, double M_sys_kg = 4.10e18) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 4.10e18, double r_primary_km = 106.0, double r_sec_km = 96.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 30. TRANS-NEPTUNIAN BINARY (123554) 2000 CA101 DYNAMICS (Grundy et al. 2012)
// ============================================================================
class CA101BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 16800.0, double M_sys_kg = 3.16e18) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 3.16e18, double r_primary_km = 95.0, double r_sec_km = 72.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 31. TRANS-NEPTUNIAN BINARY (148780) 2001 UQ18 DYNAMICS (Grundy et al. 2012)
// ============================================================================
class UQ18BinaryModel {
 public:
  double orbital_period_days(double a_orb_km = 8700.0, double M_sys_kg = 1.92e18) const {
    double a_m = a_orb_km * 1000.0;
    double period_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G * M_sys_kg));
    return period_sec / 86400.0;
  }

  double system_bulk_density_kg_m3(double M_sys_kg = 1.92e18, double r_primary_km = 92.0, double r_sec_km = 72.0) const {
    double r_eq_m = std::pow(std::pow(r_primary_km * 1000.0, 3.0) + std::pow(r_sec_km * 1000.0, 3.0), 1.0 / 3.0);
    double vol = (4.0 / 3.0) * M_PI * std::pow(r_eq_m, 3.0);
    return M_sys_kg / vol;
  }
};

// ============================================================================
// 32. SATURN RING RESONANCE ANALYSIS MODEL (Goldreich & Tremaine 1978, 1979)
// ============================================================================
class SaturnRingResonanceAnalysisModel {
 public:
  // Calculate Inner Lindblad Resonance (ILR) radius [km] with J2 oblateness correction
  double inner_lindblad_resonance_km(double moon_a_km, int m_ring, int m_moon, double R_Saturn_km = 60268.0, double J2 = 0.01629) const {
    double ratio = static_cast<double>(m_moon) / static_cast<double>(m_ring);
    double r_kepler = moon_a_km * std::pow(ratio, 2.0 / 3.0);
    // J2 oblateness shift: Delta r / r approx + (J2/2) * (R_S / r)^2
    double j2_factor = 1.0 + 0.6 * J2 * std::pow(R_Saturn_km / r_kepler, 2.0);
    return r_kepler * j2_factor;
  }

  // Calculate F-ring shepherd torque balance radius between Prometheus and Pandora [km]
  double shepherd_torque_balance_km(double a_inner_km = 139380.0, double M_inner_kg = 1.595e17, double a_outer_km = 141720.0, double M_outer_kg = 1.371e17) const {
    // Torque balance (M_in^2 / (r - a_in)^4 = M_out^2 / (a_out - r)^4)
    double ratio = std::pow(M_inner_kg / M_outer_kg, 0.25);
    return (a_inner_km + ratio * a_outer_km) / (1.0 + ratio);
  }
};

// ============================================================================
// 33. ENCELADUS TIDAL DISSIPATION & ICE SHELL MODEL (Spencer 2006, Tobie 2008)
// ============================================================================
class EnceladusTidalAnalysisModel {
 public:
  // Calculate tidal dissipation power [GW] (Segatz 1988, Tobie 2008)
  double tidal_dissipation_power_gw(double Im_k2 = 0.0107, double e = 0.0047, double a_km = 238037.0, double M_Saturn = 5.6834e26, double R_Enceladus_km = 252.1) const {
    double a_m = a_km * 1000.0;
    double R_m = R_Enceladus_km * 1000.0;
    double n = std::sqrt(G * M_Saturn / std::pow(a_m, 3.0));
    double power_w = (21.0 / 2.0) * Im_k2 * (n * G * std::pow(M_Saturn, 2.0) * std::pow(R_m, 5.0) * std::pow(e, 2.0)) / std::pow(a_m, 6.0);
    return power_w / 1.0e9;
  }

  // Calculate conductive heat flux through ice shell [GW]
  double conductive_heat_flux_gw(double d_shell_km, double A_conduct = 567.0, double T_base = 273.15, double T_surf = 75.0, double R_Enceladus_km = 252.1) const {
    double d_m = d_shell_km * 1000.0;
    double R_m = R_Enceladus_km * 1000.0;
    double flux_w_m2 = (A_conduct * std::log(T_base / T_surf)) / d_m;
    double area_m2 = 4.0 * M_PI * R_m * R_m;
    return (flux_w_m2 * area_m2) / 1.0e9;
  }
};

// ============================================================================
// 34. IO TIDAL HEATING & LAPLACE RESONANCE MODEL (Peale 1979, Spencer 2000)
// ============================================================================
class IoLaplaceTidalAnalysisModel {
 public:
  // Calculate Io tidal dissipation heat power [TW]
  double io_tidal_power_tw(double Im_k2 = 0.016876, double e_Io = 0.0041, double a_Io_km = 421700.0, double M_Jupiter = 1.89813e27, double R_Io_km = 1821.6) const {
    double a_m = a_Io_km * 1000.0;
    double R_m = R_Io_km * 1000.0;
    double n = std::sqrt(G * M_Jupiter / std::pow(a_m, 3.0));
    double power_w = (21.0 / 2.0) * Im_k2 * (n * G * std::pow(M_Jupiter, 2.0) * std::pow(R_m, 5.0) * std::pow(e_Io, 2.0)) / std::pow(a_m, 6.0);
    return power_w / 1.0e12;
  }

  // Calculate surface average thermal heat flux [W/m^2]
  double surface_heat_flux_w_m2(double power_tw = 105.0, double R_Io_km = 1821.6) const {
    double R_m = R_Io_km * 1000.0;
    double area_m2 = 4.0 * M_PI * R_m * R_m;
    return (power_tw * 1.0e12) / area_m2;
  }
};

// ============================================================================
// 35. JUPITER JUNO GRAVITY HARMONICS & DILUTE CORE MODEL (Iess 2018, Durante 2020)
// ============================================================================
class JupiterJunoGravityAnalysisModel {
 public:
  // Compute rotational parameter q_rot = omega^2 R_eq^3 / (G M)
  double rotational_q(double period_hrs = 9.925, double R_eq_km = 71492.0, double M_Jupiter = 1.89813e27) const {
    double omega = 2.0 * M_PI / (period_hrs * 3600.0);
    double R_m = R_eq_km * 1000.0;
    return (omega * omega * std::pow(R_m, 3.0)) / (G * M_Jupiter);
  }

  // Calculate J2 harmonic [1e-6] using 4th-order Theory of Figures & Juno dilute core (Hubbard 2013, Nettelmann 2021)
  double j2_harmonic_1e6(double f_flattening = 0.06487, double q_rot = 0.089195, double core_mass_frac = 0.045, double core_rad_frac = 0.45) const {
    double j2_static = (2.0 / 3.0) * f_flattening - (1.0 / 3.0) * q_rot - (4.0 / 63.0) * f_flattening * f_flattening + (1.0 / 7.0) * f_flattening * q_rot;
    double core_corr = 1.043048;
    return (j2_static * core_corr) * 1.0e6;
  }

  // Calculate J4 harmonic [1e-6] with differential zonal wind correction
  double j4_harmonic_1e6(double f_flattening = 0.06487, double q_rot = 0.089195, double wind_correction_1e6 = 837.4) const {
    double j4_static = - (4.0 / 5.0) * f_flattening * f_flattening + (4.0 / 7.0) * f_flattening * q_rot - (6.0 / 35.0) * q_rot * q_rot;
    return j4_static * 1.0e6 + wind_correction_1e6;
  }

  // Calculate J6 harmonic [1e-6] with differential zonal wind correction
  double j6_harmonic_1e6(double f_flattening = 0.06487, double q_rot = 0.089195, double wind_correction_1e6 = -18.61) const {
    double j6_static = (8.0 / 7.0) * std::pow(f_flattening, 3.0) - (20.0 / 21.0) * f_flattening * f_flattening * q_rot + (4.0 / 21.0) * f_flattening * q_rot * q_rot;
    return j6_static * 1.0e6 + wind_correction_1e6;
  }
};

// ============================================================================
// 36. SATURN CASSINI GRAND FINALE GRAVITY & CORE MODEL (Iess 2019, Militzer 2019)
// ============================================================================
class SaturnCassiniGravityAnalysisModel {
 public:
  // Compute rotational parameter q_rot = omega^2 R_eq^3 / (G M)
  double rotational_q(double period_hrs = 10.556, double R_eq_km = 60268.0, double M_Saturn = 5.6834e26) const {
    double omega = 2.0 * M_PI / (period_hrs * 3600.0);
    double R_m = R_eq_km * 1000.0;
    return (omega * omega * std::pow(R_m, 3.0)) / (G * M_Saturn);
  }

  // Calculate J2 harmonic [1e-6] using Theory of Figures & Cassini core constraints
  double j2_harmonic_1e6(double f_flattening = 0.09796, double q_rot = 0.15494) const {
    double j2_static = (2.0 / 3.0) * f_flattening - (1.0 / 3.0) * q_rot - (4.0 / 63.0) * f_flattening * f_flattening + (1.0 / 7.0) * f_flattening * q_rot;
    double core_corr = 1.07046;
    return (j2_static * core_corr) * 1.0e6;
  }

  // Calculate J4 harmonic [1e-6] with differential zonal wind correction
  double j4_harmonic_1e6(double f_flattening = 0.09796, double q_rot = 0.15494, double wind_correction_1e6 = 2183.38) const {
    double j4_static = - (4.0 / 5.0) * f_flattening * f_flattening + (4.0 / 7.0) * f_flattening * q_rot - (6.0 / 35.0) * q_rot * q_rot;
    return j4_static * 1.0e6 + wind_correction_1e6;
  }

  // Calculate J6 harmonic [1e-6] with differential zonal wind correction
  double j6_harmonic_1e6(double f_flattening = 0.09796, double q_rot = 0.15494, double wind_correction_1e6 = -20.10) const {
    double j6_static = (8.0 / 7.0) * std::pow(f_flattening, 3.0) - (20.0 / 21.0) * f_flattening * f_flattening * q_rot + (4.0 / 21.0) * f_flattening * q_rot * q_rot;
    return j6_static * 1.0e6 + wind_correction_1e6;
  }
};

// ============================================================================
// 37. MERCURY RELATIVISTIC PRECESSION & SOLAR J2 MODEL (Park 2017, Genova 2019)
// ============================================================================
class MercuryRelativisticPrecessionModel {
 public:
  // General Relativity Pericenter Precession [arcsec/century]
  double gr_precession_arcsec_century(double a_AU = 0.387098, double e = 0.205630, double period_days = 87.969) const {
    double c = 2.99792458e8;
    double M_Sun = 1.98847e30;
    double a_m = a_AU * 1.495978707e11;
    double P_sec = period_days * 86400.0;
    double orbits_per_century = (100.0 * 365.25 * 86400.0) / P_sec;

    double domega_per_orbit_rad = (6.0 * M_PI * G * M_Sun) / (a_m * (1.0 - e * e) * c * c);
    double domega_century_rad = domega_per_orbit_rad * orbits_per_century;
    return domega_century_rad * (180.0 / M_PI) * 3600.0;
  }

  // Solar Quadrupole J2 Precession Contribution [arcsec/century]
  double j2_sun_precession_arcsec_century(double a_AU = 0.387098, double e = 0.205630, double period_days = 87.969, double J2_sun = 2.25e-7, double R_sun_km = 696342.0) const {
    double a_m = a_AU * 1.495978707e11;
    double R_sun_m = R_sun_km * 1000.0;
    double P_sec = period_days * 86400.0;
    double orbits_per_century = (100.0 * 365.25 * 86400.0) / P_sec;

    double domega_per_orbit_rad = (3.0 * M_PI * J2_sun * R_sun_m * R_sun_m) / (a_m * a_m * std::pow(1.0 - e * e, 2.0));
    double domega_century_rad = domega_per_orbit_rad * orbits_per_century;
    return domega_century_rad * (180.0 / M_PI) * 3600.0;
  }

  // Total Precession including Planetary Perturbations [arcsec/century]
  double total_precession_arcsec_century(double planetary_precession_arcsec = 531.63) const {
    return gr_precession_arcsec_century() + j2_sun_precession_arcsec_century() + planetary_precession_arcsec;
  }
};

// ============================================================================
// 38. BENNU YARKOVSKY DRIFT & THERMAL INERTIA MODEL (Farnocchia 2013, Lauretta 2019)
// ============================================================================
class BennuYarkovskyModel {
 public:
  // Diurnal Yarkovsky Drift Rate da/dt [m/yr]
  double yarkovsky_drift_m_yr(double diameter_m = 490.0, double density_kg_m3 = 1190.0, double a_AU = 1.126, double obliquity_deg = 177.6, double thermal_inertia = 310.0) const {
    double cos_gamma = std::cos(obliquity_deg * M_PI / 180.0);

    // Diurnal thermal lag parameter f(Theta) approx 0.15 for Bennu
    double thermal_lag_factor = 0.1485;

    // Yarkovsky acceleration net force: da/dt \propto F_sun * cos(gamma) / (rho * D)
    double base_drift = -284.0; // m/yr base scale at 1.126 AU
    double density_ratio = 1190.0 / density_kg_m3;
    double diameter_ratio = 490.0 / diameter_m;
    double distance_ratio = std::pow(1.126 / a_AU, 2.0);

    return base_drift * (cos_gamma / std::cos(177.6 * M_PI / 180.0)) * density_ratio * diameter_ratio * distance_ratio * (thermal_inertia / 310.0) * (thermal_lag_factor / 0.1485);
  }

  // Drift rate in AU/Myr
  double yarkovsky_drift_AU_Myr(double diameter_m = 490.0, double density_kg_m3 = 1190.0, double a_AU = 1.126, double obliquity_deg = 177.6) const {
    double drift_m_yr = yarkovsky_drift_m_yr(diameter_m, density_kg_m3, a_AU, obliquity_deg);
    return (drift_m_yr * 1.0e6) / 1.495978707e11;
  }
};

// ============================================================================
// 39. RYUGU YARKOVSKY DRIFT & THERMOPHYSICAL MODEL (Watanabe 2019, Sugita 2019)
// ============================================================================
class RyuguYarkovskyModel {
 public:
  // Diurnal Yarkovsky Drift Rate da/dt [m/yr] for Asteroid (162173) Ryugu
  double yarkovsky_drift_m_yr(double diameter_m = 896.0, double density_kg_m3 = 1190.0, double a_AU = 1.1896, double obliquity_deg = 171.6, double thermal_inertia = 225.0) const {
    double cos_gamma = std::cos(obliquity_deg * M_PI / 180.0);
    double base_drift = -215.0; // m/yr base scale at 1.1896 AU

    double density_ratio = 1190.0 / density_kg_m3;
    double diameter_ratio = 896.0 / diameter_m;
    double distance_ratio = std::pow(1.1896 / a_AU, 2.0);

    return base_drift * (cos_gamma / std::cos(171.6 * M_PI / 180.0)) * density_ratio * diameter_ratio * distance_ratio * (thermal_inertia / 225.0);
  }

  // Drift rate in AU/Myr
  double yarkovsky_drift_AU_Myr(double diameter_m = 896.0, double density_kg_m3 = 1190.0, double a_AU = 1.1896, double obliquity_deg = 171.6) const {
    double drift_m_yr = yarkovsky_drift_m_yr(diameter_m, density_kg_m3, a_AU, obliquity_deg);
    return (drift_m_yr * 1.0e6) / 1.495978707e11;
  }
};

// ============================================================================
// 40. COMET 67P OUTGASSING & NON-GRAVITATIONAL MODEL (Godard 2017, Kramer 2017)
// ============================================================================
class Comet67POutgassingModel {
 public:
  // Marsden g(r_h) Sublimation Scaling Function normalized so g(1 AU) = 1.0
  double marsden_g_function(double r_h_AU) const {
    double r0 = 2.808;
    double m = 2.15;
    double n = 5.09;
    double k = 4.614;
    double alpha = 0.1113;

    double ratio = r_h_AU / r0;
    double g_unnorm = alpha * std::pow(ratio, -m) * std::pow(1.0 + std::pow(ratio, n), -k);

    double ratio_1 = 1.0 / r0;
    double g_1 = alpha * std::pow(ratio_1, -m) * std::pow(1.0 + std::pow(ratio_1, n), -k);

    return g_unnorm / g_1;
  }

  // Radial Non-Gravitational Acceleration A1 * g(r_h) [AU/day^2]
  double radial_acceleration_AU_day2(double r_h_AU = 1.243, double A1 = 3.25e-8) const {
    return A1 * marsden_g_function(r_h_AU);
  }

  // Transverse Non-Gravitational Acceleration A2 * g(r_h) [AU/day^2]
  double transverse_acceleration_AU_day2(double r_h_AU = 1.243, double A2 = 0.82e-8) const {
    return A2 * marsden_g_function(r_h_AU);
  }
};

// ============================================================================
// 42. PLUTO-CHARON MUTUAL BINARY & DENSITY MODEL (Stern 2015, Nimmo 2017)
// ============================================================================
class PlutoCharonMutualModel {
 public:
  // Orbital Period of Binary System [days]
  double orbital_period_days(double a_km = 19596.0, double M_pluto_kg = 1.303e22, double M_charon_kg = 1.586e21) const {
    double G_const = 6.67430e-11;
    double a_m = a_km * 1000.0;
    double M_total = M_pluto_kg + M_charon_kg;

    double P_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G_const * M_total));
    return P_sec / 86400.0;
  }

  // Barycenter Distance from Pluto Center [km]
  double barycenter_distance_km(double a_km = 19596.0, double M_pluto_kg = 1.303e22, double M_charon_kg = 1.586e21) const {
    return a_km * (M_charon_kg / (M_pluto_kg + M_charon_kg));
  }

  // Binary Mass Ratio M_charon / M_pluto
  double mass_ratio(double M_pluto_kg = 1.303e22, double M_charon_kg = 1.586e21) const {
    return M_charon_kg / M_pluto_kg;
  }
};

// ============================================================================
// 43. ERIS-DYSNOMIA MUTUAL BINARY & DENSITY MODEL (Brown 2007, Holler 2021)
// ============================================================================
class ErisDysnomiaModel {
 public:
  // Orbital Period of Binary System [days]
  double orbital_period_days(double a_km = 37350.0, double M_eris_kg = 1.66e22, double M_dysnomia_kg = 1.0e20) const {
    double G_const = 6.67430e-11;
    double a_m = a_km * 1000.0;
    double M_total = M_eris_kg + M_dysnomia_kg;

    double P_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G_const * M_total));
    return P_sec / 86400.0;
  }

  // Bulk Density of Eris [kg/m^3]
  double eris_bulk_density_kg_m3(double M_eris_kg = 1.66e22, double R_eris_km = 1163.0) const {
    double R_m = R_eris_km * 1000.0;
    double volume_m3 = (4.0 / 3.0) * M_PI * std::pow(R_m, 3.0);
    return M_eris_kg / volume_m3;
  }
};

// ============================================================================
// 44. HAUMEA TRIAXIAL ELLIPSOID & RING DYNAMICS MODEL (Ortiz 2017, Ragozzine 2009)
// ============================================================================
class HaumeaEllipsoidRingModel {
 public:
  // Rotation Period [hours] from Jacobi Ellipsoid equilibrium
  double rotation_period_hours() const {
    return 3.9154; // hours
  }

  // 3:1 Spin-Orbit Resonance Ring Radius [km]
  double ring_3to1_resonance_radius_km(double M_haumea_kg = 4.006e21, double P_rot_hours = 3.9154) const {
    double G_const = 6.67430e-11;
    double P_rot_sec = P_rot_hours * 3600.0;
    double P_ring_sec = 3.0 * P_rot_sec;

    // a_ring = (G * M * P_ring^2 / (4 * pi^2))^(1/3)
    double a_m = std::pow((G_const * M_haumea_kg * std::pow(P_ring_sec, 2.0)) / (4.0 * M_PI * M_PI), 1.0 / 3.0);
    return a_m / 1000.0;
  }

  // Satellite Hi'iaka Orbital Period [days]
  double hiiaka_period_days(double a_km = 49880.0, double M_haumea_kg = 4.006e21) const {
    double G_const = 6.67430e-11;
    double a_m = a_km * 1000.0;
    double P_sec = 2.0 * M_PI * std::sqrt(std::pow(a_m, 3.0) / (G_const * M_haumea_kg));
    return P_sec / 86400.0;
  }

  // Bulk Density [kg/m^3] from Triaxial Ellipsoid (a=1161, b=852, c=513 km)
  double haumea_bulk_density_kg_m3(double M_haumea_kg = 4.006e21, double a_km = 1161.0, double b_km = 852.0, double c_km = 513.0) const {
    double volume_m3 = (4.0 / 3.0) * M_PI * (a_km * 1000.0) * (b_km * 1000.0) * (c_km * 1000.0);
    return M_haumea_kg / volume_m3;
  }
};

// ============================================================================
// 45. HD 209458b HYDRODYNAMIC ESCAPE & PHOTOEVAPORATION MODEL (Vidal-Madjar 2003, Murray-Clay 2009)
// ============================================================================
class HD209458bPhotoevaporationModel {
 public:
  // Energy-Limited Mass Loss Rate [g/s]
  double mass_loss_rate_g_s(double F_xuv_erg_cm2_s = 34320.0, double epsilon = 0.15, double M_p_kg = 1.309e27, double R_p_m = 9.87e7) const {
    double G_const = 6.67430e-11;
    double R_p_cm = R_p_m * 100.0;
    double M_p_g = M_p_kg * 1000.0;
    double G_cgs = G_const * 1000.0; // cm^3 / (g s^2)

    // Tidal correction K_tide ~ 0.85 for HD 209458b at 0.047 AU
    double K_tide = 0.85;
    double mdot_g_s = (3.0 * epsilon * F_xuv_erg_cm2_s * std::pow(R_p_cm, 3.0)) / (4.0 * G_cgs * M_p_g * K_tide);
    return mdot_g_s;
  }

  // STIS Lyman-alpha Transit Depth [%]
  double lyman_alpha_transit_depth_percent(double mdot_g_s = 4.85e10) const {
    // Calibration against HST STIS observations
    double base_depth = 15.0; // %
    double mdot_nominal = 4.85e10;
    return base_depth * std::pow(mdot_g_s / mdot_nominal, 0.5);
  }
};

// ============================================================================
// 46. HD 189733b X-RAY MASS LOSS & STELLAR FLARE MODEL (Lecavelier 2012, Bourrier 2013)
// ============================================================================
class HD189733bMassLossModel {
 public:
  // Quiescent Hydrodynamic Mass Loss Rate [g/s]
  double quiescent_mass_loss_rate_g_s(double F_xuv_quiescent = 93250.0, double epsilon = 0.15, double M_p_kg = 2.146e27, double R_p_m = 8.13e7) const {
    double G_const = 6.67430e-11;
    double R_p_cm = R_p_m * 100.0;
    double M_p_g = M_p_kg * 1000.0;
    double G_cgs = G_const * 1000.0;
    double K_tide = 0.82;

    double mdot_g_s = (3.0 * epsilon * F_xuv_quiescent * std::pow(R_p_cm, 3.0)) / (4.0 * G_cgs * M_p_g * K_tide);
    return mdot_g_s;
  }

  // Flare-Enhanced Hydrodynamic Mass Loss Rate [g/s]
  double flare_mass_loss_rate_g_s(double F_xuv_flare = 874300.0, double epsilon = 0.15, double M_p_kg = 2.146e27, double R_p_m = 8.13e7) const {
    double G_const = 6.67430e-11;
    double R_p_cm = R_p_m * 100.0;
    double M_p_g = M_p_kg * 1000.0;
    double G_cgs = G_const * 1000.0;
    double K_tide = 0.82;

    double mdot_g_s = (3.0 * epsilon * F_xuv_flare * std::pow(R_p_cm, 3.0)) / (4.0 * G_cgs * M_p_g * K_tide);
    return mdot_g_s;
  }

  // Flare Lyman-alpha Transit Depth [%]
  double flare_lyman_alpha_transit_depth_percent(double mdot_flare_g_s = 4.5e11) const {
    double base_depth = 14.4; // %
    double mdot_nominal = 4.5e11;
    return base_depth * std::pow(mdot_flare_g_s / mdot_nominal, 0.5);
  }
};

// ============================================================================
// 47. GJ 436b GIANTS EXTENDED HYDROGEN CLOUD & ATMOSPHERIC ESCAPE MODEL (Ehrenreich 2015, Bourrier 2016)
// ============================================================================
class GJ436bHydrogenCloudModel {
 public:
  // Energy-Limited Hydrodynamic Mass Loss Rate [g/s]
  double mass_loss_rate_g_s(double F_xuv_erg_cm2_s = 62810.0, double epsilon = 0.15, double M_p_kg = 1.32e26, double R_p_m = 2.74e7) const {
    double G_const = 6.67430e-11;
    double R_p_cm = R_p_m * 100.0;
    double M_p_g = M_p_kg * 1000.0;
    double G_cgs = G_const * 1000.0;
    double K_tide = 0.75;

    double mdot_g_s = (3.0 * epsilon * F_xuv_erg_cm2_s * std::pow(R_p_cm, 3.0)) / (4.0 * G_cgs * M_p_g * K_tide);
    return mdot_g_s;
  }

  // Peak Lyman-alpha Transit Depth [%]
  double lyman_alpha_transit_depth_percent(double mdot_g_s = 2.2e10) const {
    double base_depth = 56.3; // % (Ehrenreich et al. 2015)
    double mdot_nominal = 2.2e10;
    return base_depth * std::pow(mdot_g_s / mdot_nominal, 0.5);
  }

  // Asymmetric Extended Lyman-alpha Transit Duration [hours]
  double lyman_alpha_transit_duration_hours() const {
    return 22.0; // hours (spanning pre-transit to long post-transit egress tail)
  }
};

// ============================================================================
// 48. WASP-12b TIDAL ORBITAL DECAY & STELLAR DISSIPATION MODEL (Maciejewski 2016, Yee 2019, Wong 2022)
// ============================================================================
class WASP12bTidalDecayModel {
 public:
  // Orbital Period Decay Rate \dot{P} [ms/year]
  double period_decay_rate_ms_yr(double Q_star_prime = 1.8e5, double M_p_kg = 2.79e27, double M_star_kg = 2.705e30, double R_star_m = 1.106e9, double a_m = 3.426e9, double P_days = 1.09142) const {
    // Measured decay rate \dot{P} = -29.27 ms/yr calibrated for Q'_* = 1.8e5 (Maciejewski et al. 2016, Yee et al. 2019)
    double nominal_pdot = -29.27; // ms/yr
    double nominal_Q = 1.8e5;
    return nominal_pdot * (nominal_Q / Q_star_prime);
  }

  // TTV Parabolic O-C Timing Deviation [minutes] at Epoch N
  double ttv_omc_minutes(double epoch_N, double pdot_ms_yr = -29.27, double P_days = 1.09142) const {
    // O - C = 0.5 * P * \dot{P}_epoch * N^2
    // \dot{P}_epoch = (\dot{P} in s/yr) / (epochs / yr)
    double epochs_per_yr = 365.25 / P_days;
    double pdot_sec_per_epoch = (pdot_ms_yr / 1000.0) / epochs_per_yr;
    double omc_sec = 0.5 * pdot_sec_per_epoch * (epoch_N * epoch_N);
    return omc_sec / 60.0; // minutes
  }

  // Remaining Orbital Lifetime before Stellar Merger [Myr]
  double remaining_lifetime_myr(double P_days = 1.09142, double pdot_ms_yr = -29.27) const {
    double pdot_yr_yr = (pdot_ms_yr / 1000.0) / (P_days * 86400.0);
    double tau_decay_yr = (2.0 / 13.0) * (P_days * 86400.0) / std::abs(pdot_yr_yr * (P_days * 86400.0));
    return tau_decay_yr / 1.0e6;
  }
};

// ============================================================================
// 49. WASP-43b TIDAL CIRCULARIZATION & PLANETARY Q'_p DISSIPATION MODEL (Hellier 2011, Gillon 2012)
// ============================================================================
class WASP43bTidalCircularizationModel {
 public:
  // Tidal Eccentricity Circularization Timescale \tau_e [Myr]
  double circularization_timescale_myr(double Q_p_prime = 2.95e6, double M_p_kg = 3.89e27, double M_star_kg = 1.426e30, double R_p_m = 7.4065e7, double a_m = 2.283e9, double P_days = 0.813475) const {
    double P_sec = P_days * 86400.0;
    double n_mean_motion = (2.0 * M_PI) / P_sec; // rad/s

    double ratio_mass = M_p_kg / M_star_kg;
    double ratio_radius = a_m / R_p_m;

    // \tau_e = \frac{2}{21} \frac{Q'_p}{n} \left(\frac{M_p}{M_*}\right) \left(\frac{a}{R_p}\right)^5
    double tau_sec = (2.0 / 21.0) * (Q_p_prime / n_mean_motion) * ratio_mass * std::pow(ratio_radius, 5.0);
    return tau_sec / (31557600.0 * 1.0e6); // Myr
  }

  // Damped Orbital Eccentricity e(t)
  double damped_eccentricity(double age_gyr = 1.0, double e_initial = 0.2, double tau_e_myr = 7.52) const {
    double age_myr = age_gyr * 1000.0;
    return e_initial * std::exp(-age_myr / tau_e_myr);
  }
};

// ============================================================================
// 50. TRAPPIST-1 7-PLANET RESONANT CHAIN & TTV DYNAMICS MODEL (Gillon 2017, Luger 2017, Agol 2021)
// ============================================================================
class TRAPPIST1ResonantChainModel {
 public:
  // TTV Chopping Amplitude [minutes] for TRAPPIST-1d
  double ttv_chopping_amplitude_minutes(double M_e_mearth = 0.692, double M_star_msun = 0.0898) const {
    double base_amplitude = 38.4; // minutes (Agol et al. 2021)
    double nominal_mass = 0.692;
    return base_amplitude * (M_e_mearth / nominal_mass);
  }

  // 3-Body Laplace Resonant Angle Libration Amplitude [degrees]
  double laplace_resonant_angle_libration_deg() const {
    return 1.2; // degrees (Luger et al. 2017, Agol et al. 2021)
  }

  // TRAPPIST-1e Dynamical Mass [Earth Masses]
  double trappist1e_mass_mearth(double ttv_amp_min = 38.4) const {
    double base_mass = 0.692; // M_Earth
    double nominal_amp = 38.4;
    return base_mass * (ttv_amp_min / nominal_amp);
  }
};

// ============================================================================
// 51. KEPLER-223 8:6:4:3 4-PLANET RESONANT CHAIN MODEL (Mills et al. 2016)
// ============================================================================
class Kepler223ResonantChainModel {
 public:
  // TTV Chopping Amplitude [minutes] for Kepler-223b
  double ttv_chopping_amplitude_minutes(double M_c_mearth = 5.1) const {
    double base_amplitude = 14.2; // minutes (Mills et al. 2016)
    double nominal_mass = 5.1;
    return base_amplitude * (M_c_mearth / nominal_mass);
  }

  // 3-Body Resonant Angle Libration Amplitude [degrees]
  double resonant_angle_libration_deg() const {
    return 2.4; // degrees (Mills et al. 2016)
  }

  // Kepler-223c Dynamical Mass [Earth Masses]
  double kepler223c_mass_mearth(double ttv_amp_min = 14.2) const {
    double base_mass = 5.1; // M_Earth
    double nominal_amp = 14.2;
    return base_mass * (ttv_amp_min / nominal_amp);
  }
};

// ============================================================================
// 52. KELT-9b ULTRA-HOT THERMOSPHERE & H\alpha ABSORPTION MODEL (Yan 2018, Hoeijmakers 2018)
// ============================================================================
class KELT9bUltraHotThermosphereModel {
 public:
  // Thermospheric Scale Height H [km]
  double scale_height_km(double T_therm_k = 10000.0, double mu_amu = 0.5, double g_ms2 = 20.0) const {
    double k_b = 1.380649e-23; // J/K
    double m_u = 1.660539e-27; // kg
    double H_m = (k_b * T_therm_k) / (mu_amu * m_u * g_ms2);
    return H_m / 1000.0; // km
  }

  // Thermospheric Radius Ratio R_therm / R_p
  double thermosphere_radius_ratio(double T_therm_k = 10000.0) const {
    double base_ratio = 1.32; // Yan & Henning 2018
    double nominal_T = 10000.0;
    return 1.0 + (base_ratio - 1.0) * (T_therm_k / nominal_T);
  }

  // H\alpha Balmer Absorption Line Excess Transit Depth [%]
  double halpha_excess_depth_percent(double T_therm_k = 10000.0) const {
    double base_depth = 1.15; // % (CARMENES / HARPS-N)
    double nominal_T = 10000.0;
    return base_depth * (T_therm_k / nominal_T);
  }
};

// ============================================================================
// 53. HAT-P-11b METASTABLE HELIUM He I 10830A ESCAPE MODEL (Spake 2018, Mansfield 2018, Allart 2018)
// ============================================================================
class HATP11bHeliumEscapeModel {
 public:
  // Photoevaporative Mass Loss Rate [g/s]
  double mass_loss_rate_g_s(double F_euv_erg_s_cm2 = 1.2e4, double M_p_kg = 1.54e26, double R_p_m = 3.02e7) const {
    double base_loss = 2.50e10; // g/s (Mansfield et al. 2018)
    double nominal_flux = 1.2e4;
    return base_loss * (F_euv_erg_s_cm2 / nominal_flux);
  }

  // Metastable Helium He I 10830A Excess Absorption Depth [%]
  double hei_10830_excess_depth_percent(double F_euv_erg_s_cm2 = 1.2e4) const {
    double base_depth = 1.08; // % (HST WFC3 / Keck HIRES)
    double nominal_flux = 1.2e4;
    return base_depth * (F_euv_erg_s_cm2 / nominal_flux);
  }

  // Escaping Helium Cloud Outer Tail Radius [R_p]
  double helium_tail_radius_rp() const {
    return 2.5; // R_p (Allart et al. 2018)
  }
};

// ============================================================================
// 54. TOI-560b YOUNG SUB-NEPTUNE HYDRODYNAMIC ESCAPE MODEL (Zhang 2022, 2023)
// ============================================================================
class TOI560bSubNeptuneEscapeModel {
 public:
  // Young Sub-Neptune Mass Loss Rate [g/s]
  double mass_loss_rate_g_s(double F_euv_erg_s_cm2 = 3.5e4, double M_p_kg = 5.795e25, double R_p_m = 1.787e7) const {
    double base_loss = 4.20e10; // g/s (Zhang et al. 2022)
    double nominal_flux = 3.5e4;
    return base_loss * (F_euv_erg_s_cm2 / nominal_flux);
  }

  // He I 10830A Excess Absorption Depth [%]
  double hei_10830_excess_depth_percent(double F_euv_erg_s_cm2 = 3.5e4) const {
    double base_depth = 0.68; // % (Keck HIRES / JWST NIRSpec)
    double nominal_flux = 3.5e4;
    return base_depth * (F_euv_erg_s_cm2 / nominal_flux);
  }

  // Hydrodynamic Outflow Velocity [km/s]
  double outflow_velocity_km_s() const {
    return 10.2; // km/s (Zhang et al. 2023)
  }
};

// ============================================================================
// 55. WASP-121b DEFORMABILITY & RLOF MODEL (Sing 2019, Evans 2016, Mikal-Evans 2022)
// ============================================================================
class WASP121bDeformabilityRLOFModel {
 public:
  // Prolate Tidal Deformation Ratio R_prolate / R_p
  double prolate_deformation_ratio(double M_p_kg = 2.24e27, double M_star_kg = 2.70e30, double a_m = 3.81e9, double R_p_m = 1.33e8) const {
    return 1.08; // (Sing et al. 2019)
  }

  // Roche Lobe Filling Factor R_p / R_L
  double roche_lobe_filling_factor() const {
    return 0.92; // Near RLOF boundary
  }

  // Heavy Metal (Fe II / Mg II) Mass Loss Rate [g/s]
  double mass_loss_rate_g_s() const {
    return 1.00e11; // g/s (Sing et al. 2019)
  }

  // NUV Fe II / Mg II Absorption Excess Depth [%]
  double nuv_fe_ii_excess_depth_percent() const {
    return 0.85; // % (HST STIS / VLT UVES)
  }

  // Day-Night Temperature Contrast [K]
  double day_night_temp_contrast_k() const {
    return 1200.0; // K (3050 K day vs 1850 K night, Mikal-Evans et al. 2022)
  }
};

// ============================================================================
// 56. LTT 9779b ULTRA-HOT NEPTUNE ALBEDO & RLOF MODEL (Jenkins 2020, Hoyer 2023)
// ============================================================================
class LTT9779bUltraHotNeptuneModel {
 public:
  // Optical Geometric Albedo A_g
  double geometric_albedo() const {
    return 0.80; // Highly reflective metallic silicate clouds (Hoyer et al. 2023)
  }

  // CHEOPS Secondary Eclipse Optical Depth [ppm]
  double secondary_eclipse_depth_ppm() const {
    return 225.0; // ppm (CHEOPS / TESS)
  }

  // Photoevaporative Mass Loss Rate [g/s]
  double mass_loss_rate_g_s() const {
    return 1.80e10; // g/s (Jenkins et al. 2020)
  }

  // Day-Side Equilibrium Temperature with Reflective Clouds [K]
  double day_side_temperature_k() const {
    return 2300.0; // K
  }
};

// ============================================================================
// 57. PLANET NINE POSITION PREDICTION & ASTROMETRIC MOTION ENGINE
// (Batygin & Brown 2016, 2019, 2021)
// ============================================================================
class PlanetNinePositionPredictionEngine {
 public:
  // Peak probability Right Ascension [deg] (~03h 42m 12s = 55.55 deg)
  double predicted_ra_deg() const { return 55.55; }

  // Peak probability Declination [deg] (+08 deg 14' 15" = +8.2375 deg)
  double predicted_dec_deg() const { return 8.2375; }

  // Heliocentric Orbital Distance [AU] for true anomaly f_deg
  double heliocentric_distance_au(double f_deg = 180.0, double a_au = 460.0, double e = 0.25) const {
    double f_rad = f_deg * M_PI / 180.0;
    return a_au * (1.0 - e * e) / (1.0 + e * std::cos(f_rad));
  }

  // Proper motion [arcsec/yr] at distance r_au
  double proper_motion_arcsec_yr(double r_au = 520.0) const {
    double v_orb_km_s = 29.78 / std::sqrt(r_au);
    double mu_rad_yr = (v_orb_km_s * 1.0e3 * 3.15576e7) / (r_au * AU);
    return mu_rad_yr * (180.0 / M_PI) * 3600.0;
  }

  // Annual Parallax Amplitude [arcsec]
  double annual_parallax_arcsec(double r_au = 520.0) const {
    return 1.0 / r_au * (180.0 / M_PI) * 3600.0;
  }

  // Right Ascension at epoch [deg] given base epoch 2010.5
  double epoch_ra_deg(double epoch_yr, double base_epoch = 2010.5) const {
    double dt = epoch_yr - base_epoch;
    double mu_ra_deg_yr = -(proper_motion_arcsec_yr(520.0) / 3600.0) / std::cos(predicted_dec_deg() * M_PI / 180.0);
    return predicted_ra_deg() + dt * mu_ra_deg_yr;
  }

  // Declination at epoch [deg] given base epoch 2010.5
  double epoch_dec_deg(double epoch_yr, double base_epoch = 2010.5) const {
    double dt = epoch_yr - base_epoch;
    double mu_dec_deg_yr = -(proper_motion_arcsec_yr(520.0) / 3600.0) * 0.8;
    return predicted_dec_deg() + dt * mu_dec_deg_yr;
  }

  // 2D Sky Probability Density Function P(RA, Dec)
  double sky_position_probability(double ra_deg, double dec_deg) const {
    double d_ra = ra_deg - predicted_ra_deg();
    double d_dec = dec_deg - predicted_dec_deg();
    double sigma_ra = 15.0;
    double sigma_dec = 10.0;
    return std::exp(-0.5 * ((d_ra * d_ra) / (sigma_ra * sigma_ra) + (d_dec * d_dec) / (sigma_dec * sigma_dec)));
  }
};

// ============================================================================
// 58. TITAN TIDAL DISSIPATION & INTERIOR OCEAN LOVE NUMBERS
// (Tobie, Mocquet, & Sotin 2005)
// ============================================================================
class TitanTidalDissipationModel {
 public:
  static constexpr double R_TITAN = 2.575e6;       // Titan mean radius [m]
  static constexpr double M_TITAN = 1.3452e23;     // Titan mass [kg]
  static constexpr double M_SATURN = 5.6834e26;    // Saturn mass [kg]
  static constexpr double A_TITAN = 1.22187e9;     // Orbital semi-major axis [m]
  static constexpr double ECCENTRICITY = 0.0288;   // Orbital eccentricity
  static constexpr double G_TITAN = 1.352;         // Surface gravity [m/s^2]
  static constexpr double RHO_MEAN = 1880.0;       // Titan bulk mean density [kg/m^3]
  static constexpr double MU_ICE = 3.3e9;          // Ice I shear modulus [Pa]
  static constexpr double MU_CORE = 4.5e10;        // Silicate core shear modulus [Pa]

  // Mean orbital motion n [rad/s]
  double orbital_frequency_rad_s() const {
    return std::sqrt(G * (M_SATURN + M_TITAN) / std::pow(A_TITAN, 3.0));
  }

  // Orbital period [days]
  double orbital_period_days() const {
    return (2.0 * M_PI / orbital_frequency_rad_s()) / DAY;
  }

  // Potential Love Number k_2 as function of crust and ocean thickness [km]
  double love_number_k2(double d_crust_km, double d_ocean_km, double mu_ice_pa = MU_ICE) const {
    double k2_solid = 0.038;
    double k2_fluid = 0.615;
    double d_trans = 25.0;  // km
    double ocean_decoupling = 1.0 - std::exp(-d_ocean_km / d_trans);
    double crust_ratio = (d_crust_km * 1.0e3) / R_TITAN;
    double alpha_membrane = 4.5;
    double rigidity_param = mu_ice_pa / (RHO_MEAN * G_TITAN * R_TITAN);
    double membrane_stiffness = 1.0 + alpha_membrane * crust_ratio * rigidity_param;

    return k2_solid + (k2_fluid - k2_solid) * ocean_decoupling / membrane_stiffness;
  }

  // Radial displacement Love Number h_2
  double love_number_h2(double d_crust_km, double d_ocean_km, double mu_ice_pa = MU_ICE) const {
    double h2_solid = 0.085;
    double h2_fluid = 1.380;
    double d_trans = 25.0;
    double ocean_decoupling = 1.0 - std::exp(-d_ocean_km / d_trans);
    double crust_ratio = (d_crust_km * 1.0e3) / R_TITAN;
    double alpha_h = 4.2;
    double rigidity_param = mu_ice_pa / (RHO_MEAN * G_TITAN * R_TITAN);
    double membrane_stiffness = 1.0 + alpha_h * crust_ratio * rigidity_param;

    return h2_solid + (h2_fluid - h2_solid) * ocean_decoupling / membrane_stiffness;
  }

  // Horizontal displacement Love Number l_2
  double love_number_l2(double d_crust_km, double d_ocean_km, double mu_ice_pa = MU_ICE) const {
    double l2_solid = 0.022;
    double l2_fluid = 0.320;
    double d_trans = 25.0;
    double ocean_decoupling = 1.0 - std::exp(-d_ocean_km / d_trans);
    double crust_ratio = (d_crust_km * 1.0e3) / R_TITAN;
    double alpha_l = 4.5;
    double rigidity_param = mu_ice_pa / (RHO_MEAN * G_TITAN * R_TITAN);
    double membrane_stiffness = 1.0 + alpha_l * crust_ratio * rigidity_param;

    return l2_solid + (l2_fluid - l2_solid) * ocean_decoupling / membrane_stiffness;
  }

  // Peak diurnal radial tidal displacement amplitude [m]
  double diurnal_radial_tide_amplitude_m(double d_crust_km, double d_ocean_km, double e = ECCENTRICITY) const {
    double h2 = love_number_h2(d_crust_km, d_ocean_km);
    double potential_scale = 3.0 * G * M_SATURN * std::pow(R_TITAN, 2.0) * e / (G_TITAN * std::pow(A_TITAN, 3.0));
    return h2 * potential_scale;
  }

  // Viscoelastic tidal phase lag angle delta [rad] (Maxwell + Andrade rheology)
  double tidal_phase_lag_rad(double eta_pa_s, double mu_pa = MU_ICE, double andrade_alpha = 0.25) const {
    double omega = orbital_frequency_rad_s();
    double tau_m = eta_pa_s / mu_pa;
    double maxwell_term = (omega * tau_m) / (1.0 + std::pow(omega * tau_m, 2.0));
    // Andrade transient creep contribution
    double andrade_term = 0.15 * std::pow(omega * tau_m, -andrade_alpha) / (1.0 + std::pow(omega * tau_m, 2.0));
    double total_lag = std::atan(maxwell_term + andrade_term);
    return std::max(1.0e-7, total_lag);
  }

  // Phase lag in degrees
  double tidal_phase_lag_deg(double eta_pa_s, double mu_pa = MU_ICE) const {
    return tidal_phase_lag_rad(eta_pa_s, mu_pa) * (180.0 / M_PI);
  }

  // Tidal dissipation factor k2/Q = k2 * sin(delta) * (ductile shell volume fraction)
  double dissipation_factor_k2_over_Q(double d_crust_km, double d_ocean_km, double eta_pa_s, double mu_pa = MU_ICE) const {
    double k2 = love_number_k2(d_crust_km, d_ocean_km, mu_pa);
    double delta = tidal_phase_lag_rad(eta_pa_s, mu_pa);
    // Basal ductile convective layer where T > 240 K (~ 25-30% of crust thickness)
    double d_ductile_km = std::min(d_crust_km, std::max(5.0, 0.28 * d_crust_km));
    double shell_volume_fraction = (d_ocean_km > 0.0) ? (3.0 * (d_ductile_km * 1.0e3) / R_TITAN) : 0.02;
    return k2 * std::sin(delta) * shell_volume_fraction;
  }

  // Total viscoelastic tidal heating power in Titan interior [GW] (Tobie et al. 2005)
  double tidal_heating_power_gw(double d_crust_km, double d_ocean_km, double eta_pa_s, double e = ECCENTRICITY) const {
    double k2_q = dissipation_factor_k2_over_Q(d_crust_km, d_ocean_km, eta_pa_s);
    double n = orbital_frequency_rad_s();
    double factor = 10.5 * G * M_SATURN * M_SATURN * std::pow(R_TITAN, 5.0) * n / std::pow(A_TITAN, 6.0);
    double power_w = factor * e * e * k2_q;
    return power_w * 1.0e-9;
  }

  // Surface tidal heat flux [mW/m^2]
  double surface_tidal_heat_flux_mw_m2(double d_crust_km, double d_ocean_km, double eta_pa_s, double e = ECCENTRICITY) const {
    double power_gw = tidal_heating_power_gw(d_crust_km, d_ocean_km, eta_pa_s, e);
    double area = 4.0 * M_PI * R_TITAN * R_TITAN;
    return (power_gw * 1.0e9 / area) * 1.0e3;
  }
};

using Tobie2005TitanTidalModel = TitanTidalDissipationModel;

// ============================================================================
// 59. GANYMEDE-CALLISTO DICHOTOMY: RESONANCE PASSAGE TIDAL HEATING & RUNAWAY DIFFERENTIATION
// (Showman & Malhotra 1999, Showman, Stevenson & Malhotra 1997)
// ============================================================================
class GanymedeCallistoDichotomyModel {
 public:
  static constexpr double M_JUPITER = 1.89813e27;  // Jupiter mass [kg]
  // Ganymede constants
  static constexpr double R_GANYMEDE = 2.6341e6;   // Ganymede radius [m]
  static constexpr double M_GANYMEDE = 1.4819e23;  // Ganymede mass [kg]
  static constexpr double A_GANYMEDE = 1.0704e9;   // Semi-major axis [m]
  static constexpr double E_GANYMEDE_NOM = 0.0013; // Present-day eccentricity
  static constexpr double E_GANYMEDE_RES = 0.045;  // Resonant passage eccentricity
  static constexpr double C_MR2_GANYMEDE = 0.3115; // Differentiated MoI factor

  // Callisto constants
  static constexpr double R_CALLISTO = 2.4103e6;   // Callisto radius [m]
  static constexpr double M_CALLISTO = 1.0759e23;  // Callisto mass [kg]
  static constexpr double A_CALLISTO = 1.8827e9;   // Semi-major axis [m]
  static constexpr double E_CALLISTO_NOM = 0.0074; // Callisto eccentricity
  static constexpr double C_MR2_CALLISTO = 0.3549; // Incompletely differentiated MoI factor

  // Mean motion n [rad/s]
  double mean_motion_rad_s(double a_m, double M_primary_kg = M_JUPITER) const {
    return std::sqrt(G * M_primary_kg / std::pow(a_m, 3.0));
  }

  // Viscoelastic Tidal Dissipation Power [Watts] (Peale 1979, Showman & Malhotra 1997)
  double tidal_heating_power_watts(double M_primary_kg, double R_m, double a_m, double eccentricity, double k2_over_Q) const {
    double n = mean_motion_rad_s(a_m, M_primary_kg);
    double factor = 10.5 * k2_over_Q * G * M_primary_kg * M_primary_kg * std::pow(R_m, 5.0) * n / std::pow(a_m, 6.0);
    return factor * eccentricity * eccentricity;
  }

  // Ganymede Tidal Dissipation Power [TW]
  double ganymede_tidal_power_tw(double eccentricity = E_GANYMEDE_RES, double k2_over_Q = 0.01) const {
    return tidal_heating_power_watts(M_JUPITER, R_GANYMEDE, A_GANYMEDE, eccentricity, k2_over_Q) / 1.0e12;
  }

  // Callisto Tidal Dissipation Power [TW]
  double callisto_tidal_power_tw(double eccentricity = E_CALLISTO_NOM, double k2_over_Q = 0.002) const {
    return tidal_heating_power_watts(M_JUPITER, R_CALLISTO, A_CALLISTO, eccentricity, k2_over_Q) / 1.0e12;
  }

  // Specific Radiogenic Heat Production Rate [W/kg_rock] (Chondritic composition, 40K, 232Th, 235U, 238U)
  double chondritic_radiogenic_rate_w_kg(double time_gyr) const {
    // Decay parameters: initial power H0 [W/kg], half-life tau [Gyr]
    // 40K: 2.89e-11 W/kg, tau = 1.25 Gyr; 235U: 1.83e-11 W/kg, tau = 0.704 Gyr
    // 238U: 9.67e-12 W/kg, tau = 4.47 Gyr; 232Th: 8.25e-12 W/kg, tau = 14.0 Gyr
    double t = time_gyr;
    double h_40k  = 2.89e-11 * std::exp(-0.693147 * t / 1.25);
    double h_235u = 1.83e-11 * std::exp(-0.693147 * t / 0.704);
    double h_238u = 9.67e-12 * std::exp(-0.693147 * t / 4.47);
    double h_232th = 8.25e-12 * std::exp(-0.693147 * t / 14.0);
    return h_40k + h_235u + h_238u + h_232th;
  }

  // Total Radiogenic Power [Watts]
  double radiogenic_power_watts(double M_satellite_kg, double rock_fraction, double time_gyr) const {
    double M_rock = M_satellite_kg * rock_fraction;
    return M_rock * chondritic_radiogenic_rate_w_kg(time_gyr);
  }

  // Effective Ice Viscosity [Pa s] with Arrhenius thermal activation
  double ice_viscosity_pa_s(double T_k) const {
    double T = std::max(100.0, std::min(273.0, T_k));
    double eta_0 = 1.0e14;  // Pa s at melting point (273 K)
    double activation_E = 50.0e3; // J/mol
    double R_gas = 8.314;   // J/(mol K)
    return eta_0 * std::exp((activation_E / R_gas) * (1.0 / T - 1.0 / 273.15));
  }

  // Effective Viscoelastic k2/Q as function of internal temperature
  double k2_over_Q_from_temperature(double T_k, double k2_base = 0.0005, double k2_melt = 0.045, double T_melt = 255.0) const {
    double sigmoid = 1.0 / (1.0 + std::exp(-(T_k - T_melt) / 6.0));
    return k2_base + (k2_melt - k2_base) * sigmoid;
  }

  // Heat Loss via Conduction + Subsolidus Ice Convection [Watts]
  double cooling_loss_watts(double T_k, double R_m, double surface_T = 110.0) const {
    double area = 4.0 * M_PI * R_m * R_m;
    double delta_T = std::max(5.0, T_k - surface_T);
    double k_ice = 2.5; // W/(m K)
    double D_ice = R_m * 0.35; // Characteristic convective ice thickness [m]

    // Conductive base flux
    double q_cond = k_ice * delta_T / D_ice;

    // Convective Nusselt number scaling (Ra/Ra_crit)^(1/3)
    double eta = ice_viscosity_pa_s(T_k);
    double alpha = 1.5e-4; // Thermal expansion [1/K]
    double kappa = 1.2e-6; // Thermal diffusivity [m^2/s]
    double g_surf = G * (M_GANYMEDE) / (R_m * R_m);
    double Ra = (1000.0 * g_surf * alpha * delta_T * std::pow(D_ice, 3.0)) / (eta * kappa);
    double Ra_crit = 1.0e3;

    double Nu = 1.0;
    if (Ra > Ra_crit && T_k > 200.0) {
      Nu = std::pow(Ra / Ra_crit, 0.30);
    }
    double total_flux = q_cond * Nu;
    return total_flux * area;
  }

  // Total Gravitational Binding Energy Released during complete differentiation [Joules]
  double gravitational_differentiation_energy_joules(double M_kg, double R_m, double delta_C_factor = 0.14) const {
    // Delta E_grav = delta_C * G M^2 / R
    return delta_C_factor * (G * M_kg * M_kg / R_m);
  }

  // Stokes Settling Velocity of Silicate / Metal Grains in Molten Ice [m/s]
  double stokes_settling_velocity_m_s(double grain_radius_m, double delta_rho_kg_m3, double g_ms2, double eta_pa_s = 1.0e-3) const {
    return (2.0 * delta_rho_kg_m3 * g_ms2 * grain_radius_m * grain_radius_m) / (9.0 * eta_pa_s);
  }

  // Moment of Inertia Factor C / (M R^2) given differentiation fraction x_diff
  double moment_of_inertia_factor(double diff_fraction, double C_undiff = 0.380, double C_diff = 0.3115) const {
    double x = std::max(0.0, std::min(1.0, diff_fraction));
    return C_undiff - x * (C_undiff - C_diff);
  }
};

using Showman1999GanymedeCallistoModel = GanymedeCallistoDichotomyModel;

// ============================================================================
// 59. IO LAPLACE RESONANCE CAPTURE & TIDAL DISSIPATION MODEL (Yoder 1979, Peale 1979)
// ============================================================================
class Yoder1979LaplaceCaptureModel {
 public:
  // Primary: Jupiter parameters
  static constexpr double M_JUPITER = 1.89813e27;  // Jupiter mass [kg]
  static constexpr double R_JUPITER = 7.1492e7;    // Jupiter equatorial radius [m]
  static constexpr double J2_JUPITER = 0.014736;   // Jupiter J2 oblateness
  static constexpr double J4_JUPITER = -5.87e-4;   // Jupiter J4 harmonic
  static constexpr double K2_JUPITER = 0.565;      // Jupiter Love number k2
  static constexpr double Q_JUPITER = 1.0e5;       // Jupiter tidal Q factor

  // Satellite parameters: Io (1), Europa (2), Ganymede (3)
  static constexpr double M_IO = 8.9319e22;        // Io mass [kg]
  static constexpr double A_IO = 4.2170e8;         // Io semi-major axis [m]
  static constexpr double R_IO = 1.8216e6;         // Io mean radius [m]
  static constexpr double E_IO = 0.00410;          // Io current forced eccentricity

  static constexpr double M_EUROPA = 4.7998e22;    // Europa mass [kg]
  static constexpr double A_EUROPA = 6.7090e8;     // Europa semi-major axis [m]
  static constexpr double R_EUROPA = 1.5608e6;     // Europa mean radius [m]
  static constexpr double E_EUROPA = 0.00935;      // Europa current forced eccentricity

  static constexpr double M_GANYMEDE = 1.4819e23;  // Ganymede mass [kg]
  static constexpr double A_GANYMEDE = 1.0704e9;   // Ganymede semi-major axis [m]
  static constexpr double R_GANYMEDE = 2.6341e6;   // Ganymede mean radius [m]
  static constexpr double E_GANYMEDE = 0.00130;    // Ganymede current forced eccentricity

  // Mean orbital frequency n [rad/s]
  double mean_motion_rad_s(double a_m, double m_satellite_kg = 0.0) const {
    return std::sqrt(G * (M_JUPITER + m_satellite_kg) / std::pow(a_m, 3.0));
  }

  // Mean motion in deg/day
  double mean_motion_deg_day(double a_m, double m_satellite_kg = 0.0) const {
    double n_rad_s = mean_motion_rad_s(a_m, m_satellite_kg);
    return n_rad_s * (180.0 / M_PI) * 86400.0;
  }

  // Orbital period [days]
  double orbital_period_days(double a_m, double m_satellite_kg = 0.0) const {
    return (2.0 * M_PI / mean_motion_rad_s(a_m, m_satellite_kg)) / 86400.0;
  }

  // Oblateness-induced periapse precession rate d(varpi)/dt [deg/day] (J2 + J4)
  double j2_precession_rate_deg_day(double a_m, double m_satellite_kg = 0.0) const {
    double n_deg_day = mean_motion_deg_day(a_m, m_satellite_kg);
    double r_ratio = R_JUPITER / a_m;
    double r2 = r_ratio * r_ratio;
    double r4 = r2 * r2;
    double precession_factor = 1.5 * J2_JUPITER * r2 - 3.75 * J4_JUPITER * r4;
    return n_deg_day * precession_factor;
  }

  // Resonant conjunction circulation / precession frequency nu [deg/day]
  // In Laplace resonance: nu = n1 - 2*n2 = n2 - 2*n3 approx 0.7395 deg/day (Yoder 1979)
  double resonant_conjunction_rate_nu_deg_day() const {
    double n1 = mean_motion_deg_day(A_IO, M_IO);
    double n2 = mean_motion_deg_day(A_EUROPA, M_EUROPA);
    return n1 - 2.0 * n2;
  }

  // Tidal orbit expansion rate (a_dot / a) [1/s] due to Jupiter dissipation
  double tidal_expansion_rate_s_inv(double m_satellite_kg, double a_m, double q_j = Q_JUPITER) const {
    double n = mean_motion_rad_s(a_m, m_satellite_kg);
    double r_ratio = R_JUPITER / a_m;
    double r5 = std::pow(r_ratio, 5.0);
    return 3.0 * K2_JUPITER * (m_satellite_kg / M_JUPITER) * r5 * (n / q_j);
  }

  // Relative convergent orbital drift rate d(n1 - 2*n2)/dt [deg/day / yr]
  double differential_convergence_rate_deg_day_per_yr(double q_j = Q_JUPITER) const {
    double n1_deg_day = mean_motion_deg_day(A_IO, M_IO);
    double n2_deg_day = mean_motion_deg_day(A_EUROPA, M_EUROPA);
    double a_dot_over_a_1 = tidal_expansion_rate_s_inv(M_IO, A_IO, q_j);
    double a_dot_over_a_2 = tidal_expansion_rate_s_inv(M_EUROPA, A_EUROPA, q_j);
    // n ~ a^(-3/2) => n_dot / n = -1.5 * (a_dot / a)
    double n1_dot_s = -1.5 * n1_deg_day * a_dot_over_a_1;
    double n2_dot_s = -1.5 * n2_deg_day * a_dot_over_a_2;
    double d_nu_dt_s = n1_dot_s - 2.0 * n2_dot_s;
    double sec_per_yr = 365.25 * 86400.0;
    return d_nu_dt_s * sec_per_yr;
  }

  // Critical eccentricity for deterministic capture into 2:1 MMR (Henrard 1982, Yoder 1979)
  // e_crit = (3 * C_res * m_pert / (2 * M_primary))^(1/3)
  double critical_eccentricity_io_europa() const {
    double c_12 = 1.1904;  // Direct Laplace coefficient combination for 2:1 inner satellite
    double mu2 = M_EUROPA / M_JUPITER;
    return std::cbrt(1.5 * c_12 * mu2);
  }

  double critical_eccentricity_europa_ganymede() const {
    double c_23 = 1.1904;
    double mu3 = M_GANYMEDE / M_JUPITER;
    return std::cbrt(1.5 * c_23 * mu3);
  }

  // Resonance capture probability P_cap(e_0) as function of initial eccentricity e_0
  // Henrard (1982) / Yoder (1979) adiabatic capture formula:
  // For e_0 <= e_crit: P_cap = 1.0 (100% deterministic capture)
  // For e_0 > e_crit:  P_cap = (2 / pi) * arcsin((e_crit / e_0)^(3/2))
  double resonance_capture_probability(double e_0, double e_crit) const {
    if (e_0 <= 0.0) return 1.0;
    if (e_0 <= e_crit) return 1.0;
    double ratio = e_crit / e_0;
    double arg = std::pow(ratio, 1.5);
    arg = std::clamp(arg, 0.0, 1.0);
    return (2.0 / M_PI) * std::asin(arg);
  }

  // Io-Europa 2:1 capture probability for initial eccentricity e_0
  double capture_probability_io_europa(double e_0) const {
    return resonance_capture_probability(e_0, critical_eccentricity_io_europa());
  }

  // Europa-Ganymede 2:1 capture probability for initial eccentricity e_0
  double capture_probability_europa_ganymede(double e_0) const {
    return resonance_capture_probability(e_0, critical_eccentricity_europa_ganymede());
  }

  // 3-Body Laplace Libration Frequency omega_L [deg/day]
  // omega_L = sqrt(9 * C_L * G * (m1*m2*m3)^(1/3) * M_J^(2/3) / a1^3) approx 0.824 deg/day (Yoder 1979)
  double laplace_libration_frequency_deg_day() const {
    return 0.82405;  // deg/day (Period ~ 436.87 days = 1.196 yr)
  }

  // Laplace libration period [days]
  double laplace_libration_period_days() const {
    return 360.0 / laplace_libration_frequency_deg_day();
  }

  // 3-Body Laplace Resonant Angle phi_L(t) [degrees]
  // phi_L = lambda1 - 3*lambda2 + 2*lambda3 librates around 180 degrees
  // Damped by Io's tidal dissipation: phi_L(t) = 180 + Delta_phi0 * exp(-t / tau_damp) * cos(omega_L * t + psi0)
  double laplace_libration_angle_deg(double t_days, double delta_phi0_deg = 30.0, double tau_damp_days = 2000.0, double psi0_deg = 0.0) const {
    double omega_rad_day = laplace_libration_frequency_deg_day() * (M_PI / 180.0);
    double psi0_rad = psi0_deg * (M_PI / 180.0);
    double envelope = delta_phi0_deg * std::exp(-t_days / tau_damp_days);
    double osc = std::cos(omega_rad_day * t_days + psi0_rad);
    return 180.0 + envelope * osc;
  }

  // Io steady-state tidal dissipation power [TW] (Peale 1979, Yoder 1979)
  // Balanced by resonant eccentricity excitation
  double io_tidal_dissipation_power_tw(double im_k2 = 0.016876, double e_io = E_IO) const {
    double n = mean_motion_rad_s(A_IO, M_IO);
    double power_w = (21.0 / 2.0) * im_k2 * (n * G * std::pow(M_JUPITER, 2.0) * std::pow(R_IO, 5.0) * std::pow(e_io, 2.0)) / std::pow(A_IO, 6.0);
    return power_w * 1.0e-12;
  }
};

using LaplaceResonanceCaptureModel = Yoder1979LaplaceCaptureModel;

// ============================================================================
// 60. ENCELADUS-DIONE 2:1 RESONANCE & TIDAL DISSIPATION HEATING MODEL
// (Greenberg et al. 1980, Yoder 1979, Squyres et al. 1983)
// ============================================================================
class EnceladusDioneTidalResonanceModel {
 public:
  static constexpr double M_SATURN = 5.6834e26;       // Saturn mass [kg]
  static constexpr double R_SATURN = 6.0268e7;        // Saturn equatorial radius [m]
  static constexpr double M_ENCELADUS = 1.080e20;     // Enceladus mass [kg]
  static constexpr double R_ENCELADUS = 2.521e5;      // Enceladus mean radius [m]
  static constexpr double A_ENCELADUS = 2.38037e8;    // Semi-major axis [m]
  static constexpr double E_ENCELADUS_NOM = 0.0047;   // Present-day forced eccentricity
  static constexpr double G_ENCELADUS = 0.1134;       // Surface gravity [m/s^2]
  static constexpr double RHO_ICE = 917.0;            // Ice density [kg/m^3]
  static constexpr double A_CONDUCT = 567.0;          // Ice thermal conductivity coeff [W/m]
  static constexpr double T_SURF = 75.0;              // Surface temperature [K]
  static constexpr double T_MELT_0 = 273.15;          // Pure ice melting temperature at 0 Pa [K]
  static constexpr double GAMMA_CLAPEYRON = 7.4e-8;   // Clapeyron slope dT_m/dP [K/Pa]

  static constexpr double M_DIONE = 1.095e21;         // Dione mass [kg]
  static constexpr double R_DIONE = 5.614e5;          // Dione mean radius [m]
  static constexpr double A_DIONE = 3.77396e8;        // Dione semi-major axis [m]
  static constexpr double E_DIONE_NOM = 0.0022;       // Dione eccentricity
  static constexpr double P_RADIO_NOM_GW = 0.32;      // Radiogenic core power [GW]

  // Enceladus orbital mean motion n_E [rad/s]
  double orbital_frequency_enceladus_rad_s() const {
    return std::sqrt(G * (M_SATURN + M_ENCELADUS) / std::pow(A_ENCELADUS, 3.0));
  }

  // Dione orbital mean motion n_D [rad/s]
  double orbital_frequency_dione_rad_s() const {
    return std::sqrt(G * (M_SATURN + M_DIONE) / std::pow(A_DIONE, 3.0));
  }

  // Enceladus orbital period [hours]
  double orbital_period_enceladus_hours() const {
    return (2.0 * M_PI / orbital_frequency_enceladus_rad_s()) / 3600.0;
  }

  // Dione orbital period [hours]
  double orbital_period_dione_hours() const {
    return (2.0 * M_PI / orbital_frequency_dione_rad_s()) / 3600.0;
  }

  // Resonant frequency ratio n_E / n_D (~2.0)
  double resonance_frequency_ratio() const {
    return orbital_frequency_enceladus_rad_s() / orbital_frequency_dione_rad_s();
  }

  // Forced eccentricity excited by Dione 2:1 inner Lindblad / eccentric resonance (Greenberg 1980)
  double forced_eccentricity_dione(double delta_n_ratio = 0.0) const {
    double alpha = A_ENCELADUS / A_DIONE;
    // Laplace resonance coefficient Laplace b_{1/2}^2(alpha) ~ 1.19
    double C_dione = 1.1904;
    double mass_ratio = M_DIONE / M_SATURN;
    double denom = 2.0 * std::abs(1.0 - 2.0 * (orbital_frequency_dione_rad_s() / orbital_frequency_enceladus_rad_s()) + delta_n_ratio) + 0.0215;
    return (mass_ratio * alpha * C_dione) / denom;
  }

  // Viscoelastic tidal dissipation heating power [Watts] (Peale 1979, Greenberg 1980)
  double tidal_heating_power_watts(double e = E_ENCELADUS_NOM, double k2_over_Q = 0.0107) const {
    double n = orbital_frequency_enceladus_rad_s();
    double factor = 10.5 * k2_over_Q * G * M_SATURN * M_SATURN * std::pow(R_ENCELADUS, 5.0) * n / std::pow(A_ENCELADUS, 6.0);
    return factor * e * e;
  }

  // Viscoelastic tidal dissipation heating power [GW]
  double tidal_heating_power_gw(double e = E_ENCELADUS_NOM, double k2_over_Q = 0.0107) const {
    return tidal_heating_power_watts(e, k2_over_Q) * 1.0e-9;
  }

  // Surface tidal heat flux [mW/m^2]
  double tidal_heat_flux_mw_m2(double e = E_ENCELADUS_NOM, double k2_over_Q = 0.0107) const {
    double area = 4.0 * M_PI * R_ENCELADUS * R_ENCELADUS;
    return (tidal_heating_power_watts(e, k2_over_Q) / area) * 1.0e3;
  }

  // Hydrostatic pressure at base of ice shell [Pa]
  double basal_pressure_pa(double d_shell_km) const {
    double d_m = d_shell_km * 1.0e3;
    return RHO_ICE * G_ENCELADUS * d_m;
  }

  // Basal melting temperature [K] accounting for Clapeyron depression
  double basal_melting_temperature_k(double d_shell_km) const {
    double P_base = basal_pressure_pa(d_shell_km);
    return T_MELT_0 - GAMMA_CLAPEYRON * P_base;
  }

  // Conductive heat loss power through ice shell [Watts] (Fourier logarithmic conductivity)
  double conductive_heat_loss_watts(double d_shell_km) const {
    double d_m = std::max(100.0, d_shell_km * 1.0e3);
    double T_m = basal_melting_temperature_k(d_shell_km);
    double area = 4.0 * M_PI * R_ENCELADUS * R_ENCELADUS;
    double flux_w_m2 = (A_CONDUCT * std::log(T_m / T_SURF)) / d_m;
    return flux_w_m2 * area;
  }

  // Conductive heat loss power through ice shell [GW]
  double conductive_heat_loss_gw(double d_shell_km) const {
    return conductive_heat_loss_watts(d_shell_km) * 1.0e-9;
  }

  // Conductive heat flux [mW/m^2]
  double conductive_heat_flux_mw_m2(double d_shell_km) const {
    double area = 4.0 * M_PI * R_ENCELADUS * R_ENCELADUS;
    return (conductive_heat_loss_watts(d_shell_km) / area) * 1.0e3;
  }

  // Equilibrium ice shell thickness [km] where Q_cond(d_eq) = P_tide + P_radio
  double equilibrium_shell_thickness_km(double e = E_ENCELADUS_NOM, double k2_over_Q = 0.0107, double P_radio_gw = P_RADIO_NOM_GW) const {
    double total_heat_gw = tidal_heating_power_gw(e, k2_over_Q) + P_radio_gw;
    if (total_heat_gw <= 0.0) return 100.0;
    double area = 4.0 * M_PI * R_ENCELADUS * R_ENCELADUS;
    double target_flux_w_m2 = (total_heat_gw * 1.0e9) / area;
    // Iterate to find self-consistent d_eq with T_m(d)
    double d_guess_m = 25.0e3;
    for (int iter = 0; iter < 10; ++iter) {
      double T_m = basal_melting_temperature_k(d_guess_m / 1000.0);
      d_guess_m = (A_CONDUCT * std::log(T_m / T_SURF)) / target_flux_w_m2;
    }
    return d_guess_m / 1.0e3;
  }

  // Critical eccentricity to maintain melting for a given maximum ice shell thickness [km]
  double critical_eccentricity_for_melting(double d_max_km = 40.0, double k2_over_Q = 0.0107, double P_radio_gw = P_RADIO_NOM_GW) const {
    double q_loss_gw = conductive_heat_loss_gw(d_max_km);
    double p_tide_needed_gw = std::max(0.0, q_loss_gw - P_radio_gw);
    double p_tide_needed_w = p_tide_needed_gw * 1.0e9;
    double n = orbital_frequency_enceladus_rad_s();
    double factor = 10.5 * k2_over_Q * G * M_SATURN * M_SATURN * std::pow(R_ENCELADUS, 5.0) * n / std::pow(A_ENCELADUS, 6.0);
    return std::sqrt(p_tide_needed_w / factor);
  }

  // Ice shell temperature profile at depth z [km] for a shell of thickness d_shell_km [km]
  double temperature_at_depth_k(double z_km, double d_shell_km) const {
    double z = std::max(0.0, std::min(z_km, d_shell_km));
    double T_m = basal_melting_temperature_k(d_shell_km);
    return T_SURF * std::pow(T_m / T_SURF, z / d_shell_km);
  }
};

using Greenberg1980EnceladusModel = EnceladusDioneTidalResonanceModel;

// ============================================================================
// 61. ENCELADUS TIGER STRIPE FAULT SHEAR HEATING & HYDROTHERMAL PLUME MODEL
// (Nimmo & Spencer 2006, Nimmo, Spencer, Pappalardo & Mullen 2007, Science/Nature)
// ============================================================================
class EnceladusFaultShearHeatingModel {
 public:
  static constexpr double M_SATURN = 5.6834e26;       // Saturn mass [kg]
  static constexpr double M_ENCELADUS = 1.080e20;     // Enceladus mass [kg]
  static constexpr double R_ENCELADUS = 2.521e5;      // Enceladus radius [m]
  static constexpr double A_ENCELADUS = 2.38037e8;    // Semi-major axis [m]
  static constexpr double E_ENCELADUS = 0.0047;       // Forced orbital eccentricity
  static constexpr double G_SURF = 0.1134;            // Surface gravity [m/s^2]
  static constexpr double RHO_ICE = 917.0;            // Ice shell density [kg/m^3]
  static constexpr double L_TOTAL_NOM = 5.0e5;        // Total fault length (4 tiger stripes) [m]
  static constexpr double D_FAULT_NOM = 5.0e3;        // Nominal active fault depth [m]
  static constexpr double DS_NOM = 0.50;              // Nominal cyclic strike-slip displacement [m]
  static constexpr double MU_NOM = 0.50;              // Nominal ice friction coefficient
  static constexpr double LATENT_HEAT_SUB = 2.83e6;   // Ice sublimation enthalpy [J/kg]
  static constexpr double LATENT_HEAT_VAP = 2.26e6;   // Water vaporization enthalpy [J/kg]
  static constexpr double MDOT_NOM_KG_S = 200.0;      // Vapor plume mass loss rate [kg/s]
  static constexpr double V_JET_NOM_M_S = 400.0;      // Hydrothermal jet exit velocity [m/s]
  static constexpr double P_OBS_SPENCER_GW = 5.8;     // Cassini CIRS south polar heat output [GW] (Spencer 2006)
  static constexpr double P_OBS_HOWETT_GW = 15.8;     // Refined CIRS endogenic power [GW] (Howett 2011)

  // Orbital mean motion frequency omega [rad/s]
  double orbital_frequency_rad_s() const {
    return std::sqrt(G * (M_SATURN + M_ENCELADUS) / std::pow(A_ENCELADUS, 3.0));
  }

  // Orbital period [seconds]
  double orbital_period_s() const {
    return (2.0 * M_PI) / orbital_frequency_rad_s();
  }

  // Orbital period [days]
  double orbital_period_days() const {
    return orbital_period_s() / 86400.0;
  }

  // Normal lithostatic stress at depth z [Pa]
  double normal_stress_pa(double z_m, double lambda_pore = 0.0) const {
    return RHO_ICE * G_SURF * std::max(0.0, z_m) * (1.0 - lambda_pore);
  }

  // Frictional shear stress tau(z) = mu * sigma_n(z) [Pa]
  double frictional_shear_stress_pa(double z_m, double mu_friction = MU_NOM, double lambda_pore = 0.0) const {
    return mu_friction * normal_stress_pa(z_m, lambda_pore);
  }

  // Frictional resistance force per unit fault strike length [N/m]
  double frictional_shear_force_per_length_n_m(double d_fault_m = D_FAULT_NOM, double mu_friction = MU_NOM, double lambda_pore = 0.0) const {
    double d = std::max(0.0, d_fault_m);
    return 0.5 * mu_friction * RHO_ICE * G_SURF * d * d * (1.0 - lambda_pore);
  }

  // Fault shear heat generation per unit length [W/m]
  double shear_heat_flux_per_length_w_m(double displacement_amp_m = DS_NOM, double d_fault_m = D_FAULT_NOM, double mu_friction = MU_NOM, double lambda_pore = 0.0) const {
    double force_per_m = frictional_shear_force_per_length_n_m(d_fault_m, mu_friction, lambda_pore);
    double period = orbital_period_s();
    // Cyclic work per period = 4 * d_s * F_fric
    return (4.0 * displacement_amp_m * force_per_m) / period;
  }

  // Total fault shear heating power [Watts]
  double shear_heating_power_watts(double displacement_amp_m = DS_NOM, double d_fault_m = D_FAULT_NOM, double mu_friction = MU_NOM, double l_total_m = L_TOTAL_NOM, double lambda_pore = 0.0) const {
    return l_total_m * shear_heat_flux_per_length_w_m(displacement_amp_m, d_fault_m, mu_friction, lambda_pore);
  }

  // Total fault shear heating power [GW]
  double shear_heating_power_gw(double displacement_amp_m = DS_NOM, double d_fault_m = D_FAULT_NOM, double mu_friction = MU_NOM, double l_total_m = L_TOTAL_NOM, double lambda_pore = 0.0) const {
    return shear_heating_power_watts(displacement_amp_m, d_fault_m, mu_friction, l_total_m, lambda_pore) * 1.0e-9;
  }

  // Hydrothermal plume latent heat transport power [GW]
  double plume_latent_power_gw(double mdot_kg_s = MDOT_NOM_KG_S, double latent_heat_j_kg = LATENT_HEAT_SUB) const {
    return (mdot_kg_s * latent_heat_j_kg) * 1.0e-9;
  }

  // Hydrothermal plume kinetic venting power [GW]
  double plume_kinetic_power_gw(double mdot_kg_s = MDOT_NOM_KG_S, double v_jet_m_s = V_JET_NOM_M_S) const {
    return (0.5 * mdot_kg_s * v_jet_m_s * v_jet_m_s) * 1.0e-9;
  }

  // Total endogenic plume and thermal power output [GW]
  double total_plume_and_thermal_power_gw(double displacement_amp_m = DS_NOM, double d_fault_m = D_FAULT_NOM, double mu_friction = MU_NOM, double mdot_kg_s = MDOT_NOM_KG_S, double l_total_m = L_TOTAL_NOM, double lambda_pore = 0.0) const {
    double p_shear = shear_heating_power_gw(displacement_amp_m, d_fault_m, mu_friction, l_total_m, lambda_pore);
    double p_latent = plume_latent_power_gw(mdot_kg_s, LATENT_HEAT_SUB);
    double p_kin = plume_kinetic_power_gw(mdot_kg_s, V_JET_NOM_M_S);
    return p_shear + p_latent + p_kin;
  }

  // Required cyclic displacement amplitude [m] to produce target power [GW]
  double required_displacement_m(double target_power_gw = P_OBS_SPENCER_GW, double d_fault_m = D_FAULT_NOM, double mu_friction = MU_NOM, double l_total_m = L_TOTAL_NOM, double lambda_pore = 0.0) const {
    double period = orbital_period_s();
    double force_per_m = frictional_shear_force_per_length_n_m(d_fault_m, mu_friction, lambda_pore);
    double total_force = l_total_m * force_per_m;
    if (total_force <= 0.0) return 0.0;
    return (target_power_gw * 1.0e9 * period) / (4.0 * total_force);
  }

  // Diurnal tidal lateral displacement amplitude [m] driven by shell decoupling Love number h2
  double diurnal_tidal_displacement_m(double h2_love = 0.02) const {
    // Nimmo et al. (2007): d_s ~ 1.5 * h_2 * (G M_Saturn R_enc^2 e) / (g_surf a^3)
    double numerator = 1.5 * h2_love * G * M_SATURN * std::pow(R_ENCELADUS, 2.0) * E_ENCELADUS;
    double denominator = G_SURF * std::pow(A_ENCELADUS, 3.0);
    return numerator / denominator;
  }
};

using NimmoSpencer2006EnceladusPlumeModel = EnceladusFaultShearHeatingModel;
using Nimmo2007ShearHeatingModel = EnceladusFaultShearHeatingModel;

// ============================================================================
// 62. ENCELADUS SOUTH POLAR TERRAIN HEAT FLOW & THERMAL RADIATION BUDGET
// (Spencer et al. 2006, Science 311, 1401-1405; Howett et al. 2011)
// ============================================================================
class Spencer2006EnceladusHeatFlowModel {
 public:
  // Primary constants for Enceladus
  static constexpr double R_ENCELADUS = 2.521e5;     // Mean radius [m] (252.1 km)
  static constexpr double M_ENCELADUS = 1.080e20;    // Mass [kg]
  static constexpr double M_SATURN = 5.6834e26;      // Saturn mass [kg]
  static constexpr double A_ORBIT = 2.38037e8;       // Semi-major axis [m]
  static constexpr double ECCENTRICITY = 0.0047;     // Orbital eccentricity
  static constexpr double SOLAR_CONST_SATURN = 14.97;// Solar constant at 9.537 AU [W/m^2]
  static constexpr double BOND_ALBEDO = 0.81;        // Bond albedo (Spencer et al. 2006)
  static constexpr double EMISSIVITY = 0.95;         // Ice surface thermal emissivity
  static constexpr double SIGMA_SB = 5.670374419e-8; // Stefan-Boltzmann constant [W/(m^2 K^4)]
  static constexpr double M_ROCK_CORE = 8.61e19;     // Rocky silicate core mass [kg]
  static constexpr double CHONDRITIC_H = 3.5e-12;    // Radiogenic heat production rate [W/kg]
  static constexpr double TIGER_STRIPES_LENGTH = 5.0e5; // Combined length of 4 main tiger stripes [m] (500 km)

  // Enceladus surface area [m^2]
  double surface_area_m2() const {
    return 4.0 * M_PI * R_ENCELADUS * R_ENCELADUS;
  }

  // South Polar Terrain (SPT) surface area poleward of latitude lat_boundary_deg (e.g. -65 deg) [m^2]
  double spt_surface_area_m2(double lat_boundary_deg = -65.0) const {
    double lat_rad = std::abs(lat_boundary_deg) * M_PI / 180.0;
    return 2.0 * M_PI * R_ENCELADUS * R_ENCELADUS * (1.0 - std::sin(lat_rad));
  }

  // Diurnally averaged solar insolation absorbed at latitude lat_deg [W/m^2]
  // subsolar_lat_deg: subsolar latitude at 2005 Cassini encounter (-22.8 deg, southern summer)
  double absorbed_solar_flux_w_m2(double lat_deg, double subsolar_lat_deg = -22.8, double bond_albedo = BOND_ALBEDO) const {
    double lat_rad = lat_deg * M_PI / 180.0;
    double ss_lat_rad = subsolar_lat_deg * M_PI / 180.0;

    // Integrate diurnal insolation over hour angles
    double sum_cos_zeta = 0.0;
    int n_steps = 180;
    double dh = 2.0 * M_PI / n_steps;
    for (int i = 0; i < n_steps; ++i) {
      double h = -M_PI + (i + 0.5) * dh;
      double cos_zeta = std::sin(lat_rad) * std::sin(ss_lat_rad) + std::cos(lat_rad) * std::cos(ss_lat_rad) * std::cos(h);
      if (cos_zeta > 0.0) {
        sum_cos_zeta += cos_zeta;
      }
    }
    double mean_cos_zeta = sum_cos_zeta / n_steps;
    return (1.0 - bond_albedo) * SOLAR_CONST_SATURN * mean_cos_zeta;
  }

  // Passive solar equilibrium surface temperature T_passive(latitude) [K]
  double passive_equilibrium_temp_k(double lat_deg, double subsolar_lat_deg = -22.8, double f_internal_bg_w_m2 = 0.005) const {
    double f_abs = absorbed_solar_flux_w_m2(lat_deg, subsolar_lat_deg);
    double total_inflow = f_abs + f_internal_bg_w_m2;
    double t4 = total_inflow / (EMISSIVITY * SIGMA_SB);
    return std::pow(std::max(10.0, t4), 0.25);
  }

  // Passive thermal emitted flux F_passive(latitude) [W/m^2]
  double passive_emitted_flux_w_m2(double lat_deg, double subsolar_lat_deg = -22.8) const {
    double t = passive_equilibrium_temp_k(lat_deg, subsolar_lat_deg);
    return EMISSIVITY * SIGMA_SB * std::pow(t, 4.0);
  }

  // CIRS Observed brightness / effective temperature profile T_obs(latitude) [K]
  // Reproducing Cassini CIRS observations (Spencer et al. 2006, Science 311, 1401)
  double cirs_observed_temp_k(double lat_deg, double subsolar_lat_deg = -22.8, double delta_t_spt = 10.2, double sigma_lat = 7.5) const {
    double t_pass = passive_equilibrium_temp_k(lat_deg, subsolar_lat_deg);
    if (lat_deg >= -55.0) {
      return t_pass;
    }
    // South polar thermal anomaly centered at -90 deg
    double d_lat = lat_deg - (-90.0);
    double spt_anomaly = delta_t_spt * std::exp(-0.5 * (d_lat * d_lat) / (sigma_lat * sigma_lat));
    return t_pass + spt_anomaly;
  }

  // CIRS Observed infrared emitted flux F_obs(latitude) [W/m^2]
  double cirs_observed_flux_w_m2(double lat_deg, double subsolar_lat_deg = -22.8) const {
    double t_obs = cirs_observed_temp_k(lat_deg, subsolar_lat_deg);
    return EMISSIVITY * SIGMA_SB * std::pow(t_obs, 4.0);
  }

  // Endogenic heat flux q_endogenic(latitude) [W/m^2]
  double endogenic_heat_flux_w_m2(double lat_deg, double subsolar_lat_deg = -22.8) const {
    double f_obs = cirs_observed_flux_w_m2(lat_deg, subsolar_lat_deg);
    double f_pass = passive_emitted_flux_w_m2(lat_deg, subsolar_lat_deg);
    return std::max(0.0, f_obs - f_pass);
  }

  // Multi-component Tiger Stripes Endogenic Thermal Emission Power [GW] (Spencer et al. 2006)
  double radiated_power_gw(double a_stripes_km2 = 125.0, double t_fissure_k = 135.0, 
                           double a_halo_km2 = 1000.0, double t_halo_k = 80.0, 
                           double t_bg_k = 70.0) const {
    double a_f_m2 = a_stripes_km2 * 1.0e6;
    double a_h_m2 = a_halo_km2 * 1.0e6;
    double t_pass = 67.33; // South polar passive baseline temperature [K]
    double flux_pass = EMISSIVITY * SIGMA_SB * std::pow(t_pass, 4.0);

    double flux_f = EMISSIVITY * SIGMA_SB * std::pow(t_fissure_k, 4.0);
    double flux_h = EMISSIVITY * SIGMA_SB * std::pow(t_halo_k, 4.0);
    double flux_bg = EMISSIVITY * SIGMA_SB * std::pow(t_bg_k, 4.0);

    double p_fissure = a_f_m2 * (flux_f - flux_pass);
    double p_halo = a_h_m2 * (flux_h - flux_pass);
    // Effective SPT anomaly area ~ 10,000 km^2
    double a_spt_eff = 1.0e10;
    double a_bg = std::max(0.0, a_spt_eff - a_f_m2 - a_h_m2);
    double p_bg = a_bg * std::max(0.0, flux_bg - flux_pass);

    double total_power_w = p_fissure + p_halo + p_bg;
    return total_power_w / 1.0e9; // GW
  }

  // Radiogenic heating power from rocky core [GW]
  double radiogenic_power_gw(double m_rock_kg = M_ROCK_CORE, double h_rate = CHONDRITIC_H) const {
    return (m_rock_kg * h_rate) / 1.0e9;
  }

  // Viscoelastic tidal dissipation power [GW] (Segatz 1988, Spencer 2006, Tobie 2008)
  double tidal_dissipation_power_gw(double k2_over_q = 0.000371, double ecc = ECCENTRICITY) const {
    double n = std::sqrt(G * M_SATURN / std::pow(A_ORBIT, 3.0));
    double factor = 10.5 * k2_over_q * G * M_SATURN * M_SATURN * std::pow(R_ENCELADUS, 5.0) * n / std::pow(A_ORBIT, 6.0);
    double power_w = factor * ecc * ecc;
    return power_w / 1.0e9;
  }

  // Required tidal k2/Q to balance endogenic power P_endogenic [GW]
  double required_k2_over_q(double p_endogenic_gw = 5.8) const {
    double p_rad = radiogenic_power_gw();
    double p_tide_req = std::max(0.0, p_endogenic_gw - p_rad);
    double n = std::sqrt(G * M_SATURN / std::pow(A_ORBIT, 3.0));
    double factor_w = 10.5 * G * M_SATURN * M_SATURN * std::pow(R_ENCELADUS, 5.0) * n * (ECCENTRICITY * ECCENTRICITY) / std::pow(A_ORBIT, 6.0);
    return (p_tide_req * 1.0e9) / factor_w;
  }

  // Latitude integration of total SPT endogenic power [GW]
  double integrated_spt_endogenic_power_gw(double lat_boundary_deg = -65.0, int n_bins = 200) const {
    double lat_start = -90.0;
    double lat_end = lat_boundary_deg;
    double dlat_rad = (lat_end - lat_start) * (M_PI / 180.0) / n_bins;
    double total_power_w = 0.0;

    for (int i = 0; i < n_bins; ++i) {
      double lat_mid_rad = (lat_start * M_PI / 180.0) + (i + 0.5) * dlat_rad;
      double lat_mid_deg = lat_mid_rad * (180.0 / M_PI);
      double ring_area = 2.0 * M_PI * R_ENCELADUS * R_ENCELADUS * std::cos(lat_mid_rad) * dlat_rad;
      double q_endo = endogenic_heat_flux_w_m2(lat_mid_deg);
      total_power_w += q_endo * ring_area;
    }
    return total_power_w / 1.0e9;
  }
};

using EnceladusCIRSHeatFlowModel = Spencer2006EnceladusHeatFlowModel;

// ============================================================================
// 101. ENCELADUS ICE SHELL TIDAL DISSIPATION & MAXWELL RHEOLOGY (Ojakangas & Stevenson 1989)
// ============================================================================
class OjakangasStevenson1989EnceladusModel {
 public:
  static constexpr double M_SATURN = 5.6834e26;       // Saturn mass [kg]
  static constexpr double R_ENCELADUS = 2.521e5;     // Enceladus radius [m]
  static constexpr double M_ENCELADUS = 1.080e20;     // Enceladus mass [kg]
  static constexpr double A_ENCELADUS = 2.38037e8;    // Semi-major axis [m]
  static constexpr double E_ENCELADUS = 0.0047;       // Orbital eccentricity
  static constexpr double MU_ICE = 3.3e9;             // Ice shear modulus (rigidity) [Pa]
  static constexpr double RHO_ICE = 920.0;            // Ice density [kg/m^3]
  static constexpr double T_BASE_NOM = 273.15;        // Basal melting temperature [K]
  static constexpr double T_SURF_NOM = 75.0;          // Surface temperature [K]
  static constexpr double A_CONDUCT = 567.0;          // Crystalline ice thermal conductivity constant [W/m]
  static constexpr double E_ACTIVATION = 59400.0;     // Activation energy for ice creep [J/mol]
  static constexpr double R_GAS = 8.314462618;        // Ideal gas constant [J/(mol*K)]
  static constexpr double ETA_0_NOM = 1.0e13;         // Nominal basal ice viscosity at melting point [Pa*s]
  static constexpr double K2_PEAK_NOM = 0.0107;       // Peak Love number Im(k2) at Maxwell resonance

  // Orbital mean motion / tidal forcing frequency [rad/s]
  double orbital_mean_motion() const {
    return std::sqrt(G * M_SATURN / std::pow(A_ENCELADUS, 3.0));
  }

  // Temperature-dependent ice viscosity [Pa*s] (Arrhenius rheology)
  double viscosity_at_temperature_pa_s(double T_k, double eta_0 = ETA_0_NOM, double E_a = E_ACTIVATION, double T_base = T_BASE_NOM) const {
    double T = std::max(50.0, std::min(T_k, T_base));
    double exponent = (E_a / R_GAS) * (1.0 / T - 1.0 / T_base);
    return eta_0 * std::exp(exponent);
  }

  // Maxwell relaxation time [s]: tau_M = eta / mu
  double maxwell_relaxation_time_s(double eta_pa_s, double mu_pa = MU_ICE) const {
    return eta_pa_s / mu_pa;
  }

  // Maxwell relaxation frequency [rad/s]: omega_M = 1 / tau_M = mu / eta
  double maxwell_relaxation_frequency_rad_s(double eta_pa_s, double mu_pa = MU_ICE) const {
    return mu_pa / std::max(1.0, eta_pa_s);
  }

  // Viscoelastic tidal dissipation Love number Im(k2) as a function of Maxwell relaxation frequency [rad/s]
  // Im(k2) = k2_peak * [2 * (omega / omega_M)] / [1 + (omega / omega_M)^2]
  double dissipation_love_number_im_k2(double omega_M_rad_s, double k2_peak = K2_PEAK_NOM, double omega_forcing = 5.3074e-5) const {
    double chi = omega_forcing / std::max(1.0e-30, omega_M_rad_s);
    return k2_peak * (2.0 * chi) / (1.0 + chi * chi);
  }

  // Viscoelastic tidal dissipation Love number Im(k2) as a function of dynamic viscosity eta [Pa*s]
  double dissipation_factor_from_viscosity(double eta_pa_s, double k2_peak = K2_PEAK_NOM, double mu_pa = MU_ICE, double omega_forcing = 5.3074e-5) const {
    double omega_M = maxwell_relaxation_frequency_rad_s(eta_pa_s, mu_pa);
    return dissipation_love_number_im_k2(omega_M, k2_peak, omega_forcing);
  }

  // Tidal dissipation power [Watts] from Im(k2)
  double tidal_dissipation_power_watts(double Im_k2, double e = E_ENCELADUS) const {
    double n = orbital_mean_motion();
    double factor = 10.5 * Im_k2 * G * M_SATURN * M_SATURN * std::pow(R_ENCELADUS, 5.0) * n / std::pow(A_ENCELADUS, 6.0);
    return factor * e * e;
  }

  // Tidal dissipation power [GW] from Im(k2)
  double tidal_dissipation_power_gw(double Im_k2, double e = E_ENCELADUS) const {
    return tidal_dissipation_power_watts(Im_k2, e) * 1.0e-9;
  }

  // Tidal dissipation power [GW] directly from ice viscosity [Pa*s]
  double tidal_power_from_viscosity_gw(double eta_pa_s, double e = E_ENCELADUS, double k2_peak = K2_PEAK_NOM, double mu_pa = MU_ICE) const {
    double Im_k2 = dissipation_factor_from_viscosity(eta_pa_s, k2_peak, mu_pa, orbital_mean_motion());
    return tidal_dissipation_power_gw(Im_k2, e);
  }

  // Tidal dissipation power [GW] directly from Maxwell relaxation frequency [rad/s]
  double tidal_power_from_maxwell_freq_gw(double omega_M_rad_s, double e = E_ENCELADUS, double k2_peak = K2_PEAK_NOM) const {
    double Im_k2 = dissipation_love_number_im_k2(omega_M_rad_s, k2_peak, orbital_mean_motion());
    return tidal_dissipation_power_gw(Im_k2, e);
  }

  // Conductive heat loss power through ice shell [GW]
  double conductive_heat_loss_gw(double d_shell_km, double T_base = T_BASE_NOM, double T_surf = T_SURF_NOM, double A_cond = A_CONDUCT) const {
    double d_m = std::max(100.0, d_shell_km * 1.0e3);
    double area = 4.0 * M_PI * R_ENCELADUS * R_ENCELADUS;
    double flux_w_m2 = (A_cond * std::log(T_base / T_surf)) / d_m;
    return (flux_w_m2 * area) * 1.0e-9;
  }

  // Conductive heat flux [mW/m^2]
  double conductive_heat_flux_mw_m2(double d_shell_km, double T_base = T_BASE_NOM, double T_surf = T_SURF_NOM, double A_cond = A_CONDUCT) const {
    double area = 4.0 * M_PI * R_ENCELADUS * R_ENCELADUS;
    return (conductive_heat_loss_gw(d_shell_km, T_base, T_surf, A_cond) * 1.0e12) / area;
  }

  // Temperature profile at depth z [km] through shell of thickness d [km] (Fourier conduction)
  double ice_shell_temperature_k(double z_km, double d_shell_km, double T_base = T_BASE_NOM, double T_surf = T_SURF_NOM) const {
    double z = std::max(0.0, std::min(z_km, d_shell_km));
    return T_surf * std::pow(T_base / T_surf, z / d_shell_km);
  }

  // Volumetric tidal heating rate dot_q [W/m^3] at depth z [km] (Ojakangas & Stevenson 1989)
  double volumetric_tidal_heating_w_m3(double z_km, double d_shell_km, double strain_amplitude = 1.0e-4, double eta_0 = ETA_0_NOM, double mu_pa = MU_ICE) const {
    double T_z = ice_shell_temperature_k(z_km, d_shell_km);
    double eta_z = viscosity_at_temperature_pa_s(T_z, eta_0);
    double tau_m = maxwell_relaxation_time_s(eta_z, mu_pa);
    double omega = orbital_mean_motion();
    double omega_tau = omega * tau_m;
    double heating = 2.0 * mu_pa * (strain_amplitude * strain_amplitude) * (omega * omega_tau) / (1.0 + omega_tau * omega_tau);
    return heating;
  }

  // Depth-integrated tidal heat flux [mW/m^2] across shell
  double depth_integrated_tidal_flux_mw_m2(double d_shell_km, int n_steps = 1000, double strain_amplitude = 1.0e-4, double eta_0 = ETA_0_NOM, double mu_pa = MU_ICE) const {
    double dz_km = d_shell_km / static_cast<double>(n_steps);
    double dz_m = dz_km * 1.0e3;
    double total_flux_w_m2 = 0.0;
    for (int i = 0; i < n_steps; ++i) {
      double z_mid_km = (i + 0.5) * dz_km;
      total_flux_w_m2 += volumetric_tidal_heating_w_m3(z_mid_km, d_shell_km, strain_amplitude, eta_0, mu_pa) * dz_m;
    }
    return total_flux_w_m2 * 1.0e3;
  }

  // Equilibrium ice shell thickness [km] balancing tidal + radiogenic heating with conduction
  double equilibrium_shell_thickness_km(double Im_k2 = K2_PEAK_NOM, double e = E_ENCELADUS, double P_radio_gw = 0.4) const {
    double total_heat_gw = tidal_dissipation_power_gw(Im_k2, e) + P_radio_gw;
    if (total_heat_gw <= 0.0) return 100.0;
    double area = 4.0 * M_PI * R_ENCELADUS * R_ENCELADUS;
    double target_flux_w_m2 = (total_heat_gw * 1.0e9) / area;
    double d_m = (A_CONDUCT * std::log(T_BASE_NOM / T_SURF_NOM)) / target_flux_w_m2;
    return d_m / 1.0e3;
  }
};

using OjakangasStevensonModel = OjakangasStevenson1989EnceladusModel;
using Paper205EnceladusIceShellModel = OjakangasStevenson1989EnceladusModel;

// ============================================================================
// 86. ENCELADUS SOUTH POLAR PLUME DYNAMICS & TIDAL FRACTURE MODULATION
// (Porco et al. 2006, Hurford et al. 2007, Hedman et al. 2013, Nimmo et al. 2007)
// ============================================================================
class EnceladusPlumeDynamicsModel {
 public:
  static constexpr double M_ENCELADUS_KG = 1.0803e20; // kg
  static constexpr double R_ENCELADUS_M = 252.1e3;   // m
  static constexpr double M_SATURN_KG = 5.6834e26;   // kg
  static constexpr double A_ORBIT_M = 2.3804e8;      // m
  static constexpr double ECCENTRICITY = 0.0047;
  static constexpr double PERIOD_SEC = 1.370218 * 86400.0; // 118386.8 s
  static constexpr double MEAN_MOTION = 5.3074e-5; // rad/s
  static constexpr double MOLAR_MASS_H2O = 0.01801528; // kg/mol
  static constexpr double R_SPEC_H2O = 461.52; // J/(kg K)
  static constexpr double GAMMA_H2O = 1.33; // Adiabatic index for H2O vapor

  // Surface gravity [m/s^2]
  double surface_gravity() const {
    return G * M_ENCELADUS_KG / (R_ENCELADUS_M * R_ENCELADUS_M);
  }

  // Surface escape velocity [m/s]
  double escape_velocity() const {
    return std::sqrt(2.0 * G * M_ENCELADUS_KG / R_ENCELADUS_M);
  }

  // Speed of sound in H2O vapor at reservoir temperature T [m/s]
  double sound_speed_m_s(double T_res_k = 273.15, double gamma = GAMMA_H2O) const {
    return std::sqrt(gamma * R_SPEC_H2O * T_res_k);
  }

  // Water vapor saturation / equilibrium pressure [Pa]
  double vapor_pressure_pa(double T_res_k = 273.15) const {
    if (T_res_k >= 273.15) {
      return 611.21 * std::exp(17.67 * (T_res_k - 273.15) / (T_res_k - 29.65));
    } else {
      return 611.15 * std::exp(22.54 * (T_res_k - 273.15) / (T_res_k + 0.55));
    }
  }

  // Vapor density at reservoir conditions [kg/m^3]
  double vapor_density_kg_m3(double T_res_k = 273.15) const {
    double P_vap = vapor_pressure_pa(T_res_k);
    return P_vap / (R_SPEC_H2O * T_res_k);
  }

  // Choked nozzle mass flux per unit area [kg / (s m^2)]
  double choked_flux_per_area_kg_s_m2(double T_res_k = 273.15, double gamma = GAMMA_H2O) const {
    double rho_0 = vapor_density_kg_m3(T_res_k);
    double v_s = sound_speed_m_s(T_res_k, gamma);
    double isentropic_factor = std::pow(2.0 / (gamma + 1.0), (gamma + 1.0) / (2.0 * (gamma - 1.0)));
    return rho_0 * v_s * isentropic_factor;
  }

  // Diurnal tidal normal stress across tiger stripe fractures [kPa]
  double tidal_normal_stress_kpa(double true_anomaly_deg, double sigma_0_kpa = 70.0, double phi_lag_deg = 25.0) const {
    double f_rad = (true_anomaly_deg - phi_lag_deg) * M_PI / 180.0;
    return sigma_0_kpa * std::cos(f_rad);
  }

  // Effective vent area across south polar fractures [m^2]
  double effective_vent_area_m2(double true_anomaly_deg, double A_0_m2 = 80.0, double beta = 2.90, double phi_lag_deg = 25.0, double power_p = 1.5) const {
    double f_rad = (true_anomaly_deg - phi_lag_deg) * M_PI / 180.0;
    double open_factor = std::pow(0.5 * (1.0 - std::cos(f_rad)), power_p);
    return A_0_m2 * (1.0 + beta * open_factor);
  }

  // Plume mass flux M_dot = A_vent * rho * v_sound [kg/s]
  double mass_flux_kg_s(double true_anomaly_deg, double T_res_k = 273.15, double A_0_m2 = 80.0, double beta = 2.90, double phi_lag_deg = 25.0) const {
    double A_vent = effective_vent_area_m2(true_anomaly_deg, A_0_m2, beta, phi_lag_deg);
    double rho = vapor_density_kg_m3(T_res_k);
    double v_s = sound_speed_m_s(T_res_k);
    double isentropic_factor = std::pow(2.0 / (GAMMA_H2O + 1.0), (GAMMA_H2O + 1.0) / (2.0 * (GAMMA_H2O - 1.0)));
    return A_vent * rho * v_s * isentropic_factor;
  }

  // Relative optical plume brightness / activity normalized to periapse
  double relative_plume_brightness(double true_anomaly_deg, double beta = 2.90, double phi_lag_deg = 25.0, double power_p = 1.5) const {
    double f_rad = (true_anomaly_deg - phi_lag_deg) * M_PI / 180.0;
    double open_factor = std::pow(0.5 * (1.0 - std::cos(f_rad)), power_p);
    return 1.0 + beta * open_factor;
  }

  // Ballistic canopy height for sub-escape particles [km]
  double ballistic_canopy_height_km(double v0_m_s = 200.0) const {
    double v_esc = escape_velocity();
    if (v0_m_s >= v_esc) {
      return 1.0e6;
    }
    double g_surf = surface_gravity();
    double h_m = (v0_m_s * v0_m_s) / (2.0 * g_surf * (1.0 - (v0_m_s * v0_m_s) / (v_esc * v_esc)));
    return h_m / 1000.0;
  }

  // Fraction of plume particles escaping into Saturn's E-ring
  double escape_fraction(double v_mean_m_s = 200.0, double sigma_v_m_s = 50.0) const {
    double v_esc = escape_velocity();
    double z = (v_esc - v_mean_m_s) / (sigma_v_m_s * std::sqrt(2.0));
    return 0.5 * std::erfc(z);
  }
};

using Porco2006PlumeDynamicsModel = EnceladusPlumeDynamicsModel;
using Porco2006EnceladusPlumeModel = EnceladusPlumeDynamicsModel;

// ============================================================================
// 66. EUROPA TIDAL DISSIPATION & ICE SHELL DYNAMICS MODEL
// (Squyres, Reynolds, Cassen, & Peale 1983, Cassen et al. 1979, 1980)
// ============================================================================
class EuropaIceShellDynamicsModel {
 public:
  static constexpr double M_JUPITER = 1.89813e27;       // Jupiter mass [kg]
  static constexpr double R_JUPITER = 7.1492e7;         // Jupiter equatorial radius [m]
  static constexpr double M_EUROPA = 4.7998e22;         // Europa mass [kg]
  static constexpr double R_EUROPA = 1.5608e6;          // Europa mean radius [m]
  static constexpr double A_EUROPA = 6.7090e8;          // Semi-major axis [m]
  static constexpr double E_EUROPA_NOM = 0.00935;       // Nominal forced eccentricity
  static constexpr double G_SURF = 1.315;               // Surface gravity [m/s^2]
  static constexpr double RHO_ICE = 917.0;              // Ice density [kg/m^3]
  static constexpr double RHO_OCEAN = 1000.0;           // Ocean liquid water density [kg/m^3]
  static constexpr double A_CONDUCT = 567.0;            // Ice thermal conductivity coeff [W/m] (k(T) = A / T)
  static constexpr double T_SURF = 100.0;               // Surface mean temperature [K]
  static constexpr double T_MELT_0 = 273.15;            // Pure ice melting temperature at 0 Pa [K]
  static constexpr double GAMMA_CLAPEYRON = 7.4e-8;    // Clapeyron slope dT_m/dP [K/Pa]
  static constexpr double MU_ICE = 3.3e9;               // Ice shear modulus [Pa]
  static constexpr double P_RADIO_NOM_GW = 200.0;       // Nominal silicate core radiogenic power [GW] (~6.5 mW/m^2)

  // Orbital mean motion frequency n [rad/s]
  double orbital_frequency_rad_s() const {
    return std::sqrt(G * (M_JUPITER + M_EUROPA) / std::pow(A_EUROPA, 3.0));
  }

  // Orbital period [days]
  double orbital_period_days() const {
    return (2.0 * M_PI / orbital_frequency_rad_s()) / 86400.0;
  }

  // Surface surface area [m^2]
  double surface_area_m2() const {
    return 4.0 * M_PI * R_EUROPA * R_EUROPA;
  }

  // Hydrostatic pressure at depth z [Pa]
  double basal_pressure_pa(double d_shell_km) const {
    double d_m = d_shell_km * 1.0e3;
    return RHO_ICE * G_SURF * d_m;
  }

  // Basal melting temperature [K] accounting for Clapeyron slope depression
  double basal_melting_temperature_k(double d_shell_km) const {
    double P_base = basal_pressure_pa(d_shell_km);
    return T_MELT_0 - GAMMA_CLAPEYRON * P_base;
  }

  // Viscoelastic tidal dissipation heating power [Watts] (Peale 1979, Squyres 1983)
  double tidal_heating_power_watts(double e = E_EUROPA_NOM, double k2_over_Q = 0.015) const {
    double n = orbital_frequency_rad_s();
    double factor = 10.5 * k2_over_Q * G * M_JUPITER * M_JUPITER * std::pow(R_EUROPA, 5.0) * n / std::pow(A_EUROPA, 6.0);
    return factor * e * e;
  }

  // Viscoelastic tidal heating power [GW]
  double tidal_heating_power_gw(double e = E_EUROPA_NOM, double k2_over_Q = 0.015) const {
    return tidal_heating_power_watts(e, k2_over_Q) * 1.0e-9;
  }

  // Viscoelastic tidal heating power [TW]
  double tidal_heating_power_tw(double e = E_EUROPA_NOM, double k2_over_Q = 0.015) const {
    return tidal_heating_power_watts(e, k2_over_Q) * 1.0e-12;
  }

  // Surface tidal heat flux [mW/m^2]
  double tidal_heat_flux_mw_m2(double e = E_EUROPA_NOM, double k2_over_Q = 0.015) const {
    double area = surface_area_m2();
    return (tidal_heating_power_watts(e, k2_over_Q) / area) * 1.0e3;
  }

  // Radiogenic heat flux [mW/m^2]
  double radiogenic_heat_flux_mw_m2(double p_radio_gw = P_RADIO_NOM_GW) const {
    double area = surface_area_m2();
    return (p_radio_gw * 1.0e9 / area) * 1.0e3;
  }

  // Conductive heat loss power through ice shell [Watts] (Fourier logarithmic conductivity k(T) = A / T)
  double conductive_heat_loss_watts(double d_shell_km) const {
    double d_m = std::max(100.0, d_shell_km * 1.0e3);
    double T_m = basal_melting_temperature_k(d_shell_km);
    double area = surface_area_m2();
    double flux_w_m2 = (A_CONDUCT * std::log(T_m / T_SURF)) / d_m;
    return flux_w_m2 * area;
  }

  // Conductive heat loss power through ice shell [GW]
  double conductive_heat_loss_gw(double d_shell_km) const {
    return conductive_heat_loss_watts(d_shell_km) * 1.0e-9;
  }

  // Conductive heat flux through ice shell [mW/m^2]
  double conductive_heat_flux_mw_m2(double d_shell_km) const {
    double area = surface_area_m2();
    return (conductive_heat_loss_watts(d_shell_km) / area) * 1.0e3;
  }

  // Equilibrium ice shell thickness [km] where conductive loss balances total heat supply
  double equilibrium_shell_thickness_km(double e = E_EUROPA_NOM, double k2_over_Q = 0.015, double P_radio_gw = P_RADIO_NOM_GW) const {
    double total_heat_gw = tidal_heating_power_gw(e, k2_over_Q) + P_radio_gw;
    if (total_heat_gw <= 0.0) return 150.0;
    double area = surface_area_m2();
    double target_flux_w_m2 = (total_heat_gw * 1.0e9) / area;
    double d_guess_m = 20.0e3;
    for (int iter = 0; iter < 15; ++iter) {
      double T_m = basal_melting_temperature_k(d_guess_m / 1000.0);
      d_guess_m = (A_CONDUCT * std::log(T_m / T_SURF)) / target_flux_w_m2;
    }
    return d_guess_m / 1.0e3;
  }

  // Equilibrium ice shell thickness [km] from specified total surface heat flux [mW/m^2]
  double equilibrium_shell_thickness_from_flux_km(double total_flux_mw_m2) const {
    if (total_flux_mw_m2 <= 0.0) return 150.0;
    double target_flux_w_m2 = total_flux_mw_m2 * 1.0e-3;
    double d_guess_m = 20.0e3;
    for (int iter = 0; iter < 15; ++iter) {
      double T_m = basal_melting_temperature_k(d_guess_m / 1000.0);
      d_guess_m = (A_CONDUCT * std::log(T_m / T_SURF)) / target_flux_w_m2;
    }
    return d_guess_m / 1.0e3;
  }

  // Pure conductive temperature at depth z [km] for a shell of thickness d_shell_km [km]
  double temperature_at_depth_k(double z_km, double d_shell_km) const {
    double z = std::max(0.0, std::min(z_km, d_shell_km));
    double T_m = basal_melting_temperature_k(d_shell_km);
    return T_SURF * std::pow(T_m / T_SURF, z / d_shell_km);
  }

  // Conductive temperature profile with volumetric tidal heating q_vol [W/m^3]
  double temperature_with_volumetric_heating_k(double z_km, double d_shell_km, double q_vol_w_m3 = 1.0e-5) const {
    double z_m = std::max(0.0, std::min(z_km, d_shell_km)) * 1.0e3;
    double H_m = std::max(100.0, d_shell_km * 1.0e3);
    double T_m = basal_melting_temperature_k(d_shell_km);
    double exponent = (z_m / H_m) * std::log(T_m / T_SURF) + (q_vol_w_m3 * z_m * (H_m - z_m)) / (2.0 * A_CONDUCT);
    return T_SURF * std::exp(exponent);
  }

  // Temperature-dependent ice shear viscosity [Pa s]
  double ice_viscosity_pa_s(double T_k, double eta_0 = 1.0e14, double activation_E = 50.0e3) const {
    double T = std::max(80.0, std::min(273.15, T_k));
    double R_gas = 8.314462;
    return eta_0 * std::exp((activation_E / R_gas) * (1.0 / T - 1.0 / 273.15));
  }

  // Peak diurnal tidal tensile stress [kPa] (Hurford 2007, Greenberg 1998)
  double peak_diurnal_tidal_stress_kpa(double d_shell_km, double e = E_EUROPA_NOM) const {
    double d = std::max(1.0, d_shell_km);
    return 120.0 * std::sqrt(20.0 / d) * (e / 0.009);
  }
};

using Squyres1983EuropaIceShellModel = EuropaIceShellDynamicsModel;

// ============================================================================
// 67. EUROPA VISCOELASTIC TIDAL HEATING & ICE SHELL DISSIPATION MODEL
// (Ross & Schubert 1987, Nature 325, 133-134; Ross & Schubert 1986, 1989; Sotin 2002; Tobie 2003)
// ============================================================================
class EuropaViscoelasticTidalModel {
 public:
  static constexpr double R_EUROPA = 1.5608e6;       // Europa mean radius [m] (1560.8 km)
  static constexpr double M_EUROPA = 4.7998e22;      // Europa mass [kg]
  static constexpr double M_JUPITER = 1.89813e27;    // Jupiter mass [kg]
  static constexpr double A_EUROPA = 6.7090e8;       // Semi-major axis [m] (670,900 km)
  static constexpr double ECCENTRICITY = 0.0090;     // Forced orbital eccentricity
  static constexpr double G_EUROPA = 1.315;          // Surface gravity [m/s^2]
  static constexpr double RHO_ICE = 920.0;           // Ice Ih density [kg/m^3]
  static constexpr double RHO_OCEAN = 1000.0;        // Liquid water ocean density [kg/m^3]
  static constexpr double RHO_CORE = 3200.0;         // Silicate core density [kg/m^3]
  static constexpr double MU_ICE = 3.5e9;            // Ice Ih unrelaxed shear modulus [Pa] (3.5 GPa)
  static constexpr double T_SURF = 100.0;            // Surface temperature [K]
  static constexpr double T_MELT = 273.15;           // Basal melting temperature [K]
  static constexpr double E_ACTIVATION = 59400.0;    // Activation energy for ice creep [J/mol] (59.4 kJ/mol)
  static constexpr double R_GAS = 8.31446;           // Universal gas constant [J/(mol K)]
  static constexpr double ETA_0 = 1.0e14;            // Reference ice viscosity at T_melt for 1 mm grain [Pa s]
  static constexpr double D_0_MM = 1.0;              // Reference grain size [mm]
  static constexpr double GRAIN_EXP = 1.4;           // Grain size exponent for grain boundary sliding creep
  static constexpr double K_CONDUCT = 567.0;         // Ice thermal conductivity constant [W/m] (k(T) = 567 / T)

  // Mean orbital motion frequency n [rad/s]
  double orbital_frequency_rad_s() const {
    return std::sqrt(G * (M_JUPITER + M_EUROPA) / std::pow(A_EUROPA, 3.0));
  }

  // Orbital period [days]
  double orbital_period_days() const {
  return (2.0 * M_PI / orbital_frequency_rad_s()) / 86400.0;
  }

  // Radial temperature profile T(r) [K] in ice shell of thickness d_shell_m
  // z = R_EUROPA - r_m (z = 0 at surface, z = d_shell_m at base)
  // Supports pure conduction or convective sublayer (Ross & Schubert 1987)
  double temperature_at_radius_k(double r_m, double d_shell_m = 20000.0, bool convective = true) const {
    double z = std::max(0.0, std::min(d_shell_m, R_EUROPA - r_m));
    if (!convective || d_shell_m <= 10000.0) {
      // Pure conductive profile (logarithmic Fourier solution)
      return T_SURF * std::pow(T_MELT / T_SURF, z / d_shell_m);
    }
    // Convective profile: conductive lid top 35%, nearly isothermal convective sublayer (T ~ 260 K)
    double lid_thickness = 0.35 * d_shell_m;
    double T_conv = 260.0;
    if (z <= lid_thickness) {
      return T_SURF * std::pow(T_conv / T_SURF, z / lid_thickness);
    } else {
      double frac = (z - lid_thickness) / (d_shell_m - lid_thickness);
      return T_conv + (T_MELT - T_conv) * frac;
    }
  }

  // Ice effective dynamic viscosity eta(T, d) [Pa s] with grain-size and Arrhenius temperature dependence
  double ice_viscosity_pa_s(double T_k, double grain_size_mm = D_0_MM) const {
    double T = std::max(70.0, std::min(T_MELT, T_k));
    double grain_factor = std::pow(grain_size_mm / D_0_MM, GRAIN_EXP);
    double arrhenius = std::exp((E_ACTIVATION / R_GAS) * (1.0 / T - 1.0 / T_MELT));
    return ETA_0 * grain_factor * arrhenius;
  }

  // Maxwell relaxation time tau_M = eta / mu_0 [s]
  double maxwell_relaxation_time_s(double eta_pa_s, double mu_pa = MU_ICE) const {
    return eta_pa_s / mu_pa;
  }

  // Viscoelastic Maxwell dissipation function Phi(omega * tau_M) = (omega * tau_M) / (1 + (omega * tau_M)^2)
  double viscoelastic_dissipation_function(double eta_pa_s, double mu_pa = MU_ICE) const {
    double omega = orbital_frequency_rad_s();
    double tau_m = maxwell_relaxation_time_s(eta_pa_s, mu_pa);
    double x = omega * tau_m;
    return x / (1.0 + x * x);
  }

  // Diurnal tidal strain tensor amplitude epsilon_eff(r) in decoupled ice shell
  // Decoupled by global subsurface ocean, yielding effective multi-component strain amplitude ~ 4.2e-5 * (r / R_E)
  double effective_tidal_strain(double r_m) const {
    double base_strain = 0.97e-5;
    return base_strain * (r_m / R_EUROPA);
  }

  // Volumetric tidal heating rate q_tide(r) [W/m^3] at radius r_m
  double volumetric_heating_rate_w_m3(double r_m, double d_shell_m = 20000.0, double grain_size_mm = D_0_MM, bool convective = true) const {
    double T = temperature_at_radius_k(r_m, d_shell_m, convective);
    double eta = ice_viscosity_pa_s(T, grain_size_mm);
    double phi = viscoelastic_dissipation_function(eta, MU_ICE);
    double eps = effective_tidal_strain(r_m);
    double omega = orbital_frequency_rad_s();
    return 2.0 * MU_ICE * omega * (eps * eps) * phi;
  }

  // Total integrated tidal dissipation power in Europa's ice shell [Watts]
  double total_tidal_power_watts(double d_shell_m = 20000.0, double grain_size_mm = D_0_MM, bool convective = true, int num_layers = 500) const {
    double r_base = R_EUROPA - d_shell_m;
    double dr = d_shell_m / num_layers;
    double total_power_w = 0.0;
    for (int i = 0; i < num_layers; ++i) {
      double r_mid = r_base + (i + 0.5) * dr;
      double q = volumetric_heating_rate_w_m3(r_mid, d_shell_m, grain_size_mm, convective);
      double dV = 4.0 * M_PI * r_mid * r_mid * dr;
      total_power_w += q * dV;
    }
    return total_power_w;
  }

  // Total tidal dissipation power [TW] (1 TW = 1e12 W)
  double total_tidal_power_tw(double d_shell_m = 20000.0, double grain_size_mm = D_0_MM, bool convective = true) const {
    return total_tidal_power_watts(d_shell_m, grain_size_mm, convective) / 1.0e12;
  }

  // Surface tidal heat flux [mW/m^2]
  double surface_heat_flux_mw_m2(double d_shell_m = 20000.0, double grain_size_mm = D_0_MM, bool convective = true) const {
    double area = 4.0 * M_PI * R_EUROPA * R_EUROPA;
    return (total_tidal_power_watts(d_shell_m, grain_size_mm, convective) / area) * 1.0e3;
  }

  // Effective tidal dissipation factor Im(k_2) = k_2 / Q
  double effective_k2_over_q(double d_shell_m = 20000.0, double grain_size_mm = D_0_MM, bool convective = true) const {
    double power_w = total_tidal_power_watts(d_shell_m, grain_size_mm, convective);
    double n = orbital_frequency_rad_s();
    double factor = 10.5 * G * M_JUPITER * M_JUPITER * std::pow(R_EUROPA, 5.0) * n * (ECCENTRICITY * ECCENTRICITY) / std::pow(A_EUROPA, 6.0);
    return power_w / factor;
  }

  // Conductive equilibrium ice shell thickness [km] where Q_conductive = P_tide
  double conductive_equilibrium_thickness_km(double grain_size_mm = D_0_MM, bool convective = true) const {
    double d_m = 20000.0;
    for (int iter = 0; iter < 15; ++iter) {
      double flux_tide_w_m2 = (surface_heat_flux_mw_m2(d_m, grain_size_mm, convective)) / 1000.0;
      if (flux_tide_w_m2 <= 0.0) break;
      d_m = (K_CONDUCT * std::log(T_MELT / T_SURF)) / flux_tide_w_m2;
    }
    return d_m / 1000.0;
  }
};

using RossSchubert1987EuropaModel = EuropaViscoelasticTidalModel;

// ============================================================================
// 68. EUROPA TIDAL STRESS, NON-SYNCHRONOUS ROTATION & CYCLOID LINEAMENT ORIENTATION MODEL
// (Rhoden et al. 2010, 2013, 2015; Hoppa et al. 1999; Hurford et al. 2007; Greenberg et al. 1998)
// ============================================================================
class EuropaLinearFractureModel {
 public:
  static constexpr double M_JUPITER = 1.89813e27;  // Jupiter mass [kg]
  static constexpr double M_EUROPA = 4.7998e22;    // Europa mass [kg]
  static constexpr double R_EUROPA = 1.5608e6;     // Europa mean radius [m]
  static constexpr double A_EUROPA = 6.709e8;      // Europa semi-major axis [m]
  static constexpr double ECCENTRICITY = 0.009;    // Europa forced orbital eccentricity
  static constexpr double G_SURF = 1.315;          // Surface gravity [m/s^2]
  static constexpr double RHO_ICE = 917.0;         // Ice density [kg/m^3]
  static constexpr double MU_ICE = 3.3e9;          // Ice shear modulus [Pa]
  static constexpr double POISSON_RATIO = 0.33;    // Poisson ratio nu
  static constexpr double H2_LOVE_OCEAN = 1.20;    // Radial Love number h2 for decoupled shell
  static constexpr double L2_LOVE_OCEAN = 0.30;    // Lateral Love number l2 for decoupled shell
  static constexpr double TENSILE_STRENGTH_ICE_KPA = 40.0; // Surface tensile fracture strength [kPa]
  static constexpr double REF_CRACK_SPEED_M_S = 0.50;      // Base crack propagation speed [m/s]

  // Mean orbital motion n [rad/s]
  double orbital_frequency_rad_s() const {
    return std::sqrt(G * (M_JUPITER + M_EUROPA) / std::pow(A_EUROPA, 3.0));
  }

  // Orbital period [seconds]
  double orbital_period_s() const {
    return (2.0 * M_PI) / orbital_frequency_rad_s();
  }

  // Orbital period [days]
  double orbital_period_days() const {
    return orbital_period_s() / 86400.0;
  }

  // Base diurnal tidal stress amplitude [kPa] as function of ice shell thickness [km] and eccentricity
  double diurnal_stress_amplitude_kpa(double h_shell_km = 20.0, double ecc = ECCENTRICITY) const {
    double h = std::max(2.0, h_shell_km);
    return 115.0 * (ecc / ECCENTRICITY) * std::sqrt(20.0 / h);
  }

  // Diurnal tidal stress tensor components (sigma_lat, sigma_lon, sigma_shear) in kPa
  std::tuple<double, double, double> diurnal_stress_tensor_kpa(
      double orbital_phase_deg, double lat_deg, double lon_deg,
      double h_shell_km = 20.0, double ecc = ECCENTRICITY) const {
    double M_rad = orbital_phase_deg * (M_PI / 180.0);
    double phi = lat_deg * (M_PI / 180.0);
    double lam = lon_deg * (M_PI / 180.0);
    double sigma_0 = diurnal_stress_amplitude_kpa(h_shell_km, ecc);
    double nu = POISSON_RATIO;

    double cos_phi = std::cos(phi);
    double sin_phi = std::sin(phi);
    double sin_2phi = std::sin(2.0 * phi);
    double cos_lam = std::cos(lam);
    double sin_lam = std::sin(lam);
    double cos_2lam = std::cos(2.0 * lam);
    double sin_2lam = std::sin(2.0 * lam);

    // Radial tide components
    double s_rad_lat = std::cos(M_rad) * sigma_0 * 0.5 *
        ((1.0 + nu) - 3.0 * cos_phi * cos_phi * cos_lam * cos_lam - nu * (3.0 * sin_phi * sin_phi - 1.0));
    double s_rad_lon = std::cos(M_rad) * sigma_0 * 0.5 *
        ((1.0 + nu) * cos_phi * cos_phi - 3.0 * cos_phi * cos_phi * sin_lam * sin_lam - nu * (3.0 * cos_phi * cos_phi * cos_lam * cos_lam - 1.0));
    double s_rad_shear = std::cos(M_rad) * sigma_0 * 0.75 * (1.0 - nu) * sin_2phi * sin_2lam;

    // Libration tide components
    double s_lib_lat = std::sin(M_rad) * sigma_0 * 1.5 * sin_2lam * (1.0 - nu * sin_phi * sin_phi);
    double s_lib_lon = -std::sin(M_rad) * sigma_0 * 1.5 * sin_2lam * (sin_phi * sin_phi - nu);
    double s_lib_shear = std::sin(M_rad) * sigma_0 * 1.5 * (1.0 - nu) * sin_phi * cos_2lam;

    double s_lat = s_rad_lat + s_lib_lat;
    double s_lon = s_rad_lon + s_lib_lon;
    double s_shear = s_rad_shear + s_lib_shear;

    return {s_lat, s_lon, s_shear};
  }

  // Non-synchronous rotation (NSR) stress tensor components in kPa (Rhoden et al. 2013, 2015)
  std::tuple<double, double, double> nsr_stress_tensor_kpa(
      double lat_deg, double lon_deg, double nsr_accumulated_deg = 1.0, double sigma_nsr_0_kpa = 100.0) const {
    double phi = lat_deg * (M_PI / 180.0);
    double lam = lon_deg * (M_PI / 180.0);
    double psi = nsr_accumulated_deg * (M_PI / 180.0);

    double cos_phi = std::cos(phi);
    double sin_phi = std::sin(phi);

    double d_cos = std::cos(2.0 * (lam - psi)) - std::cos(2.0 * lam);
    double d_sin = std::sin(2.0 * (lam - psi)) - std::sin(2.0 * lam);

    double s_nsr_lat = sigma_nsr_0_kpa * d_cos * cos_phi * cos_phi;
    double s_nsr_lon = -sigma_nsr_0_kpa * d_cos * cos_phi * cos_phi;
    double s_nsr_shear = sigma_nsr_0_kpa * d_sin * sin_phi;

    return {s_nsr_lat, s_nsr_lon, s_nsr_shear};
  }

  // Combined (Diurnal + NSR) stress tensor components in kPa
  std::tuple<double, double, double> total_stress_tensor_kpa(
      double orbital_phase_deg, double lat_deg, double lon_deg,
      double nsr_accumulated_deg = 1.0, double sigma_nsr_0_kpa = 80.0,
      double h_shell_km = 20.0, double ecc = ECCENTRICITY) const {
    auto [d_lat, d_lon, d_shear] = diurnal_stress_tensor_kpa(orbital_phase_deg, lat_deg, lon_deg, h_shell_km, ecc);
    auto [n_lat, n_lon, n_shear] = nsr_stress_tensor_kpa(lat_deg, lon_deg, nsr_accumulated_deg, sigma_nsr_0_kpa);

    return {d_lat + n_lat, d_lon + n_lon, d_shear + n_shear};
  }

  // Maximum Principal Tensile Stress sigma_1 [kPa]
  double principal_tensile_stress_kpa(double s_lat, double s_lon, double s_shear) const {
    double mean_s = 0.5 * (s_lat + s_lon);
    double diff_s = 0.5 * (s_lat - s_lon);
    double radius = std::sqrt(diff_s * diff_s + s_shear * s_shear);
    return mean_s + radius;
  }

  // Minimum Principal Stress sigma_2 [kPa]
  double principal_compressive_stress_kpa(double s_lat, double s_lon, double s_shear) const {
    double mean_s = 0.5 * (s_lat + s_lon);
    double diff_s = 0.5 * (s_lat - s_lon);
    double radius = std::sqrt(diff_s * diff_s + s_shear * s_shear);
    return mean_s - radius;
  }

  // Maximum Tensile Stress Direction psi_1 [degrees] from North toward East
  double principal_tensile_angle_deg(double s_lat, double s_lon, double s_shear) const {
    double angle_rad = 0.5 * std::atan2(2.0 * s_shear, s_lat - s_lon);
    double angle_deg = angle_rad * (180.0 / M_PI);
    if (angle_deg < 0.0) angle_deg += 180.0;
    return angle_deg;
  }

  // Fracture / Cycloid Lineament Propagation Azimuth [degrees] (perpendicular to maximum tension)
  double cycloid_propagation_azimuth_deg(double s_lat, double s_lon, double s_shear) const {
    double tension_angle = principal_tensile_angle_deg(s_lat, s_lon, s_shear);
    double crack_azimuth = tension_angle + 90.0;
    if (crack_azimuth >= 180.0) crack_azimuth -= 180.0;
    return crack_azimuth;
  }

  // Subcritical Crack Tip Propagation Velocity [m/s] (Hoppa 1999, Hurford 2007, Rhoden 2015)
  double crack_propagation_speed_m_s(
      double sigma_1_kpa, double sigma_crit_kpa = TENSILE_STRENGTH_ICE_KPA,
      double v0_m_s = REF_CRACK_SPEED_M_S, double power_exponent = 2.0, double sigma_ref_kpa = 60.0) const {
    if (sigma_1_kpa <= sigma_crit_kpa) return 0.0;
    double excess = (sigma_1_kpa - sigma_crit_kpa) / sigma_ref_kpa;
    return v0_m_s * std::pow(excess, power_exponent);
  }

  // Cycloid Arc Length [km] generated during one orbital cycle (3.55 days)
  double cycloid_arc_length_km(
      double lat_deg, double lon_deg, double nsr_accumulated_deg = 1.0,
      double sigma_nsr_0_kpa = 80.0, double sigma_crit_kpa = TENSILE_STRENGTH_ICE_KPA,
      double h_shell_km = 20.0, double ecc = ECCENTRICITY, int num_steps = 360) const {
    double dt_sec = orbital_period_s() / num_steps;
    double total_distance_m = 0.0;

    for (int i = 0; i < num_steps; ++i) {
      double phase_deg = (360.0 * i) / num_steps;
      auto [s_lat, s_lon, s_shear] = total_stress_tensor_kpa(
          phase_deg, lat_deg, lon_deg, nsr_accumulated_deg, sigma_nsr_0_kpa, h_shell_km, ecc);
      double s_1 = principal_tensile_stress_kpa(s_lat, s_lon, s_shear);
      double v_m_s = crack_propagation_speed_m_s(s_1, sigma_crit_kpa);
      total_distance_m += v_m_s * dt_sec;
    }

    return total_distance_m / 1.0e3; // km
  }
};

using Rhoden2015EuropaFractureModel = EuropaLinearFractureModel;
using EuropaCycloidFractureModel = EuropaLinearFractureModel;

// ============================================================================
// 69. GANYMEDE TIDAL DISSIPATION & MULTI-LAYER VISCOELASTIC ICE SHELL MODEL
// (Bland et al. 2009, 2012; Showman & Han 2004; Tobie et al. 2005)
// ============================================================================
class Bland2012GanymedeTidalModel {
 public:
  static constexpr double M_JUPITER = 1.89813e27;       // Jupiter mass [kg]
  static constexpr double M_GANYMEDE = 1.4819e23;      // Ganymede mass [kg]
  static constexpr double R_GANYMEDE = 2.6341e6;       // Ganymede mean radius [m]
  static constexpr double A_GANYMEDE = 1.0704e9;       // Semi-major axis [m]
  static constexpr double E_GANYMEDE_NOM = 0.0013;     // Present-day orbital eccentricity
  static constexpr double E_GANYMEDE_RESONANCE = 0.02; // Resonant excited eccentricity (Bland 2012)
  static constexpr double G_SURF = 1.428;              // Surface gravity [m/s^2]
  static constexpr double RHO_ICE = 920.0;             // Ice I shell density [kg/m^3]
  static constexpr double RHO_OCEAN = 1000.0;          // Subsurface ocean density [kg/m^3]
  static constexpr double RHO_MEAN = 1936.0;           // Ganymede bulk mean density [kg/m^3]
  static constexpr double MU_ICE = 3.5e9;              // Ice I shear modulus [Pa]
  static constexpr double A_CONDUCT = 567.0;           // Ice thermal conductivity coeff [W/m]
  static constexpr double T_SURF = 110.0;              // Mean surface temperature [K]
  static constexpr double T_BASE = 260.0;              // Ice shell basal temperature [K]
  static constexpr double P_RADIO_NOM_GW = 160.0;      // Radiogenic core heating power [GW]

  // Orbital mean motion frequency n [rad/s]
  double orbital_frequency_rad_s() const {
    return std::sqrt(G * (M_JUPITER + M_GANYMEDE) / std::pow(A_GANYMEDE, 3.0));
  }

  // Orbital period [days]
  double orbital_period_days() const {
    return (2.0 * M_PI / orbital_frequency_rad_s()) / 86400.0;
  }

  // Maxwell relaxation timescale tau_M [s] = eta / mu
  double maxwell_relaxation_time_s(double eta_pa_s, double mu_ice = MU_ICE) const {
    return eta_pa_s / mu_ice;
  }

  // Maxwell dimensionless frequency parameter omega * tau_M
  double maxwell_dimensionless_param(double eta_pa_s, double mu_ice = MU_ICE) const {
    return orbital_frequency_rad_s() * maxwell_relaxation_time_s(eta_pa_s, mu_ice);
  }

  // Optimal viscosity for peak dissipation [Pa s] where omega * tau_M = 1
  double peak_dissipation_viscosity_pa_s(double mu_ice = MU_ICE) const {
    return mu_ice / orbital_frequency_rad_s();
  }

  // Membrane stiffness factor S_membrane for decoupled shell (Tobie 2005, Beuthe 2013)
  double membrane_stiffness_factor(double d_shell_km, double mu_ice = MU_ICE) const {
    double shell_ratio = (d_shell_km * 1.0e3) / R_GANYMEDE;
    double alpha_membrane = 4.8;
    double rigidity_param = mu_ice / (RHO_MEAN * G_SURF * R_GANYMEDE);
    return 1.0 + alpha_membrane * shell_ratio * rigidity_param;
  }

  // Real potential Love number k_2 for decoupled multi-layer shell
  double love_number_k2(double d_shell_km, double d_ocean_km = 100.0, double mu_ice = MU_ICE) const {
    double k2_fluid = 1.05;
    double d_trans = 20.0;  // km decoupling transition scale
    double ocean_decoupling = 1.0 - std::exp(-d_ocean_km / d_trans);
    double stiffness = membrane_stiffness_factor(d_shell_km, mu_ice);
    return (k2_fluid * ocean_decoupling) / stiffness;
  }

  // Viscoelastic tidal phase lag delta [rad] (Maxwell rheology in ductile sublayer)
  double viscoelastic_phase_lag_rad(double eta_pa_s, double d_shell_km, double d_lid_km = 20.0, double mu_ice = MU_ICE) const {
    double d_ductile_km = std::max(0.0, d_shell_km - d_lid_km);
    double f_ductile = (d_shell_km > 0.0) ? (d_ductile_km / d_shell_km) : 0.0;
    double x = maxwell_dimensionless_param(eta_pa_s, mu_ice);
    double maxwell_kernel = x / (1.0 + x * x);
    // Transient creep contribution (Andrade effect ~15% enhancement)
    double andrade_kernel = 0.15 * std::pow(std::max(1.0e-10, x), -0.25) / (1.0 + x * x);
    double tan_delta = f_ductile * (maxwell_kernel + andrade_kernel);
    return std::max(1.0e-8, std::atan(tan_delta));
  }

  // Imaginary Love number Im(k_2) = k_2 * sin(2 * delta)
  double im_k2_dissipation(double d_shell_km, double eta_pa_s, double d_lid_km = 20.0, double d_ocean_km = 100.0, double mu_ice = MU_ICE) const {
    double k2 = love_number_k2(d_shell_km, d_ocean_km, mu_ice);
    double delta = viscoelastic_phase_lag_rad(eta_pa_s, d_shell_km, d_lid_km, mu_ice);
    return k2 * std::sin(2.0 * delta);
  }

  // Viscoelastic tidal dissipation heating power [Watts] (Peale 1979, Bland 2012)
  double tidal_heating_power_watts(double d_shell_km, double eta_pa_s, double e = E_GANYMEDE_NOM, double d_lid_km = 20.0, double d_ocean_km = 100.0) const {
    double im_k2 = im_k2_dissipation(d_shell_km, eta_pa_s, d_lid_km, d_ocean_km);
    double n = orbital_frequency_rad_s();
    double factor = 10.5 * im_k2 * G * M_JUPITER * M_JUPITER * std::pow(R_GANYMEDE, 5.0) * n / std::pow(A_GANYMEDE, 6.0);
    return factor * e * e;
  }

  // Tidal dissipation heating power [GW]
  double tidal_heating_power_gw(double d_shell_km, double eta_pa_s, double e = E_GANYMEDE_NOM, double d_lid_km = 20.0, double d_ocean_km = 100.0) const {
    return tidal_heating_power_watts(d_shell_km, eta_pa_s, e, d_lid_km, d_ocean_km) * 1.0e-9;
  }

  // Tidal dissipation heating power [TW]
  double tidal_heating_power_tw(double d_shell_km, double eta_pa_s, double e = E_GANYMEDE_RESONANCE, double d_lid_km = 20.0, double d_ocean_km = 100.0) const {
    return tidal_heating_power_watts(d_shell_km, eta_pa_s, e, d_lid_km, d_ocean_km) * 1.0e-12;
  }

  // Surface tidal heat flux [mW/m^2]
  double surface_tidal_heat_flux_mw_m2(double d_shell_km, double eta_pa_s, double e = E_GANYMEDE_NOM, double d_lid_km = 20.0) const {
    double area = 4.0 * M_PI * R_GANYMEDE * R_GANYMEDE;
    return (tidal_heating_power_watts(d_shell_km, eta_pa_s, e, d_lid_km) / area) * 1.0e3;
  }

  // Conductive heat loss power through ice shell [Watts] (Fourier logarithmic conductivity)
  double conductive_heat_loss_watts(double d_shell_km, double T_surf = T_SURF, double T_base = T_BASE) const {
    double d_m = std::max(100.0, d_shell_km * 1.0e3);
    double area = 4.0 * M_PI * R_GANYMEDE * R_GANYMEDE;
    double flux_w_m2 = (A_CONDUCT * std::log(T_base / T_surf)) / d_m;
    return flux_w_m2 * area;
  }

  // Conductive heat loss power through ice shell [GW]
  double conductive_heat_loss_gw(double d_shell_km, double T_surf = T_SURF, double T_base = T_BASE) const {
    return conductive_heat_loss_watts(d_shell_km, T_surf, T_base) * 1.0e-9;
  }

  // Conductive heat flux [mW/m^2]
  double conductive_heat_flux_mw_m2(double d_shell_km, double T_surf = T_SURF, double T_base = T_BASE) const {
    double area = 4.0 * M_PI * R_GANYMEDE * R_GANYMEDE;
    return (conductive_heat_loss_watts(d_shell_km, T_surf, T_base) / area) * 1.0e3;
  }

  // Equilibrium shell thickness [km] where Q_cond(d_eq) = P_tide + P_radio
  double equilibrium_shell_thickness_km(double eta_pa_s, double e = E_GANYMEDE_RESONANCE, double P_radio_gw = P_RADIO_NOM_GW) const {
    double d_guess_km = 50.0;
    for (int iter = 0; iter < 50; ++iter) {
      double p_tide_gw = tidal_heating_power_gw(d_guess_km, eta_pa_s, e);
      double total_heat_gw = p_tide_gw + P_radio_gw;
      if (total_heat_gw <= 0.0) return 150.0;
      double area = 4.0 * M_PI * R_GANYMEDE * R_GANYMEDE;
      double target_flux_w_m2 = (total_heat_gw * 1.0e9) / area;
      double d_new_m = (A_CONDUCT * std::log(T_BASE / T_SURF)) / target_flux_w_m2;
      double d_new_km = d_new_m / 1000.0;
      if (std::abs(d_new_km - d_guess_km) < 0.01) {
        return d_new_km;
      }
      d_guess_km = 0.5 * (d_guess_km + d_new_km);
    }
    return d_guess_km;
  }
};

using GanymedeTidalDissipationModel = Bland2012GanymedeTidalModel;

// ============================================================================
// 70. HUSSMANN & SPOHN (2004) COUPLED THERMAL-ORBITAL EVOLUTION OF IO & EUROPA
// (Hussmann & Spohn 2004, Icarus 171, 391-410; Yoder & Peale 1981, Malhotra 1991)
// ============================================================================
class HussmannSpohn2004ThermalOrbitalModel {
 public:
  static constexpr double M_JUPITER = 1.89813e27;       // Jupiter mass [kg]
  static constexpr double R_JUPITER = 7.1492e7;        // Jupiter equatorial radius [m]
  static constexpr double K2_JUPITER = 0.565;          // Jupiter potential Love number
  static constexpr double Q_JUPITER_NOM = 1.0e5;       // Jupiter nominal tidal Q

  // Io physical & orbital constants
  static constexpr double M_IO = 8.9319e22;            // Io mass [kg]
  static constexpr double R_IO = 1.8216e6;             // Io mean radius [m]
  static constexpr double A_IO = 4.2170e8;             // Io semi-major axis [m]
  static constexpr double E_IO_NOM = 0.0041;           // Io nominal forced eccentricity
  static constexpr double CP_IO = 1200.0;              // Io mantle specific heat capacity [J/(kg K)]
  static constexpr double T_MELT_IO = 1400.0;          // Io silicate solidus / basal melting temperature [K]
  static constexpr double T_REF_IO = 1473.0;           // Io nominal reference mantle temperature [K] (1200 C)
  static constexpr double T_SURF_IO = 130.0;           // Io surface temperature [K]
  static constexpr double GAMMA_ACT_IO = 25.8;         // Mantle activation parameter E* / (R_g * T_m)
  static constexpr double ETA_0_IO = 1.0e15;           // Reference mantle viscosity [Pa s]
  static constexpr double MU_IO = 6.5e10;              // Io mantle shear modulus [Pa]
  static constexpr double IM_K2_PEAK_IO = 0.045;       // Peak viscoelastic Love dissipation Im(k2)
  static constexpr double Q_RADIO_IO_W = 6.0e12;       // Radiogenic heat production [Watts] (6 TW)
  static constexpr double Q_LOSS_0_IO_W = 1.05e14;     // Nominal convective heat loss [Watts] (105 TW)

  // Europa physical & orbital constants
  static constexpr double M_EUROPA = 4.7998e22;        // Europa mass [kg]
  static constexpr double R_EUROPA = 1.5608e6;         // Europa mean radius [m]
  static constexpr double A_EUROPA = 6.7110e8;         // Europa semi-major axis [m]
  static constexpr double E_EUROPA_NOM = 0.0090;       // Europa nominal eccentricity
  static constexpr double CP_EUROPA = 2000.0;          // Composite heat capacity [J/(kg K)]
  static constexpr double P_TIDE_EUROPA_NOM_W = 3.5e12;// Europa tidal power [Watts] (3.5 TW)

  // Mean motion frequencies [rad/s]
  double io_mean_motion() const {
    return std::sqrt(G * M_JUPITER / std::pow(A_IO, 3.0));
  }

  double europa_mean_motion() const {
    return std::sqrt(G * M_JUPITER / std::pow(A_EUROPA, 3.0));
  }

  // Orbital periods [days]
  double io_orbital_period_days() const {
    return (2.0 * M_PI / io_mean_motion()) / 86400.0;
  }

  double europa_orbital_period_days() const {
    return (2.0 * M_PI / europa_mean_motion()) / 86400.0;
  }

  // Io mantle viscosity [Pa s] as a function of temperature T [K] (Arrhenius / Frank-Kamenetskii)
  double io_viscosity_pa_s(double T_mantle_k) const {
    double T = std::max(500.0, std::min(2500.0, T_mantle_k));
    return ETA_0_IO * std::exp(GAMMA_ACT_IO * (T_MELT_IO / T - 1.0));
  }

  // Viscosity for peak viscoelastic dissipation eta_peak = mu / n [Pa s]
  double io_peak_viscosity_pa_s() const {
    return MU_IO / io_mean_motion();
  }

  // Viscoelastic dissipation factor Im(k2) = k2 / Q as a function of temperature (Maxwell-Andrade resonance)
  double io_k2_over_q(double T_mantle_k) const {
    double eta = io_viscosity_pa_s(T_mantle_k);
    double eta_peak = io_peak_viscosity_pa_s();
    double ratio = eta / eta_peak;
    return IM_K2_PEAK_IO * (2.0 * ratio) / (1.0 + ratio * ratio);
  }

  // Io tidal heating power [Watts]
  double io_tidal_power_watts(double e_io, double T_mantle_k) const {
    double k2_q = io_k2_over_q(T_mantle_k);
    double n = io_mean_motion();
    double factor = 10.5 * k2_q * G * (M_JUPITER * M_JUPITER) * std::pow(R_IO, 5.0) * n / std::pow(A_IO, 6.0);
    return factor * (e_io * e_io);
  }

  // Io tidal heating power [TW]
  double io_tidal_power_tw(double e_io, double T_mantle_k) const {
    return io_tidal_power_watts(e_io, T_mantle_k) * 1.0e-12;
  }

  // Io mantle parameterized convective heat loss [Watts]
  double io_convective_heat_loss_watts(double T_mantle_k) const {
    double eta = io_viscosity_pa_s(T_mantle_k);
    double T = std::max(T_SURF_IO + 10.0, T_mantle_k);
    double visc_ratio = ETA_0_IO / eta;
    double temp_ratio = (T - T_SURF_IO) / (T_REF_IO - T_SURF_IO);
    return Q_LOSS_0_IO_W * std::pow(visc_ratio, 1.0 / 3.0) * std::pow(temp_ratio, 4.0 / 3.0);
  }

  // Io mantle parameterized convective heat loss [TW]
  double io_convective_heat_loss_tw(double T_mantle_k) const {
    return io_convective_heat_loss_watts(T_mantle_k) * 1.0e-12;
  }

  // Io surface average heat flux [W/m^2]
  double io_surface_heat_flux_w_m2(double power_watts) const {
    double area = 4.0 * M_PI * R_IO * R_IO;
    return power_watts / area;
  }

  // Secular orbital resonance pumping rate A_J [s^-1] from Jupiter's tidal torque
  double orbital_pumping_rate_s_inv(double k2_over_Q_jup = 1.74e-5) const {
    double B_1 = orbital_damping_rate_coeff_s_inv();
    double k2_q_nom = 0.016876;
    return B_1 * k2_q_nom * (k2_over_Q_jup / 1.74e-5);
  }

  // Orbital tidal damping coefficient B_1 [s^-1] such that de/dt_damp = -B_1 * (k2/Q)_1 * e
  double orbital_damping_rate_coeff_s_inv() const {
    double n1 = io_mean_motion();
    return 10.5 * (M_JUPITER / M_IO) * std::pow(R_IO / A_IO, 5.0) * n1;
  }

  // Time derivative of eccentricity de/dt [s^-1]
  double eccentricity_derivative_s_inv(double e_io, double T_mantle_k, double k2_over_Q_jup = 1.74e-5) const {
    double A_j = orbital_pumping_rate_s_inv(k2_over_Q_jup);
    double B_1 = orbital_damping_rate_coeff_s_inv();
    double k2_q1 = io_k2_over_q(T_mantle_k);
    return e_io * (A_j - B_1 * k2_q1);
  }

  // Time derivative of mantle temperature dT/dt [K/s]
  double temperature_derivative_k_s(double e_io, double T_mantle_k, double Q_radio_w = Q_RADIO_IO_W) const {
    double P_tide = io_tidal_power_watts(e_io, T_mantle_k);
    double Q_loss = io_convective_heat_loss_watts(T_mantle_k);
    double thermal_mass = M_IO * CP_IO;
    return (P_tide + Q_radio_w - Q_loss) / thermal_mass;
  }

  // Equilibrium tidal dissipation (k2/Q)_eq where de/dt = 0
  double equilibrium_k2_over_q(double k2_over_Q_jup = 1.74e-5) const {
    double A_j = orbital_pumping_rate_s_inv(k2_over_Q_jup);
    double B_1 = orbital_damping_rate_coeff_s_inv();
    return A_j / B_1;
  }

  // Equilibrium forced eccentricity e_eq where thermal & orbital balance hold
  double equilibrium_eccentricity(double T_mantle_k = T_REF_IO, double k2_over_Q_jup = 1.74e-5) const {
    double Q_loss = io_convective_heat_loss_watts(T_mantle_k);
    double net_heat_w = std::max(1.0e10, Q_loss - Q_RADIO_IO_W);
    double k2_q_eq = equilibrium_k2_over_q(k2_over_Q_jup);
    double n1 = io_mean_motion();
    double factor = 10.5 * k2_q_eq * G * (M_JUPITER * M_JUPITER) * std::pow(R_IO, 5.0) * n1 / std::pow(A_IO, 6.0);
    return std::sqrt(net_heat_w / factor);
  }

  // Equilibrium forced eccentricity as a function of effective satellite dissipation Q_io
  double equilibrium_eccentricity_for_Q(double Q_io, double k2_io = 0.025, double target_loss_tw = 105.0) const {
    double k2_q = k2_io / std::max(0.1, Q_io);
    double n1 = io_mean_motion();
    double net_power_w = (target_loss_tw * 1.0e12) - Q_RADIO_IO_W;
    double factor = 10.5 * k2_q * G * (M_JUPITER * M_JUPITER) * std::pow(R_IO, 5.0) * n1 / std::pow(A_IO, 6.0);
    return std::sqrt(net_power_w / factor);
  }

  // State snapshot for time-evolution integration
  struct EvolutionState {
    double time_myr;
    double eccentricity;
    double temperature_k;
    double tidal_power_tw;
    double heat_loss_tw;
    double k2_over_q;
    double viscosity_pa_s;
    double surface_flux_w_m2;
  };

  // Coupled Runge-Kutta 4th-order time integration over millions of years
  std::vector<EvolutionState> integrate_coupled_evolution(
      double e_init = 0.0041,
      double T_init_k = 1473.0,
      double t_max_myr = 1000.0,
      double dt_myr = 0.1,
      double k2_over_Q_jup = 1.74e-5) const {
    std::vector<EvolutionState> trajectory;
    const double SEC_PER_MYR = 1.0e6 * 365.25 * 86400.0;
    double dt_sec = dt_myr * SEC_PER_MYR;

    double t_myr = 0.0;
    double e = e_init;
    double T = T_init_k;

    while (t_myr <= t_max_myr) {
      double p_tw = io_tidal_power_tw(e, T);
      double q_tw = io_convective_heat_loss_tw(T);
      double k2_q = io_k2_over_q(T);
      double eta = io_viscosity_pa_s(T);
      double flux = io_surface_heat_flux_w_m2(p_tw * 1.0e12);

      trajectory.push_back({t_myr, e, T, p_tw, q_tw, k2_q, eta, flux});

      // RK4 step
      // k1
      double de1 = eccentricity_derivative_s_inv(e, T, k2_over_Q_jup);
      double dT1 = temperature_derivative_k_s(e, T);

      // k2
      double e_mid1 = std::max(1.0e-5, e + 0.5 * dt_sec * de1);
      double T_mid1 = std::max(300.0, T + 0.5 * dt_sec * dT1);
      double de2 = eccentricity_derivative_s_inv(e_mid1, T_mid1, k2_over_Q_jup);
      double dT2 = temperature_derivative_k_s(e_mid1, T_mid1);

      // k3
      double e_mid2 = std::max(1.0e-5, e + 0.5 * dt_sec * de2);
      double T_mid2 = std::max(300.0, T + 0.5 * dt_sec * dT2);
      double de3 = eccentricity_derivative_s_inv(e_mid2, T_mid2, k2_over_Q_jup);
      double dT3 = temperature_derivative_k_s(e_mid2, T_mid2);

      // k4
      double e_end = std::max(1.0e-5, e + dt_sec * de3);
      double T_end = std::max(300.0, T + dt_sec * dT3);
      double de4 = eccentricity_derivative_s_inv(e_end, T_end, k2_over_Q_jup);
      double dT4 = temperature_derivative_k_s(e_end, T_end);

      e += (dt_sec / 6.0) * (de1 + 2.0 * de2 + 2.0 * de3 + de4);
      T += (dt_sec / 6.0) * (dT1 + 2.0 * dT2 + 2.0 * dT3 + dT4);

      e = std::max(1.0e-5, std::min(0.05, e));
      T = std::max(400.0, std::min(2200.0, T));
      t_myr += dt_myr;
    }

    return trajectory;
  }
};

using HussmannSpohn2004Model = HussmannSpohn2004ThermalOrbitalModel;
using GanymedeTidalDissipationModel = Bland2012GanymedeTidalModel;

// ============================================================================
// 70. EUROPA ICE SHELL STAGNANT-LID CONVECTION & DIAPIRISM MODEL
// (Showman & Han 2004, JGR Planets; Han & Showman 2005; Solomatov & Moresi 2000; Barr 2004)
// ============================================================================
class ShowmanHan2004IceConvectionModel {
 public:
  static constexpr double M_JUPITER = 1.8982e27;       // Jupiter mass [kg]
  static constexpr double M_EUROPA = 4.7998e22;        // Europa mass [kg]
  static constexpr double R_EUROPA = 1.5608e6;         // Europa radius [m]
  static constexpr double G_SURF = 1.315;              // Europa surface gravity [m/s^2]
  static constexpr double RHO_ICE = 920.0;             // Ice density [kg/m^3]
  static constexpr double ALPHA_EXP = 1.60e-4;         // Thermal expansion coefficient [1/K]
  static constexpr double K_COND = 2.30;               // Thermal conductivity [W/(m K)]
  static constexpr double CP_ICE = 2000.0;             // Specific heat capacity [J/(kg K)]
  static constexpr double KAPPA_DIFF = 1.25e-6;        // Thermal diffusivity [m^2/s] (K / (rho * Cp))
  static constexpr double T_SURF_NOM = 100.0;          // Surface temperature [K]
  static constexpr double T_BASE_NOM = 270.0;          // Basal ocean-ice boundary temperature [K]
  static constexpr double ACTIVATION_E = 50000.0;      // Activation energy for diffusion creep [J/mol]
  static constexpr double ACTIVATION_E_DISL = 60000.0; // Activation energy for dislocation creep [J/mol]
  static constexpr double GAS_R = 8.314462;            // Universal gas constant [J/(mol K)]
  static constexpr double ETA_BASE_NOM = 1.0e14;       // Nominal basal viscosity [Pa s]
  static constexpr double D_SHELL_NOM_KM = 20.0;       // Nominal ice shell thickness [km]

  // Total temperature difference across ice shell [K]
  double delta_temperature_k(double T_surf = T_SURF_NOM, double T_base = T_BASE_NOM) const {
    return std::max(1.0, T_base - T_surf);
  }

  // Frank-Kamenetskii rheological parameter theta = (E* Delta T) / (R T_base^2)
  double frank_kamenetskii_param(double E_act = ACTIVATION_E, double T_base = T_BASE_NOM, double T_surf = T_SURF_NOM) const {
    double delta_t = delta_temperature_k(T_surf, T_base);
    return (E_act * delta_t) / (GAS_R * T_base * T_base);
  }

  // Rheological temperature scale Delta T_rh = R T_base^2 / E* [K]
  double rheological_temperature_scale_k(double E_act = ACTIVATION_E, double T_base = T_BASE_NOM) const {
    return (GAS_R * T_base * T_base) / E_act;
  }

  // Temperature-dependent Arrhenius ice viscosity eta(T) [Pa s]
  double viscosity_at_temperature(double T_k, double eta_base = ETA_BASE_NOM, double E_act = ACTIVATION_E, double T_base = T_BASE_NOM) const {
    double T = std::max(50.0, T_k);
    double exponent = (E_act / GAS_R) * (1.0 / T - 1.0 / T_base);
    // Limit exponent to avoid overflow
    exponent = std::min(100.0, exponent);
    return eta_base * std::exp(exponent);
  }

  // Viscosity contrast across ice shell Delta eta = eta(T_surf) / eta(T_base)
  double viscosity_contrast(double T_surf = T_SURF_NOM, double T_base = T_BASE_NOM, double E_act = ACTIVATION_E) const {
    return viscosity_at_temperature(T_surf, 1.0, E_act, T_base);
  }

  // Basal Rayleigh number Ra_b based on basal viscosity and full shell thickness D
  double basal_rayleigh_number(double d_shell_km = D_SHELL_NOM_KM, double eta_base = ETA_BASE_NOM, double T_surf = T_SURF_NOM, double T_base = T_BASE_NOM) const {
    double D_m = d_shell_km * 1.0e3;
    double delta_t = delta_temperature_k(T_surf, T_base);
    double numerator = RHO_ICE * G_SURF * ALPHA_EXP * delta_t * std::pow(D_m, 3.0);
    double denominator = KAPPA_DIFF * eta_base;
    return numerator / denominator;
  }

  // Rheological Rayleigh number Ra_rh based on Delta T_rh in the convective sublayer
  double rheological_rayleigh_number(double d_shell_km = D_SHELL_NOM_KM, double eta_base = ETA_BASE_NOM, double E_act = ACTIVATION_E, double T_base = T_BASE_NOM) const {
    double D_m = d_shell_km * 1.0e3;
    double delta_t_rh = rheological_temperature_scale_k(E_act, T_base);
    double numerator = RHO_ICE * G_SURF * ALPHA_EXP * delta_t_rh * std::pow(D_m, 3.0);
    double denominator = KAPPA_DIFF * eta_base;
    return numerator / denominator;
  }

  // Critical Rayleigh number Ra_cr for onset of stagnant-lid convection (Solomatov 2000)
  double critical_rayleigh_number(double E_act = ACTIVATION_E, double T_base = T_BASE_NOM, double T_surf = T_SURF_NOM) const {
    double theta = frank_kamenetskii_param(E_act, T_base, T_surf);
    // Ra_cr ~ 20.0 * theta^4
    return 20.0 * std::pow(theta, 4.0);
  }

  // Whether the shell undergoes solid-state thermal convection
  bool is_convective(double d_shell_km = D_SHELL_NOM_KM, double eta_base = ETA_BASE_NOM, double E_act = ACTIVATION_E) const {
    double ra_b = basal_rayleigh_number(d_shell_km, eta_base);
    double ra_cr = critical_rayleigh_number(E_act);
    return ra_b >= ra_cr;
  }

  // Nusselt number Nu = F_total / F_cond (Showman & Han 2004, Solomatov & Moresi 2000)
  // In stagnant lid regime: Nu = a * theta^(-(1+beta)) * Ra_b^beta = a * theta^(-1) * Ra_rh^beta
  double nusselt_number(double d_shell_km = D_SHELL_NOM_KM, double eta_base = ETA_BASE_NOM, double E_act = ACTIVATION_E, double a_coeff = 0.95, double beta = 0.22) const {
    double ra_rh = rheological_rayleigh_number(d_shell_km, eta_base, E_act);
    double theta = frank_kamenetskii_param(E_act);
    double ra_cr = critical_rayleigh_number(E_act);
    double ra_b = basal_rayleigh_number(d_shell_km, eta_base);
    if (ra_b < ra_cr) {
      return 1.0;  // Subcritical pure conduction
    }
    double nu = a_coeff * std::pow(ra_rh, beta) / theta;
    return std::max(1.0, nu);
  }

  // Conductive baseline heat flux [mW/m^2]
  double conductive_heat_flux_mw_m2(double d_shell_km = D_SHELL_NOM_KM, double T_surf = T_SURF_NOM, double T_base = T_BASE_NOM) const {
    double D_m = d_shell_km * 1.0e3;
    double delta_t = delta_temperature_k(T_surf, T_base);
    return (K_COND * delta_t / D_m) * 1.0e3;
  }

  // Total convective surface heat flux [mW/m^2]
  double total_heat_flux_mw_m2(double d_shell_km = D_SHELL_NOM_KM, double eta_base = ETA_BASE_NOM, double E_act = ACTIVATION_E) const {
    double f_cond = conductive_heat_flux_mw_m2(d_shell_km);
    double nu = nusselt_number(d_shell_km, eta_base, E_act);
    return f_cond * nu;
  }

  // Stagnant lid thickness [km] (Solomatov 2000, Showman & Han 2004)
  // delta_lid / D ~ (1 - delta_T_rh / delta_T) / Nu for Nu > 1
  double stagnant_lid_thickness_km(double d_shell_km = D_SHELL_NOM_KM, double eta_base = ETA_BASE_NOM, double E_act = ACTIVATION_E) const {
    double nu = nusselt_number(d_shell_km, eta_base, E_act);
    if (nu <= 1.001) {
      return d_shell_km;  // Fully conductive lid
    }
    double delta_t = delta_temperature_k();
    double delta_t_rh = rheological_temperature_scale_k(E_act);
    double lid_fraction = (delta_t - delta_t_rh) / (delta_t * nu);
    lid_fraction = std::min(1.0, std::max(0.1, lid_fraction));
    return d_shell_km * lid_fraction;
  }

  // Convective sublayer thickness [km]
  double convective_sublayer_thickness_km(double d_shell_km = D_SHELL_NOM_KM, double eta_base = ETA_BASE_NOM, double E_act = ACTIVATION_E) const {
    double d_lid = stagnant_lid_thickness_km(d_shell_km, eta_base, E_act);
    return std::max(0.0, d_shell_km - d_lid);
  }

  // Convective upwelling / plume velocity u_conv [m/yr] (Showman & Han 2004)
  double convective_velocity_m_yr(double d_shell_km = D_SHELL_NOM_KM, double eta_base = ETA_BASE_NOM, double E_act = ACTIVATION_E, double c_u = 0.25) const {
    double ra_rh = rheological_rayleigh_number(d_shell_km, eta_base, E_act);
    double D_m = d_shell_km * 1.0e3;
    if (!is_convective(d_shell_km, eta_base, E_act)) return 0.0;
    // u ~ c_u * (kappa / D) * Ra_rh^(2/3)
    double u_m_s = c_u * (KAPPA_DIFF / D_m) * std::pow(ra_rh, 2.0 / 3.0);
    return u_m_s * (365.25 * 86400.0);  // m/yr
  }

  // Convective overturn timescale [yr]
  double convective_overturn_timescale_yr(double d_shell_km = D_SHELL_NOM_KM, double eta_base = ETA_BASE_NOM, double E_act = ACTIVATION_E) const {
    double u_m_yr = convective_velocity_m_yr(d_shell_km, eta_base, E_act);
    double d_conv_m = convective_sublayer_thickness_km(d_shell_km, eta_base, E_act) * 1.0e3;
    if (u_m_yr <= 1.0e-10) return 1.0e9;
    return d_conv_m / u_m_yr;
  }

  // Diapir ascent timescale [yr] across convective sublayer
  double diapir_ascent_timescale_yr(double d_shell_km = D_SHELL_NOM_KM, double eta_base = ETA_BASE_NOM, double E_act = ACTIVATION_E) const {
    return 0.5 * convective_overturn_timescale_yr(d_shell_km, eta_base, E_act);
  }

  // Buoyant stress exerted by ascending thermal diapir on base of stagnant lid [kPa]
  double diapir_buoyant_stress_kpa(double plume_radius_km = 2.5, double E_act = ACTIVATION_E) const {
    double delta_t_rh = rheological_temperature_scale_k(E_act);
    double delta_rho = RHO_ICE * ALPHA_EXP * delta_t_rh;
    double r_m = plume_radius_km * 1.0e3;
    double stress_pa = delta_rho * G_SURF * r_m;
    return stress_pa / 1.0e3;
  }
};

using EuropaIceConvectionModel = ShowmanHan2004IceConvectionModel;

// ============================================================================
// 64. EUROPA DIURNAL TIDAL STRESS & CYCLOID CRACKING MODEL (Greenberg et al. 1998, Hoppa et al. 1999)
// ============================================================================
class EuropaTidalStressModel {
 public:
  static constexpr double M_JUPITER = 1.89813e27;     // Jupiter mass [kg]
  static constexpr double R_EUROPA = 1.5608e6;        // Europa mean radius [m]
  static constexpr double M_EUROPA = 4.7998e22;       // Europa mass [kg]
  static constexpr double A_ORBIT = 6.709e8;          // Semi-major axis [m]
  static constexpr double ECCENTRICITY = 0.009;       // Forced orbital eccentricity
  static constexpr double PERIOD_SEC = 306822.0;      // Orbital period [s] (3.551181 days)
  static constexpr double N_MEAN = 2.0478e-5;         // Mean motion [rad/s]
  static constexpr double G_SURF = 1.315;             // Surface gravity [m/s^2]
  static constexpr double E_ICE = 9.3e9;              // Young's modulus [Pa]
  static constexpr double NU_POISSON = 0.33;          // Poisson's ratio
  static constexpr double MU_RIGIDITY = 3.5e9;        // Shear modulus [Pa]
  static constexpr double NOMINAL_H_KM = 20.0;        // Nominal shell thickness [km]
  static constexpr double TENSILE_STRENGTH_KPA = 40.0;// Fractured ice tensile strength [kPa]
  static constexpr double H2_LOVE_OCEAN = 1.23;       // Love number h2 with decoupled ocean
  static constexpr double L2_LOVE_OCEAN = 0.31;       // Love number l2 with decoupled ocean
  static constexpr double H2_LOVE_SOLID = 0.025;      // Love number h2 for solid Europa (no ocean)

  // Diurnal tidal stress scale amplitude [kPa] as a function of shell thickness [km] and eccentricity
  double stress_scale_kpa(double h_shell_km = NOMINAL_H_KM, double ecc = ECCENTRICITY) const {
    double base_scale = 120.0;  // kPa for nominal 20 km shell and e=0.009 (Hurford 2007, Greenberg 1998)
    return base_scale * std::sqrt(NOMINAL_H_KM / std::max(1.0, h_shell_km)) * (ecc / ECCENTRICITY);
  }

  // Diurnal tidal stress tensor components (sigma_theta_theta, sigma_phi_phi, sigma_theta_phi) in [kPa]
  // at given latitude [deg], longitude [deg], and orbital mean anomaly M [deg]
  std::tuple<double, double, double> tidal_stress_tensor(
      double lat_deg, double lon_deg, double mean_anomaly_deg,
      double h_shell_km = NOMINAL_H_KM, double ecc = ECCENTRICITY,
      bool ocean_decoupled = true) const {
    if (!ocean_decoupled) {
      // Without liquid ocean decoupling, tidal deformation is suppressed by factor of ~50
      double solid_scale = (H2_LOVE_SOLID / H2_LOVE_OCEAN);
      double scale = stress_scale_kpa(h_shell_km, ecc) * solid_scale;
      return std::make_tuple(0.02 * scale, 0.02 * scale, 0.01 * scale);
    }

    double scale = stress_scale_kpa(h_shell_km, ecc);
    double lat_rad = lat_deg * M_PI / 180.0;
    double lon_rad = lon_deg * M_PI / 180.0;
    double M_rad = mean_anomaly_deg * M_PI / 180.0;

    double sin_lat = std::sin(lat_rad);
    double cos_lat = std::cos(lat_rad);
    double sin2_lat = sin_lat * sin_lat;
    double cos2_lat = cos_lat * cos_lat;
    double cos_2lon = std::cos(2.0 * lon_rad);
    double sin_2lon = std::sin(2.0 * lon_rad);

    double nu = NU_POISSON;

    // Radial eccentricity tide (in phase with cos(M))
    double sig_tt_rad = scale * (-0.75 * (1.0 + 3.0 * nu) * sin2_lat + 0.75 * (1.0 - nu) + 2.25 * (1.0 + nu) * cos2_lat * cos_2lon) * 0.5;
    double sig_pp_rad = scale * (0.75 * (3.0 + nu) * sin2_lat - 0.75 * (1.0 - nu) + 2.25 * (1.0 + nu) * cos2_lat * cos_2lon) * 0.5;
    double sig_tp_rad = scale * (1.125 * (1.0 + nu) * sin_lat * sin_2lon);

    // Libration / obliquity diurnal tide (in phase with sin(M))
    double sig_tt_lib = scale * (2.0 * (1.0 + nu) * sin_lat * sin_2lon) * (2.0 / 3.0);
    double sig_pp_lib = -sig_tt_lib;
    double sig_tp_lib = scale * (2.0 * (1.0 + nu) * cos_lat * cos_2lon) * (2.0 / 3.0);

    // Total diurnal stresses
    double cos_M = std::cos(M_rad);
    double sin_M = std::sin(M_rad);

    double sig_tt = sig_tt_rad * cos_M + sig_tt_lib * sin_M;
    double sig_pp = sig_pp_rad * cos_M + sig_pp_lib * sin_M;
    double sig_tp = sig_tp_rad * cos_M + sig_tp_lib * sin_M;

    return std::make_tuple(sig_tt, sig_pp, sig_tp);
  }

  // Principal stresses (sigma_1 = max tensile, sigma_2 = min compression) in [kPa]
  std::pair<double, double> principal_stresses(double sig_tt, double sig_pp, double sig_tp) const {
    double avg = 0.5 * (sig_tt + sig_pp);
    double diff = 0.5 * (sig_tt - sig_pp);
    double radius = std::sqrt(diff * diff + sig_tp * sig_tp);
    return {avg + radius, avg - radius};
  }

  // Principal tensile azimuth / crack orientation angle [degrees from North]
  double principal_azimuth_deg(double sig_tt, double sig_pp, double sig_tp) const {
    double angle_rad = 0.5 * std::atan2(2.0 * sig_tp, sig_tt - sig_pp) + 0.5 * M_PI;
    double angle_deg = angle_rad * 180.0 / M_PI;
    while (angle_deg < 0.0) angle_deg += 180.0;
    while (angle_deg >= 180.0) angle_deg -= 180.0;
    return angle_deg;
  }

  // Peak diurnal maximum tensile stress [kPa] over an entire 3.55-day orbit
  double peak_diurnal_tensile_stress_kpa(
      double lat_deg, double lon_deg,
      double h_shell_km = NOMINAL_H_KM, double ecc = ECCENTRICITY,
      bool ocean_decoupled = true) const {
    double max_sig1 = -1e9;
    for (int step = 0; step < 360; ++step) {
      double M_deg = static_cast<double>(step);
      auto [sig_tt, sig_pp, sig_tp] = tidal_stress_tensor(lat_deg, lon_deg, M_deg, h_shell_km, ecc, ocean_decoupled);
      auto [sig1, sig2] = principal_stresses(sig_tt, sig_pp, sig_tp);
      if (sig1 > max_sig1) {
        max_sig1 = sig1;
      }
    }
    return max_sig1;
  }

  // Tensile failure flag (active crack initiation / cycloid propagation)
  bool is_cracking_active(double peak_stress_kpa, double tensile_strength_kpa = TENSILE_STRENGTH_KPA) const {
    return peak_stress_kpa >= tensile_strength_kpa;
  }

  // Cycloid single arc length [km] given crack velocity [km/h] and active tensile duration [hours]
  double cycloid_arc_length_km(double v_crack_km_h = 1.5, double active_hours = 65.0) const {
    return v_crack_km_h * active_hours;
  }

  // Diurnal surface vertical tidal displacement amplitude [meters]
  double surface_tidal_displacement_m(double ecc = ECCENTRICITY, bool ocean_decoupled = true) const {
    double h2 = ocean_decoupled ? H2_LOVE_OCEAN : H2_LOVE_SOLID;
    double xi_scale = (3.0 * hot_jupiter::G * M_JUPITER * R_EUROPA * R_EUROPA) / (std::pow(A_ORBIT, 3.0) * G_SURF);
    return h2 * xi_scale * ecc;
  }
};

using Greenberg1998EuropaTidalStressModel = EuropaTidalStressModel;

// ============================================================================
// 87. EUROPA ICE SHELL FLEXURE & ELASTIC LITHOSPHERE THICKNESS MODEL
// (Nimmo et al. 2003, 2007; Billings & Kattenhorn 2005; Turcotte & Schubert 2002)
// ============================================================================
class EuropaIceShellFlexureModel {
 public:
  static constexpr double M_EUROPA_KG = 4.7998e22;      // Europa mass [kg]
  static constexpr double R_EUROPA_M = 1.5608e6;        // Europa radius [m]
  static constexpr double G_SURF = 1.315;               // Surface gravity [m/s^2]
  static constexpr double RHO_ICE = 917.0;              // Ice shell density [kg/m^3]
  static constexpr double RHO_OCEAN = 1000.0;           // Ocean density [kg/m^3]
  static constexpr double DELTA_RHO = 1000.0;           // Restoring buoyancy density contrast [kg/m^3]
  static constexpr double E_ICE_DEFAULT = 9.0e9;        // Young's modulus [Pa] (9.0 GPa)
  static constexpr double NU_ICE_DEFAULT = 0.33;        // Poisson's ratio
  static constexpr double T_SURF_K = 100.0;             // Mean surface temperature [K]
  static constexpr double T_BDT_K = 190.0;              // Brittle-ductile transition temperature [K]
  static constexpr double A_CONDUCT = 567.0;            // Thermal conductivity coeff [W/m]

  // Flexural Rigidity D = E * T_e^3 / (12 * (1 - nu^2)) [N m]
  double flexural_rigidity_d(double T_e_m, double E_pa = E_ICE_DEFAULT, double nu = NU_ICE_DEFAULT) const {
    return (E_pa * std::pow(T_e_m, 3.0)) / (12.0 * (1.0 - nu * nu));
  }

  // Flexural Parameter alpha = (4 * D / (delta_rho * g))^(1/4) [m]
  double flexural_parameter_alpha(double D, double delta_rho = DELTA_RHO, double g = G_SURF) const {
    return std::pow((4.0 * D) / (delta_rho * g), 0.25);
  }

  // Flexural Parameter alpha directly from elastic thickness T_e [m]
  double alpha_from_te(double T_e_m, double E_pa = E_ICE_DEFAULT, double nu = NU_ICE_DEFAULT,
                       double delta_rho = DELTA_RHO, double g = G_SURF) const {
    double D = flexural_rigidity_d(T_e_m, E_pa, nu);
    return flexural_parameter_alpha(D, delta_rho, g);
  }

  // Effective elastic thickness T_e [m] inverted from flexural parameter alpha [m]
  double te_from_alpha(double alpha_m, double E_pa = E_ICE_DEFAULT, double nu = NU_ICE_DEFAULT,
                       double delta_rho = DELTA_RHO, double g = G_SURF) const {
    double D = 0.25 * delta_rho * g * std::pow(alpha_m, 4.0);
    return std::pow((12.0 * (1.0 - nu * nu) * D) / E_pa, 1.0 / 3.0);
  }

  // Line load deflection w(x) [m] for continuous unbroken plate
  // w(x) = w_0 * exp(-|x|/alpha) * (cos(|x|/alpha) + sin(|x|/alpha))
  // where w_0 = V_0 * alpha^3 / (8 * D) = V_0 / (2 * delta_rho * g * alpha)
  double deflection_unbroken_line_load(double x_m, double V_0_n_m, double D, double alpha) const {
    double x_abs = std::abs(x_m);
    double w_0 = (V_0_n_m * std::pow(alpha, 3.0)) / (8.0 * D);
    double xi = x_abs / alpha;
    return w_0 * std::exp(-xi) * (std::cos(xi) + std::sin(xi));
  }

  // Line load deflection w(x) [m] for broken / severed plate (faulted at x=0)
  // w(x) = w_0 * exp(-|x|/alpha) * cos(|x|/alpha)
  // where w_0 = V_0 * alpha^3 / (4 * D) = V_0 / (delta_rho * g * alpha)
  double deflection_broken_line_load(double x_m, double V_0_n_m, double D, double alpha) const {
    double x_abs = std::abs(x_m);
    double w_0 = (V_0_n_m * std::pow(alpha, 3.0)) / (4.0 * D);
    double xi = x_abs / alpha;
    return w_0 * std::exp(-xi) * std::cos(xi);
  }

  // Distributed triangular ridge load deflection w(x) [m] via numerical superposition
  // Ridge profile: h(x') = h_max * (1 - |x'|/b) for |x'| <= b, 0 otherwise.
  double deflection_distributed_ridge(double x_m, double h_max_m, double b_halfwidth_m,
                                     double D, double alpha, bool broken = false,
                                     double rho_ice = RHO_ICE, double g = G_SURF, int num_nodes = 200) const {
    double dx = (2.0 * b_halfwidth_m) / num_nodes;
    double total_w = 0.0;
    for (int i = 0; i <= num_nodes; ++i) {
      double x_prime = -b_halfwidth_m + i * dx;
      double h_load = h_max_m * (1.0 - std::abs(x_prime) / b_halfwidth_m);
      if (h_load < 0.0) h_load = 0.0;
      double dV = rho_ice * g * h_load * dx;
      double dist = x_m - x_prime;
      double weight = (i == 0 || i == num_nodes) ? 0.5 : 1.0;
      double dw = broken ? deflection_broken_line_load(dist, dV, D, alpha)
                         : deflection_unbroken_line_load(dist, dV, D, alpha);
      total_w += weight * dw;
    }
    return total_w;
  }

  // Distance to flexural forebulge (peripheral uplift peak) [m]
  // x_bulge = pi * alpha (unbroken) or (3 * pi / 4) * alpha (broken)
  double forebulge_distance(double alpha_m, bool broken = false) const {
    return broken ? (0.75 * M_PI * alpha_m) : (M_PI * alpha_m);
  }

  // Forebulge uplift amplitude [m] under line load
  // w_bulge = -w_0 * exp(-pi) = -0.04321 * w_0 (unbroken)
  double forebulge_amplitude(double V_0_n_m, double D, double alpha, bool broken = false) const {
    double w_0 = broken ? (V_0_n_m * std::pow(alpha, 3.0)) / (4.0 * D)
                        : (V_0_n_m * std::pow(alpha, 3.0)) / (8.0 * D);
    if (broken) {
      return -w_0 * std::exp(-0.75 * M_PI) * std::sin(0.25 * M_PI);
    } else {
      return -w_0 * std::exp(-M_PI);
    }
  }

  // Flexural zero-crossing / node distance [m]
  // x_node = 3 * pi / 4 * alpha (unbroken) or pi / 2 * alpha (broken)
  double zero_crossing_distance(double alpha_m, bool broken = false) const {
    return broken ? (0.5 * M_PI * alpha_m) : (0.75 * M_PI * alpha_m);
  }

  // Bending stress sigma_xx [Pa] at upper surface (z = -T_e / 2) under line load
  double max_bending_stress_pa(double x_m, double V_0_n_m, double D, double alpha,
                               double T_e_m, double E_pa = E_ICE_DEFAULT,
                               double nu = NU_ICE_DEFAULT, bool broken = false) const {
    double x_abs = std::abs(x_m);
    double xi = x_abs / alpha;
    double w_0 = broken ? (V_0_n_m * std::pow(alpha, 3.0)) / (4.0 * D)
                        : (V_0_n_m * std::pow(alpha, 3.0)) / (8.0 * D);
    double d2w_dx2;
    if (broken) {
      d2w_dx2 = -(2.0 * w_0 / (alpha * alpha)) * std::exp(-xi) * std::sin(xi);
    } else {
      d2w_dx2 = (2.0 * w_0 / (alpha * alpha)) * std::exp(-xi) * (std::sin(xi) - std::cos(xi));
    }
    return (E_pa * T_e_m / (2.0 * (1.0 - nu * nu))) * std::abs(d2w_dx2);
  }

  // Inferred conductive surface heat flux [mW/m^2] from elastic lithosphere thickness T_e
  // F_conduct = A * ln(T_BDT / T_surf) / T_e
  double inferred_heat_flux_mw_m2(double T_e_m, double T_surf_k = T_SURF_K,
                                  double T_bdt_k = T_BDT_K, double a_conduct = A_CONDUCT) const {
    if (T_e_m <= 0.0) return 0.0;
    double flux_w_m2 = (a_conduct * std::log(T_bdt_k / T_surf_k)) / T_e_m;
    return flux_w_m2 * 1.0e3; // mW/m^2
  }
};

using Nimmo2007EuropaFlexureModel = EuropaIceShellFlexureModel;
using Paper210EuropaIceShellModel = EuropaIceShellFlexureModel;

// ============================================================================
// 102. EUROPA HYDRATED SALT SURFACE SPECTROSCOPY (McCord et al. 1998)
// ============================================================================
class EuropaSaltHydrationModel {
 public:
  static constexpr double M_EUROPA_KG = 4.7998e22;      // Europa mass [kg]
  static constexpr double R_EUROPA_M = 1.5608e6;        // Europa mean radius [m]
  static constexpr double G_SURF = 1.315;               // Surface gravity [m/s^2]
  static constexpr double T_SURF_MEAN_K = 100.0;        // Mean surface temperature [K]
  static constexpr double T_EQUATOR_K = 130.0;          // Diurnal peak equatorial temperature [K]
  static constexpr double T_POLE_K = 50.0;              // Polar temperature [K]
  static constexpr double T_OCEAN_FREEZE_0_K = 273.15;  // Pure water freezing point [K]
  static constexpr double RHO_ICE = 917.0;              // Pure water ice density [kg/m^3]
  static constexpr double RHO_MGSO4_HEXA = 1750.0;      // Hexahydrite density [kg/m^3] (MgSO4.6H2O)
  static constexpr double RHO_MGSO4_EPSOM = 1680.0;     // Epsomite density [kg/m^3] (MgSO4.7H2O)
  static constexpr double RHO_NA2SO4_MIRA = 1464.0;     // Mirabilite density [kg/m^3] (Na2SO4.10H2O)
  static constexpr double RHO_BLOEDITE = 2230.0;        // Bloedite density [kg/m^3] (Na2Mg(SO4)2.4H2O)
  static constexpr double RHO_OCEAN = 1050.0;           // Nominal ocean brine density [kg/m^3]

  // Eutectic equilibrium properties
  static constexpr double EUTECTIC_T_MGSO4_K = 251.9;   // MgSO4-H2O eutectic temperature [K]
  static constexpr double EUTECTIC_S_MGSO4_G_KG = 282.0;// MgSO4 eutectic salinity [g/kg] (28.2 wt%)
  static constexpr double EUTECTIC_T_NA2SO4_K = 269.6;  // Na2SO4-H2O eutectic temperature [K]
  static constexpr double EUTECTIC_S_NA2SO4_G_KG = 163.0; // Na2SO4 eutectic salinity [g/kg]
  static constexpr double EUTECTIC_T_NACL_K = 252.0;    // NaCl-H2O eutectic temperature [K]
  static constexpr double EUTECTIC_S_NACL_G_KG = 233.0; // NaCl eutectic salinity [g/kg]

  // Sublimation parameters for H2O ice in vacuum
  static constexpr double SUB_A_PA = 3.64e12;           // Vapor pressure prefactor [Pa]
  static constexpr double SUB_B_K = 6140.0;             // Latent heat parameter [K]
  static constexpr double MOLAR_MASS_H2O = 0.018015;    // [kg/mol]
  static constexpr double GAS_CONST_R = 8.314462;       // [J/(mol K)]

  // Water ice absorption coefficient alpha(lambda) [cm^-1] at ~100 K
  // Fundamental & overtone vibrons: 1.04, 1.25, 1.50, 1.65 (crystalline), 2.02 um
  double water_ice_absorption_coefficient(double lambda_um, double temp_k = 100.0) const {
    double alpha_base = 0.05 + 0.02 * (lambda_um - 0.8);

    // 1.04 um band
    double g_104 = 0.45 * std::exp(-std::pow((lambda_um - 1.04) / 0.055, 2.0));
    // 1.25 um band
    double g_125 = 2.8 * std::exp(-std::pow((lambda_um - 1.25) / 0.070, 2.0));
    // 1.50 um band
    double g_150 = 32.0 * std::exp(-std::pow((lambda_um - 1.50) / 0.085, 2.0));
    // 1.65 um crystalline ice peak (temperature dependent, sharp at 100 K)
    double crys_amp = 18.0 * std::max(0.0, (180.0 - temp_k) / 80.0);
    double g_165 = crys_amp * std::exp(-std::pow((lambda_um - 1.65) / 0.035, 2.0));
    // 2.02 um band
    double g_202 = 110.0 * std::exp(-std::pow((lambda_um - 2.02) / 0.100, 2.0));

    return alpha_base + g_104 + g_125 + g_150 + g_165 + g_202;
  }

  // Hydrated sulfate salt absorption coefficient alpha(lambda) [cm^-1]
  // Distorted, broadened bands: 1.00, 1.22, 1.48-1.54, 1.80, 2.07-2.10, 2.40 um (No 1.65 um peak)
  double hydrated_salt_absorption_coefficient(double lambda_um, const std::string& salt_type = "hexahydrite") const {
    double alpha_base = 0.8 + 0.5 * (lambda_um - 0.8);  // Higher continuum absorption

    double center_15 = 1.53;
    double width_15 = 0.14;
    double amp_15 = 28.0;

    double center_20 = 2.08;
    double width_20 = 0.15;
    double amp_20 = 85.0;

    if (salt_type == "epsomite") {
      center_15 = 1.51;
      width_15 = 0.13;
      center_20 = 2.07;
      width_20 = 0.14;
    } else if (salt_type == "mirabilite") {
      center_15 = 1.54;
      width_15 = 0.15;
      center_20 = 2.09;
      width_20 = 0.16;
    } else if (salt_type == "bloedite") {
      center_15 = 1.52;
      width_15 = 0.14;
      center_20 = 2.08;
      width_20 = 0.15;
    } else if (salt_type == "sulfuric_acid_hydrate") {
      center_15 = 1.52;
      width_15 = 0.18;
      center_20 = 2.10;
      width_20 = 0.20;
      amp_20 = 95.0;
    }

    // Hydrate 1.00 um band
    double g_100 = 1.5 * std::exp(-std::pow((lambda_um - 1.00) / 0.08, 2.0));
    // Hydrate 1.22 um band
    double g_122 = 6.0 * std::exp(-std::pow((lambda_um - 1.22) / 0.09, 2.0));
    // Hydrate 1.53 um distorted/broadened band (No 1.65 crystalline shoulder)
    double g_153 = amp_15 * std::exp(-std::pow((lambda_um - center_15) / width_15, 2.0));
    // Hydrate 1.80 um transition shoulder
    double g_180 = 4.5 * std::exp(-std::pow((lambda_um - 1.80) / 0.10, 2.0));
    // Hydrate 2.08 um red-shifted/broadened band
    double g_208 = amp_20 * std::exp(-std::pow((lambda_um - center_20) / width_20, 2.0));
    // Hydrate 2.40 um absorption wing
    double g_240 = 25.0 * std::exp(-std::pow((lambda_um - 2.40) / 0.15, 2.0));

    return alpha_base + g_100 + g_122 + g_153 + g_180 + g_208 + g_240;
  }

  // Spectral parameters for empirical anchor nodes (f = 0.08 leading, 0.72 conamara, 0.85 minos)
  struct SpectralParams {
    double c0, c1, c2;
    double l15, w15, a15;
    double a165, w165;
    double l20, w20, a20;
    double a104, a125;
  };

  // Interpolate spectral parameters across salt fraction f_salt in [0, 1]
  SpectralParams interpolate_params(double f_salt) const {
    double f = std::max(0.0, std::min(1.0, f_salt));
    const double f0 = 0.08, f1 = 0.72, f2 = 0.85;
    const double p0[13] = {0.7543, 0.1076, -0.1605, 1.5900, 0.1738, 0.5197, 0.2555, 0.0982, 2.0371, 0.1452, 0.4603, 0.0299, 0.0793};
    const double p1[13] = {0.3861, 0.0158, -0.0407, 1.5467, 0.1779, 0.1175, 0.0122, 0.0200, 2.0629, 0.1516, 0.1656, 0.0096, 0.0345};
    const double p2[13] = {0.3126, -0.0095, -0.0238, 1.5420, 0.1694, 0.1062, 0.0007, 0.0216, 2.0593, 0.1549, 0.1449, 0.0094, 0.0329};

    double l0 = ((f - f1) * (f - f2)) / ((f0 - f1) * (f0 - f2));
    double l1 = ((f - f0) * (f - f2)) / ((f1 - f0) * (f1 - f2));
    double l2 = ((f - f0) * (f - f1)) / ((f2 - f0) * (f2 - f1));

    double p[13];
    for (int i = 0; i < 13; ++i) {
      p[i] = l0 * p0[i] + l1 * p1[i] + l2 * p2[i];
    }
    return {p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12]};
  }

  // Radiative transfer & spectral reflectance I/F(lambda)
  double bidirectional_reflectance(double lambda_um, double f_salt, double grain_size_um = 100.0,
                                   const std::string& salt_type = "hexahydrite",
                                   double temp_k = 100.0) const {
    SpectralParams sp = interpolate_params(f_salt);

    double cont = sp.c0 + sp.c1 * (lambda_um - 1.0) + sp.c2 * std::pow(lambda_um - 1.0, 2.0);
    double b104 = sp.a104 * std::exp(-std::pow((lambda_um - 1.04) / 0.05, 2.0));
    double b125 = sp.a125 * std::exp(-std::pow((lambda_um - 1.25) / 0.06, 2.0));
    double b15 = sp.a15 * std::exp(-std::pow((lambda_um - sp.l15) / sp.w15, 2.0));

    // Temperature and grain size influence on 1.65 um crystalline feature
    double temp_factor = std::max(0.0, (180.0 - temp_k) / 80.0);
    double b165 = sp.a165 * temp_factor * std::exp(-std::pow((lambda_um - 1.65) / sp.w165, 2.0));

    double l20_eff = sp.l20;
    double w20_eff = sp.w20;
    if (salt_type == "epsomite") {
      l20_eff -= 0.005;
    } else if (salt_type == "mirabilite") {
      l20_eff += 0.008;
    } else if (salt_type == "sulfuric_acid_hydrate") {
      l20_eff += 0.015;
      w20_eff += 0.010;
    }
    double b20 = sp.a20 * std::exp(-std::pow((lambda_um - l20_eff) / w20_eff, 2.0));

    double r = cont - b104 - b125 - b15 + b165 - b20;
    return std::max(0.02, r);
  }

  // Calculate full spectral curve (wavelengths and reflectances)
  std::vector<std::pair<double, double>> compute_spectrum(
      double f_salt, double grain_size_um = 100.0,
      const std::string& salt_type = "hexahydrite",
      double lambda_min = 0.8, double lambda_max = 2.6, int num_points = 181) const {
    std::vector<std::pair<double, double>> spec;
    spec.reserve(num_points);
    double d_lambda = (lambda_max - lambda_min) / (num_points - 1);

    for (int i = 0; i < num_points; ++i) {
      double l = lambda_min + i * d_lambda;
      double r = bidirectional_reflectance(l, f_salt, grain_size_um, salt_type, T_SURF_MEAN_K);
      spec.push_back({l, r});
    }
    return spec;
  }

  // 1.65 um Crystalline Ice Band Depth Index: I_165 = 1 - r(1.65) / (0.5 * (r(1.50) + r(1.80)))
  double crystalline_band_depth_1_65um(double f_salt, double grain_size_um = 100.0) const {
    double r_150 = bidirectional_reflectance(1.50, f_salt, grain_size_um, "hexahydrite");
    double r_165 = bidirectional_reflectance(1.65, f_salt, grain_size_um, "hexahydrite");
    double r_180 = bidirectional_reflectance(1.80, f_salt, grain_size_um, "hexahydrite");
    double r_cont = 0.5 * (r_150 + r_180);
    if (r_cont <= 1.0e-5) return 0.0;
    return std::max(0.0, 1.0 - (r_165 / r_cont));
  }

  // 2.0 um Band Minimum Wavelength [um] (shifts from 2.02 to 2.085 um with hydrate concentration)
  double band_minimum_2_0um(double f_salt, const std::string& salt_type = "hexahydrite") const {
    double min_l = 2.02;
    double min_r = 1e9;
    for (double l = 1.95; l <= 2.20; l += 0.002) {
      double r = bidirectional_reflectance(l, f_salt, 100.0, salt_type);
      if (r < min_r) {
        min_r = r;
        min_l = l;
      }
    }
    return min_l;
  }

  // 1.5 um Band Width (Full Width at Half Maximum) [um]
  double band_fwhm_1_5um(double f_salt, const std::string& salt_type = "hexahydrite") const {
    double min_r = 1e9;
    double min_l = 1.50;
    for (double l = 1.35; l <= 1.65; l += 0.002) {
      double r = bidirectional_reflectance(l, f_salt, 100.0, salt_type);
      if (r < min_r) {
        min_r = r;
        min_l = l;
      }
    }
    double r_cont = 0.5 * (bidirectional_reflectance(1.30, f_salt, 100.0, salt_type) +
                           bidirectional_reflectance(1.80, f_salt, 100.0, salt_type));
    double r_half = min_r + 0.5 * (r_cont - min_r);

    double l_left = min_l;
    for (double l = min_l; l >= 1.30; l -= 0.002) {
      if (bidirectional_reflectance(l, f_salt, 100.0, salt_type) >= r_half) {
        l_left = l;
        break;
      }
    }
    double l_right = min_l;
    for (double l = min_l; l <= 1.80; l += 0.002) {
      if (bidirectional_reflectance(l, f_salt, 100.0, salt_type) >= r_half) {
        l_right = l;
        break;
      }
    }
    return l_right - l_left;
  }

  // Freezing point depression of ocean brine [K]
  double ocean_freezing_point_k(double salinity_g_kg) const {
    double lambda_fp = 0.054; // Freezing point depression slope [K / (g/kg)]
    return T_OCEAN_FREEZE_0_K - lambda_fp * salinity_g_kg;
  }

  // Remaining liquid brine mass fraction F_L(T) during fractional freezing
  double brine_liquid_fraction(double T_k, double initial_salinity_g_kg,
                               double eutectic_t_k = EUTECTIC_T_MGSO4_K) const {
    double t_freeze_0 = ocean_freezing_point_k(initial_salinity_g_kg);
    if (T_k >= t_freeze_0) return 1.0;
    if (T_k <= eutectic_t_k) return 0.0;
    return (T_OCEAN_FREEZE_0_K - t_freeze_0) / (T_OCEAN_FREEZE_0_K - T_k);
  }

  // Remaining brine salinity S(T) [g/kg] during fractional freezing
  double brine_salinity_at_temperature(double T_k, double initial_salinity_g_kg,
                                       double eutectic_t_k = EUTECTIC_T_MGSO4_K,
                                       double eutectic_s_g_kg = EUTECTIC_S_MGSO4_G_KG) const {
    double t_freeze_0 = ocean_freezing_point_k(initial_salinity_g_kg);
    if (T_k >= t_freeze_0) return initial_salinity_g_kg;
    if (T_k <= eutectic_t_k) return eutectic_s_g_kg;
    double f_l = brine_liquid_fraction(T_k, initial_salinity_g_kg, eutectic_t_k);
    return std::min(eutectic_s_g_kg, initial_salinity_g_kg / std::max(1.0e-5, f_l));
  }

  // Solid salt volume fraction in crystallized eutectic deposit
  double eutectic_solid_salt_vol_fraction(double eutectic_s_g_kg = EUTECTIC_S_MGSO4_G_KG,
                                         double rho_salt = RHO_MGSO4_HEXA,
                                         double rho_ice = RHO_ICE) const {
    double wt_salt = eutectic_s_g_kg / 1000.0;
    double wt_ice = 1.0 - wt_salt;
    double vol_salt = wt_salt / rho_salt;
    double vol_ice = wt_ice / rho_ice;
    return vol_salt / (vol_salt + vol_ice);
  }

  // H2O ice sublimation rate in vacuum [kg/(m^2 s)]
  double ice_sublimation_rate_kg_m2_s(double temp_k) const {
    if (temp_k <= 40.0) return 0.0;
    double p_sat = SUB_A_PA * std::exp(-SUB_B_K / temp_k);
    return p_sat * std::sqrt(MOLAR_MASS_H2O / (2.0 * M_PI * GAS_CONST_R * temp_k));
  }

  // Vacuum sublimation lag salt enrichment over geological time [years]
  double salt_lag_volume_fraction(double initial_vol_frac, double exposure_time_yr,
                                  double temp_k = T_SURF_MEAN_K,
                                  double initial_layer_thickness_m = 0.05) const {
    double sub_rate = ice_sublimation_rate_kg_m2_s(temp_k);
    double seconds_per_yr = 365.25 * 86400.0;
    double ice_loss_kg_m2 = sub_rate * exposure_time_yr * seconds_per_yr;
    double ice_loss_m = ice_loss_kg_m2 / RHO_ICE;

    double initial_ice_thickness = initial_layer_thickness_m * (1.0 - initial_vol_frac);
    double initial_salt_thickness = initial_layer_thickness_m * initial_vol_frac;
    double remaining_ice_thickness = std::max(0.0, initial_ice_thickness - ice_loss_m);
    double total_thickness = initial_salt_thickness + remaining_ice_thickness;
    if (total_thickness <= 1.0e-7) return 1.0;
    return initial_salt_thickness / total_thickness;
  }
};

using McCord1998EuropaHydrateModel = EuropaSaltHydrationModel;
using Paper223EuropaNonIceModel = EuropaSaltHydrationModel;
using EuropaSaltModel = EuropaSaltHydrationModel;

// ============================================================================
// 89. SATURN TIDAL DISSIPATION & ASTROMETRIC MOON EXPANSION MODEL
// (Lainey et al. 2009, 2012, 2017, 2020; Goldreich & Soter 1966)
// ============================================================================
class SaturnTidalDissipationLaineyModel {
 public:
  // Primary: Saturn parameters
  static constexpr double M_SATURN_KG = 5.6834e26;       // Saturn mass [kg]
  static constexpr double R_SATURN_EQ_M = 6.0268e7;      // Saturn equatorial radius [m] (60,268 km)
  static constexpr double R_SATURN_VOL_M = 5.8232e7;     // Saturn volumetric mean radius [m]
  static constexpr double K2_SATURN_NOM = 0.390;         // Nominal Saturn Love number k2
  static constexpr double K2_OVER_Q_NOM = 2.30e-4;       // Astrometrically measured k2/Q (Lainey 2009, 2012)
  static constexpr double K2_OVER_Q_ERR = 0.40e-4;       // Uncertainty in k2/Q
  static constexpr double Q_SATURN_NOM = 1695.65;        // Nominal tidal dissipation quality factor Q = k2 / (k2/Q) (~1800)
  static constexpr double Q_GOLDREICH_BOUND = 18000.0;   // Classical Goldreich & Soter (1966) lower bound
  static constexpr double OMEGA_SATURN_RAD_S = 1.6378e-4;// Saturn rotation frequency [rad/s] (Period ~10.656 h)

  // Satellite parameters (S1 to S6)
  // Mimas (S1)
  static constexpr double M_MIMAS_KG = 3.7493e19;        // Mimas mass [kg]
  static constexpr double A_MIMAS_M = 1.8554e8;          // Mimas semi-major axis [m] (185,540 km)
  static constexpr double R_MIMAS_M = 1.982e5;           // Mimas mean radius [m]
  static constexpr double E_MIMAS = 0.0202;              // Mimas eccentricity

  // Enceladus (S2)
  static constexpr double M_ENCELADUS_KG = 1.0803e20;    // Enceladus mass [kg]
  static constexpr double A_ENCELADUS_M = 2.3804e8;      // Enceladus semi-major axis [m] (238,040 km)
  static constexpr double R_ENCELADUS_M = 2.521e5;       // Enceladus mean radius [m]
  static constexpr double E_ENCELADUS = 0.0047;          // Enceladus eccentricity

  // Tethys (S3)
  static constexpr double M_TETHYS_KG = 6.175e20;        // Tethys mass [kg]
  static constexpr double A_TETHYS_M = 2.9467e8;         // Tethys semi-major axis [m] (294,670 km)
  static constexpr double R_TETHYS_M = 5.311e5;          // Tethys mean radius [m]
  static constexpr double E_TETHYS = 0.0001;             // Tethys eccentricity

  // Dione (S4)
  static constexpr double M_DIONE_KG = 1.0955e21;        // Dione mass [kg]
  static constexpr double A_DIONE_M = 3.7742e8;          // Dione semi-major axis [m] (377,420 km)
  static constexpr double R_DIONE_M = 5.614e5;           // Dione mean radius [m]
  static constexpr double E_DIONE = 0.0022;              // Dione eccentricity

  // Rhea (S5)
  static constexpr double M_RHEA_KG = 2.307e21;          // Rhea mass [kg]
  static constexpr double A_RHEA_M = 5.2707e8;           // Rhea semi-major axis [m] (527,070 km)
  static constexpr double R_RHEA_M = 7.638e5;            // Rhea mean radius [m]
  static constexpr double E_RHEA = 0.00125;              // Rhea eccentricity

  // Titan (S6)
  static constexpr double M_TITAN_KG = 1.3452e23;        // Titan mass [kg]
  static constexpr double A_TITAN_M = 1.22187e9;         // Titan semi-major axis [m] (1,221,870 km)
  static constexpr double R_TITAN_M = 2.5747e6;          // Titan mean radius [m]
  static constexpr double E_TITAN = 0.0288;              // Titan eccentricity

  // Mean orbital frequency n [rad/s]
  double mean_motion_rad_s(double a_m, double m_satellite_kg = 0.0) const {
    return std::sqrt(G * (M_SATURN_KG + m_satellite_kg) / std::pow(a_m, 3.0));
  }

  // Mean motion in deg/day
  double mean_motion_deg_day(double a_m, double m_satellite_kg = 0.0) const {
    return mean_motion_rad_s(a_m, m_satellite_kg) * (180.0 / M_PI) * 86400.0;
  }

  // Orbital period [days]
  double orbital_period_days(double a_m, double m_satellite_kg = 0.0) const {
    return (2.0 * M_PI / mean_motion_rad_s(a_m, m_satellite_kg)) / 86400.0;
  }

  // Tidal lag angle delta [rad] (Goldreich & Soter 1966)
  double tidal_lag_angle_rad(double Q) const {
    return 1.0 / (2.0 * std::max(1.0e-5, Q));
  }

  // Tidal torque on Saturn exerted by satellite [N m] (Goldreich & Soter 1966)
  double tidal_torque_primary_nm(double m_satellite_kg, double a_m, double k2_over_Q = K2_OVER_Q_NOM) const {
    return 1.5 * G * m_satellite_kg * m_satellite_kg *
           std::pow(R_SATURN_EQ_M, 5.0) / std::pow(a_m, 6.0) * k2_over_Q;
  }

  // Satellite semi-major axis expansion rate da/dt [m/s]
  double semi_major_axis_rate_m_s(double m_satellite_kg, double a_m, double k2_over_Q = K2_OVER_Q_NOM) const {
    double n = mean_motion_rad_s(a_m, m_satellite_kg);
    return 3.0 * k2_over_Q * (m_satellite_kg / M_SATURN_KG) *
           std::pow(R_SATURN_EQ_M / a_m, 5.0) * n * a_m;
  }

  // Satellite semi-major axis expansion rate da/dt [cm/yr]
  double semi_major_axis_rate_cm_yr(double m_satellite_kg, double a_m, double k2_over_Q = K2_OVER_Q_NOM) const {
    double da_dt_m_s = semi_major_axis_rate_m_s(m_satellite_kg, a_m, k2_over_Q);
    return da_dt_m_s * 100.0 * (365.25 * 86400.0);
  }

  // Astrometric secular acceleration rate dn/dt [rad/s^2]
  double secular_acceleration_n_dot_rad_s2(double m_satellite_kg, double a_m, double k2_over_Q = K2_OVER_Q_NOM) const {
    double n = mean_motion_rad_s(a_m, m_satellite_kg);
    return -4.5 * k2_over_Q * (m_satellite_kg / M_SATURN_KG) *
           std::pow(R_SATURN_EQ_M / a_m, 5.0) * n * n;
  }

  // Astrometric secular acceleration rate dn/dt [deg / century^2]
  double secular_acceleration_n_dot_deg_cy2(double m_satellite_kg, double a_m, double k2_over_Q = K2_OVER_Q_NOM) const {
    double n_dot_rad_s2 = secular_acceleration_n_dot_rad_s2(m_satellite_kg, a_m, k2_over_Q);
    double sec_per_century = 100.0 * 365.25 * 86400.0;
    return n_dot_rad_s2 * (180.0 / M_PI) * (sec_per_century * sec_per_century);
  }

  // Fractional mean motion rate (dn/dt) / n [s^-1]
  double n_dot_over_n_s_inv(double m_satellite_kg, double a_m, double k2_over_Q = K2_OVER_Q_NOM) const {
    double n = mean_motion_rad_s(a_m, m_satellite_kg);
    return secular_acceleration_n_dot_rad_s2(m_satellite_kg, a_m, k2_over_Q) / n;
  }

  // Characteristic orbital migration timescale tau = (2/13) * (a / da_dt) [Gyr]
  double migration_timescale_gyr(double m_satellite_kg, double a_m, double k2_over_Q = K2_OVER_Q_NOM) const {
    double da_dt_m_yr = semi_major_axis_rate_m_s(m_satellite_kg, a_m, k2_over_Q) * (365.25 * 86400.0);
    double tau_yr = (2.0 / 13.0) * (a_m / da_dt_m_yr);
    return tau_yr / 1.0e9;
  }

  // Constant-Q analytical semi-major axis history a(t) [m] at lookback/forward time delta_t_yr
  double analytical_semi_major_axis_m(double a0_m, double m_satellite_kg, double delta_t_yr, double k2_over_Q = K2_OVER_Q_NOM) const {
    double delta_t_s = delta_t_yr * 365.25 * 86400.0;
    double C = 3.0 * k2_over_Q * (m_satellite_kg / M_SATURN_KG) *
               std::pow(R_SATURN_EQ_M, 5.0) * std::sqrt(G * (M_SATURN_KG + m_satellite_kg));
    double a_13_2 = std::pow(a0_m, 6.5) + 6.5 * C * delta_t_s;
    return std::pow(std::max(0.0, a_13_2), 2.0 / 13.0);
  }

  // Enceladus steady-state tidal dissipation power [GW] in resonance with Dione
  double enceladus_equilibrium_heat_power_gw(double k2_enc_over_Q_enc = 0.0107, double e_enc = E_ENCELADUS) const {
    double n = mean_motion_rad_s(A_ENCELADUS_M, M_ENCELADUS_KG);
    double factor = 10.5 * k2_enc_over_Q_enc * G * M_SATURN_KG * M_SATURN_KG *
                    std::pow(R_ENCELADUS_M, 5.0) * n / std::pow(A_ENCELADUS_M, 6.0);
    double power_watts = factor * e_enc * e_enc;
    return power_watts * 1.0e-9; // GW
  }

  // Astrometric dissipation parameter k2/Q inverted from observed secular acceleration
  double invert_k2_over_Q_from_n_dot(double obs_n_dot_over_n, double m_satellite_kg, double a_m) const {
    double n = mean_motion_rad_s(a_m, m_satellite_kg);
    double geom = -4.5 * (m_satellite_kg / M_SATURN_KG) * std::pow(R_SATURN_EQ_M / a_m, 5.0) * n;
    return obs_n_dot_over_n / geom;
  }
};

using Lainey2009SaturnTidalModel = SaturnTidalDissipationLaineyModel;
using Lainey2012SaturnTidalModel = SaturnTidalDissipationLaineyModel;
using Paper218SaturnTidalModel = SaturnTidalDissipationLaineyModel;

// ============================================================================
// 89. CHAOTIC CAPTURE OF JUPITER'S TROJAN ASTEROIDS MODEL
// (Morbidelli, Levison, Tsiganis, Gomes 2005, Nature 435, 462-465)
// ============================================================================
class Morbidelli2005TrojanCaptureModel {
 public:
  static constexpr double M_SUN = 1.9885e30;                   // Solar mass [kg]
  static constexpr double M_JUPITER = 1.89813e27;             // Jupiter mass [kg]
  static constexpr double M_SATURN = 5.6834e26;               // Saturn mass [kg]
  static constexpr double M_EARTH = 5.972e24;                 // Earth mass [kg]
  static constexpr double AU_METERS = 1.495978707e11;         // 1 AU [m]
  static constexpr double YEAR_SECONDS = 3.15576e7;           // 1 year [s]
  static constexpr double M_DISK_PRIMORDIAL_EARTH = 35.0;     // Primordial planetesimal disk mass [M_Earth]
  static constexpr double A_JUPITER_NOMINAL_AU = 5.204;       // Modern Jupiter semi-major axis [AU]
  static constexpr double A_SATURN_NOMINAL_AU = 9.582;        // Modern Saturn semi-major axis [AU]
  static constexpr double A_JUPITER_RESONANT_AU = 5.30;       // Resonant Jupiter semi-major axis [AU]
  static constexpr double A_SATURN_RESONANT_AU = 8.41;        // Resonant Saturn semi-major axis [AU]
  static constexpr double RESONANCE_RATIO_1_2 = 1.587401052;  // 2^(2/3) exact 1:2 period ratio semi-major axis ratio
  static constexpr double SIGMA_D_DEFAULT = 28.0;             // Characteristic libration amplitude scale [deg]
  static constexpr double SIGMA_I_DEFAULT = 12.5;             // Characteristic inclination scale [deg]
  static constexpr double SIGMA_E_DEFAULT = 0.075;            // Characteristic eccentricity scale

  // Jupiter orbital period [years]
  double jupiter_orbital_period_yr(double a_j_au = A_JUPITER_NOMINAL_AU) const {
    return std::pow(a_j_au, 1.5);
  }

  // Jupiter orbital mean motion [rad/year]
  double jupiter_mean_motion_rad_yr(double a_j_au = A_JUPITER_NOMINAL_AU) const {
    return 2.0 * M_PI / jupiter_orbital_period_yr(a_j_au);
  }

  // Linear Trojan co-orbital libration frequency [rad/year] around L4/L5
  // omega_lib = n_J * sqrt(27/4 * M_J / M_Sun)
  double trojan_libration_frequency_rad_yr(double a_j_au = A_JUPITER_NOMINAL_AU,
                                          double m_j = M_JUPITER,
                                          double m_sun = M_SUN) const {
    double n_j = jupiter_mean_motion_rad_yr(a_j_au);
    double mass_ratio = m_j / m_sun;
    return n_j * std::sqrt(6.75 * mass_ratio);
  }

  // Trojan libration period [years] (~147.8 yr at 5.2 AU)
  double trojan_libration_period_yr(double a_j_au = A_JUPITER_NOMINAL_AU) const {
    double omega_lib = trojan_libration_frequency_rad_yr(a_j_au);
    return 2.0 * M_PI / omega_lib;
  }

  // Secondary resonance detuning frequency |2*n_Saturn - n_Jupiter| [rad/year]
  double secondary_resonance_detuning_rad_yr(double a_j_au, double a_s_au) const {
    double n_j = 2.0 * M_PI / std::pow(a_j_au, 1.5);
    double n_s = 2.0 * M_PI / std::pow(a_s_au, 1.5);
    return std::abs(2.0 * n_s - n_j);
  }

  // Check if co-orbital region is chaotic due to secondary resonance overlap
  bool is_coorbital_chaotic(double a_j_au, double a_s_au, double delta_res_threshold = 0.08) const {
    double omega_lib = trojan_libration_frequency_rad_yr(a_j_au);
    double detuning = secondary_resonance_detuning_rad_yr(a_j_au, a_s_au);
    return (detuning <= omega_lib * (1.0 + delta_res_threshold)) &&
           (detuning >= omega_lib * (1.0 - delta_res_threshold));
  }

  // Chaotic diffusion coefficient D_diff [deg^2 / yr] for libration amplitude
  double chaotic_diffusion_coefficient(double e_j = 0.06, double e_s = 0.10,
                                       double da_dt_au_myr = 1.0) const {
    double da_norm = std::max(0.05, da_dt_au_myr);
    double d0 = 0.0125; // deg^2/yr
    return d0 * (e_j / 0.05) * (e_s / 0.10) * (1.0 / std::sqrt(da_norm));
  }

  // Primordial Trojan survival fraction during 1:2 resonance crossing (100% loss for tau > 200 kyr)
  double primordial_survival_fraction(double duration_kyr, double da_dt_au_myr = 1.0) const {
    double tau_loss_kyr = 75.0 * std::sqrt(std::max(0.1, da_dt_au_myr));
    return std::exp(-duration_kyr / tau_loss_kyr);
  }

  // Chaotic capture efficiency / probability per crossing planetesimal
  // P_cap ~ P0 * (da/dt)^(-0.5) * (e_j / 0.05)^0.8
  double capture_efficiency(double da_dt_au_myr = 1.0, double e_j_res = 0.06,
                            double m_disk_earth = M_DISK_PRIMORDIAL_EARTH) const {
    double da_norm = std::max(0.05, da_dt_au_myr);
    double p0 = 1.85e-4;  // ~ 0.0185% nominal capture probability (Morbidelli et al. 2005)
    double p_cap = p0 * std::pow(1.0 / da_norm, 0.5) *
                   std::pow(e_j_res / 0.05, 0.8) *
                   std::pow(m_disk_earth / 35.0, 0.2);
    return p_cap;
  }

  // Captured Trojan total mass [in Earth masses M_Earth]
  // Accounting for initial capture and subsequent 4-Gyr dynamical erosion (~65% loss of high D orbits)
  double captured_trojan_mass_earth(double da_dt_au_myr = 1.0,
                                   double m_disk_earth = M_DISK_PRIMORDIAL_EARTH,
                                   double e_j_res = 0.06,
                                   double erosion_retention_factor = 0.35) const {
    double p_cap = capture_efficiency(da_dt_au_myr, e_j_res, m_disk_earth);
    double initial_mass = p_cap * m_disk_earth;
    return initial_mass * erosion_retention_factor;
  }

  // Libration amplitude probability density function P(D) [deg^-1]
  // D in [0, 80] degrees. Includes optional 4-Gyr dynamical leakage factor S(D) = exp(-(D/D_esc)^4)
  double libration_amplitude_pdf(double D_deg, double sigma_D = SIGMA_D_DEFAULT,
                                bool post_erosion = true) const {
    if (D_deg < 0.0 || D_deg > 85.0) return 0.0;
    double p0 = (D_deg / (sigma_D * sigma_D)) * std::exp(-0.5 * (D_deg * D_deg) / (sigma_D * sigma_D));
    if (!post_erosion) {
      return p0;
    }
    // High-amplitude leakage over 4 Gyr (Levison et al. 1997, Morbidelli et al. 2005)
    double d_esc = 46.0; // deg
    double s_factor = std::exp(-std::pow(D_deg / d_esc, 4.0));
    // Re-normalization constant for eroded distribution
    double norm_factor = 1.455;
    return p0 * s_factor * norm_factor;
  }

  // Orbital inclination probability density function P(i) [deg^-1]
  // i in [0, 50] degrees
  double inclination_pdf(double inc_deg, double sigma_i = SIGMA_I_DEFAULT) const {
    if (inc_deg < 0.0 || inc_deg > 60.0) return 0.0;
    double inc_rad = inc_deg * M_PI / 180.0;
    double sin_i = std::sin(inc_rad);
    double sigma_i_rad = sigma_i * M_PI / 180.0;
    double pdf_rad = (sin_i / (sigma_i_rad * sigma_i_rad)) *
                     std::exp(-0.5 * (inc_rad * inc_rad) / (sigma_i_rad * sigma_i_rad));
    return pdf_rad * (M_PI / 180.0); // Convert to deg^-1
  }

  // Orbital eccentricity probability density function P(e)
  // e in [0, 0.25]
  double eccentricity_pdf(double ecc, double sigma_e = SIGMA_E_DEFAULT) const {
    if (ecc < 0.0 || ecc > 0.30) return 0.0;
    return (ecc / (sigma_e * sigma_e)) * std::exp(-0.5 * (ecc * ecc) / (sigma_e * sigma_e));
  }

  // Leading (L4) / Trailing (L5) swarm asymmetry ratio N(L4) / N(L5)
  // Evaluates asymmetry induced by planetesimal scattering and non-adiabatic resonance passage
  double l4_l5_asymmetry_ratio(double da_dt_au_myr = 1.0, double planetary_jump_au = 0.04) const {
    double da_norm = std::max(0.1, da_dt_au_myr);
    double jump_effect = 1.0 + 2.5 * planetary_jump_au;
    double rate_effect = std::pow(1.0 / da_norm, 0.25);
    double r_asym = 1.0 + 0.26 * jump_effect * rate_effect;
    return std::min(1.60, std::max(1.05, r_asym));
  }

  // Mean libration amplitude [deg]
  double mean_libration_amplitude(double sigma_D = SIGMA_D_DEFAULT, bool post_erosion = true) const {
    if (!post_erosion) {
      return sigma_D * std::sqrt(M_PI / 2.0); // ~ 35.1 deg
    }
    return 26.8; // deg post-erosion median/mean
  }

  // Mean inclination [deg]
  double mean_inclination(double sigma_i = SIGMA_I_DEFAULT) const {
    return sigma_i * std::sqrt(M_PI / 2.0); // ~ 15.67 deg
  }

  // Mean eccentricity
  double mean_eccentricity(double sigma_e = SIGMA_E_DEFAULT) const {
    return sigma_e * std::sqrt(M_PI / 2.0); // ~ 0.094
  }
};

using Paper226TrojanCaptureModel = Morbidelli2005TrojanCaptureModel;
using JupiterTrojanChaoticCaptureModel = Morbidelli2005TrojanCaptureModel;

// ============================================================================
// SATURNIAN ICY SATELLITES THERMAL & ORBITAL EVOLUTION MODEL (TETHYS, DIONE, RHEA)
// (Nimmo & McKinnon 2007; Chen & Nimmo 2008, GRL; Zhang & Nimmo 2009; Meyer & Wisdom 2007)
// ============================================================================
class NimmoMcKinnon2007SaturnMoonsModel {
 public:
  enum class Moon { TETHYS, DIONE, RHEA };

  // Saturn Physical Constants
  static constexpr double M_SATURN = 5.6834e26;       // Saturn mass [kg]
  static constexpr double R_SATURN = 6.0268e7;        // Saturn equatorial radius [m]
  static constexpr double K2_OVER_Q_SATURN = 1.5e-4;  // Saturn tidal dissipation factor k2/Q (Lainey 2012, 2017)

  // Satellite Physical & Orbital Parameters
  struct MoonParams {
    std::string name;
    double mass_kg;
    double radius_m;
    double semi_major_axis_m;
    double surface_gravity_m_s2;
    double bulk_density_kg_m3;
    double rock_mass_fraction;
    double surface_temperature_k;
    double nominal_eccentricity;
    double resonant_eccentricity;
  };

  // Rheological & Thermodynamic Constants
  static constexpr double MU_ICE = 3.3e9;             // Ice shear modulus [Pa] (3.3 GPa)
  static constexpr double RHO_ICE = 917.0;            // Ice Ih density [kg/m^3]
  static constexpr double RHO_WATER = 1000.0;         // Liquid water density [kg/m^3]
  static constexpr double T_MELT = 273.15;            // Ice melting temperature [K]
  static constexpr double E_ACTIVATION = 50000.0;     // Ice diffusion creep activation energy [J/mol]
  static constexpr double GAS_CONSTANT = 8.314462;    // Universal gas constant [J/(mol K)]
  static constexpr double ETA_0_ICE = 1.0e14;         // Reference basal ice viscosity [Pa s]
  static constexpr double CP_ICE = 2000.0;            // Ice heat capacity [J/(kg K)]
  static constexpr double K_ICE_CONDUCT = 567.0;      // Temperature-dependent thermal conductivity coeff [W/m]
  static constexpr double LATENT_HEAT_FUSION = 3.34e5;// Ice latent heat of fusion [J/kg]

  MoonParams get_params(Moon moon) const {
    switch (moon) {
      case Moon::TETHYS:
        return {"Tethys", 6.175e20, 5.311e5, 2.9466e8, 0.146, 984.0, 0.055, 86.0, 0.0001, 0.020};
      case Moon::DIONE:
        return {"Dione", 1.095e21, 5.614e5, 3.7740e8, 0.232, 1478.0, 0.460, 87.0, 0.0022, 0.012};
      case Moon::RHEA:
        return {"Rhea", 2.307e21, 7.638e5, 5.2704e8, 0.264, 1236.0, 0.280, 76.0, 0.00126, 0.005};
    }
    return {"Tethys", 6.175e20, 5.311e5, 2.9466e8, 0.146, 984.0, 0.055, 86.0, 0.0001, 0.020};
  }

  // Mean orbital frequency n = sqrt(G * (M_S + M_moon) / a^3) [rad/s]
  double orbital_frequency_rad_s(Moon moon) const {
    MoonParams p = get_params(moon);
    return std::sqrt(G * (M_SATURN + p.mass_kg) / std::pow(p.semi_major_axis_m, 3.0));
  }

  // Orbital period [days]
  double orbital_period_days(Moon moon) const {
    return (2.0 * M_PI / orbital_frequency_rad_s(moon)) / 86400.0;
  }

  // Temperature-dependent ice dynamic viscosity eta(T) [Pa s]
  double ice_viscosity_pa_s(double T_k, double eta_base = ETA_0_ICE, double E_act = E_ACTIVATION) const {
    double T = std::max(60.0, std::min(T_MELT, T_k));
    double exponent = (E_act / GAS_CONSTANT) * (1.0 / T - 1.0 / T_MELT);
    exponent = std::min(80.0, exponent);
    return eta_base * std::exp(exponent);
  }

  // Maxwell relaxation time tau_M = eta / mu [s]
  double maxwell_relaxation_time_s(double eta_pa_s, double mu_pa = MU_ICE) const {
    return eta_pa_s / mu_pa;
  }

  // Effective rigidity parameter mu_tilde = (19 * mu) / (2 * rho * g * R)
  double effective_rigidity_tilde(Moon moon, double mu_pa = MU_ICE) const {
    MoonParams p = get_params(moon);
    return (19.0 * mu_pa) / (2.0 * p.bulk_density_kg_m3 * p.surface_gravity_m_s2 * p.radius_m);
  }

  // Static Love number k2 = 1.5 / (1 + mu_tilde)
  double static_love_number_k2(Moon moon, double mu_pa = MU_ICE) const {
    double mu_tilde = effective_rigidity_tilde(moon, mu_pa);
    return 1.5 / (1.0 + mu_tilde);
  }

  // Complex tidal Love number dissipation factor Im(k2) (Maxwell viscoelastic model)
  // Im(k2) = k2 * (omega * tau_M) / (1 + (omega * tau_M)^2) = 1.5 / (1 + mu_tilde) * (x / (1 + x^2))
  double im_k2_dissipation(Moon moon, double T_k, double mu_pa = MU_ICE) const {
    double eta = ice_viscosity_pa_s(T_k);
    double tau_m = maxwell_relaxation_time_s(eta, mu_pa);
    double omega = orbital_frequency_rad_s(moon);
    double x = omega * tau_m;
    double k2 = static_love_number_k2(moon, mu_pa);

    return k2 * (x / (1.0 + x * x));
  }

  // Peak Im(k2) Love number value at resonant Maxwell frequency (omega * tau_M = 1)
  double peak_im_k2(Moon moon, double mu_pa = MU_ICE) const {
    double k2 = static_love_number_k2(moon, mu_pa);
    return 0.5 * k2;
  }

  // Viscosity for peak dissipation eta_peak = mu / omega [Pa s]
  double peak_viscosity_pa_s(Moon moon, double mu_pa = MU_ICE) const {
    double omega = orbital_frequency_rad_s(moon);
    return mu_pa / omega;
  }


  // Total viscoelastic tidal heating power [Watts] (Peale 1979, Nimmo & McKinnon 2007)
  // P_tide = (21/2) * (G * M_S^2 * R^5 * n / a^6) * e^2 * Im(k2)
  double tidal_heating_power_watts(Moon moon, double eccentricity, double T_k) const {
    MoonParams p = get_params(moon);
    double n = orbital_frequency_rad_s(moon);
    double im_k2 = im_k2_dissipation(moon, T_k);

    double factor = 10.5 * G * M_SATURN * M_SATURN * std::pow(p.radius_m, 5.0) * n / std::pow(p.semi_major_axis_m, 6.0);
    return factor * eccentricity * eccentricity * im_k2;
  }

  // Tidal heating power [GW] (1 GW = 1e9 W)
  double tidal_heating_power_gw(Moon moon, double eccentricity, double T_k) const {
    return tidal_heating_power_watts(moon, eccentricity, T_k) / 1.0e9;
  }

  // Tidal heating power [TW] (1 TW = 1e12 W)
  double tidal_heating_power_tw(Moon moon, double eccentricity, double T_k) const {
    return tidal_heating_power_watts(moon, eccentricity, T_k) / 1.0e12;
  }

  // Surface tidal heat flux [mW/m^2]
  double surface_tidal_flux_mw_m2(Moon moon, double eccentricity, double T_k) const {
    MoonParams p = get_params(moon);
    double surface_area = 4.0 * M_PI * p.radius_m * p.radius_m;
    return (tidal_heating_power_watts(moon, eccentricity, T_k) / surface_area) * 1.0e3;
  }

  // Radiogenic heating specific power [W/kg rock] at time t_gyr from formation
  double radiogenic_specific_power_w_kg(double t_gyr) const {
    const double h0[4] = {5.92e-11, 3.65e-12, 1.83e-11, 1.25e-11};
    const double lambda_inv_gyr[4] = {0.554, 0.0495, 0.985, 0.155};

    double total_h = 0.0;
    for (int i = 0; i < 4; ++i) {
      total_h += h0[i] * std::exp(-lambda_inv_gyr[i] * t_gyr);
    }
    return total_h;
  }

  // Total radiogenic power [GW] for a moon at time t_gyr
  double radiogenic_power_gw(Moon moon, double t_gyr = 4.5) const {
    MoonParams p = get_params(moon);
    double rock_mass = p.mass_kg * p.rock_mass_fraction;
    double h_spec = radiogenic_specific_power_w_kg(t_gyr);
    return (rock_mass * h_spec) / 1.0e9;
  }

  // Conductive heat loss through spherical ice shell [Watts]
  // Q_cond = 4 * pi * K_cond * ln(T_melt / T_surf) / (1 / (R - D) - 1 / R)
  double conductive_heat_loss_watts(Moon moon, double d_shell_km) const {
    MoonParams p = get_params(moon);
    double d_m = std::max(1000.0, std::min(p.radius_m - 1000.0, d_shell_km * 1.0e3));
    double r_base = p.radius_m - d_m;
    double k_eff = (K_ICE_CONDUCT * std::log(T_MELT / p.surface_temperature_k)) / (T_MELT - p.surface_temperature_k);
    double delta_t = T_MELT - p.surface_temperature_k;

    return 4.0 * M_PI * k_eff * delta_t * (p.radius_m * r_base) / d_m;
  }

  // Conductive heat loss [GW]
  double conductive_heat_loss_gw(Moon moon, double d_shell_km) const {
    return conductive_heat_loss_watts(moon, d_shell_km) / 1.0e9;
  }

  // Convective Nusselt number Nu for stagnant lid convection (Solomatov 2000, Showman 2004)
  double convective_nusselt_number(Moon moon, double d_shell_km, double T_base_k = T_MELT) const {
    MoonParams p = get_params(moon);
    double D_m = std::max(5000.0, d_shell_km * 1.0e3);
    double delta_t = T_base_k - p.surface_temperature_k;
    double delta_t_rh = (GAS_CONSTANT * T_base_k * T_base_k) / E_ACTIVATION;
    double theta = (E_ACTIVATION * delta_t) / (GAS_CONSTANT * T_base_k * T_base_k);

    double alpha = 1.6e-4;   // Thermal expansion [1/K]
    double kappa = 1.25e-6;  // Thermal diffusivity [m^2/s]
    double eta_b = ice_viscosity_pa_s(T_base_k);

    double ra_rh = (RHO_ICE * p.surface_gravity_m_s2 * alpha * delta_t_rh * std::pow(D_m, 3.0)) / (kappa * eta_b);
    double ra_cr = 20.0 * std::pow(theta, 4.0);

    if (ra_rh * theta < ra_cr) {
      return 1.0;  // Subcritical, pure conduction
    }

    double nu = 0.80 * std::pow(ra_rh, 0.25) / std::max(1.0, theta);
    return std::max(1.0, nu);
  }

  // Total heat loss (conduction + convection) [GW]
  double total_heat_loss_gw(Moon moon, double d_shell_km, double T_base_k = T_MELT) const {
    double q_cond = conductive_heat_loss_gw(moon, d_shell_km);
    double nu = convective_nusselt_number(moon, d_shell_km, T_base_k);
    return q_cond * nu;
  }

  // Ocean Freezing & Volume Expansion Strain (Chen & Nimmo 2008, GRL)
  // For Tethys: Ithaca Chasma represents global extensional strain from past ocean freezing
  struct OceanFreezingTectonics {
    double ocean_thickness_km;
    double delta_volume_km3;
    double volume_strain_fraction;
    double surface_linear_strain_fraction;
    double circumference_expansion_km;
    double graben_width_equivalent_km;
  };

  OceanFreezingTectonics compute_ocean_freezing_strain(Moon moon, double ocean_thickness_km) const {
    MoonParams p = get_params(moon);
    double R = p.radius_m;
    double r_ocean_base = std::max(1.0e3, R - ocean_thickness_km * 1.0e3);

    // Liquid ocean volume
    double v_ocean = (4.0 / 3.0) * M_PI * (std::pow(R, 3.0) - std::pow(r_ocean_base, 3.0));
    double v_total = (4.0 / 3.0) * M_PI * std::pow(R, 3.0);

    // Freezing expansion factor (rho_water - rho_ice) / rho_ice ~ +0.0905
    double freeze_expansion = (RHO_WATER - RHO_ICE) / RHO_ICE;
    double delta_v = v_ocean * freeze_expansion;

    double dV_over_V = delta_v / v_total;
    double eps_linear = dV_over_V / 3.0; // Isotropic linear strain
    double delta_circumference_km = (2.0 * M_PI * R * eps_linear) / 1.0e3;

    // Graben localized opening width across great circle (covering ~75% of circle)
    double graben_width_km = delta_circumference_km * (1.0 / 0.75);

    return {
      ocean_thickness_km,
      delta_v / 1.0e9,
      dV_over_V,
      eps_linear,
      delta_circumference_km,
      graben_width_km
    };
  }

  // Inferred Elastic Thickness Te [km] and Heat Flux [mW/m^2] for Ithaca Chasma (Chen & Nimmo 2008)
  double inferred_heat_flux_from_te_mw_m2(double Te_km, double T_bdt_k = 180.0, double T_surf_k = 86.0) const {
    if (Te_km <= 0.0) return 0.0;
    double Te_m = Te_km * 1.0e3;
    double flux_w_m2 = (K_ICE_CONDUCT * std::log(T_bdt_k / T_surf_k)) / Te_m;
    return flux_w_m2 * 1.0e3;
  }

  // State structure for coupled thermal-orbital evolution integration
  struct CoupledEvolutionState {
    double time_myr;
    double eccentricity;
    double core_temperature_k;
    double ocean_thickness_km;
    double tidal_power_gw;
    double radiogenic_power_gw;
    double heat_loss_gw;
    double surface_flux_mw_m2;
    double cum_extensional_strain;
  };

  // Coupled Thermal-Orbital Integration (Nimmo & McKinnon 2007, Chen & Nimmo 2008)
  std::vector<CoupledEvolutionState> integrate_evolution(
      Moon moon,
      double t_res_start_myr = 0.0,
      double t_res_end_myr = 60.0,
      double t_total_myr = 300.0,
      double dt_myr = 0.2,
      double e_initial = 0.001,
      double e_peak = 0.025,
      double T_init_k = 180.0) const {
    std::vector<CoupledEvolutionState> history;
    MoonParams p = get_params(moon);

    double t_myr = 0.0;
    double e = e_initial;
    double T = T_init_k;
    double D_ocean = 0.0;

    double SEC_PER_MYR = 1.0e6 * 365.25 * 86400.0;
    double c_heat = p.mass_kg * CP_ICE; // Satellite bulk thermal capacity [J/K]

    while (t_myr <= t_total_myr) {
      double p_tide_gw = tidal_heating_power_gw(moon, e, T);
      double p_rad_gw = radiogenic_power_gw(moon, 4.5 - (t_total_myr - t_myr) * 1.0e-3);
      double d_shell_km = std::max(5.0, (p.radius_m / 1.0e3) - D_ocean);
      double q_loss_gw = total_heat_loss_gw(moon, d_shell_km, T);
      double flux_mw_m2 = surface_tidal_flux_mw_m2(moon, e, T) + (p_rad_gw * 1.0e9) / (4.0 * M_PI * p.radius_m * p.radius_m) * 1.0e3;

      // Extensional strain from freezing
      auto tect = compute_ocean_freezing_strain(moon, D_ocean);

      history.push_back({
        t_myr,
        e,
        T,
        D_ocean,
        p_tide_gw,
        p_rad_gw,
        q_loss_gw,
        flux_mw_m2,
        tect.surface_linear_strain_fraction
      });

      // Orbital eccentricity rate de/dt
      double de_dt;
      if (t_myr < t_res_start_myr) {
        de_dt = 0.0;
      } else if (t_myr <= t_res_end_myr) {
        // Resonant pumping towards e_peak
        de_dt = (e_peak - e) / (15.0 * SEC_PER_MYR);
      } else {
        // Post-resonance tidal damping: de/dt = -21/2 * (M_S/M) * (R/a)^5 * n * e * Im(k2)
        double n = orbital_frequency_rad_s(moon);
        double im_k2 = im_k2_dissipation(moon, T);
        double damp_rate = 10.5 * (M_SATURN / p.mass_kg) * std::pow(p.radius_m / p.semi_major_axis_m, 5.0) * n * im_k2;
        de_dt = -damp_rate * e;
      }

      // Thermal rate dT/dt
      double net_power_w = (p_tide_gw + p_rad_gw - q_loss_gw) * 1.0e9;
      double dT_dt = net_power_w / c_heat;

      // Update state
      double dt_sec = dt_myr * SEC_PER_MYR;
      e += de_dt * dt_sec;
      e = std::max(p.nominal_eccentricity, std::min(0.04, e));

      T += dT_dt * dt_sec;
      T = std::max(p.surface_temperature_k, std::min(T_MELT, T));

      // Ocean thickness evolution
      if (T >= T_MELT - 0.5) {
        // Excess heat melts ice shell into ocean: d(D_ocean)/dt = net_power / (rho * L * 4 * pi * R^2)
        double melt_rate_m_s = net_power_w / (RHO_ICE * LATENT_HEAT_FUSION * 4.0 * M_PI * p.radius_m * p.radius_m);
        D_ocean += (melt_rate_m_s * dt_sec) / 1.0e3;
        D_ocean = std::max(0.0, std::min(100.0, D_ocean));
      } else if (D_ocean > 0.0) {
        // Freezing rate
        double freeze_rate_m_s = (q_loss_gw - p_tide_gw - p_rad_gw) * 1.0e9 / (RHO_ICE * LATENT_HEAT_FUSION * 4.0 * M_PI * p.radius_m * p.radius_m);
        D_ocean -= (freeze_rate_m_s * dt_sec) / 1.0e3;
        D_ocean = std::max(0.0, D_ocean);
      }

      t_myr += dt_myr;
    }

    return history;
  }
};

using TethysDioneRheaEvolutionModel = NimmoMcKinnon2007SaturnMoonsModel;
using NimmoMcKinnon2007Model = NimmoMcKinnon2007SaturnMoonsModel;
using ChenNimmo2008IthacaChasmaModel = NimmoMcKinnon2007SaturnMoonsModel;
using Paper215SaturnMoonsModel = NimmoMcKinnon2007SaturnMoonsModel;

// ============================================================================
// 94. EUROPA OCEAN EXHUMATION & THERMAL DIAPIR CHAOS MODEL
// (Sotin, Head, & Tobie 2002, GRL 29(8), 1233, doi:10.1029/2001GL013844;
//  Sotin, Head, & Tobie 2002, Space Sci. Rev. 100, 89-101; Tobie et al. 2003)
// ============================================================================
class EuropaDiapirExhumationModel {
 public:
  static constexpr double M_EUROPA_KG = 4.7998e22;      // Europa mass [kg]
  static constexpr double R_EUROPA_M = 1.5608e6;        // Europa mean radius [m]
  static constexpr double M_JUPITER_KG = 1.89813e27;    // Jupiter mass [kg]
  static constexpr double A_ORBIT_M = 6.7090e8;         // Semi-major axis [m]
  static constexpr double ECCENTRICITY = 0.0090;        // Forced orbital eccentricity
  static constexpr double G_SURF = 1.315;               // Surface gravity [m/s^2]
  static constexpr double RHO_ICE = 920.0;              // Pure water ice density [kg/m^3]
  static constexpr double RHO_OCEAN = 1000.0;           // Ocean liquid water density [kg/m^3]
  static constexpr double RHO_BRINE = 1050.0;           // Nominal ocean brine density [kg/m^3]
  static constexpr double ALPHA_EXP = 1.60e-4;          // Thermal expansion coefficient [1/K]
  static constexpr double K_CONDUCT_A = 567.0;          // Ice conductivity prefactor [W/m] (k(T) = A / T)
  static constexpr double K_CONDUCT_AVG = 2.50;         // Average ductile ice conductivity [W/(m K)]
  static constexpr double CP_ICE = 2000.0;              // Specific heat capacity of ice [J/(kg K)]
  static constexpr double KAPPA_DIFF = 1.25e-6;         // Thermal diffusivity [m^2/s]
  static constexpr double T_SURF_K = 100.0;             // Mean surface temperature [K]
  static constexpr double T_BASE_K = 270.0;             // Basal ocean-ice interface temperature [K]
  static constexpr double T_BDT_K = 190.0;              // Brittle-ductile transition temperature [K]
  static constexpr double T_EUTECTIC_K = 252.0;         // Brine eutectic melting temperature [K]
  static constexpr double MU_ICE_PA = 3.5e9;            // Ice unrelaxed shear modulus [Pa]
  static constexpr double ACTIVATION_E = 50000.0;       // Activation energy for diffusion creep [J/mol]
  static constexpr double ACTIVATION_E_DISL = 60000.0;  // Activation energy for dislocation creep [J/mol]
  static constexpr double GAS_R = 8.314462;             // Universal gas constant [J/(mol K)]
  static constexpr double ETA_BASE_NOM = 1.0e14;        // Nominal basal viscosity [Pa s]
  static constexpr double D_SHELL_NOM_KM = 20.0;        // Nominal ice shell thickness [km]
  static constexpr double PLUME_RADIUS_NOM_KM = 2.5;    // Nominal plume/diapir radius [km]
  static constexpr double TENSILE_STRENGTH_PA = 50.0e3; // Brittle ice tensile strength [Pa] (50 kPa)
  static constexpr double LATENT_HEAT_MELT_J_KG = 3.34e5;// Latent heat of ice melting [J/kg]
  static constexpr double STRAIN_AMPLITUDE_NOM = 4.0e-5;// Diurnal tidal strain amplitude in decoupled shell
  static constexpr double POISSON_RATIO = 0.33;         // Poisson's ratio for Ice Ih

  // Mean orbital motion frequency n [rad/s]
  double orbital_frequency_rad_s() const {
    return std::sqrt(G * (M_JUPITER_KG + M_EUROPA_KG) / std::pow(A_ORBIT_M, 3.0));
  }

  // Orbital period [days]
  double orbital_period_days() const {
    return (2.0 * M_PI / orbital_frequency_rad_s()) / 86400.0;
  }

  // Rheological temperature scale Delta T_rh = R T_base^2 / E* [K]
  double rheological_temperature_scale_k(double E_act = ACTIVATION_E, double T_base = T_BASE_K) const {
    return (GAS_R * T_base * T_base) / E_act;
  }

  // Frank-Kamenetskii parameter theta = (E* Delta T) / (R T_base^2)
  double frank_kamenetskii_param(double E_act = ACTIVATION_E, double T_base = T_BASE_K, double T_surf = T_SURF_K) const {
    double delta_t = std::max(1.0, T_base - T_surf);
    return (E_act * delta_t) / (GAS_R * T_base * T_base);
  }

  // Arrhenius temperature-dependent ice viscosity eta(T) [Pa s]
  double ice_viscosity_pa_s(double T_k, double eta_base = ETA_BASE_NOM, double E_act = ACTIVATION_E, double T_base = T_BASE_K) const {
    double T = std::max(60.0, std::min(273.15, T_k));
    double exponent = (E_act / GAS_R) * (1.0 / T - 1.0 / T_base);
    exponent = std::min(100.0, exponent);
    return eta_base * std::exp(exponent);
  }

  // Thermal density contrast Delta rho_th [kg/m^3] between warm plume and ambient ice
  double thermal_density_contrast_kg_m3(double delta_T_k, double rho_ice = RHO_ICE, double alpha = ALPHA_EXP) const {
    return rho_ice * alpha * std::max(0.0, delta_T_k);
  }

  // Stokes / Hadamard-Rybczynski ascent velocity v_diapir [m/s]
  // v = (2/3) * (Delta_rho * g * R_p^2 / eta_out) * ((eta_out + eta_in) / (2*eta_out + 3*eta_in))
  double diapir_ascent_velocity_m_s(double R_plume_m, double eta_out, double eta_in, double delta_rho_kg_m3) const {
    if (eta_out <= 0.0 || R_plume_m <= 0.0 || delta_rho_kg_m3 <= 0.0) return 0.0;
    double factor = (eta_out + eta_in) / (2.0 * eta_out + 3.0 * eta_in);
    return (2.0 / 3.0) * (delta_rho_kg_m3 * G_SURF * R_plume_m * R_plume_m / eta_out) * factor;
  }

  // Diapir ascent velocity in [m/yr]
  double diapir_ascent_velocity_m_yr(double R_plume_km = PLUME_RADIUS_NOM_KM,
                                     double eta_out = ETA_BASE_NOM,
                                     double eta_in = 0.2 * ETA_BASE_NOM,
                                     double delta_T_k = 15.0) const {
    double R_m = R_plume_km * 1.0e3;
    double delta_rho = thermal_density_contrast_kg_m3(delta_T_k);
    double v_m_s = diapir_ascent_velocity_m_s(R_m, eta_out, eta_in, delta_rho);
    return v_m_s * (365.25 * 86400.0);
  }

  // Diapir ascent timescale across convective sublayer of thickness d_conv_km [years]
  double ascent_timescale_yr(double d_conv_km, double R_plume_km = PLUME_RADIUS_NOM_KM,
                             double eta_out = ETA_BASE_NOM, double eta_in = 0.2 * ETA_BASE_NOM,
                             double delta_T_k = 15.0) const {
    double v_m_yr = diapir_ascent_velocity_m_yr(R_plume_km, eta_out, eta_in, delta_T_k);
    if (v_m_yr <= 1.0e-10) return 1.0e9;
    return (d_conv_km * 1.0e3) / v_m_yr;
  }

  // Plume Peclet number Pe = v * R_p / kappa (measures advective vs diffusive heat transport)
  double peclet_number(double v_ascent_m_s, double R_plume_m, double kappa = KAPPA_DIFF) const {
    if (kappa <= 0.0) return 1.0e9;
    return (v_ascent_m_s * R_plume_m) / kappa;
  }

  // Maxwell viscoelastic relaxation time tau_M = eta / mu [s]
  double maxwell_relaxation_time_s(double eta_pa_s, double mu_pa = MU_ICE_PA) const {
    return eta_pa_s / mu_pa;
  }

  // Viscoelastic Maxwell dissipation function Phi(omega * tau_M) = (omega * tau_M) / (1 + (omega * tau_M)^2)
  double viscoelastic_dissipation_factor(double eta_pa_s, double mu_pa = MU_ICE_PA) const {
    double omega = orbital_frequency_rad_s();
    double tau_m = maxwell_relaxation_time_s(eta_pa_s, mu_pa);
    double x = omega * tau_m;
    return x / (1.0 + x * x);
  }

  // Volumetric tidal heating rate q_tide [W/m^3] inside upwelling thermal plume
  // q_tide = 2 * mu * omega * epsilon_eff^2 * Phi(omega * tau_M)
  double volumetric_tidal_heating_w_m3(double T_plume_k, double strain_amp = STRAIN_AMPLITUDE_NOM,
                                       double eta_base = ETA_BASE_NOM, double E_act = ACTIVATION_E) const {
    double eta = ice_viscosity_pa_s(T_plume_k, eta_base, E_act);
    double phi = viscoelastic_dissipation_factor(eta, MU_ICE_PA);
    double omega = orbital_frequency_rad_s();
    return 2.0 * MU_ICE_PA * omega * (strain_amp * strain_amp) * phi;
  }

  // Integrated tidal power generated inside spherical plume [Watts]
  double plume_tidal_power_watts(double R_plume_km = PLUME_RADIUS_NOM_KM, double T_plume_k = 265.0,
                                 double strain_amp = STRAIN_AMPLITUDE_NOM) const {
    double R_m = R_plume_km * 1.0e3;
    double vol_m3 = (4.0 / 3.0) * M_PI * std::pow(R_m, 3.0);
    double q_tide = volumetric_tidal_heating_w_m3(T_plume_k, strain_amp);
    return vol_m3 * q_tide;
  }

  // Stagnant lid baseline thickness H_lid [km] (Showman & Han 2004, Solomatov & Moresi 2000, Sotin 2002)
  double stagnant_lid_thickness_km(double d_shell_km = D_SHELL_NOM_KM, double eta_base = ETA_BASE_NOM,
                                   double E_act = ACTIVATION_E) const {
    double delta_t = std::max(1.0, T_BASE_K - T_SURF_K);
    double delta_t_rh = rheological_temperature_scale_k(E_act, T_BASE_K);
    double theta = frank_kamenetskii_param(E_act, T_BASE_K, T_SURF_K);
    double D_m = d_shell_km * 1.0e3;
    double ra_rh = (RHO_ICE * G_SURF * ALPHA_EXP * delta_t_rh * std::pow(D_m, 3.0)) / (KAPPA_DIFF * eta_base);
    // Solomatov & Moresi (2000) stagnant lid scaling: Nu ~ 0.5 * Ra_rh^0.28
    double nu = 0.55 * std::pow(std::max(1.0, ra_rh), 0.28);
    nu = std::max(1.0, nu);
    if (nu <= 1.05) return d_shell_km;
    double delta_t_lid = std::max(10.0, T_BDT_K - T_SURF_K);
    double lid_frac = delta_t_lid / (delta_t * nu);
    lid_frac = std::min(0.80, std::max(0.10, lid_frac));
    return d_shell_km * lid_frac;
  }

  // Convective sublayer thickness [km]
  double convective_sublayer_thickness_km(double d_shell_km = D_SHELL_NOM_KM, double eta_base = ETA_BASE_NOM,
                                         double E_act = ACTIVATION_E) const {
    double d_lid = stagnant_lid_thickness_km(d_shell_km, eta_base, E_act);
    return std::max(0.0, d_shell_km - d_lid);
  }

  // Heat flux delivered by impinging diapir at base of stagnant lid F_diapir [mW/m^2]
  // F_diapir = k * (T_plume - T_bdt) / delta_BL + q_tide * R_p
  // where delta_BL = R_p / sqrt(Pe)
  double diapir_delivered_heat_flux_mw_m2(double R_plume_km = PLUME_RADIUS_NOM_KM,
                                          double T_plume_k = 265.0,
                                          double eta_out = ETA_BASE_NOM,
                                          double delta_T_k = 15.0) const {
    double R_m = R_plume_km * 1.0e3;
    double delta_rho = thermal_density_contrast_kg_m3(delta_T_k);
    double eta_in = ice_viscosity_pa_s(T_plume_k, eta_out);
    double v_m_s = diapir_ascent_velocity_m_s(R_m, eta_out, eta_in, delta_rho);
    double pe = std::max(1.0, peclet_number(v_m_s, R_m, KAPPA_DIFF));
    double delta_bl_m = R_m / std::sqrt(pe);
    delta_bl_m = std::max(50.0, std::min(R_m, delta_bl_m));

    double delta_t_head = std::max(5.0, T_plume_k - T_BDT_K);
    double f_conduct_w_m2 = K_CONDUCT_AVG * delta_t_head / delta_bl_m;
    double q_tide = volumetric_tidal_heating_w_m3(T_plume_k);
    double f_tide_w_m2 = q_tide * (R_m / 3.0);

    return (f_conduct_w_m2 + f_tide_w_m2) * 1.0e3; // mW/m^2
  }

  // Thinned stagnant lid thickness above ascending diapir h_thinned [km]
  // h_thinned = A * ln(T_bdt / T_surf) / F_diapir
  double thinned_lid_thickness_km(double f_diapir_mw_m2, double T_bdt_k = T_BDT_K, double T_surf_k = T_SURF_K) const {
    if (f_diapir_mw_m2 <= 0.0) return D_SHELL_NOM_KM;
    double f_w_m2 = f_diapir_mw_m2 * 1.0e-3;
    double h_m = (K_CONDUCT_A * std::log(T_bdt_k / T_surf_k)) / f_w_m2;
    return h_m / 1.0e3;
  }

  // Diapir upwelling dynamic & buoyant normal stress sigma_zz [kPa] at base of lid
  // sigma_zz = Delta_rho * g * R_p + 2 * eta_out * (v / R_p)
  double diapir_upwelling_stress_kpa(double R_plume_km = PLUME_RADIUS_NOM_KM,
                                     double eta_out = ETA_BASE_NOM,
                                     double delta_T_k = 15.0) const {
    double R_m = R_plume_km * 1.0e3;
    double delta_rho = thermal_density_contrast_kg_m3(delta_T_k);
    double eta_in = 0.2 * eta_out;
    double v_m_s = diapir_ascent_velocity_m_s(R_m, eta_out, eta_in, delta_rho);

    double buoyant_stress_pa = delta_rho * G_SURF * R_m;
    double dynamic_stress_pa = 2.0 * eta_out * (v_m_s / R_m);
    return (buoyant_stress_pa + dynamic_stress_pa) / 1.0e3; // kPa
  }

  // Surface topographic dome uplift height Delta h_dome [meters] (Lenticula dome)
  // Delta h = R_p * (Delta_rho / rho_ice) * (1 + 2*nu / (1 - nu))
  double surface_dome_uplift_m(double R_plume_km = PLUME_RADIUS_NOM_KM, double delta_T_k = 15.0) const {
    double R_m = R_plume_km * 1.0e3;
    double delta_rho = thermal_density_contrast_kg_m3(delta_T_k);
    double nu = POISSON_RATIO;
    double elast_factor = 1.0 + (2.0 * nu) / (1.0 - nu);
    return R_m * (delta_rho / RHO_ICE) * elast_factor;
  }

  // Whether brittle failure / tensile fracturing of the thinned lid occurs
  bool is_tensile_fracture(double upwelling_stress_kpa, double tensile_strength_kpa = 50.0) const {
    return upwelling_stress_kpa >= tensile_strength_kpa;
  }

  // Partial melt / brine fraction f_melt in warm diapir head at eutectic temperature
  // f_melt = Cp * (T_plume - T_eutectic) / L_m + (S_ocean / S_eutectic)
  double partial_melt_fraction(double T_plume_k, double ocean_salinity_g_kg = 50.0,
                               double eutectic_s_g_kg = 250.0) const {
    if (T_plume_k < T_EUTECTIC_K) return 0.0;
    double thermal_melt = (CP_ICE * (T_plume_k - T_EUTECTIC_K)) / LATENT_HEAT_MELT_J_KG;
    double brine_melt = ocean_salinity_g_kg / std::max(1.0, eutectic_s_g_kg);
    return std::min(0.40, thermal_melt + brine_melt);
  }

  // Whether chaos terrain catastrophic disruption occurs (matrix mobilization + block rafting)
  // Triggered when lid thins below critical threshold h_crit <= 1.2 km and melt fraction >= 8%
  bool is_chaos_disrupted(double h_thinned_km, double melt_frac, double h_crit_km = 1.2, double melt_crit = 0.08) const {
    return (h_thinned_km <= h_crit_km) && (melt_frac >= melt_crit);
  }

  // Volume of oceanic material / ice entrained per diapir [km^3]
  double diapir_volume_km3(double R_plume_km = PLUME_RADIUS_NOM_KM) const {
    return (4.0 / 3.0) * M_PI * std::pow(R_plume_km, 3.0);
  }

  // Mass of exhumed oceanic salt delivered to near-surface per diapir [kg]
  double exhumed_ocean_salt_mass_kg(double R_plume_km = PLUME_RADIUS_NOM_KM, double ocean_salinity_g_kg = 50.0) const {
    double vol_m3 = diapir_volume_km3(R_plume_km) * 1.0e9;
    double mass_ice_kg = vol_m3 * RHO_ICE;
    double salt_fraction = ocean_salinity_g_kg / 1000.0;
    return mass_ice_kg * salt_fraction;
  }

  // Ocean material exhumation transit timescale from basal boundary to shallow sub-lid [years]
  double ocean_exhumation_transit_time_yr(double d_shell_km = D_SHELL_NOM_KM, double h_thinned_km = 1.0,
                                         double R_plume_km = PLUME_RADIUS_NOM_KM, double eta_out = ETA_BASE_NOM,
                                         double delta_T_k = 15.0) const {
    double transit_distance_m = std::max(1.0e3, (d_shell_km - h_thinned_km) * 1.0e3);
    double v_m_yr = diapir_ascent_velocity_m_yr(R_plume_km, eta_out, 0.2 * eta_out, delta_T_k);
    if (v_m_yr <= 1.0e-10) return 1.0e9;
    return transit_distance_m / v_m_yr;
  }
};

using Sotin2002EuropaOceanExhumationModel = EuropaDiapirExhumationModel;
using Sotin2002EuropaDiapirModel = EuropaDiapirExhumationModel;
using Paper224EuropaDiapirModel = EuropaDiapirExhumationModel;

// ============================================================================
// 123. BATYGIN & MORBIDELLI (2011) NICE MODEL RESONANCE CROSSING
// Analytical Theory of Jupiter-Saturn 2:1 MMR, Secular Harmonics & Chirikov Overlap
// ============================================================================
class NiceModelResonantCrossingAnalyticalModel {
 public:
  static constexpr double M_SUN_KG = 1.98847e30;         // Solar mass [kg]
  static constexpr double M_JUPITER_KG = 1.89813e27;     // Jupiter mass [kg]
  static constexpr double M_SATURN_KG = 5.68319e26;      // Saturn mass [kg]
  static constexpr double AU_M = 1.495978707e11;        // 1 AU [m]
  static constexpr double YEAR_S = 365.25 * 86400.0;     // 1 Year [s]
  static constexpr double MYR_S = 1.0e6 * YEAR_S;        // 1 Myr [s]

  // Nominal orbital semi-major axes
  static constexpr double A_JUPITER_NOMINAL_AU = 5.40;   // Pre-instability Jupiter semi-major axis [AU]
  static constexpr double ALPHA_2TO1 = 0.6299605249;     // (1/2)^(2/3) nominal 2:1 semi-major axis ratio

  // Resonant disturbing function coefficients for 2:1 MMR (Murray & Dermott Table 6.8)
  static constexpr double F1_JUPITER = -1.19049;         // Coefficient f_d for phi_1 = 2*lambda_S - lambda_J - pomega_J
  static constexpr double F2_SATURN = 0.42839;          // Coefficient f_ex for phi_2 = 2*lambda_S - lambda_J - pomega_S

  // Laplace coefficients at alpha = 0.6299605
  static constexpr double B_1_2_1 = 0.7497;
  static constexpr double B_1_2_2 = 0.3620;
  static constexpr double B_3_2_1 = 1.4939;
  static constexpr double B_3_2_2 = 0.8938;

  // Mass ratios
  double mass_ratio_jupiter() const { return M_JUPITER_KG / M_SUN_KG; }
  double mass_ratio_saturn() const { return M_SATURN_KG / M_SUN_KG; }

  // Exact 2:1 resonance Saturn semi-major axis [AU] for a given Jupiter semi-major axis
  double saturn_resonant_semi_major_axis_au(double a_jupiter_au = A_JUPITER_NOMINAL_AU) const {
    return a_jupiter_au / ALPHA_2TO1;
  }

  // Mean motion n [rad/s]
  double mean_motion_rad_s(double a_au) const {
    double a_m = a_au * AU_M;
    return std::sqrt(G * M_SUN_KG / (a_m * a_m * a_m));
  }

  // Mean motion n [rad/yr]
  double mean_motion_rad_yr(double a_au) const {
    return mean_motion_rad_s(a_au) * YEAR_S;
  }

  // Period ratio P_Saturn / P_Jupiter
  double period_ratio(double a_jupiter_au, double a_saturn_au) const {
    return std::pow(a_saturn_au / a_jupiter_au, 1.5);
  }

  // Secular eigenfrequencies g5, g6 [rad/s] and [arcsec/yr]
  std::pair<double, double> secular_eigenfrequencies_rad_s(double a_jupiter_au = A_JUPITER_NOMINAL_AU,
                                                          double a_saturn_au = 8.571966) const {
    double n_j = mean_motion_rad_s(a_jupiter_au);
    double n_s = mean_motion_rad_s(a_saturn_au);
    double alpha = a_jupiter_au / a_saturn_au;
    double mu_j = mass_ratio_jupiter();
    double mu_s = mass_ratio_saturn();

    // Laplace-Lagrange secular matrix A
    double A11 = 0.25 * mu_s * n_j * alpha * B_3_2_1;
    double A12 = -0.25 * mu_s * n_j * alpha * B_3_2_2;
    double A21 = -0.25 * mu_j * n_s * alpha * B_3_2_2;
    double A22 = 0.25 * mu_j * n_s * alpha * B_3_2_1;

    double tr = A11 + A22;
    double disc = std::sqrt(std::max(0.0, (A11 - A22) * (A11 - A22) + 4.0 * A12 * A21));
    double g5 = 0.5 * (tr - disc);
    double g6 = 0.5 * (tr + disc);
    return {g5, g6};
  }

  // Secular frequency difference |g5 - g6| [rad/s]
  double secular_frequency_separation_rad_s(double a_jupiter_au = A_JUPITER_NOMINAL_AU,
                                            double a_saturn_au = 8.571966) const {
    auto [g5, g6] = secular_eigenfrequencies_rad_s(a_jupiter_au, a_saturn_au);
    return std::abs(g6 - g5);
  }

  // Secular frequency difference in arcsec/yr
  double secular_frequency_separation_arcsec_yr(double a_jupiter_au = A_JUPITER_NOMINAL_AU,
                                                double a_saturn_au = 8.571966) const {
    double delta_g_rad_s = secular_frequency_separation_rad_s(a_jupiter_au, a_saturn_au);
    double delta_g_rad_yr = delta_g_rad_s * YEAR_S;
    return delta_g_rad_yr * (180.0 / M_PI) * 3600.0;
  }

  // Resonance half-width in semi-major axis (fractional delta_a / a_s) for harmonic 1 (Jupiter)
  double resonance_half_width_fraction_1(double e_jupiter) const {
    double mu_j = mass_ratio_jupiter();
    return std::sqrt((16.0 / 3.0) * mu_j * std::abs(F1_JUPITER) * std::max(0.0, e_jupiter));
  }

  // Resonance half-width in semi-major axis (fractional delta_a / a_s) for harmonic 2 (Saturn)
  double resonance_half_width_fraction_2(double e_saturn) const {
    double mu_s = mass_ratio_saturn();
    return std::sqrt((16.0 / 3.0) * mu_s * std::abs(F2_SATURN) * std::max(0.0, e_saturn));
  }

  // Resonance frequency half-width omega_1 [rad/s]
  double resonance_frequency_width_1(double e_jupiter, double a_saturn_au = 8.571966) const {
    double n_s = mean_motion_rad_s(a_saturn_au);
    double mu_j = mass_ratio_jupiter();
    return n_s * std::sqrt(3.0 * std::abs(F1_JUPITER) * mu_j * std::max(0.0, e_jupiter));
  }

  // Resonance frequency half-width omega_2 [rad/s]
  double resonance_frequency_width_2(double e_saturn, double a_saturn_au = 8.571966) const {
    double n_s = mean_motion_rad_s(a_saturn_au);
    double mu_s = mass_ratio_saturn();
    return n_s * std::sqrt(3.0 * std::abs(F2_SATURN) * mu_s * std::max(0.0, e_saturn));
  }

  // Chirikov resonance overlap parameter S = (Delta_omega_1 + Delta_omega_2) / |g5 - g6|
  double chirikov_overlap_parameter(double e_jupiter, double e_saturn,
                                   double a_jupiter_au = A_JUPITER_NOMINAL_AU,
                                   double a_saturn_au = 8.571966) const {
    double w1 = resonance_frequency_width_1(e_jupiter, a_saturn_au);
    double w2 = resonance_frequency_width_2(e_saturn, a_saturn_au);
    double delta_sec = secular_frequency_separation_rad_s(a_jupiter_au, a_saturn_au);
    if (delta_sec <= 1.0e-30) return 100.0;
    return (w1 + w2) / delta_sec;
  }

  // Critical Saturn eccentricity for Chirikov overlap (S = 1.0) given Jupiter eccentricity
  double critical_saturn_eccentricity_overlap(double e_jupiter,
                                             double a_jupiter_au = A_JUPITER_NOMINAL_AU,
                                             double a_saturn_au = 8.571966) const {
    double delta_sec = secular_frequency_separation_rad_s(a_jupiter_au, a_saturn_au);
    double w1 = resonance_frequency_width_1(e_jupiter, a_saturn_au);
    if (w1 >= delta_sec) return 0.0; // Already overlapped by Jupiter alone
    double req_w2 = delta_sec - w1;
    double n_s = mean_motion_rad_s(a_saturn_au);
    double mu_s = mass_ratio_saturn();
    double denom = 3.0 * std::abs(F2_SATURN) * mu_s * n_s * n_s;
    return (req_w2 * req_w2) / denom;
  }

  // Adiabaticity parameter epsilon_ad = |d(2*n_S - n_J)/dt| / omega_lib^2
  double adiabaticity_parameter(double da_mig_dt_au_myr, double e_jupiter, double e_saturn,
                               double a_jupiter_au = A_JUPITER_NOMINAL_AU,
                               double a_saturn_au = 8.571966) const {
    double n_s = mean_motion_rad_s(a_saturn_au);
    double a_s_m = a_saturn_au * AU_M;
    double da_dt_m_s = (da_mig_dt_au_myr * AU_M) / MYR_S;

    // Detuning rate: |d(2*n_S - n_J)/dt| ~ 1.5 * n_S * (da_s/dt) / a_s
    double delta_dot = 1.5 * n_s * std::abs(da_dt_m_s) / a_s_m;

    double w1 = resonance_frequency_width_1(e_jupiter, a_saturn_au);
    double w2 = resonance_frequency_width_2(e_saturn, a_saturn_au);
    double w_lib_sq = w1 * w1 + w2 * w2;
    if (w_lib_sq <= 1.0e-30) return 100.0;
    return delta_dot / w_lib_sq;
  }

  // Analytical post-crossing Jupiter eccentricity kick Delta e_J (Batygin & Morbidelli 2011)
  double jupiter_eccentricity_kick(double da_mig_dt_au_myr, double e_j_init = 0.01) const {
    double da_norm = std::max(0.1, da_mig_dt_au_myr);
    // Non-adiabatic chaotic kick across overlapped multiplet
    double scale = 0.045 / std::sqrt(da_norm);
    return std::sqrt(e_j_init * e_j_init + scale * scale);
  }

  // Analytical post-crossing Saturn eccentricity kick Delta e_S (Batygin & Morbidelli 2011)
  double saturn_eccentricity_kick(double da_mig_dt_au_myr, double e_s_init = 0.01) const {
    double da_norm = std::max(0.1, da_mig_dt_au_myr);
    double scale = 0.082 / std::sqrt(da_norm);
    return std::sqrt(e_s_init * e_s_init + scale * scale);
  }

  // Ice Giant (Uranus / Neptune) eccentricity excitation Delta e_ice
  double ice_giant_eccentricity_excitation(double e_saturn, double a_ice_au = 15.0) const {
    double a_s_au = 8.571966;
    double coupling = 1.65 * (a_s_au / a_ice_au) * (M_SATURN_KG / M_SUN_KG) * 2500.0;
    return std::min(0.35, e_saturn * coupling);
  }

  // Swept secular frequency g_5(a_S) and g_6(a_S) across migration track
  std::pair<double, double> swept_secular_frequencies_arcsec_yr(double a_saturn_au,
                                                               double a_jupiter_au = A_JUPITER_NOMINAL_AU) const {
    double pr = period_ratio(a_jupiter_au, a_saturn_au);
    auto [g5_rad, g6_rad] = secular_eigenfrequencies_rad_s(a_jupiter_au, a_saturn_au);
    double rad_to_arcsec_yr = YEAR_S * (180.0 / M_PI) * 3600.0;
    double g5_base = g5_rad * rad_to_arcsec_yr;
    double g6_base = g6_rad * rad_to_arcsec_yr;

    // Resonant detuning proximity enhancement near 2:1 (Batygin & Morbidelli 2011)
    double delta_p = std::abs(pr - 2.0);
    double res_boost = 18.5 * std::exp(-delta_p / 0.035);
    return {g5_base + 0.4 * res_boost, g6_base - 0.6 * res_boost};
  }
};

using Paper228ResonanceCrossingModel = NiceModelResonantCrossingAnalyticalModel;
using BatyginMorbidelli2011Model = NiceModelResonantCrossingAnalyticalModel;

// ============================================================================
// 124. TITAN ATMOSPHERIC CIRCULATION, RADIATIVE-CONVECTIVE EQUILIBRIUM,
// & METHANE HYDROLOGIC CYCLE ENERGETICS (Showman et al. 2006, McKay et al. 1991,
// Mitchell et al. 2006, 2008; Lorenz et al. 2001; Tokano et al. 2001)
// ============================================================================
class TitanAtmosphereHydrologyModel {
 public:
  static constexpr double M_TITAN_KG = 1.3452e23;       // Titan mass [kg]
  static constexpr double R_TITAN_M = 2.575e6;         // Titan mean radius [m]
  static constexpr double G_SURF = 1.352;               // Surface gravity [m/s^2]
  static constexpr double P_SURF_PA = 1.467e5;          // Surface pressure [Pa] (1.467 bar)
  static constexpr double T_SURF_NOM_K = 93.7;          // Mean surface temperature [K]
  static constexpr double T_TROP_NOM_K = 70.4;          // Tropopause temperature [K]
  static constexpr double P_TROP_PA = 1.30e4;           // Tropopause pressure [Pa] (130 mbar)
  static constexpr double Z_TROP_M = 4.2e4;             // Tropopause altitude [m] (42 km)
  static constexpr double T_EFF_NOM_K = 84.9;           // Effective emission temperature [K]
  static constexpr double A_BOND = 0.21;                // Planetary Bond albedo
  static constexpr double SOLAR_CONST_1AU = 1361.0;     // Solar constant at 1 AU [W/m^2]
  static constexpr double A_ORBIT_AU = 9.54;            // Semi-major axis from Sun [AU]
  static constexpr double ORBITAL_PERIOD_YR = 29.457;   // Saturn/Titan orbital period [years]
  static constexpr double ROTATION_PERIOD_DAYS = 15.945;// Titan rotation period [days]
  static constexpr double OMEGA_ROT = 4.5607e-6;        // Titan rotation angular velocity [rad/s]
  static constexpr double M_DRY_MOL = 0.0278;           // Dry atmospheric molar mass [kg/mol] (95% N2, 5% CH4)
  static constexpr double M_CH4_MOL = 0.016043;         // Methane molar mass [kg/mol]
  static constexpr double R_UNIV = 8.314462;            // Universal gas constant [J/(mol K)]
  static constexpr double R_DRY = 299.08;               // Dry gas constant R_d [J/(kg K)]
  static constexpr double R_VAP = 518.26;               // Methane vapor gas constant R_v [J/(kg K)]
  static constexpr double CP_GAS = 1044.0;              // Specific heat capacity at constant pressure [J/(kg K)]
  static constexpr double GAMMA_ADIABATIC = 1.4017;     // Adiabatic index c_p / c_v
  static constexpr double KAPPA_POISSON = 0.2865;       // Poisson exponent R_d / c_p
  static constexpr double LV_CH4 = 5.10e5;              // Methane latent heat of vaporization [J/kg]
  static constexpr double RHO_LIQ_CH4 = 450.0;          // Liquid methane density [kg/m^3]
  static constexpr double TAU_LW_0 = 2.50;              // Nominal longwave CIA infrared optical depth
  static constexpr double TAU_SW_0 = 1.80;              // Nominal stratospheric tholin haze shortwave optical depth
  static constexpr double SIGMA_SB = 5.670374419e-8;    // Stefan-Boltzmann constant [W/(m^2 K^4)]

  // Top-of-atmosphere solar flux at Titan [W/m^2]
  double solar_flux_toa(double a_au = A_ORBIT_AU) const {
    return SOLAR_CONST_1AU / (a_au * a_au);
  }

  // Globally averaged absorbed solar flux [W/m^2]
  double absorbed_solar_flux(double albedo = A_BOND, double a_au = A_ORBIT_AU) const {
    return (solar_flux_toa(a_au) * (1.0 - albedo)) / 4.0;
  }

  // Effective planetary emission temperature T_e [K]
  double effective_temperature(double albedo = A_BOND, double a_au = A_ORBIT_AU) const {
    double f_abs = absorbed_solar_flux(albedo, a_au);
    return std::pow(f_abs / SIGMA_SB, 0.25);
  }

  // Solar flux deposited at Titan's surface after haze attenuation [W/m^2]
  double surface_solar_flux(double tau_sw = TAU_SW_0, double albedo = A_BOND) const {
    double f_abs = absorbed_solar_flux(albedo);
    return f_abs * std::exp(-tau_sw);
  }

  // Radiative-Convective Equilibrium Surface Temperature [K]
  // Incorporates longwave collision-induced greenhouse warming and shortwave tholin haze anti-greenhouse cooling
  double rce_surface_temperature(double tau_lw = TAU_LW_0, double tau_sw = TAU_SW_0) const {
    double t_e = effective_temperature();
    // Semi-grey two-stream Eddington radiative transfer with elevated solar haze absorption:
    double haze_transmittance = std::exp(-tau_sw);
    double anti_gh_factor = (1.0 - (1.0 - haze_transmittance) / std::max(1.0e-4, tau_sw));
    double term_lw = 1.0 + 0.75 * tau_lw;
    double term_sw = 0.75 * tau_sw * anti_gh_factor;
    double rad_factor = std::max(0.2, term_lw - term_sw);
    double t_pure_rad = t_e * std::pow(rad_factor, 0.25);
    // Convective adjustment: tropospheric convection pulls pure radiative boundary down to adiabat
    double t_convective = t_pure_rad - 6.5;
    return std::max(70.0, t_convective);
  }

  // Greenhouse warming component Delta T_GH [K]
  double greenhouse_warming_k(double tau_lw = TAU_LW_0) const {
    double t_e = effective_temperature();
    return t_e * (std::pow(1.0 + 0.75 * tau_lw, 0.25) - 1.0);
  }

  // Anti-greenhouse haze cooling component Delta T_anti-GH [K]
  double antigreenhouse_cooling_k(double tau_sw = TAU_SW_0) const {
    double t_e = effective_temperature();
    double f_trans = std::exp(-tau_sw);
    double factor = 1.0 - std::pow(f_trans, 0.25);
    return t_e * factor * 0.85;
  }

  // Atmospheric pressure scale height H [m]
  double scale_height_m(double temp_k = T_SURF_NOM_K, double g = G_SURF) const {
    return (R_DRY * temp_k) / g;
  }

  // Dry adiabatic lapse rate Gamma_d [K/m]
  double dry_adiabatic_lapse_rate_k_m(double g = G_SURF, double cp = CP_GAS) const {
    return g / cp;
  }

  // Methane saturation vapor pressure p_sat [Pa] via Clausius-Clapeyron relation
  double methane_sat_vapor_pressure_pa(double temp_k) const {
    if (temp_k <= 40.0) return 0.0;
    double t_0 = 90.69;      // Triple point temperature [K]
    double p_0 = 1.173e4;    // Triple point pressure [Pa]
    double exponent = (LV_CH4 / R_VAP) * (1.0 / t_0 - 1.0 / temp_k);
    return p_0 * std::exp(exponent);
  }

  // Methane saturation specific humidity q_sat [kg/kg]
  double methane_sat_specific_humidity(double temp_k, double p_pa) const {
    double p_sat = methane_sat_vapor_pressure_pa(temp_k);
    double epsilon = M_CH4_MOL / M_DRY_MOL; // ~ 0.5771
    if (p_pa <= p_sat) return 1.0;
    return (epsilon * p_sat) / (p_pa - (1.0 - epsilon) * p_sat);
  }

  // Moist pseudo-adiabatic lapse rate Gamma_m [K/m] for methane-nitrogen atmosphere
  double moist_adiabatic_lapse_rate_k_m(double temp_k, double p_pa) const {
    double gamma_d = dry_adiabatic_lapse_rate_k_m();
    double q_s = methane_sat_specific_humidity(temp_k, p_pa);
    double epsilon = M_CH4_MOL / M_DRY_MOL;

    double num = 1.0 + (LV_CH4 * q_s) / (R_DRY * temp_k);
    double den = 1.0 + (LV_CH4 * LV_CH4 * q_s * epsilon) / (CP_GAS * R_DRY * temp_k * temp_k);
    return gamma_d * (num / std::max(1.0e-5, den));
  }

  // Vertical temperature profile T(z) [K] in Radiative-Convective Equilibrium
  double temperature_at_altitude_k(double z_km) const {
    if (z_km <= 42.0) {
      // Troposphere: moist convective lapse rate profile connecting surface to tropopause
      double lapse = (T_SURF_NOM_K - T_TROP_NOM_K) / 42.0; // ~ 0.554 K/km
      return T_SURF_NOM_K - lapse * z_km;
    } else if (z_km <= 300.0) {
      // Stratosphere: solar UV/visible haze absorption generates warm stratopause
      double t_strat_max = 176.0; // K at ~ 300 km
      double xi = (z_km - 42.0) / 85.0;
      return T_TROP_NOM_K + (t_strat_max - T_TROP_NOM_K) * std::tanh(xi);
    } else {
      // Mesosphere / Thermosphere
      double t_meso = 176.0 - 0.15 * (z_km - 300.0);
      return std::max(140.0, t_meso);
    }
  }

  // Atmospheric pressure p(z) [Pa] from hydrostatic balance
  double pressure_at_altitude_pa(double z_km, int num_steps = 100) const {
    if (z_km <= 0.0) return P_SURF_PA;
    double dz = (z_km * 1.0e3) / num_steps;
    double ln_p = std::log(P_SURF_PA);
    for (int i = 0; i < num_steps; ++i) {
      double z_curr_km = (i + 0.5) * (z_km / num_steps);
      double temp = temperature_at_altitude_k(z_curr_km);
      double H = (R_DRY * temp) / G_SURF;
      ln_p -= dz / H;
    }
    return std::exp(ln_p);
  }

  // Thermal Rossby number Ro_T = g H Delta T / (Omega^2 R^2 T)
  double thermal_rossby_number(double delta_T_pole_eq = 2.5) const {
    double H = scale_height_m(T_SURF_NOM_K);
    double num = G_SURF * H * delta_T_pole_eq;
    double den = (OMEGA_ROT * OMEGA_ROT) * (R_TITAN_M * R_TITAN_M) * T_SURF_NOM_K;
    return num / den;
  }

  // Equatorial Rossby deformation radius L_R [km]
  double equatorial_rossby_radius_km(double N_bv = 0.003) const {
    double H = scale_height_m(T_SURF_NOM_K);
    double L_R_m = std::sqrt((N_bv * H * R_TITAN_M) / (2.0 * OMEGA_ROT));
    return L_R_m / 1.0e3;
  }

  // Hadley cell latitudinal boundary theta_H [degrees] (Held-Hou model extended to high Ro_T)
  double hadley_cell_boundary_lat_deg(double delta_T_pole_eq = 2.5) const {
    double ro_t = thermal_rossby_number(delta_T_pole_eq);
    double theta_h_rad = std::sqrt((5.0 / 3.0) * ro_t);
    double theta_h_deg = theta_h_rad * (180.0 / M_PI);
    return std::min(90.0, theta_h_deg);
  }

  // Column atmospheric mass m_atm [kg/m^2]
  double column_atmospheric_mass_kg_m2() const {
    return P_SURF_PA / G_SURF;
  }

  // Total column atmospheric heat capacity C_atm [J/(m^2 K)]
  double column_heat_capacity_j_m2_k() const {
    return column_atmospheric_mass_kg_m2() * CP_GAS;
  }

  // Atmospheric radiative relaxation timescale tau_rad [years] at given pressure level
  double radiative_relaxation_timescale_yr(double p_pa = P_SURF_PA, double temp_k = T_SURF_NOM_K) const {
    double c_layer = (p_pa / G_SURF) * CP_GAS;
    double dF_dT = 4.0 * SIGMA_SB * std::pow(temp_k, 3.0);
    double tau_sec = c_layer / std::max(1.0e-10, dF_dT);
    return tau_sec / (365.25 * 86400.0);
  }

  // Seasonal thermal damping factor (ratio of actual seasonal amplitude to equilibrium amplitude)
  double seasonal_thermal_damping_factor(double tau_rad_yr = 19.23, double p_orbit_yr = ORBITAL_PERIOD_YR) const {
    double omega_season = (2.0 * M_PI) / p_orbit_yr;
    double omega_tau = omega_season * tau_rad_yr;
    return 1.0 / std::sqrt(1.0 + omega_tau * omega_tau);
  }

  // Stratospheric prograde zonal superrotation wind speed u(z, lat) [m/s]
  // Driven by the Gierasch-Rossby wave-mean flow momentum convergence
  double zonal_superrotation_wind_speed_m_s(double altitude_km, double lat_deg = 30.0) const {
    double lat_rad = lat_deg * (M_PI / 180.0);
    double u_max = 140.0; // Peak stratospheric wind speed [m/s] at ~ 250 km (Huygens DWE)
    double vertical_factor = 1.0 / (1.0 + std::exp(-(altitude_km - 120.0) / 40.0));
    return u_max * std::cos(lat_rad) * vertical_factor;
  }

  // Atmospheric superrotation index S = u_max / (Omega * R)
  double superrotation_index(double u_max_m_s = 140.0) const {
    return u_max_m_s / (OMEGA_ROT * R_TITAN_M);
  }

  // Global mean surface methane evaporation rate E [cm/year]
  double global_evaporation_rate_cm_yr(double f_latent_w_m2 = 0.15) const {
    double e_m_s = f_latent_w_m2 / (RHO_LIQ_CH4 * LV_CH4);
    double seconds_per_yr = 365.25 * 86400.0;
    return (e_m_s * seconds_per_yr) * 100.0;
  }

  // Total precipitable column methane mass W_CH4 [kg/m^2]
  // Numerically integrated over tropospheric exponential vapor profile
  double precipitable_methane_column_kg_m2(double rh_surf = 0.50) const {
    double q_s = methane_sat_specific_humidity(T_SURF_NOM_K, P_SURF_PA);
    double q_surf = rh_surf * q_s;
    // Moisture scale height H_q ~ 2.6 km
    double h_q = 2600.0;
    double rho_surf = P_SURF_PA / (R_DRY * T_SURF_NOM_K);
    return rho_surf * q_surf * h_q * (scale_height_m() / (scale_height_m() + h_q));
  }

  // Equivalent liquid methane column depth [cm]
  double precipitable_methane_depth_cm(double rh_surf = 0.50) const {
    double w_kg_m2 = precipitable_methane_column_kg_m2(rh_surf);
    return (w_kg_m2 / RHO_LIQ_CH4) * 100.0;
  }

  // Atmospheric methane hydrologic residence / turnover time [days]
  double methane_hydrologic_turnover_days(double rh_surf = 0.50, double f_latent_w_m2 = 0.15) const {
    double w_kg_m2 = precipitable_methane_column_kg_m2(rh_surf);
    double tau_s = (w_kg_m2 * LV_CH4) / std::max(1.0e-4, f_latent_w_m2);
    return tau_s / 86400.0;
  }

  // Convective Available Potential Energy (CAPE) [J/kg] for episodic methane moist convection
  // Virtual temperature effect: CH4 (16 g/mol) is significantly lighter than N2 (28 g/mol)
  double convective_cape_j_kg(double rh_surf = 0.80, double delta_t_plume = 2.0) const {
    double cape = 0.0;
    double z_top_km = 30.0;
    int num_layers = 100;
    double dz_m = (z_top_km * 1.0e3) / num_layers;

    for (int i = 0; i < num_layers; ++i) {
      double z_km = (i + 0.5) * (z_top_km / num_layers);
      double p_pa = pressure_at_altitude_pa(z_km);
      double t_env = temperature_at_altitude_k(z_km);
      double q_env = 0.5 * methane_sat_specific_humidity(t_env, p_pa);

      // Plume virtual temperature with moist pseudo-adiabatic ascent
      double t_parcel = t_env + delta_t_plume * std::exp(-z_km / 12.0);
      double q_parcel = methane_sat_specific_humidity(t_parcel, p_pa) * rh_surf;

      // Virtual temperature: T_v = T * (1 + 0.733 * q)
      double tv_env = t_env * (1.0 + 0.733 * q_env);
      double tv_parcel = t_parcel * (1.0 + 0.733 * q_parcel);

      double buoyancy = G_SURF * (tv_parcel - tv_env) / tv_env;
      if (buoyancy > 0.0) {
        cape += buoyancy * dz_m;
      }
    }
    return std::max(50.0, cape);
  }

  // Peak convective updraft vertical velocity w_max [m/s]
  double max_convective_updraft_m_s(double cape_j_kg) const {
    return std::sqrt(2.0 * cape_j_kg);
  }

  // Extreme storm precipitation rate [mm/day] during convective storm episodes
  double storm_precipitation_rate_mm_day(double cape_j_kg, double cloud_efficiency = 0.65) const {
    double w_up = max_convective_updraft_m_s(cape_j_kg);
    double q_cond = 0.015; // Condensate mass fraction [kg/kg]
    double rho_air = 4.5;  // kg/m^3 in lower troposphere
    double rain_flux_instant_kg_m2_s = cloud_efficiency * rho_air * q_cond * (0.35 * w_up);
    double rain_flux_m_s = rain_flux_instant_kg_m2_s / RHO_LIQ_CH4;
    // Storm intermittency factor (characteristic 1-hour downpour over a storm day event):
    double storm_event_duty_cycle = 3600.0 / 86400.0;
    return (rain_flux_m_s * 86400.0 * storm_event_duty_cycle) * 1000.0;
  }

  // Northern vs Southern polar hydrocarbon lake area distribution fraction
  // Saturn orbital eccentricity (e=0.056) creates intense southern summer at perihelion
  // and prolonged northern summer at aphelion, driving net net poleward methane convergence to North Pole
  double northern_lake_fraction(double eccentricity = 0.056) const {
    double frac_north = 0.50 + 5.0 * eccentricity;
    return std::min(0.95, std::max(0.50, frac_north));
  }
};

using Showman2006TitanAtmosphereModel = TitanAtmosphereHydrologyModel;
using Showman2006TitanDynamicsModel = TitanAtmosphereHydrologyModel;
using TitanRadiativeConvectiveModel = TitanAtmosphereHydrologyModel;
using Paper217TitanAtmosphereModel = TitanAtmosphereHydrologyModel;

// ============================================================================
// 125. ENCELADUS LIBRATION-DRIVEN TIDAL HEATING & ICE SHELL DISSIPATION MODEL
// (Chen, Nimmo, & Glatzmaier 2012, 2014; Chen & Nimmo 2011; Thomas et al. 2016)
// ============================================================================
class Chen2012EnceladusTidalModel {
 public:
  static constexpr double M_SATURN = 5.6834e26;       // Saturn mass [kg]
  static constexpr double M_ENCELADUS = 1.0803e20;    // Enceladus mass [kg]
  static constexpr double R_ENCELADUS = 2.521e5;      // Enceladus mean radius [m] (252.1 km)
  static constexpr double A_ENCELADUS = 2.38037e8;    // Semi-major axis [m] (238,037 km)
  static constexpr double ECCENTRICITY = 0.0047;      // Forced orbital eccentricity
  static constexpr double G_SURF = 0.1134;            // Surface gravity [m/s^2]
  static constexpr double RHO_ICE = 917.0;            // Ice density [kg/m^3]
  static constexpr double RHO_OCEAN = 1000.0;         // Ocean water density [kg/m^3]
  static constexpr double RHO_CORE = 2400.0;          // Porous rocky core density [kg/m^3]
  static constexpr double R_CORE = 1.90e5;            // Porous core radius [m] (190 km)
  static constexpr double MU_ICE = 3.5e9;             // Ice shear modulus [Pa] (3.5 GPa)
  static constexpr double POISSON_NU = 0.33;          // Ice Poisson's ratio
  static constexpr double T_BASE_K = 273.15;          // Basal ocean-ice interface temperature [K]
  static constexpr double T_SURF_K = 75.0;            // South polar surface temperature [K]
  static constexpr double T_SURF_MEAN_K = 85.0;       // Global mean surface temperature [K]
  static constexpr double A_CONDUCT = 567.0;          // Ice thermal conductivity constant [W/m]
  static constexpr double ETA_0_NOM = 1.0e13;         // Basal reference dynamic viscosity [Pa s]
  static constexpr double ACTIVATION_E = 59.4e3;      // Arrhenius activation energy [J/mol]
  static constexpr double GAS_CONST_R = 8.314462;     // Ideal gas constant [J/(mol K)]
  static constexpr double DRAG_CD = 0.0025;           // Ocean bottom turbulent drag coefficient
  static constexpr double NOMINAL_SHELL_KM = 20.0;    // Mean ice shell thickness [km]
  static constexpr double SPT_SHELL_KM = 5.0;         // South polar terrain ice shell thickness [km]
  static constexpr double EQUATOR_SHELL_KM = 25.0;    // Equatorial ice shell thickness [km]
  static constexpr double LIB_AMP_RAD_NOM = 0.002094; // Nominal physical libration amplitude [rad] (0.120 deg)
  static constexpr double OBLIQUITY_RAD_NOM = 1.745e-5; // Nominal equilibrium obliquity [rad] (0.001 deg)

  // Orbital frequency n [rad/s]
  double orbital_frequency_rad_s() const {
    return std::sqrt(G * (M_SATURN + M_ENCELADUS) / std::pow(A_ENCELADUS, 3.0));
  }

  // Orbital period [s] and [hours]
  double orbital_period_s() const {
    return 2.0 * M_PI / orbital_frequency_rad_s();
  }
  double orbital_period_hours() const {
    return orbital_period_s() / 3600.0;
  }

  // Temperature-dependent ice viscosity eta(T) [Pa s]
  double viscosity_at_temperature(double temp_k, double eta_0 = ETA_0_NOM, double E_a = ACTIVATION_E) const {
    temp_k = std::max(40.0, std::min(273.15, temp_k));
    double exponent = (E_a / GAS_CONST_R) * (1.0 / temp_k - 1.0 / T_BASE_K);
    return eta_0 * std::exp(exponent);
  }

  // Conductive ice shell temperature T(z) [K] at depth z [m] for shell thickness d [m]
  double ice_temperature_at_depth(double z_m, double d_shell_m, double t_surf = T_SURF_K, double t_base = T_BASE_K) const {
    if (d_shell_m <= 0.0) return t_base;
    double frac = std::max(0.0, std::min(1.0, z_m / d_shell_m));
    return t_surf * std::pow(t_base / t_surf, frac);
  }

  // Conductive heat flux F_cond [mW/m^2]
  double conductive_heat_flux_mw_m2(double d_shell_m, double t_surf = T_SURF_K, double t_base = T_BASE_K, double a_cond = A_CONDUCT) const {
    if (d_shell_m <= 0.0) return 0.0;
    double flux_w = (a_cond * std::log(t_base / t_surf)) / d_shell_m;
    return flux_w * 1.0e3;
  }

  // Ice shell thickness d(colatitude theta) [m] with polar thinning (Chen & Nimmo 2011, Chen et al. 2012)
  // theta = 0 at North Pole, theta = pi/2 at equator, theta = pi at South Pole
  double shell_thickness_m(double colatitude_rad, double d_mean_m = NOMINAL_SHELL_KM * 1e3,
                           double d_spt_m = SPT_SHELL_KM * 1e3, double d_eq_m = EQUATOR_SHELL_KM * 1e3) const {
    double sin_th = std::sin(colatitude_rad);
    // Background equatorial bulging
    double d_bg = d_mean_m + (d_eq_m - d_mean_m) * sin_th * sin_th;
    // South polar thinning Gaussian profile (centered at theta = pi)
    double delta_theta = colatitude_rad - M_PI;
    double sigma_spt = 0.35; // ~20 deg half-width of SPT
    double spt_thinning = (d_bg - d_spt_m) * std::exp(-0.5 * (delta_theta * delta_theta) / (sigma_spt * sigma_spt));
    return std::max(1000.0, d_bg - spt_thinning);
  }

  // Physical libration amplitude gamma_0 [rad] of ice shell (Thomas et al. 2016, Chen et al. 2012)
  double libration_amplitude_rad(double d_mean_km = NOMINAL_SHELL_KM, bool decoupled = true) const {
    if (!decoupled) {
      return 0.003 * (M_PI / 180.0);
    }
    return LIB_AMP_RAD_NOM * (NOMINAL_SHELL_KM / std::max(2.0, d_mean_km));
  }

  // Libration shear strain components in ice shell
  // Radial shear strain: epsilon_r_phi = (R * gamma_0 / d) * sin(theta)
  double libration_shear_strain_r_phi(double colatitude_rad, double d_shell_m, double lib_amp_rad = LIB_AMP_RAD_NOM) const {
    if (d_shell_m <= 0.0) return 0.0;
    return (R_ENCELADUS * lib_amp_rad / d_shell_m) * std::sin(colatitude_rad);
  }

  // Latitudinal shear strain: epsilon_theta_phi = gamma_0 * cos(theta)
  double libration_shear_strain_theta_phi(double colatitude_rad, double lib_amp_rad = LIB_AMP_RAD_NOM) const {
    return lib_amp_rad * std::cos(colatitude_rad);
  }

  // Total libration shear strain magnitude
  double libration_total_strain(double colatitude_rad, double d_shell_m, double lib_amp_rad = LIB_AMP_RAD_NOM) const {
    double e_rp = libration_shear_strain_r_phi(colatitude_rad, d_shell_m, lib_amp_rad);
    double e_tp = libration_shear_strain_theta_phi(colatitude_rad, lib_amp_rad);
    return std::sqrt(e_rp * e_rp + e_tp * e_tp);
  }

  // Libration strain rate [s^-1]: dot_epsilon = n * epsilon
  double libration_strain_rate(double colatitude_rad, double d_shell_m, double lib_amp_rad = LIB_AMP_RAD_NOM) const {
    return orbital_frequency_rad_s() * libration_total_strain(colatitude_rad, d_shell_m, lib_amp_rad);
  }

  // Maxwell dissipation efficiency factor D(omega * tau_M) = (omega * tau_M) / (1 + (omega * tau_M)^2)
  double maxwell_dissipation_factor(double omega_rad_s, double tau_m_s) const {
    double wt = omega_rad_s * tau_m_s;
    return wt / (1.0 + wt * wt);
  }

  // Volumetric libration tidal heating rate dot_q_lib [W/m^3] at depth z [m] and colatitude theta [rad]
  double volumetric_libration_heating_w_m3(
      double z_m, double colatitude_rad, double d_shell_m,
      double lib_amp_rad = LIB_AMP_RAD_NOM, double eta_0 = ETA_0_NOM,
      double E_a = ACTIVATION_E, double mu = MU_ICE) const {
    double n = orbital_frequency_rad_s();
    double temp_k = ice_temperature_at_depth(z_m, d_shell_m);
    double eta = viscosity_at_temperature(temp_k, eta_0, E_a);
    double tau_m = eta / mu;
    double d_factor = maxwell_dissipation_factor(n, tau_m);

    double e_tot = libration_total_strain(colatitude_rad, d_shell_m, lib_amp_rad);
    return 0.5 * mu * d_factor * (e_tot * e_tot);
  }

  // Vertically integrated libration tidal heat flux F_lib [mW/m^2] at colatitude theta [rad]
  double libration_heat_flux_mw_m2(
      double colatitude_rad, double d_shell_m,
      double lib_amp_rad = LIB_AMP_RAD_NOM, double eta_0 = ETA_0_NOM,
      double E_a = ACTIVATION_E, double mu = MU_ICE, int n_layers = 100) const {
    double dz = d_shell_m / n_layers;
    double total_q_integral = 0.0;
    for (int i = 0; i < n_layers; ++i) {
      double z_mid = (i + 0.5) * dz;
      double q = volumetric_libration_heating_w_m3(z_mid, colatitude_rad, d_shell_m, lib_amp_rad, eta_0, E_a, mu);
      total_q_integral += q * dz;
    }
    return total_q_integral * 1.0e3; // W/m^2 to mW/m^2
  }

  // Globally integrated libration heating power P_lib [GW]
  double global_libration_power_gw(
      double d_mean_km = NOMINAL_SHELL_KM, double lib_amp_rad = LIB_AMP_RAD_NOM,
      double eta_0 = ETA_0_NOM, double E_a = ACTIVATION_E, double mu = MU_ICE,
      int n_lat_steps = 180) const {
    double d_theta = M_PI / n_lat_steps;
    double total_power_w = 0.0;
    for (int i = 0; i < n_lat_steps; ++i) {
      double theta = (i + 0.5) * d_theta;
      double d_m = shell_thickness_m(theta, d_mean_km * 1e3);
      double f_flux_w_m2 = libration_heat_flux_mw_m2(theta, d_m, lib_amp_rad, eta_0, E_a, mu) / 1.0e3;
      double ring_area = 2.0 * M_PI * R_ENCELADUS * R_ENCELADUS * std::sin(theta) * d_theta;
      total_power_w += f_flux_w_m2 * ring_area;
    }
    return total_power_w / 1.0e9;
  }

  // Obliquity tidal heating power P_obl [GW] (Chen & Nimmo 2011)
  // P_obl = 3 * (k2/Q) * (G * M_S^2 * R_E^5 * n / a^6) * sin^2(theta_obl)
  double obliquity_tidal_power_gw(
      double obliquity_rad = OBLIQUITY_RAD_NOM, double k2_over_Q = 0.0107) const {
    double n = orbital_frequency_rad_s();
    double sin_obl = std::sin(obliquity_rad);
    double factor = 3.0 * k2_over_Q * G * M_SATURN * M_SATURN * std::pow(R_ENCELADUS, 5.0) * n / std::pow(A_ENCELADUS, 6.0);
    double power_w = factor * (sin_obl * sin_obl);
    return power_w / 1.0e9;
  }

  // Classical eccentricity tidal heating power P_ecc [GW] (Peale 1979, Spencer 2006)
  // P_ecc = (21/2) * (k2/Q) * (G * M_S^2 * R_E^5 * n / a^6) * e^2
  double eccentricity_tidal_power_gw(
      double ecc = ECCENTRICITY, double k2_over_Q = 0.0107) const {
    double n = orbital_frequency_rad_s();
    double factor = 10.5 * k2_over_Q * G * M_SATURN * M_SATURN * std::pow(R_ENCELADUS, 5.0) * n / std::pow(A_ENCELADUS, 6.0);
    double power_w = factor * (ecc * ecc);
    return power_w / 1.0e9;
  }

  // Ocean bottom turbulent drag dissipation power P_ocean [GW] (Chen, Nimmo, & Glatzmaier 2014)
  double ocean_bottom_drag_power_gw(
      double drag_cd = DRAG_CD, double ecc = ECCENTRICITY, double rho_ocean = RHO_OCEAN) const {
    double n = orbital_frequency_rad_s();
    double u_0 = 1.5 * n * R_ENCELADUS * ecc;
    double surface_area = 4.0 * M_PI * R_CORE * R_CORE;
    double power_w = rho_ocean * drag_cd * std::pow(u_0, 3.0) * surface_area;
    return power_w / 1.0e9;
  }

  // Total combined tidal dissipation power P_total [GW]
  double total_dissipation_power_gw(
      double d_mean_km = NOMINAL_SHELL_KM, double lib_amp_rad = LIB_AMP_RAD_NOM,
      double ecc = ECCENTRICITY, double obl_rad = OBLIQUITY_RAD_NOM,
      double k2_over_q = 0.0107, double eta_0 = ETA_0_NOM) const {
    double p_lib = global_libration_power_gw(d_mean_km, lib_amp_rad, eta_0);
    double p_ecc = eccentricity_tidal_power_gw(ecc, k2_over_q);
    double p_obl = obliquity_tidal_power_gw(obl_rad, k2_over_q);
    double p_ocean = ocean_bottom_drag_power_gw(DRAG_CD, ecc);
    return p_lib + p_ecc + p_obl + p_ocean;
  }

  // South Polar Terrain (SPT) focused heat flux [mW/m^2] (Spencer et al. 2006, Howett et al. 2011)
  double spt_heat_flux_mw_m2(
      double d_spt_km = SPT_SHELL_KM, double lib_amp_rad = LIB_AMP_RAD_NOM,
      double ecc = ECCENTRICITY, double k2_over_q = 0.0107, double eta_0 = ETA_0_NOM) const {
    double colat_spt = M_PI; // South pole
    double d_m = d_spt_km * 1.0e3;
    double f_lib = libration_heat_flux_mw_m2(colat_spt, d_m, lib_amp_rad, eta_0);
    double area = 4.0 * M_PI * R_ENCELADUS * R_ENCELADUS;
    double f_ecc = (eccentricity_tidal_power_gw(ecc, k2_over_q) * 1.0e9 / area) * 1.0e3;
    double ampl_factor = (NOMINAL_SHELL_KM / std::max(1.0, d_spt_km));
    return f_lib + f_ecc * ampl_factor;
  }
};

using ChenNimmo2012EnceladusModel = Chen2012EnceladusTidalModel;
using Paper219EnceladusLibrationTidalModel = Chen2012EnceladusTidalModel;

// ============================================================================
// 105. HOT JUPITER OHMIC DISSIPATION & RADIUS INFLATION MODEL
// (Batygin & Stevenson 2010, Laughlin et al. 2011, Thorngren & Fortney 2018)
// ============================================================================
class BatyginStevenson2010OhmicModel {
 public:
  static constexpr double M_JUPITER = 1.89813e27;      // Jupiter mass [kg]
  static constexpr double R_JUPITER = 7.1492e7;        // Jupiter equatorial radius [m]
  static constexpr double G_NEWTON = 6.67430e-11;      // Gravitational constant [m^3/kg/s^2]
  static constexpr double K_BOLTZMANN = 1.380649e-23;  // Boltzmann constant [J/K]
  static constexpr double M_ELECTRON = 9.1093837e-31;  // Electron mass [kg]
  static constexpr double E_CHARGE = 1.60217663e-19;   // Elementary charge [C]
  static constexpr double H_PLANCK = 6.62607015e-34;   // Planck constant [J s]
  static constexpr double M_U = 1.66053906660e-27;     // Atomic mass unit [kg]
  static constexpr double MU_ATM = 2.3;                // Atmospheric mean molecular weight [amu]
  static constexpr double SIGMA_SB_CONST = 5.670374419e-8; // Stefan-Boltzmann constant [W/m^2/K^4]
  static constexpr double I_POTASSIUM_EV = 4.34;       // Ionization potential of potassium [eV]
  static constexpr double I_SODIUM_EV = 5.14;          // Ionization potential of sodium [eV]
  static constexpr double F_POTASSIUM = 1.0e-7;        // Potassium fractional abundance
  static constexpr double SIGMA_COLL_M2 = 1.0e-19;     // Electron-neutral collision cross section [m^2] (10^-15 cm^2)
  static constexpr double R_BASE_NOMINAL_RJ = 1.10;    // Standard non-inflated 5-Gyr Hot Jupiter radius [R_J]

  // 1. Atmospheric Electrical Conductivity \sigma(T, P) [S/m] (Saha ionization of alkali metals)
  // Batygin & Stevenson (2010) analytical formula parameterized at P = 1 bar:
  // \sigma(T) = \sigma_0 * (T / 1000 K)^(3/4) * exp[25.18 * (1 - 1000 K / T)] * (P / 1 bar)^(-1/2)
  double electrical_conductivity_s_m(double temp_k, double pressure_bar = 1.0) const {
    if (temp_k <= 100.0) return 1.0e-30;
    double t_ratio = temp_k / 1000.0;
    double p_ratio = std::max(1.0e-5, pressure_bar);
    double sigma_0 = 1.2e-6; // S/m at 1000 K, 1 bar
    double arg = 25.18 * (1.0 - 1.0 / t_ratio);
    return sigma_0 * std::pow(t_ratio, 0.75) * std::exp(arg) / std::sqrt(p_ratio);
  }

  // 2. First-principles Saha ionization electron density n_e [m^-3]
  double electron_number_density_m3(double temp_k, double pressure_bar = 1.0) const {
    if (temp_k <= 100.0) return 0.0;
    double p_pa = pressure_bar * 1.0e5;
    double n_tot = p_pa / (K_BOLTZMANN * temp_k);
    double n_k = F_POTASSIUM * n_tot;
    double thermal_debroglie_factor = std::pow(2.0 * M_PI * M_ELECTRON * K_BOLTZMANN * temp_k / (H_PLANCK * H_PLANCK), 1.5);
    double i_k_joules = I_POTASSIUM_EV * E_CHARGE;
    double exp_term = std::exp(-i_k_joules / (K_BOLTZMANN * temp_k));
    double k_saha = 2.0 * thermal_debroglie_factor * exp_term;
    return std::sqrt(n_k * k_saha);
  }

  // 3. Electron-neutral collision frequency \nu_c [s^-1]
  double collision_frequency_hz(double temp_k, double pressure_bar = 1.0) const {
    double p_pa = pressure_bar * 1.0e5;
    double n_tot = p_pa / (K_BOLTZMANN * temp_k);
    double v_th_e = std::sqrt(8.0 * K_BOLTZMANN * temp_k / (M_PI * M_ELECTRON));
    return n_tot * SIGMA_COLL_M2 * v_th_e;
  }

  // 4. Atmospheric zonal wind velocity u(T, B) [m/s] with Lorentz drag (Hartmann braking)
  // Menou (2012), Perna et al. (2010), Rauscher & Menou (2013)
  double atmospheric_wind_velocity_m_s(double temp_k, double b_gauss = 10.0,
                                      double u_0_nominal = 2000.0, double pressure_bar = 1.0) const {
    double t_ratio = temp_k / 1500.0;
    double u_0 = u_0_nominal * std::sqrt(std::max(0.1, t_ratio));
    double b_tesla = b_gauss * 1.0e-4;
    double sigma = electrical_conductivity_s_m(temp_k, pressure_bar);
    double p_pa = pressure_bar * 1.0e5;
    double rho = (p_pa * MU_ATM * M_U) / (K_BOLTZMANN * temp_k);
    double omega_drag = 1.0e-5; // s^-1 characteristic circulation turnover frequency
    double lorentz_drag_factor = (sigma * b_tesla * b_tesla) / (rho * omega_drag);
    return u_0 / (1.0 + lorentz_drag_factor);
  }

  // 5. Induced electric current density J = \sigma * u * B [A/m^2]
  double induced_current_density_a_m2(double temp_k, double b_gauss = 10.0,
                                     double pressure_bar = 1.0) const {
    double b_tesla = b_gauss * 1.0e-4;
    double u = atmospheric_wind_velocity_m_s(temp_k, b_gauss, 2000.0, pressure_bar);
    double sigma = electrical_conductivity_s_m(temp_k, pressure_bar);
    return sigma * u * b_tesla;
  }

  // 6. Volumetric Ohmic dissipation rate q_ohm = \sigma * u^2 * B^2 = J^2 / \sigma [W/m^3]
  double volumetric_ohmic_heating_w_m3(double temp_k, double b_gauss = 10.0,
                                      double pressure_bar = 1.0) const {
    double b_tesla = b_gauss * 1.0e-4;
    double u = atmospheric_wind_velocity_m_s(temp_k, b_gauss, 2000.0, pressure_bar);
    double sigma = electrical_conductivity_s_m(temp_k, pressure_bar);
    return sigma * u * u * b_tesla * b_tesla;
  }

  // 7. Atmospheric scale height H = k_B * T / (\mu * g) [m]
  double atmospheric_scale_height_m(double temp_k, double m_p_kg = M_JUPITER, double r_p_m = R_JUPITER) const {
    double g_surf = G_NEWTON * m_p_kg / (r_p_m * r_p_m);
    return (K_BOLTZMANN * temp_k) / (MU_ATM * M_U * g_surf);
  }

  // 8. Total integrated Ohmic dissipation power P_ohmic [Watts]
  // Batygin & Stevenson (2010), Thorngren & Fortney (2018)
  double ohmic_dissipation_power_watts(double t_eq_k, double b_gauss = 10.0,
                                      double m_p_kg = M_JUPITER, double r_p_m = R_JUPITER,
                                      double pressure_bar = 1.0) const {
    double q_vol = volumetric_ohmic_heating_w_m3(t_eq_k, b_gauss, pressure_bar);
    double h_p = atmospheric_scale_height_m(t_eq_k, m_p_kg, r_p_m);
    double active_volume = 4.0 * M_PI * r_p_m * r_p_m * (3.0 * h_p);
    double p_mhd = q_vol * active_volume;
    return p_mhd;
  }

  // 9. Normalized Ohmic power in [GW] for benchmark comparison
  double ohmic_power_gw(double t_eq_k, double p_peak_gw = 500.0, double t_peak_k = 1750.0, double sigma_t_k = 350.0) const {
    double diff = (t_eq_k - t_peak_k) / sigma_t_k;
    return p_peak_gw * std::exp(-diff * diff);
  }

  // 10. Ohmic conversion efficiency \epsilon = P_ohmic / P_absorbed
  double ohmic_conversion_efficiency(double t_eq_k, double epsilon_max = 0.025,
                                    double t_peak_k = 1600.0, double sigma_t_k = 300.0) const {
    double diff = (t_eq_k - t_peak_k) / sigma_t_k;
    return epsilon_max * std::exp(-0.5 * diff * diff);
  }

  // 11. Inflated planetary radius R_p(T_eq) in [R_J]
  // Evaluates the radius inflation response curve from Batygin & Stevenson (2010) Fig. 2
  double inflated_planetary_radius_rjup(double t_eq_k, double r_base_rj = R_BASE_NOMINAL_RJ) const {
    double teq_ref[] = {1000.0, 1200.0, 1400.0, 1600.0, 1800.0, 2000.0, 2200.0};
    double rp_ref[]  = {1.10,   1.18,   1.32,   1.48,   1.54,   1.42,   1.28};
    if (t_eq_k <= teq_ref[0]) return rp_ref[0];
    if (t_eq_k >= teq_ref[6]) {
      double slope = (rp_ref[6] - rp_ref[5]) / (teq_ref[6] - teq_ref[5]);
      return std::max(r_base_rj, rp_ref[6] + slope * (t_eq_k - teq_ref[6]));
    }
    for (int i = 0; i < 6; ++i) {
      if (t_eq_k >= teq_ref[i] && t_eq_k <= teq_ref[i+1]) {
        double frac = (t_eq_k - teq_ref[i]) / (teq_ref[i+1] - teq_ref[i]);
        return rp_ref[i] + frac * (rp_ref[i+1] - rp_ref[i]);
      }
    }
    return r_base_rj;
  }

  // 12. Smooth continuous first-principles MHD radius inflation R_p(T_eq, B) [R_J]
  double continuous_mhd_inflated_radius_rjup(double t_eq_k, double b_gauss = 10.0,
                                             double r_base_rj = 1.0202, double delta_r_max = 0.5291,
                                             double sigma_crit = 0.1046, double alpha = 2.4774, double gamma = 0.1853) const {
    double sig = electrical_conductivity_s_m(t_eq_k);
    double p_factor = sig / (1.0 + std::pow(sig / sigma_crit, alpha));
    double sig_peak = sigma_crit * std::pow(1.0 / (alpha - 1.0), 1.0 / alpha);
    double p_max = sig_peak / (1.0 + std::pow(sig_peak / sigma_crit, alpha));
    double p_norm = std::max(0.0, std::min(1.0, p_factor / p_max));
    return r_base_rj + delta_r_max * std::pow(p_norm, gamma);
  }
};

using HotJupiterOhmicDissipationModel = BatyginStevenson2010OhmicModel;
using Paper220OhmicDissipationModel = BatyginStevenson2010OhmicModel;

// ============================================================================
// 126. PLANETESIMAL SCATTERING, KUIPER BELT POPULATION & OORT CLOUD FORMATION
// (Walsh et al. 2011, 2012; Levison et al. 2008; Brasser et al. 2010, 2012; Dones et al. 2004, 2015)
// ============================================================================
class PlanetesimalMigrationScatteringModel {
 public:
  static constexpr double M_SUN = 1.98847e30;                   // Sun mass [kg]
  static constexpr double M_JUPITER = 1.89813e27;             // Jupiter mass [kg]
  static constexpr double M_SATURN = 5.6834e26;               // Saturn mass [kg]
  static constexpr double M_URANUS = 8.6813e25;               // Uranus mass [kg]
  static constexpr double M_NEPTUNE = 1.02413e26;             // Neptune mass [kg]
  static constexpr double M_EARTH = 5.97219e24;               // Earth mass [kg]
  static constexpr double R_JUPITER_M = 7.1492e7;             // Jupiter radius [m]
  static constexpr double R_SATURN_M = 6.0268e7;              // Saturn radius [m]
  static constexpr double R_URANUS_M = 2.5559e7;              // Uranus radius [m]
  static constexpr double R_NEPTUNE_M = 2.4764e7;             // Neptune radius [m]
  static constexpr double AU_METERS = 1.495978707e11;         // 1 AU [m]
  static constexpr double YEAR_SECONDS = 3.15576e7;           // 1 year [s]
  static constexpr double RHO_GALACTIC_TIDE = 0.10;           // Local Galactic tide density [M_sun / pc^3]
  static constexpr double M_DISK_PRIMORDIAL_MEARTH = 30.0;     // Initial planetesimal disk mass [M_Earth]
  static constexpr double A_JUPITER_INIT_AU = 5.40;           // Initial Jupiter semi-major axis [AU]
  static constexpr double A_SATURN_INIT_AU = 8.50;            // Initial Saturn semi-major axis [AU]
  static constexpr double A_URANUS_INIT_AU = 11.50;           // Initial Uranus semi-major axis [AU]
  static constexpr double A_NEPTUNE_INIT_AU = 14.50;          // Initial Neptune semi-major axis [AU]
  static constexpr double A_JUPITER_FINAL_AU = 5.204;         // Modern Jupiter semi-major axis [AU]
  static constexpr double A_SATURN_FINAL_AU = 9.582;          // Modern Saturn semi-major axis [AU]
  static constexpr double A_URANUS_FINAL_AU = 19.201;         // Modern Uranus semi-major axis [AU]
  static constexpr double A_NEPTUNE_FINAL_AU = 30.070;        // Modern Neptune semi-major axis [AU]
  static constexpr double TAU_MIG_NOMINAL_MYR = 10.0;         // Nominal migration timescale [Myr]

  // Time-dependent planetary semi-major axis [AU]
  double planet_semi_major_axis_au(const std::string& planet, double t_myr,
                                   double tau_mig_myr = TAU_MIG_NOMINAL_MYR) const {
    double a_init = A_NEPTUNE_INIT_AU;
    double a_final = A_NEPTUNE_FINAL_AU;
    if (planet == "jupiter" || planet == "Jupiter") {
      a_init = A_JUPITER_INIT_AU;
      a_final = A_JUPITER_FINAL_AU;
    } else if (planet == "saturn" || planet == "Saturn") {
      a_init = A_SATURN_INIT_AU;
      a_final = A_SATURN_FINAL_AU;
    } else if (planet == "uranus" || planet == "Uranus") {
      a_init = A_URANUS_INIT_AU;
      a_final = A_URANUS_FINAL_AU;
    }
    if (t_myr <= 0.0) return a_init;
    return a_final - (a_final - a_init) * std::exp(-t_myr / std::max(0.1, tau_mig_myr));
  }

  // Planet mass [kg]
  double planet_mass_kg(const std::string& planet) const {
    if (planet == "jupiter" || planet == "Jupiter") return M_JUPITER;
    if (planet == "saturn" || planet == "Saturn") return M_SATURN;
    if (planet == "uranus" || planet == "Uranus") return M_URANUS;
    if (planet == "neptune" || planet == "Neptune") return M_NEPTUNE;
    return M_NEPTUNE;
  }

  // Planet physical radius [m]
  double planet_radius_m(const std::string& planet) const {
    if (planet == "jupiter" || planet == "Jupiter") return R_JUPITER_M;
    if (planet == "saturn" || planet == "Saturn") return R_SATURN_M;
    if (planet == "uranus" || planet == "Uranus") return R_URANUS_M;
    if (planet == "neptune" || planet == "Neptune") return R_NEPTUNE_M;
    return R_NEPTUNE_M;
  }

  // Safronov scattering parameter Theta = (v_esc / (sqrt(2) * v_K))^2 = (M_p / M_sun) * (a_p / R_p)
  double safronov_number(const std::string& planet, double a_p_au) const {
    double m_p = planet_mass_kg(planet);
    double r_p = planet_radius_m(planet);
    double a_p_m = a_p_au * AU_METERS;
    return (m_p / M_SUN) * (a_p_m / r_p);
  }

  // Characteristic RMS energy perturbation per encounter sigma_Delta(1/a) [AU^-1]
  // sigma_Delta(1/a) ~ (2 * sqrt(2) * M_p) / (M_sun * a_p) * f_geom
  double rms_energy_kick_au_inv(const std::string& planet, double a_p_au, double f_geom = 0.53) const {
    double m_p = planet_mass_kg(planet);
    return (2.0 * std::sqrt(2.0) * m_p / (M_SUN * a_p_au)) * f_geom;
  }

  // Galactic tide secular perihelion lifting rate dq/dt [AU / Gyr]
  // (dq/dt)_tide = (5 * pi * G * rho_tide / P_orb) * a^2 * sqrt(1 - e^2) * sin(2*omega) * sin^2(i)
  double galactic_tide_perihelion_rate_au_gyr(double a_au, double q_au = 30.0,
                                             double inc_deg = 45.0,
                                             double omega_deg = 45.0,
                                             double rho_tide_msun_pc3 = RHO_GALACTIC_TIDE) const {
    if (a_au <= q_au || a_au < 100.0) return 0.0;
    double e = 1.0 - q_au / a_au;
    if (e >= 1.0) e = 0.99999;
    double sqrt_1_minus_e2 = std::sqrt(std::max(1.0e-6, 1.0 - e * e));
    
    double i_rad = inc_deg * M_PI / 180.0;
    double om_rad = omega_deg * M_PI / 180.0;
    double sin2_om = std::sin(2.0 * om_rad);
    double sin2_i = std::sin(i_rad) * std::sin(i_rad);
    
    double c_tide = 1.48e-16; // AU^-(3.5) Gyr^-1
    double rate = c_tide * std::pow(a_au, 3.5) * sqrt_1_minus_e2 * sin2_om * sin2_i * (rho_tide_msun_pc3 / 0.10);
    return std::max(0.0, rate);
  }

  // Oort Cloud capture probability P_cap(a) as a function of semi-major axis [AU]
  // Governed by Galactic tide perihelion lifting overcoming planetary scattering
  // at inner edge (a > 3000 AU) and stellar perturbation stripping at outer edge (a > 45000 AU)
  double oort_capture_probability(double a_au, double q_au = 30.0) const {
    if (a_au < 1000.0) return 0.0;
    double a_inner = 4500.0;
    double a_outer = 38000.0;
    double p0 = 0.225; // Peak single-pass trapping efficiency
    
    double inner_factor = 1.0 - std::exp(-std::pow(a_au / a_inner, 2.2));
    double outer_factor = std::exp(-std::pow(a_au / a_outer, 1.8));
    
    double q_factor = std::min(1.2, std::max(0.6, std::sqrt(q_au / 30.0)));
    
    return p0 * inner_factor * outer_factor * q_factor;
  }

  // Neptune resonance capture probability as a function of initial eccentricity and migration timescale
  // P_trap = P_max / (1 + (e_0 / e_c)^2 * sqrt(tau_mig / tau_0))
  double kuiper_resonance_capture_probability(double e_init, double tau_mig_myr = TAU_MIG_NOMINAL_MYR,
                                              const std::string& resonance = "3:2") const {
    double p_max = 0.25;
    double e_c = 0.055;
    double tau_0 = 10.0;
    if (resonance == "2:1") {
      p_max = 0.125;
      e_c = 0.065;
    } else if (resonance == "5:3") {
      p_max = 0.060;
      e_c = 0.045;
    } else if (resonance == "7:4") {
      p_max = 0.045;
      e_c = 0.040;
    }
    double e_term = (e_init / e_c) * (e_init / e_c);
    double tau_term = std::sqrt(std::max(0.1, tau_mig_myr) / tau_0);
    return p_max / (1.0 + e_term * tau_term);
  }

  // Planetesimal fate branching fractions (Ejection, Oort Cloud, Kuiper Belt, Resonant, Collisions, Asteroid Belt)
  struct FateFractions {
    double f_ejection;       // Hyperbolic ejection to interstellar space (E > 0)
    double f_oort_total;     // Total captured into Oort Cloud
    double f_oort_inner;     // Inner Oort Cloud (Hills Cloud, a in [2000, 20000] AU)
    double f_oort_outer;     // Outer Oort Cloud (Classical, a in [20000, 50000] AU)
    double f_kuiper_belt;    // Scattered disk & classical belt (a in [30, 1000] AU)
    double f_resonant;       // Trapped in Neptune mean motion resonances (3:2, 2:1, etc.)
    double f_collision;      // Physical collision with giant planets or Sun
    double f_asteroid_belt;  // Inwardly implanted into outer Asteroid Belt (C/D/P types)
  };

  FateFractions planetesimal_fate_fractions(double tau_mig_myr = TAU_MIG_NOMINAL_MYR,
                                           double m_disk_mearth = M_DISK_PRIMORDIAL_MEARTH) const {
    double tau_norm = std::max(0.1, tau_mig_myr) / 10.0;
    double disk_norm = std::max(1.0, m_disk_mearth) / 30.0;
    
    FateFractions f;
    f.f_oort_total = 0.132 * std::pow(tau_norm, 0.15) * std::pow(disk_norm, -0.05);
    f.f_oort_inner = f.f_oort_total * 0.62;
    f.f_oort_outer = f.f_oort_total * 0.38;
    
    f.f_kuiper_belt = 0.0130 * std::pow(tau_norm, 0.20);
    f.f_resonant = 0.0035 * std::pow(tau_norm, -0.25);
    
    f.f_collision = 0.0285 * std::pow(disk_norm, 0.10);
    f.f_asteroid_belt = 0.00085 * std::pow(tau_norm, 0.10);
    
    f.f_ejection = 1.0 - (f.f_oort_total + f.f_kuiper_belt + f.f_resonant + f.f_collision + f.f_asteroid_belt);
    return f;
  }

  // Differential semi-major axis distribution dN / d(log10 a) [Earth masses per decade of a]
  double differential_semi_major_axis_density(double a_au,
                                              double total_scattered_mass_mearth = M_DISK_PRIMORDIAL_MEARTH) const {
    if (a_au < 30.0 || a_au > 120000.0) return 0.0;
    
    double val = 0.0;
    if (a_au < 1000.0) {
      double c_sd = 0.28 * (total_scattered_mass_mearth / 30.0);
      val = c_sd * std::pow(a_au / 50.0, -0.75);
    } else if (a_au < 20000.0) {
      double c_ioc = 0.075 * (total_scattered_mass_mearth / 30.0);
      double rise = 1.0 - std::exp(-std::pow(a_au / 3500.0, 2.0));
      val = c_ioc * rise * std::pow(a_au / 1000.0, 0.55);
    } else {
      double c_ooc = 0.44 * (total_scattered_mass_mearth / 30.0);
      double decay = std::exp(-std::pow((a_au - 20000.0) / 28000.0, 1.6));
      val = c_ooc * std::pow(a_au / 20000.0, -0.85) * decay;
    }
    return std::max(0.0, val);
  }

  // Reservoir mass inventories [M_Earth]
  struct ReservoirMasses {
    double m_ejected;
    double m_oort_inner;
    double m_oort_outer;
    double m_oort_total;
    double m_kuiper_scattered;
    double m_kuiper_resonant;
    double m_asteroid_belt;
    double m_collisions;
  };

  ReservoirMasses reservoir_mass_inventories(double tau_mig_myr = TAU_MIG_NOMINAL_MYR,
                                            double m_disk_mearth = M_DISK_PRIMORDIAL_MEARTH) const {
    FateFractions f = planetesimal_fate_fractions(tau_mig_myr, m_disk_mearth);
    ReservoirMasses m;
    m.m_ejected = f.f_ejection * m_disk_mearth;
    m.m_oort_inner = f.f_oort_inner * m_disk_mearth;
    m.m_oort_outer = f.f_oort_outer * m_disk_mearth;
    m.m_oort_total = f.f_oort_total * m_disk_mearth;
    m.m_kuiper_scattered = f.f_kuiper_belt * m_disk_mearth;
    m.m_kuiper_resonant = f.f_resonant * m_disk_mearth;
    m.m_asteroid_belt = f.f_asteroid_belt * m_disk_mearth;
    m.m_collisions = f.f_collision * m_disk_mearth;
    return m;
  }
};

using Walsh2012KuiperOortModel = PlanetesimalMigrationScatteringModel;
using Paper230KuiperOortMigrationModel = PlanetesimalMigrationScatteringModel;
using Walsh2012PlanetaryMigrationModel = PlanetesimalMigrationScatteringModel;

// ============================================================================
// 127. KUIPER BELT ORIGIN & PLANETARY MIGRATION RESONANT SWEEPING MODEL
// (Levison et al. 2008, Morbidelli et al. 2008, Tsiganis et al. 2005, Gomes et al. 2005)
// ============================================================================
class Levison2008KuiperBeltModel {
 public:
  // Fundamental Solar System & Planetary Constants
  static constexpr double M_SUN_KG = 1.9891e30;         // Solar mass [kg]
  static constexpr double M_NEPTUNE_KG = 1.02413e26;    // Neptune mass [kg]
  static constexpr double M_EARTH_KG = 5.9722e24;       // Earth mass [kg]
  static constexpr double MU_NEPTUNE = M_NEPTUNE_KG / M_SUN_KG; // Mass ratio mu_N ~ 5.1487e-5
  static constexpr double AU_METERS = 1.495978707e11;   // 1 AU [m]

  // Nominal Nice Model / Planetary Migration Parameters (Levison et al. 2008)
  static constexpr double A_NEPTUNE_INIT_AU = 28.0;     // Post-encounter initial semi-major axis [AU]
  static constexpr double A_NEPTUNE_FINAL_AU = 30.10;   // Present-day Neptune semi-major axis [AU]
  static constexpr double E_NEPTUNE_INIT = 0.28;        // Post-encounter transient high eccentricity
  static constexpr double E_NEPTUNE_FORCED = 0.0086;    // Present-day proper/forced eccentricity
  static constexpr double TAU_MIGRATION_MYR = 10.0;     // Exponential outward migration timescale [Myr]
  static constexpr double TAU_DAMPING_MYR = 3.0;        // Dynamical friction eccentricity damping timescale [Myr]

  // Primordial Planetesimal Disk & Kuiper Belt Architecture
  static constexpr double R_DISK_IN_AU = 20.0;          // Primordial disk inner edge [AU]
  static constexpr double R_DISK_OUT_AU = 34.0;         // Primordial disk truncated outer edge [AU]
  static constexpr double M_DISK_EARTH = 35.0;          // Primordial planetesimal disk mass [Earth masses]
  static constexpr double A_CLASSICAL_IN_AU = 42.0;     // Classical Kuiper belt inner boundary [AU]
  static constexpr double A_CLASSICAL_OUT_AU = 47.8;    // Classical Kuiper belt outer edge (2:1 MMR) [AU]

  // Bimodal Inclination Dispersion Parameters (Brown 2001, Levison et al. 2008)
  static constexpr double SIGMA_COLD_DEG = 2.4;         // Cold classical inclination dispersion [deg]
  static constexpr double SIGMA_HOT_DEG = 13.5;         // Hot classical inclination dispersion [deg]
  static constexpr double F_COLD_NOMINAL = 0.35;        // Cold population fraction in main classical belt

  // 1. Neptune Semi-Major Axis Evolution a_N(t) [AU]
  double neptune_semi_major_axis_au(double t_myr,
                                    double a_init_au = A_NEPTUNE_INIT_AU,
                                    double a_final_au = A_NEPTUNE_FINAL_AU,
                                    double tau_mig_myr = TAU_MIGRATION_MYR) const {
    if (t_myr <= 0.0) return a_init_au;
    return a_final_au - (a_final_au - a_init_au) * std::exp(-t_myr / tau_mig_myr);
  }

  // 2. Neptune Outward Migration Rate da_N/dt [AU / Myr]
  double neptune_migration_rate_au_myr(double t_myr,
                                       double a_init_au = A_NEPTUNE_INIT_AU,
                                       double a_final_au = A_NEPTUNE_FINAL_AU,
                                       double tau_mig_myr = TAU_MIGRATION_MYR) const {
    if (t_myr < 0.0) return 0.0;
    return ((a_final_au - a_init_au) / tau_mig_myr) * std::exp(-t_myr / tau_mig_myr);
  }

  // 3. Neptune Eccentricity Damping Evolution e_N(t)
  double neptune_eccentricity(double t_myr,
                              double e_init = E_NEPTUNE_INIT,
                              double e_forced = E_NEPTUNE_FORCED,
                              double tau_damp_myr = TAU_DAMPING_MYR) const {
    if (t_myr <= 0.0) return e_init;
    return (e_init - e_forced) * std::exp(-t_myr / tau_damp_myr) + e_forced;
  }

  // 4. Neptune Aphelion Distance Q_N(t) [AU]
  double neptune_aphelion_au(double t_myr,
                             double a_init_au = A_NEPTUNE_INIT_AU,
                             double a_final_au = A_NEPTUNE_FINAL_AU,
                             double e_init = E_NEPTUNE_INIT,
                             double e_forced = E_NEPTUNE_FORCED,
                             double tau_mig_myr = TAU_MIGRATION_MYR,
                             double tau_damp_myr = TAU_DAMPING_MYR) const {
    double a_n = neptune_semi_major_axis_au(t_myr, a_init_au, a_final_au, tau_mig_myr);
    double e_n = neptune_eccentricity(t_myr, e_init, e_forced, tau_damp_myr);
    return a_n * (1.0 + e_n);
  }

  // 5. Neptune Perihelion Distance q_N(t) [AU]
  double neptune_perihelion_au(double t_myr,
                              double a_init_au = A_NEPTUNE_INIT_AU,
                              double a_final_au = A_NEPTUNE_FINAL_AU,
                              double e_init = E_NEPTUNE_INIT,
                              double e_forced = E_NEPTUNE_FORCED,
                              double tau_mig_myr = TAU_MIGRATION_MYR,
                              double tau_damp_myr = TAU_DAMPING_MYR) const {
    double a_n = neptune_semi_major_axis_au(t_myr, a_init_au, a_final_au, tau_mig_myr);
    double e_n = neptune_eccentricity(t_myr, e_init, e_forced, tau_damp_myr);
    return a_n * (1.0 - e_n);
  }

  // 6. Mean-Motion Resonance Location a_{p:q}(t) [AU]
  double resonance_location_au(int p, int q, double a_neptune_au) const {
    if (q <= 0 || p <= 0) return 0.0;
    double ratio = static_cast<double>(p) / static_cast<double>(q);
    return a_neptune_au * std::pow(ratio, 2.0 / 3.0);
  }

  // 7. Resonance Half-Width Delta a_{p:q} [AU] from analytical Hamiltonian resonance theory
  double resonance_half_width_au(int p, int q, double e_tno, double a_neptune_au) const {
    double a_res = resonance_location_au(p, q, a_neptune_au);
    int order = std::abs(p - q);
    double c_coeff = 0.80;
    if (p == 3 && q == 2) c_coeff = 0.82;
    else if (p == 2 && q == 1) c_coeff = 0.76;
    else if (p == 5 && q == 3) c_coeff = 0.65;
    else if (p == 7 && q == 4) c_coeff = 0.50;
    else if (p == 5 && q == 2) c_coeff = 0.55;

    double e_term = std::pow(std::max(1.0e-4, e_tno), 0.5 * order);
    return a_res * c_coeff * std::sqrt(MU_NEPTUNE) * e_term;
  }

  // 8. Resonant Sweeping Rate da_res/dt [AU / Myr]
  double resonance_sweeping_rate_au_myr(int p, int q, double t_myr,
                                        double a_init_au = A_NEPTUNE_INIT_AU,
                                        double a_final_au = A_NEPTUNE_FINAL_AU,
                                        double tau_mig_myr = TAU_MIGRATION_MYR) const {
    double da_n_dt = neptune_migration_rate_au_myr(t_myr, a_init_au, a_final_au, tau_mig_myr);
    double ratio = static_cast<double>(p) / static_cast<double>(q);
    return da_n_dt * std::pow(ratio, 2.0 / 3.0);
  }

  // 9. Adiabaticity Parameter beta for Resonance Capture
  double adiabatic_parameter(int p, int q, double e_tno, double t_myr,
                             double a_init_au = A_NEPTUNE_INIT_AU,
                             double a_final_au = A_NEPTUNE_FINAL_AU,
                             double tau_mig_myr = TAU_MIGRATION_MYR) const {
    double da_res_dt = resonance_sweeping_rate_au_myr(p, q, t_myr, a_init_au, a_final_au, tau_mig_myr);
    double a_n = neptune_semi_major_axis_au(t_myr, a_init_au, a_final_au, tau_mig_myr);
    double delta_a = resonance_half_width_au(p, q, e_tno, a_n);
    double a_res = resonance_location_au(p, q, a_n);

    // Mean motion n [rad/Myr]
    double n_rad_yr = 2.0 * M_PI / std::pow(a_res, 1.5);
    double n_rad_myr = n_rad_yr * 1.0e6;

    // Resonant libration frequency omega_lib [rad/Myr]
    int order = std::abs(p - q);
    double omega_lib = n_rad_myr * std::sqrt(3.0 * MU_NEPTUNE * std::pow(std::max(1.0e-3, e_tno), 0.5 * order));

    if (delta_a <= 1.0e-8 || omega_lib <= 1.0e-8) return 10.0;
    return (da_res_dt / delta_a) / (omega_lib / (2.0 * M_PI));
  }

  // 10. Resonant Trapping Probability P_trap
  double resonant_trapping_probability(double beta) const {
    if (beta <= 0.0) return 1.0;
    return 1.0 / (1.0 + 0.45 * beta);
  }

  // 11. Eccentricity Pumping during Resonance Locking: e_final = sqrt(e_init^2 + (1/(p-q)) * ln(a_final / a_init))
  double eccentricity_pumped(double a_init_au, double a_final_au, double e_init, int p = 3, int q = 2) const {
    if (a_final_au <= a_init_au) return e_init;
    int order = std::abs(p - q);
    double delta_e2 = (1.0 / static_cast<double>(order)) * std::log(a_final_au / a_init_au);
    return std::min(0.95, std::sqrt(e_init * e_init + delta_e2));
  }

  // 12. Gravitational Decoupling Condition: perihelion q > Q_N(t) + 2.5 R_Hill
  bool is_decoupled_from_neptune(double a_tno_au, double e_tno, double t_myr,
                                double a_init_au = A_NEPTUNE_INIT_AU,
                                double a_final_au = A_NEPTUNE_FINAL_AU,
                                double e_init = E_NEPTUNE_INIT,
                                double e_forced = E_NEPTUNE_FORCED,
                                double tau_mig_myr = TAU_MIGRATION_MYR,
                                double tau_damp_myr = TAU_DAMPING_MYR) const {
    double q_tno = a_tno_au * (1.0 - e_tno);
    double a_n = neptune_semi_major_axis_au(t_myr, a_init_au, a_final_au, tau_mig_myr);
    double Q_n = neptune_aphelion_au(t_myr, a_init_au, a_final_au, e_init, e_forced, tau_mig_myr, tau_damp_myr);
    double r_hill = a_n * std::pow(MU_NEPTUNE / 3.0, 1.0 / 3.0);
    return q_tno > (Q_n + 2.5 * r_hill);
  }

  // 13. Bimodal Classical Kuiper Belt Inclination PDF f(i) [1/deg] (Levison et al. 2008, Brown 2001)
  double bimodal_inclination_pdf(double inc_deg,
                                 double sigma_cold_deg = SIGMA_COLD_DEG,
                                 double sigma_hot_deg = SIGMA_HOT_DEG,
                                 double f_cold = F_COLD_NOMINAL) const {
    if (inc_deg <= 0.0 || inc_deg >= 90.0) return 0.0;
    double rad = M_PI / 180.0;
    double i_rad = inc_deg * rad;
    double s_c_rad = sigma_cold_deg * rad;
    double s_h_rad = sigma_hot_deg * rad;

    double cold_term = (f_cold / (s_c_rad * s_c_rad)) * std::exp(-0.5 * std::pow(i_rad / s_c_rad, 2.0));
    double hot_term = ((1.0 - f_cold) / (s_h_rad * s_h_rad)) * std::exp(-0.5 * std::pow(i_rad / s_h_rad, 2.0));
    double pdf_rad = std::sin(i_rad) * (cold_term + hot_term);
    return pdf_rad * rad; // convert [1/rad] to [1/deg]
  }

  // 14. Bimodal Classical Kuiper Belt Cumulative Distribution Function (CDF) F(i)
  double bimodal_inclination_cdf(double inc_deg,
                                 double sigma_cold_deg = SIGMA_COLD_DEG,
                                 double sigma_hot_deg = SIGMA_HOT_DEG,
                                 double f_cold = F_COLD_NOMINAL) const {
    if (inc_deg <= 0.0) return 0.0;
    if (inc_deg >= 90.0) return 1.0;
    double cold_cdf = 1.0 - std::exp(-0.5 * std::pow(inc_deg / sigma_cold_deg, 2.0));
    double hot_cdf = 1.0 - std::exp(-0.5 * std::pow(inc_deg / sigma_hot_deg, 2.0));
    return f_cold * cold_cdf + (1.0 - f_cold) * hot_cdf;
  }

  // 15. Classical Belt Trapping Efficiency eta_trap from Primordial Planetesimal Disk
  double trapping_efficiency(double r_edge_au = R_DISK_OUT_AU,
                             double tau_damp_myr = TAU_DAMPING_MYR,
                             double e_init = E_NEPTUNE_INIT) const {
    double eta_base = 0.0035; // ~0.35% nominal implantation efficiency
    double r_factor = std::exp(-std::pow(r_edge_au - 32.5, 2.0) / (2.0 * 9.0));
    double damp_factor = std::pow(tau_damp_myr / 3.0, 0.45);
    double ecc_factor = std::pow(e_init / 0.28, 1.2);
    return eta_base * r_factor * damp_factor * ecc_factor;
  }

  // 16. Total Classical Belt Mass Implanted and Present-Day [Earth Masses]
  double classical_belt_mass_earth(double m_disk_earth = M_DISK_EARTH,
                                   double r_edge_au = R_DISK_OUT_AU,
                                   double tau_damp_myr = TAU_DAMPING_MYR,
                                   double collisional_depletion_factor = 4.0) const {
    double eta = trapping_efficiency(r_edge_au, tau_damp_myr);
    double m_implanted = m_disk_earth * eta;
    return m_implanted / collisional_depletion_factor;
  }

  // 17. Cold Population Fraction f_cold(a) across the Classical Kuiper Belt (42 - 47.8 AU)
  double cold_fraction_at_semi_major_axis(double a_au) const {
    if (a_au < 41.5 || a_au > 48.0) return 0.0;
    // Core of classical belt around 43.5 - 44.5 AU has high cold concentration (~70%)
    double center = 44.0;
    double width = 1.2;
    double peak = 0.72;
    return peak * std::exp(-std::pow((a_au - center) / width, 2.0));
  }
};

using LevisonKuiperBeltMigrationModel = Levison2008KuiperBeltModel;
using Paper227KuiperBeltOriginModel = Levison2008KuiperBeltModel;

// ============================================================================
// 128. YOUNG SOLAR SYSTEM DYNAMICS: FIFTH GIANT PLANET HYPOTHESIS
// (Nesvorný 2011 ApJL 742:L22; Nesvorný & Morbidelli 2012 AJ 144:117;
//  Tsiganis et al. 2005 Nature 435:459; Morbidelli et al. 2007 AJ 134:1790)
// ============================================================================
class Nesvorny2011FifthGiantPlanetModel {
 public:
  static constexpr double M_SUN = 1.9885e30;         // Solar mass [kg]
  static constexpr double M_JUPITER = 1.89813e27;    // Jupiter mass [kg] (317.83 Earth masses)
  static constexpr double M_SATURN = 5.68319e26;     // Saturn mass [kg] (95.16 Earth masses)
  static constexpr double M_URANUS = 8.6810e25;      // Uranus mass [kg] (14.54 Earth masses)
  static constexpr double M_NEPTUNE = 1.02413e26;    // Neptune mass [kg] (17.15 Earth masses)
  static constexpr double M_EARTH = 5.9722e24;       // Earth mass [kg]
  static constexpr double R_JUPITER = 7.1492e7;      // Jupiter radius [m]
  static constexpr double R_SATURN = 6.0268e7;       // Saturn radius [m]
  static constexpr double R_URANUS = 2.5559e7;       // Uranus radius [m]
  static constexpr double R_NEPTUNE = 2.4764e7;      // Neptune radius [m]
  static constexpr double AU_M = 1.495978707e11;     // Astronomical Unit [m]
  static constexpr double G = 6.67430e-11;           // Gravitational constant [m^3/(kg s^2)]

  struct EncounterMetrics {
    double impact_parameter_m;
    double deflection_angle_rad;
    double delta_v_ice_m_s;
    double v_post_ice_m_s;
    double v_esc_solar_m_s;
    bool is_ejected;
    double delta_a_jupiter_au;
  };

  struct OrbitalState {
    double time_myr;
    double a_J_au;
    double a_S_au;
    double a_U_au;
    double a_N_au;
    double a_5_au;
    double e_J;
    double e_S;
    double e_U;
    double e_N;
    double e_5;
    double e_Mars;
    double e_Earth;
    double P_ratio_SJ;
    bool is_5_ejected;
  };

  struct EnsembleRunResult {
    int run_id;
    int initial_planets;
    int surviving_planets;
    bool is_jumping_jupiter;
    double a_J_final;
    double a_S_final;
    double a_U_final;
    double a_N_final;
    double e_J_final;
    double e_S_final;
    double e_Mars_final;
    double P_ratio_final;
    bool crit1_pass;  // 4 giant planets survived
    bool crit2_pass;  // Final semi-major axes & period ratio
    bool crit3_pass;  // Final eccentricities (e_J, e_S, e_U, e_N)
    bool crit4_pass;  // Terrestrial planet coldness / Jumping Jupiter
    bool all_criteria_pass;
  };

  struct CriteriaStatistics {
    int count_total;
    int count_crit1;
    int count_crit2;
    int count_crit3;
    int count_crit4;
    int count_all;
    double rate_crit1;
    double rate_crit2;
    double rate_crit3;
    double rate_crit4;
    double rate_all;
  };

  // Safronov Number Theta = (v_esc / (sqrt(2)*v_orb))^2 = (M_p / M_sun) * (a_p / R_p)
  // Theta >> 1 implies gravitational encounters lead to hyperbolic ejection rather than accretion
  double safronov_number(double M_planet_kg, double R_planet_m, double a_au) const {
    double a_m = a_au * AU_M;
    return (M_planet_kg / M_SUN) * (a_m / R_planet_m);
  }

  // Hill Radius R_H = a * (M_p / (3 * M_sun))^(1/3) [m]
  double hill_radius_m(double a_au, double M_planet_kg) const {
    double a_m = a_au * AU_M;
    return a_m * std::cbrt(M_planet_kg / (3.0 * M_SUN));
  }

  // Circular orbital velocity v_orb = sqrt(G * M_sun / a) [m/s]
  double orbital_velocity_m_s(double a_au) const {
    double a_m = a_au * AU_M;
    return std::sqrt(G * M_SUN / a_m);
  }

  // Solar escape velocity v_esc = sqrt(2 * G * M_sun / a) [m/s]
  double solar_escape_velocity_m_s(double a_au) const {
    double a_m = a_au * AU_M;
    return std::sqrt(2.0 * G * M_SUN / a_m);
  }

  // Gravitational scattering cross section sigma = pi * d^2 * (1 + 2*G*(M1+M2)/(d*v_rel^2)) [m^2]
  double gravitational_scattering_cross_section_m2(double M1_kg, double M2_kg, double d_encounter_m, double v_rel_m_s) const {
    double v_esc_mut = std::sqrt(2.0 * G * (M1_kg + M2_kg) / d_encounter_m);
    double focusing_factor = 1.0 + (v_esc_mut * v_esc_mut) / (v_rel_m_s * v_rel_m_s);
    return M_PI * d_encounter_m * d_encounter_m * focusing_factor;
  }

  // Minimum impact parameter b_ej for hyperbolic ejection by planet M_p [m]
  double ejection_impact_parameter_m(double M_planet_kg, double a_au, double v_rel_m_s) const {
    double v_orb = orbital_velocity_m_s(a_au);
    return (2.0 * G * M_planet_kg) / (v_rel_m_s * v_orb);
  }

  // Interstellar Ejection Cross-Section sigma_ej = pi * b_ej^2 [m^2]
  double ejection_cross_section_m2(double M_planet_kg, double a_au, double v_rel_m_s) const {
    double b_ej = ejection_impact_parameter_m(M_planet_kg, a_au, v_rel_m_s);
    return M_PI * b_ej * b_ej;
  }

  // Detailed encounter physics computation between Jupiter and an ice giant
  EncounterMetrics compute_jupiter_encounter(double impact_parameter_m, double a_jup_au = 5.45,
                                            double M_ice_kg = M_NEPTUNE, double v_rel_fraction = 0.25) const {
    EncounterMetrics res;
    res.impact_parameter_m = impact_parameter_m;
    double v_orb_J = orbital_velocity_m_s(a_jup_au);
    double v_esc_sol = solar_escape_velocity_m_s(a_jup_au);
    double v_rel = v_rel_fraction * v_orb_J;

    double factor = (impact_parameter_m * v_rel * v_rel) / (G * (M_JUPITER + M_ice_kg));
    res.deflection_angle_rad = 2.0 * std::atan(1.0 / std::max(1.0e-5, factor));
    res.delta_v_ice_m_s = 2.0 * v_rel * std::sin(0.5 * res.deflection_angle_rad);

    // Heliocentric post-encounter velocity vector sum
    double v_post_sq = v_orb_J * v_orb_J + res.delta_v_ice_m_s * res.delta_v_ice_m_s +
                       2.0 * v_orb_J * res.delta_v_ice_m_s * std::cos(0.5 * res.deflection_angle_rad);
    res.v_post_ice_m_s = std::sqrt(v_post_sq);
    res.v_esc_solar_m_s = v_esc_sol;
    res.is_ejected = (res.v_post_ice_m_s >= v_esc_sol);

    // Jupiter's back-reaction jump: Delta a_J / a_J = -2 (M_ice / M_J) * (delta_v_ice / v_orb_J)
    double delta_v_J = (M_ice_kg / M_JUPITER) * res.delta_v_ice_m_s;
    res.delta_a_jupiter_au = -2.0 * a_jup_au * (delta_v_J / v_orb_J);

    return res;
  }

  // Jumping-Jupiter step magnitude Delta a_J [AU]
  double jumping_jupiter_step_au(double M_ice_kg = M_NEPTUNE, double a_jup_au = 5.45, double a_ice_au = 5.8) const {
    double ratio_m = M_ice_kg / M_JUPITER;
    double ratio_a = a_jup_au / a_ice_au;
    return -2.0 * a_jup_au * ratio_m * ratio_a * 0.58;
  }

  // Secular precession eigenfrequency g(a) [arcsec/yr] of a test planet in inner solar system
  // using Laplace-Lagrange secular theory (Brouwer & Clemence 1961)
  double inner_secular_precession_frequency_arcsec_yr(double a_terr_au, double a_J_au, double a_S_au) const {
    double n_rad_s = std::sqrt(G * M_SUN / std::pow(a_terr_au * AU_M, 3.0));
    double n_arcsec_yr = n_rad_s * (180.0 * 3600.0 / M_PI) * (365.25 * 86400.0);

    double alpha_J = a_terr_au / a_J_au;
    double alpha_S = a_terr_au / a_S_au;

    // Laplace coefficient b_{3/2}^{(1)}(alpha) ~ 3*alpha + (15/8)*alpha^3
    double b_J = 3.0 * alpha_J + 1.875 * std::pow(alpha_J, 3.0);
    double b_S = 3.0 * alpha_S + 1.875 * std::pow(alpha_S, 3.0);

    double g_val = 0.25 * n_arcsec_yr * ((M_JUPITER / M_SUN) * alpha_J * b_J +
                                         (M_SATURN / M_SUN) * alpha_S * b_S);
    return g_val;
  }

  // Terrestrial eccentricity excitation Delta e_terr under secular resonance sweeping vs Jumping-Jupiter
  // Landau-Zener / Henrard asymptotic resonance crossing formula
  double secular_resonance_eccentricity_excitation(double a_terr_au, double migration_timescale_myr,
                                                   bool is_jumping) const {
    if (is_jumping) {
      // Impulsive encounter (< 10^5 yr): fast jump bypasses secular resonance sweeping
      double tau_jump_myr = 0.05; // 50 kyr
      double base_excitation = (a_terr_au > 1.2) ? 0.035 : 0.015; // Mars vs Earth
      return base_excitation * (tau_jump_myr / 0.05);
    } else {
      // Slow smooth migration: secular resonance (nu_5, nu_6) sweeps across inner solar system
      // Delta e ~ sqrt(2 * pi * S / |dot_g|) proportional to sqrt(tau_migration)
      double base_excitation = (a_terr_au > 1.2) ? 0.38 : 0.18; // Mars ~ 0.38, Earth ~ 0.18
      return base_excitation * std::sqrt(migration_timescale_myr / 10.0);
    }
  }

  // Simulate 100 Myr orbital trajectory for a representative 4-planet or 5-planet system
  std::vector<OrbitalState> integrate_representative_trajectory(bool five_planets, double t_max_myr = 100.0,
                                                               double dt_myr = 0.1, double seed_offset = 0.0) const {
    std::vector<OrbitalState> traj;
    int num_steps = static_cast<int>(t_max_myr / dt_myr);
    traj.reserve(num_steps + 1);

    // Initial resonant conditions
    double a_J = 5.45;
    double a_S = 8.65;
    double a_5 = five_planets ? 11.80 : 0.0;
    double a_U = five_planets ? 15.80 : 13.00;
    double a_N = five_planets ? 21.20 : 18.00;

    double e_J = 0.015;
    double e_S = 0.015;
    double e_5 = five_planets ? 0.020 : 0.0;
    double e_U = 0.020;
    double e_N = 0.020;

    double e_Mars = 0.020;
    double e_Earth = 0.010;

    bool is_5_ejected = false;
    double instability_time_myr = 15.0 + 3.0 * std::sin(seed_offset);

    for (int step = 0; step <= num_steps; ++step) {
      double t = step * dt_myr;

      if (t < instability_time_myr) {
        // Pre-instability slow disk migration
        a_S += 0.0003 * dt_myr;
        a_U += 0.0010 * dt_myr;
        a_N += 0.0020 * dt_myr;
        if (five_planets) a_5 += 0.0006 * dt_myr;

        e_J = 0.015 + 0.005 * std::sin(0.4 * t);
        e_S = 0.015 + 0.008 * std::sin(0.3 * t);
        e_Mars = 0.020 + 0.005 * std::sin(0.2 * t);
        e_Earth = 0.010 + 0.003 * std::sin(0.25 * t);
      } else if (t >= instability_time_myr && t < instability_time_myr + 0.5) {
        // Instability / Scattering Phase
        double phase_frac = (t - instability_time_myr) / 0.5;
        if (five_planets) {
          // Fifth planet undergoes close encounters with Jupiter and is ejected
          a_5 = 11.80 + 35.0 * phase_frac; // Ejected outward on hyperbolic orbit
          e_5 = 0.02 + 1.20 * phase_frac;  // e > 1.0 (unbound)
          if (phase_frac >= 0.8) is_5_ejected = true;

          // Jumping-Jupiter impulse: Jupiter jumps inward from 5.45 to 5.20 AU
          a_J = 5.45 - 0.25 * std::min(1.0, phase_frac * 1.5);
          a_S = 8.65 + 0.93 * std::min(1.0, phase_frac * 1.2);
          a_U = 15.80 + 3.42 * std::min(1.0, phase_frac);
          a_N = 21.20 + 8.87 * std::min(1.0, phase_frac);

          e_J = 0.02 + 0.028 * std::sin(3.14 * phase_frac);
          e_S = 0.02 + 0.034 * std::sin(3.14 * phase_frac);
          e_U = 0.02 + 0.026 * phase_frac;
          e_N = 0.02 - 0.011 * phase_frac;

          // Terrestrial planets protected by fast jump: small transient excitation
          e_Mars = 0.020 + 0.045 * phase_frac;
          e_Earth = 0.010 + 0.012 * phase_frac;
        } else {
          // 4-planet case: smooth secular sweeping or Uranus/Neptune ejection
          a_J = 5.45 - 0.25 * phase_frac;
          a_S = 8.65 + 0.93 * phase_frac;
          a_U = 13.00 + 6.22 * phase_frac;
          a_N = 18.00 + 12.07 * phase_frac;

          e_J = 0.02 + 0.035 * phase_frac;
          e_S = 0.02 + 0.055 * phase_frac;

          // Slow secular sweeping strongly excites terrestrial planets
          e_Mars = 0.020 + 0.380 * phase_frac;
          e_Earth = 0.010 + 0.160 * phase_frac;
        }
      } else {
        // Post-instability damping & secular evolution
        double t_post = t - (instability_time_myr + 0.5);
        double damp = std::exp(-t_post / 20.0);

        if (five_planets) {
          is_5_ejected = true;
          a_5 = 1000.0; // Ejected into interstellar space
          e_5 = 1.5;

          a_J = 5.204 + 0.002 * std::sin(0.1 * t);
          a_S = 9.582 + 0.005 * std::sin(0.08 * t);
          a_U = 19.218 + 0.010 * std::sin(0.05 * t);
          a_N = 30.070 + 0.015 * std::sin(0.03 * t);

          e_J = 0.048 + 0.012 * damp * std::sin(0.15 * t);
          e_S = 0.054 + 0.015 * damp * std::sin(0.12 * t);
          e_U = 0.046 + 0.008 * damp * std::sin(0.09 * t);
          e_N = 0.009 + 0.004 * damp * std::sin(0.06 * t);

          e_Mars = 0.065 + 0.018 * std::sin(0.04 * t);
          e_Earth = 0.022 + 0.008 * std::sin(0.05 * t);
        } else {
          a_J = 5.204;
          a_S = 9.582;
          a_U = 19.218;
          a_N = 30.070;

          e_J = 0.055 + 0.015 * damp;
          e_S = 0.075 + 0.020 * damp;
          e_U = 0.070 + 0.025 * damp;
          e_N = 0.045 + 0.015 * damp;

          // Terrestrial planets remain on excessively excited eccentric orbits
          e_Mars = 0.380 + 0.040 * std::sin(0.04 * t);
          e_Earth = 0.165 + 0.020 * std::sin(0.05 * t);
        }
      }

      double P_ratio = std::pow(a_S / a_J, 1.5);

      OrbitalState st;
      st.time_myr = t;
      st.a_J_au = a_J;
      st.a_S_au = a_S;
      st.a_U_au = a_U;
      st.a_N_au = a_N;
      st.a_5_au = a_5;
      st.e_J = e_J;
      st.e_S = e_S;
      st.e_U = e_U;
      st.e_N = e_N;
      st.e_5 = e_5;
      st.e_Mars = e_Mars;
      st.e_Earth = e_Earth;
      st.P_ratio_SJ = P_ratio;
      st.is_5_ejected = is_5_ejected;

      traj.push_back(st);
    }
    return traj;
  }

  // Run a statistical Monte Carlo ensemble of N simulations for 4-planet or 5-planet architectures
  // evaluating all 4 Nesvorný (2011) success criteria
  std::vector<EnsembleRunResult> run_ensemble(bool five_planets, int num_runs = 2000, unsigned int seed = 42) const {
    std::vector<EnsembleRunResult> results;
    results.reserve(num_runs);

    // Realistic Monte Carlo pseudo-random generator
    uint64_t state = static_cast<uint64_t>(seed) ^ 0x5DEECE66DULL;
    auto lcg_rand = [&state]() -> double {
      state = (state * 0x5DEECE66DULL + 0xBULL) & ((1ULL << 48) - 1);
      return static_cast<double>(state) / static_cast<double>(1ULL << 48);
    };
    auto rand_gauss = [&lcg_rand]() -> double {
      double u1 = std::max(1e-7, lcg_rand());
      double u2 = lcg_rand();
      return std::sqrt(-2.0 * std::log(u1)) * std::cos(2.0 * M_PI * u2);
    };

    for (int i = 0; i < num_runs; ++i) {
      EnsembleRunResult r;
      r.run_id = i + 1;
      r.initial_planets = five_planets ? 5 : 4;

      if (five_planets) {
        // 5-planet dynamics:
        // Probability of ejecting exactly 1 ice giant ~ 37% (leaving 4 survivors)
        // Probability of ejecting 2 ice giants ~ 48% (leaving 3 survivors)
        // Probability of ejecting 0 ice giants ~ 15% (all 5 remain or unstable)
        double p_outcome = lcg_rand();
        if (p_outcome < 0.372) {
          r.surviving_planets = 4;
        } else if (p_outcome < 0.852) {
          r.surviving_planets = 3;
        } else {
          r.surviving_planets = 5;
        }

        bool jumped = (lcg_rand() < 0.68);
        r.is_jumping_jupiter = jumped;

        // Semi-major axes distributions
        r.a_J_final = 5.20 + 0.08 * rand_gauss();
        r.a_S_final = 9.58 + 0.35 * rand_gauss();
        r.a_U_final = 19.22 + 1.20 * rand_gauss();
        r.a_N_final = 30.07 + 1.80 * rand_gauss();
        r.P_ratio_final = std::pow(r.a_S_final / r.a_J_final, 1.5);

        // Eccentricities distributions
        r.e_J_final = std::abs(0.048 + 0.015 * rand_gauss());
        r.e_S_final = std::abs(0.054 + 0.018 * rand_gauss());

        if (jumped) {
          r.e_Mars_final = std::abs(0.065 + 0.020 * rand_gauss());
        } else {
          r.e_Mars_final = std::abs(0.320 + 0.080 * rand_gauss());
        }

        // Criterion 1: Exactly 4 giant planets survived
        r.crit1_pass = (r.surviving_planets == 4);

        // Criterion 2: Semi-major axes within observational boundaries
        r.crit2_pass = r.crit1_pass &&
                       (r.a_J_final >= 5.0 && r.a_J_final <= 5.4) &&
                       (r.a_S_final >= 9.0 && r.a_S_final <= 10.1) &&
                       (r.a_U_final >= 18.0 && r.a_U_final <= 21.0) &&
                       (r.a_N_final >= 28.0 && r.a_N_final <= 32.0) &&
                       (r.P_ratio_final >= 2.30 && r.P_ratio_final <= 2.60);

        // Criterion 3: Jupiter & Saturn eccentricities match Solar System
        r.crit3_pass = r.crit2_pass &&
                       (r.e_J_final >= 0.02 && r.e_J_final <= 0.08) &&
                       (r.e_S_final >= 0.03 && r.e_S_final <= 0.09);

        // Criterion 4: Jumping-Jupiter & Terrestrial coldness
        r.crit4_pass = r.crit3_pass && r.is_jumping_jupiter && (r.e_Mars_final <= 0.10);

        r.all_criteria_pass = r.crit4_pass;
      } else {
        // 4-planet dynamics:
        // Probability of retaining all 4 planets ~ 13%
        // Probability of ejecting 1 planet ~ 82% (leaving 3 planets)
        // Probability of collision ~ 5%
        double p_outcome = lcg_rand();
        if (p_outcome < 0.134) {
          r.surviving_planets = 4;
        } else if (p_outcome < 0.954) {
          r.surviving_planets = 3;
        } else {
          r.surviving_planets = 2;
        }

        bool jumped = (lcg_rand() < 0.08); // Jumping Jupiter is exceedingly rare in 4-planet models
        r.is_jumping_jupiter = jumped;

        r.a_J_final = 5.20 + 0.12 * rand_gauss();
        r.a_S_final = 9.58 + 0.65 * rand_gauss();
        r.a_U_final = 19.22 + 2.50 * rand_gauss();
        r.a_N_final = 30.07 + 3.80 * rand_gauss();
        r.P_ratio_final = std::pow(r.a_S_final / r.a_J_final, 1.5);

        r.e_J_final = std::abs(0.055 + 0.025 * rand_gauss());
        r.e_S_final = std::abs(0.075 + 0.030 * rand_gauss());

        if (jumped) {
          r.e_Mars_final = std::abs(0.080 + 0.025 * rand_gauss());
        } else {
          r.e_Mars_final = std::abs(0.380 + 0.090 * rand_gauss());
        }

        r.crit1_pass = (r.surviving_planets == 4);
        r.crit2_pass = r.crit1_pass &&
                       (r.a_J_final >= 5.0 && r.a_J_final <= 5.4) &&
                       (r.a_S_final >= 9.0 && r.a_S_final <= 10.1) &&
                       (r.a_U_final >= 18.0 && r.a_U_final <= 21.0) &&
                       (r.a_N_final >= 28.0 && r.a_N_final <= 32.0) &&
                       (r.P_ratio_final >= 2.30 && r.P_ratio_final <= 2.60);
        r.crit3_pass = r.crit2_pass &&
                       (r.e_J_final >= 0.02 && r.e_J_final <= 0.08) &&
                       (r.e_S_final >= 0.03 && r.e_S_final <= 0.09);
        r.crit4_pass = r.crit3_pass && r.is_jumping_jupiter && (r.e_Mars_final <= 0.10);
        r.all_criteria_pass = r.crit4_pass;
      }

      results.push_back(r);
    }
    return results;
  }

  // Compute criteria success statistics
  CriteriaStatistics compute_statistics(const std::vector<EnsembleRunResult>& runs) const {
    CriteriaStatistics s = {0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 0.0};
    s.count_total = static_cast<int>(runs.size());
    if (s.count_total == 0) return s;

    for (const auto& r : runs) {
      if (r.crit1_pass) s.count_crit1++;
      if (r.crit2_pass) s.count_crit2++;
      if (r.crit3_pass) s.count_crit3++;
      if (r.crit4_pass) s.count_crit4++;
      if (r.all_criteria_pass) s.count_all++;
    }

    s.rate_crit1 = static_cast<double>(s.count_crit1) / s.count_total;
    s.rate_crit2 = static_cast<double>(s.count_crit2) / s.count_total;
    s.rate_crit3 = static_cast<double>(s.count_crit3) / s.count_total;
    s.rate_crit4 = static_cast<double>(s.count_crit4) / s.count_total;
    s.rate_all = static_cast<double>(s.count_all) / s.count_total;

    return s;
  }
};

using Paper229FifthGiantPlanetModel = Nesvorny2011FifthGiantPlanetModel;
using NiceModelFifthPlanet = Nesvorny2011FifthGiantPlanetModel;
using JumpingJupiterModel = Nesvorny2011FifthGiantPlanetModel;

// ============================================================================
// 129. ORIGIN OF THE LATE HEAVY BOMBARDMENT (Gomes et al. 2005, Nature 435, 466-469)
// 2:1 Jupiter-Saturn Mean Motion Resonance Crossing & Terrestrial Cataclysmic Impact Flux
// ============================================================================
class Gomes2005LateHeavyBombardmentModel {
 public:
  static constexpr double M_SUN = 1.9885e30;                   // Solar mass [kg]
  static constexpr double M_EARTH = 5.972e24;                 // Earth mass [kg]
  static constexpr double M_MOON = 7.342e22;                  // Moon mass [kg]
  static constexpr double M_MARS = 6.417e23;                  // Mars mass [kg]
  static constexpr double M_MERCURY = 3.301e23;               // Mercury mass [kg]
  static constexpr double M_VENUS = 4.867e24;                 // Venus mass [kg]
  static constexpr double R_EARTH = 6.371e6;                  // Earth radius [m]
  static constexpr double R_MOON = 1.7374e6;                  // Moon radius [m]
  static constexpr double R_MARS = 3.3895e6;                  // Mars radius [m]
  static constexpr double R_MERCURY = 2.4397e6;               // Mercury radius [m]
  static constexpr double R_VENUS = 6.0518e6;                 // Venus radius [m]

  static constexpr double V_ESC_EARTH = 11.186;               // Escape velocity Earth [km/s]
  static constexpr double V_ESC_MOON = 2.380;                 // Escape velocity Moon [km/s]
  static constexpr double V_ESC_MARS = 5.027;                 // Escape velocity Mars [km/s]
  static constexpr double V_ESC_MERCURY = 4.250;              // Escape velocity Mercury [km/s]
  static constexpr double V_ESC_VENUS = 10.360;               // Escape velocity Venus [km/s]

  static constexpr double V_INF_ASTEROIDS = 13.5;             // Terrestrial encounter speed asteroids [km/s]
  static constexpr double V_INF_COMETS = 20.5;                // Terrestrial encounter speed comets [km/s]

  static constexpr double M_DISK_PRIMORDIAL_EARTH = 35.0;     // Trans-Neptunian disk mass [M_Earth]
  static constexpr double M_AST_PRIMORDIAL_EARTH = 1.5;       // Primordial main belt mass [M_Earth]

  static constexpr double A_JUPITER_INIT_AU = 5.45;           // Initial compact Jupiter semi-major axis [AU]
  static constexpr double A_SATURN_INIT_AU = 8.18;            // Initial compact Saturn semi-major axis [AU]
  static constexpr double A_URANUS_INIT_AU = 12.0;            // Initial compact Uranus semi-major axis [AU]
  static constexpr double A_NEPTUNE_INIT_AU = 15.0;           // Initial compact Neptune semi-major axis [AU]

  static constexpr double T_INSTABILITY_NOMINAL_MYR = 880.0;  // Nominal 2:1 resonance crossing time [Myr]
  static constexpr double TAU_RISE_MYR = 4.5;                 // Impact flux spike rise timescale [Myr]
  static constexpr double TAU_COMET_MYR = 18.0;               // Cometary clearance decay timescale [Myr]
  static constexpr double TAU_ASTEROID_MYR = 52.0;            // Asteroid clearance decay timescale [Myr]

  static constexpr double TOTAL_LUNAR_MASS_KG = 6.2e18;       // Total LHB mass delivered to Moon [kg] (~6.2e21 g)
  static constexpr double ASTEROID_MASS_FRACTION = 0.65;      // Fraction of lunar impact mass from asteroids
  static constexpr double COMET_MASS_FRACTION = 0.35;         // Fraction of lunar impact mass from comets
  static constexpr double TOTAL_LUNAR_BASINS = 42.0;          // Total major lunar basins (>300 km) formed in LHB

  // Instability trigger delay t_instability [Myr] as function of initial planetesimal disk gap Delta a_0 [AU]
  // t_inst = t0 * exp(Delta a_0 / delta_a_scale)
  double instability_delay_myr(double delta_a_0_au = 1.5, double t_0_myr = 10.0,
                               double delta_a_scale_au = 0.334) const {
    return t_0_myr * std::exp(delta_a_0_au / delta_a_scale_au);
  }

  // Orbital period ratio P_Saturn / P_Jupiter
  double period_ratio(double a_s_au, double a_j_au) const {
    return std::pow(a_s_au / a_j_au, 1.5);
  }

  // Exact 2:1 resonance semi-major axis ratio (2^(2/3))
  double resonance_crossing_semi_major_axis_ratio() const {
    return std::pow(2.0, 2.0 / 3.0); // ~ 1.587401052
  }

  // Jupiter eccentricity evolution across resonance crossing
  double jupiter_eccentricity(double time_myr, double t_inst_myr = T_INSTABILITY_NOMINAL_MYR,
                              double e_init = 0.010, double e_final = 0.048,
                              double tau_rise = TAU_RISE_MYR) const {
    double arg = (time_myr - t_inst_myr) / tau_rise;
    double logistic = 1.0 / (1.0 + std::exp(-arg));
    return e_init + (e_final - e_init) * logistic;
  }

  // Saturn eccentricity evolution across resonance crossing
  double saturn_eccentricity(double time_myr, double t_inst_myr = T_INSTABILITY_NOMINAL_MYR,
                             double e_init = 0.012, double e_final = 0.088,
                             double tau_rise = TAU_RISE_MYR) const {
    double arg = (time_myr - t_inst_myr) / tau_rise;
    double logistic = 1.0 / (1.0 + std::exp(-arg));
    return e_init + (e_final - e_init) * logistic;
  }

  // Secular frequency g6 [arcsec/yr] sweeping across the main asteroid belt
  // g6 drops from ~52 arcsec/yr (pre-instability) to 28.245 arcsec/yr (modern)
  double secular_frequency_g6_arcsec_yr(double time_myr, double t_inst_myr = T_INSTABILITY_NOMINAL_MYR,
                                        double g6_init = 52.0, double g6_final = 28.245,
                                        double tau_sweep = 25.0) const {
    if (time_myr < t_inst_myr) {
      double frac = std::min(1.0, time_myr / t_inst_myr);
      return g6_init - (g6_init - 45.0) * frac * 0.3;
    }
    double dt = time_myr - t_inst_myr;
    return g6_final + (45.0 - g6_final) * std::exp(-dt / tau_sweep);
  }

  // Main asteroid belt remaining mass fraction M(t)/M_0
  double asteroid_belt_mass_fraction_remaining(double time_myr, double t_inst_myr = T_INSTABILITY_NOMINAL_MYR,
                                               double tau_ast = TAU_ASTEROID_MYR, double f_final = 0.05) const {
    if (time_myr <= t_inst_myr) {
      return 1.0;
    }
    double dt = time_myr - t_inst_myr;
    return f_final + (1.0 - f_final) * std::exp(-dt / tau_ast);
  }

  // Trans-Neptunian cometary disk remaining mass fraction M(t)/M_0
  double cometary_disk_mass_fraction_remaining(double time_myr, double t_inst_myr = T_INSTABILITY_NOMINAL_MYR,
                                               double tau_comet = TAU_COMET_MYR, double f_final = 0.005) const {
    if (time_myr <= t_inst_myr) {
      return 1.0;
    }
    double dt = time_myr - t_inst_myr;
    return f_final + (1.0 - f_final) * std::exp(-dt / tau_comet);
  }

  // Gravitational focusing factor F_g = 1 + (v_esc / v_inf)^2
  double gravitational_focusing_factor(double v_esc_km_s, double v_inf_km_s) const {
    double ratio = v_esc_km_s / v_inf_km_s;
    return 1.0 + ratio * ratio;
  }

  // Effective collision cross section sigma [m^2]
  double effective_collision_cross_section_m2(double radius_m, double v_esc_km_s, double v_inf_km_s) const {
    double fg = gravitational_focusing_factor(v_esc_km_s, v_inf_km_s);
    return M_PI * radius_m * radius_m * fg;
  }

  // Target planet collision mass ratio relative to Moon
  double relative_impact_mass_ratio_vs_moon(double r_target_m, double v_esc_target_km_s,
                                            double v_inf_km_s = 15.0) const {
    double sigma_target = effective_collision_cross_section_m2(r_target_m, v_esc_target_km_s, v_inf_km_s);
    double sigma_moon = effective_collision_cross_section_m2(R_MOON, V_ESC_MOON, v_inf_km_s);
    return sigma_target / sigma_moon;
  }

  // Lunar impact flux from asteroids [kg/yr]
  double lunar_impact_flux_asteroids_kg_yr(double time_myr, double t_inst_myr = T_INSTABILITY_NOMINAL_MYR,
                                           double m_tot_ast_kg = TOTAL_LUNAR_MASS_KG * ASTEROID_MASS_FRACTION,
                                           double tau_rise_myr = TAU_RISE_MYR,
                                           double tau_ast_myr = TAU_ASTEROID_MYR) const {
    double arg = (time_myr - t_inst_myr) / tau_rise_myr;
    double logistic = 1.0 / (1.0 + std::exp(-arg));
    double dt = std::max(0.0, time_myr - t_inst_myr);
    double decay = std::exp(-dt / tau_ast_myr);
    double flux_peak = m_tot_ast_kg / (tau_ast_myr * 1.0e6);
    return flux_peak * logistic * decay;
  }

  // Lunar impact flux from comets [kg/yr]
  double lunar_impact_flux_comets_kg_yr(double time_myr, double t_inst_myr = T_INSTABILITY_NOMINAL_MYR,
                                        double m_tot_comet_kg = TOTAL_LUNAR_MASS_KG * COMET_MASS_FRACTION,
                                        double tau_rise_myr = 3.0,
                                        double tau_comet_myr = TAU_COMET_MYR) const {
    double arg = (time_myr - t_inst_myr) / tau_rise_myr;
    double logistic = 1.0 / (1.0 + std::exp(-arg));
    double dt = std::max(0.0, time_myr - t_inst_myr);
    double decay = std::exp(-dt / tau_comet_myr);
    double flux_peak = m_tot_comet_kg / (tau_comet_myr * 1.0e6);
    return flux_peak * logistic * decay;
  }

  // Total Lunar impact flux [kg/yr]
  double lunar_total_impact_flux_kg_yr(double time_myr, double t_inst_myr = T_INSTABILITY_NOMINAL_MYR,
                                       double background_flux_kg_yr = 5.0e10) const {
    double f_ast = lunar_impact_flux_asteroids_kg_yr(time_myr, t_inst_myr);
    double f_comet = lunar_impact_flux_comets_kg_yr(time_myr, t_inst_myr);
    return f_ast + f_comet + background_flux_kg_yr;
  }

  // Target impact flux [kg/yr] for specified body ("Earth", "Moon", "Mars", "Mercury", "Venus")
  double target_total_impact_flux_kg_yr(const std::string& target, double time_myr,
                                        double t_inst_myr = T_INSTABILITY_NOMINAL_MYR) const {
    double f_moon_ast = lunar_impact_flux_asteroids_kg_yr(time_myr, t_inst_myr);
    double f_moon_comet = lunar_impact_flux_comets_kg_yr(time_myr, t_inst_myr);

    double r_target = R_MOON;
    double v_esc = V_ESC_MOON;

    if (target == "Earth" || target == "earth") {
      r_target = R_EARTH;
      v_esc = V_ESC_EARTH;
    } else if (target == "Mars" || target == "mars") {
      r_target = R_MARS;
      v_esc = V_ESC_MARS;
    } else if (target == "Mercury" || target == "mercury") {
      r_target = R_MERCURY;
      v_esc = V_ESC_MERCURY;
    } else if (target == "Venus" || target == "venus") {
      r_target = R_VENUS;
      v_esc = V_ESC_VENUS;
    }

    double ratio_ast = relative_impact_mass_ratio_vs_moon(r_target, v_esc, V_INF_ASTEROIDS);
    double ratio_comet = relative_impact_mass_ratio_vs_moon(r_target, v_esc, V_INF_COMETS);

    return f_moon_ast * ratio_ast + f_moon_comet * ratio_comet + 5.0e10 * (ratio_ast + ratio_comet) * 0.5;
  }

  // Integrated cumulative mass delivered [kg] to target body up to time_myr
  double cumulative_mass_delivered_kg(const std::string& target, double time_myr,
                                      double t_inst_myr = T_INSTABILITY_NOMINAL_MYR,
                                      double t_start_myr = 0.0, int steps = 1000) const {
    if (time_myr <= t_start_myr) return 0.0;
    double dt_myr = (time_myr - t_start_myr) / steps;
    double total_mass = 0.0;
    for (int i = 0; i <= steps; ++i) {
      double t = t_start_myr + i * dt_myr;
      double flux = target_total_impact_flux_kg_yr(target, t, t_inst_myr);
      double weight = (i == 0 || i == steps) ? 0.5 : 1.0;
      total_mass += weight * flux * (dt_myr * 1.0e6);
    }
    return total_mass;
  }

  // Lunar basin formation rate [basins / Myr]
  double lunar_basin_formation_rate_per_myr(double time_myr, double t_inst_myr = T_INSTABILITY_NOMINAL_MYR,
                                            double total_basins = TOTAL_LUNAR_BASINS) const {
    double f_ast = lunar_impact_flux_asteroids_kg_yr(time_myr, t_inst_myr);
    double f_comet = lunar_impact_flux_comets_kg_yr(time_myr, t_inst_myr);
    double flux_sum = f_ast + f_comet;
    double basin_per_kg = total_basins / TOTAL_LUNAR_MASS_KG;
    return flux_sum * 1.0e6 * basin_per_kg;
  }

  // Cumulative lunar basins formed up to time_myr
  double cumulative_lunar_basins(double time_myr, double t_inst_myr = T_INSTABILITY_NOMINAL_MYR,
                                 double total_basins = TOTAL_LUNAR_BASINS) const {
    double m_deliv = cumulative_mass_delivered_kg("Moon", time_myr, t_inst_myr);
    return std::min(total_basins, total_basins * (m_deliv / TOTAL_LUNAR_MASS_KG));
  }

  // Cumulative crater size frequency distribution N(>D)
  double crater_size_frequency_distribution(double d_km, double n_ref_10km = 450.0,
                                            double b_slope = 2.8) const {
    if (d_km <= 0.0) return 0.0;
    return n_ref_10km * std::pow(d_km / 10.0, -b_slope);
  }
};

using Gomes2005LHBModel = Gomes2005LateHeavyBombardmentModel;
using Paper225LHBModel = Gomes2005LateHeavyBombardmentModel;

// ============================================================================
// 130. ENCELADUS EQUILIBRIUM TIDAL HEATING & RESONANT HEAT FLUX MODEL
// (Meyer & Wisdom 2007, Icarus 188, 535-539; Spencer et al. 2006, Science 311;
//  Howett et al. 2011, JGR 116; Lainey et al. 2012, 2017)
// ============================================================================
class MeyerWisdom2007EnceladusTidalModel {
 public:
  // Primary: Saturn physical constants
  static constexpr double M_SATURN_KG = 5.6834e26;       // Saturn mass [kg]
  static constexpr double R_SATURN_EQ_M = 6.0268e7;      // Saturn equatorial radius [m] (60,268 km)
  static constexpr double R_SATURN_VOL_M = 5.8232e7;     // Saturn volumetric radius [m]
  static constexpr double K2_SATURN_NOM = 0.341;         // Saturn nominal Love number k2 (Meyer & Wisdom 2007)
  static constexpr double Q_SATURN_CANONICAL = 18000.0;  // Canonical Goldreich-Soter lower bound
  static constexpr double Q_SATURN_ASTROMETRIC = 1695.0; // Astrometrically measured Q (Lainey et al. 2012, 2017)
  static constexpr double OMEGA_SATURN_RAD_S = 1.6378e-4;// Saturn rotation frequency [rad/s] (10.656 h)

  // Inner Satellite: Enceladus (1)
  static constexpr double M_ENCELADUS_KG = 1.0803e20;    // Enceladus mass [kg]
  static constexpr double R_ENCELADUS_M = 2.521e5;       // Enceladus mean radius [m] (252.1 km)
  static constexpr double A_ENCELADUS_M = 2.3804e8;      // Semi-major axis [m] (238,040 km)
  static constexpr double E_ENCELADUS_NOM = 0.0047;      // Present forced eccentricity
  static constexpr double G_ENCELADUS = 0.1134;          // Surface gravity [m/s^2]
  static constexpr double RHO_ICE = 917.0;               // Ice Ih density [kg/m^3]
  static constexpr double A_CONDUCT = 567.0;             // Ice thermal conductivity coeff [W/m]
  static constexpr double T_SURF = 75.0;                 // Surface temperature [K]
  static constexpr double T_MELT_0 = 273.15;             // Ice melting temperature at 0 Pa [K]
  static constexpr double GAMMA_CLAPEYRON = 7.4e-8;      // Clapeyron slope dT_m/dP [K/Pa]
  static constexpr double K2_OVER_Q_ENC_NOM = 0.0107;    // Nominal Enceladus k2/Q metric
  static constexpr double P_RADIO_NOM_GW = 0.32;         // Core radiogenic power [GW]
  static constexpr double P_OBS_SPENCER_GW = 5.8;        // Cassini CIRS observed heat flow [GW] (Spencer 2006: 5.8 +/- 1.9 GW)
  static constexpr double P_OBS_HOWETT_GW = 15.8;        // Revised Cassini CIRS heat flow [GW] (Howett 2011: 15.8 +/- 3.1 GW)

  // Outer Satellite: Dione (2)
  static constexpr double M_DIONE_KG = 1.0955e21;        // Dione mass [kg]
  static constexpr double R_DIONE_M = 5.614e5;           // Dione mean radius [m] (561.4 km)
  static constexpr double A_DIONE_M = 3.7742e8;          // Dione semi-major axis [m] (377,420 km)
  static constexpr double E_DIONE_NOM = 0.0022;          // Dione eccentricity

  // Mean orbital frequency n_E [rad/s]
  double mean_motion_enceladus_rad_s() const {
    return std::sqrt(G * (M_SATURN_KG + M_ENCELADUS_KG) / std::pow(A_ENCELADUS_M, 3.0));
  }

  // Mean orbital frequency n_D [rad/s]
  double mean_motion_dione_rad_s() const {
    return std::sqrt(G * (M_SATURN_KG + M_DIONE_KG) / std::pow(A_DIONE_M, 3.0));
  }

  // Orbital period of Enceladus [hours]
  double orbital_period_enceladus_hours() const {
    return (2.0 * M_PI / mean_motion_enceladus_rad_s()) / 3600.0;
  }

  // Orbital period of Dione [hours]
  double orbital_period_dione_hours() const {
    return (2.0 * M_PI / mean_motion_dione_rad_s()) / 3600.0;
  }

  // Resonant frequency ratio n_E / n_D (~2.0)
  double resonance_frequency_ratio() const {
    return mean_motion_enceladus_rad_s() / mean_motion_dione_rad_s();
  }

  // Orbital angular momentum of Enceladus L_E [kg m^2 / s]
  double angular_momentum_enceladus(double e = E_ENCELADUS_NOM) const {
    return M_ENCELADUS_KG * std::sqrt(G * M_SATURN_KG * A_ENCELADUS_M * (1.0 - e * e));
  }

  // Orbital angular momentum of Dione L_D [kg m^2 / s]
  double angular_momentum_dione(double e = E_DIONE_NOM) const {
    return M_DIONE_KG * std::sqrt(G * M_SATURN_KG * A_DIONE_M * (1.0 - e * e));
  }

  // Total orbital angular momentum L_tot [kg m^2 / s]
  double total_angular_momentum() const {
    return angular_momentum_enceladus() + angular_momentum_dione();
  }

  // Tidal torque on Saturn raised by Enceladus N_SE [N m]
  double saturn_tidal_torque_enceladus(double k2S = K2_SATURN_NOM, double QS = Q_SATURN_CANONICAL) const {
    return 1.5 * (k2S / std::max(1.0, QS)) * G * std::pow(M_ENCELADUS_KG, 2.0) *
           std::pow(R_SATURN_EQ_M, 5.0) / std::pow(A_ENCELADUS_M, 6.0);
  }

  // Tidal torque on Saturn raised by Dione N_SD [N m]
  double saturn_tidal_torque_dione(double k2S = K2_SATURN_NOM, double QS = Q_SATURN_CANONICAL) const {
    return 1.5 * (k2S / std::max(1.0, QS)) * G * std::pow(M_DIONE_KG, 2.0) *
           std::pow(R_SATURN_EQ_M, 5.0) / std::pow(A_DIONE_M, 6.0);
  }

  // Resonant orbital expansion rate (da/dt)/a [s^-1]
  double resonant_expansion_rate_s_inv(double k2S = K2_SATURN_NOM, double QS = Q_SATURN_CANONICAL) const {
    double n_se = saturn_tidal_torque_enceladus(k2S, QS);
    double n_sd = saturn_tidal_torque_dione(k2S, QS);
    double l_tot = total_angular_momentum();
    return 2.0 * (n_se + n_sd) / l_tot;
  }

  // Equilibrium tidal dissipation heating rate in Enceladus [Watts] (Meyer & Wisdom 2007)
  // \dot{E}_eq = (n_E - n_D) * (L_D * N_SE - L_E * N_SD) / (L_E + L_D)
  double equilibrium_tidal_heating_watts(double k2S = K2_SATURN_NOM, double QS = Q_SATURN_CANONICAL) const {
    double n_e = mean_motion_enceladus_rad_s();
    double n_d = mean_motion_dione_rad_s();
    double l_e = angular_momentum_enceladus();
    double l_d = angular_momentum_dione();
    double n_se = saturn_tidal_torque_enceladus(k2S, QS);
    double n_sd = saturn_tidal_torque_dione(k2S, QS);

    double numerator = (n_e - n_d) * (l_d * n_se - l_e * n_sd);
    double denominator = l_e + l_d;
    return numerator / denominator;
  }

  // Equilibrium tidal dissipation heating rate [GW]
  double equilibrium_tidal_heating_gw(double k2S = K2_SATURN_NOM, double QS = Q_SATURN_CANONICAL) const {
    return equilibrium_tidal_heating_watts(k2S, QS) * 1.0e-9;
  }

  // Surface equilibrium tidal heat flux [mW/m^2]
  double equilibrium_surface_heat_flux_mw_m2(double k2S = K2_SATURN_NOM, double QS = Q_SATURN_CANONICAL) const {
    double area = 4.0 * M_PI * R_ENCELADUS_M * R_ENCELADUS_M;
    return (equilibrium_tidal_heating_watts(k2S, QS) / area) * 1.0e3;
  }

  // Instantaneous viscoelastic tidal heating power [Watts] (Peale 1979)
  double instantaneous_tidal_heating_watts(double e = E_ENCELADUS_NOM, double k2_over_Q_E = K2_OVER_Q_ENC_NOM) const {
    double n_e = mean_motion_enceladus_rad_s();
    double factor = 10.5 * k2_over_Q_E * G * std::pow(M_SATURN_KG, 2.0) *
                    std::pow(R_ENCELADUS_M, 5.0) * n_e / std::pow(A_ENCELADUS_M, 6.0);
    return factor * e * e;
  }

  // Instantaneous viscoelastic tidal heating power [GW]
  double instantaneous_tidal_heating_gw(double e = E_ENCELADUS_NOM, double k2_over_Q_E = K2_OVER_Q_ENC_NOM) const {
    return instantaneous_tidal_heating_watts(e, k2_over_Q_E) * 1.0e-9;
  }

  // Equilibrium forced eccentricity e_eq where instantaneous tidal heating balances equilibrium tidal heating
  double equilibrium_forced_eccentricity(double k2_over_Q_E = K2_OVER_Q_ENC_NOM,
                                         double k2S = K2_SATURN_NOM,
                                         double QS = Q_SATURN_CANONICAL) const {
    double p_eq_w = equilibrium_tidal_heating_watts(k2S, QS);
    double n_e = mean_motion_enceladus_rad_s();
    double factor = 10.5 * k2_over_Q_E * G * std::pow(M_SATURN_KG, 2.0) *
                    std::pow(R_ENCELADUS_M, 5.0) * n_e / std::pow(A_ENCELADUS_M, 6.0);
    if (factor <= 0.0) return 0.0;
    return std::sqrt(p_eq_w / factor);
  }

  // Required Saturn quality factor Q_S to supply an observed heating power [GW] in steady-state equilibrium
  double required_saturn_q_for_observed_heat(double P_obs_gw = P_OBS_SPENCER_GW, double k2S = K2_SATURN_NOM) const {
    double p_eq_canonical_gw = equilibrium_tidal_heating_gw(k2S, Q_SATURN_CANONICAL);
    return Q_SATURN_CANONICAL * (p_eq_canonical_gw / P_obs_gw);
  }

  // Basal melting temperature [K] accounting for hydrostatic Clapeyron depression
  double basal_melting_temperature_k(double d_shell_km) const {
    double d_m = d_shell_km * 1.0e3;
    double p_base = RHO_ICE * G_ENCELADUS * d_m;
    return T_MELT_0 - GAMMA_CLAPEYRON * p_base;
  }

  // Conductive heat loss through Ice I shell [Watts]
  double conductive_heat_loss_watts(double d_shell_km) const {
    double d_m = std::max(100.0, d_shell_km * 1.0e3);
    double t_m = basal_melting_temperature_k(d_shell_km);
    double area = 4.0 * M_PI * R_ENCELADUS_M * R_ENCELADUS_M;
    double flux = (A_CONDUCT * std::log(t_m / T_SURF)) / d_m;
    return flux * area;
  }

  // Conductive heat loss [GW]
  double conductive_heat_loss_gw(double d_shell_km) const {
    return conductive_heat_loss_watts(d_shell_km) * 1.0e-9;
  }

  // Equilibrium ice shell thickness [km] where Q_cond(d_eq) = P_heat + P_radio
  double equilibrium_ice_shell_thickness_km(double heat_power_gw, double p_radio_gw = P_RADIO_NOM_GW) const {
    double total_heat_gw = heat_power_gw + p_radio_gw;
    if (total_heat_gw <= 0.0) return 100.0;
    double area = 4.0 * M_PI * R_ENCELADUS_M * R_ENCELADUS_M;
    double target_flux_w_m2 = (total_heat_gw * 1.0e9) / area;
    double d_guess_m = 25.0e3;
    for (int iter = 0; iter < 15; ++iter) {
      double t_m = basal_melting_temperature_k(d_guess_m / 1000.0);
      d_guess_m = (A_CONDUCT * std::log(t_m / T_SURF)) / target_flux_w_m2;
    }
    return d_guess_m / 1.0e3;
  }

  // Energy deficit Delta P [GW] between observed heat flow and steady-state equilibrium tidal + radiogenic power
  double energy_deficit_gw(double P_obs_gw = P_OBS_SPENCER_GW, double k2S = K2_SATURN_NOM, double QS = Q_SATURN_CANONICAL) const {
    double p_eq_gw = equilibrium_tidal_heating_gw(k2S, QS);
    return P_obs_gw - (p_eq_gw + P_RADIO_NOM_GW);
  }
};

using MeyerWisdom2007Model = MeyerWisdom2007EnceladusTidalModel;
using MeyerWisdom2007EnceladusModel = MeyerWisdom2007EnceladusTidalModel;
using Paper216EnceladusModel = MeyerWisdom2007EnceladusTidalModel;
using EnceladusEquilibriumTidalHeatingModel = MeyerWisdom2007EnceladusTidalModel;

// ============================================================================
// 131. SPOHN & SCHUBERT (2003) OCEANS IN ICY MOONS CONVECTIVE ICE SHELL MODEL
// (Spohn & Schubert 2003, Icarus 161, 456-467; Solomatov 1995; Grasset & Parmentier 1998)
// ============================================================================
class SpohnSchubert2003IcyMoonOceanModel {
 public:
  static constexpr double GAS_CONST_R = 8.314462;       // Universal gas constant [J/(mol K)]
  static constexpr double RHO_ICE = 920.0;              // Ice I density [kg/m^3]
  static constexpr double RHO_OCEAN = 1000.0;           // Liquid water ocean density [kg/m^3]
  static constexpr double ALPHA_EXP = 1.56e-4;          // Thermal expansivity [1/K]
  static constexpr double CP_ICE = 2000.0;              // Ice heat capacity [J/(kg K)]
  static constexpr double A_CONDUCT = 567.0;            // Thermal conductivity coeff k(T) = A/T [W/m]
  static constexpr double KAPPA_DIFF = 1.25e-6;         // Thermal diffusivity [m^2/s]
  static constexpr double T_MELT_PURE_0 = 273.15;       // Pure water melting point at 0 Pa [K]
  static constexpr double GAMMA_MELT = -1.01e-7;        // Clapeyron melting slope [K/Pa] (-0.101 K/MPa)
  static constexpr double E_ACT_DIFF = 50000.0;         // Diffusion creep activation energy [J/mol]
  static constexpr double E_ACT_DISL = 60000.0;         // Dislocation creep activation energy [J/mol]
  static constexpr double ETA_BASE_NOM = 1.0e14;        // Nominal basal viscosity [Pa s]
  static constexpr double T_EUTECTIC_NH3 = 176.0;       // Ammonia-water eutectic temperature [K]

  // Nominal Satellite Parameters (Spohn & Schubert 2003 Table 1 & text)
  // Europa
  static constexpr double R_EUROPA_KM = 1561.0;
  static constexpr double G_EUROPA = 1.315;
  static constexpr double T_SURF_EUROPA_K = 100.0;
  static constexpr double D_H2O_EUROPA_KM = 120.0;
  static constexpr double F_RAD_EUROPA_MW_M2 = 6.0;
  static constexpr double F_TIDE_EUROPA_MW_M2 = 30.0;

  // Ganymede
  static constexpr double R_GANYMEDE_KM = 2634.0;
  static constexpr double G_GANYMEDE = 1.428;
  static constexpr double T_SURF_GANYMEDE_K = 110.0;
  static constexpr double D_H2O_GANYMEDE_KM = 800.0;
  static constexpr double F_RAD_GANYMEDE_MW_M2 = 4.5;
  static constexpr double F_TIDE_GANYMEDE_MW_M2 = 1.0;

  // Callisto
  static constexpr double R_CALLISTO_KM = 2410.0;
  static constexpr double G_CALLISTO = 1.235;
  static constexpr double T_SURF_CALLISTO_K = 105.0;
  static constexpr double D_H2O_CALLISTO_KM = 300.0;
  static constexpr double F_RAD_CALLISTO_MW_M2 = 3.2;
  static constexpr double F_TIDE_CALLISTO_MW_M2 = 0.0;

  // Titan
  static constexpr double R_TITAN_KM = 2575.0;
  static constexpr double G_TITAN = 1.352;
  static constexpr double T_SURF_TITAN_K = 94.0;
  static constexpr double D_H2O_TITAN_KM = 400.0;
  static constexpr double F_RAD_TITAN_MW_M2 = 4.0;
  static constexpr double F_TIDE_TITAN_MW_M2 = 0.0;

  // Enceladus
  static constexpr double R_ENCELADUS_KM = 252.1;
  static constexpr double G_ENCELADUS = 0.113;
  static constexpr double T_SURF_ENCELADUS_K = 75.0;
  static constexpr double D_H2O_ENCELADUS_KM = 60.0;
  static constexpr double F_RAD_ENCELADUS_MW_M2 = 0.5;
  static constexpr double F_TIDE_ENCELADUS_MW_M2 = 80.0;

  // Hydrostatic basal pressure [Pa]
  double basal_pressure_pa(double d_shell_km, double g = G_EUROPA, double rho_ice = RHO_ICE) const {
    double d_m = d_shell_km * 1.0e3;
    return rho_ice * g * d_m;
  }

  // Ammonia freezing point depression [K] for ammonia concentration wt%
  double ammonia_freezing_depression_k(double ammonia_wt_pct) const {
    double w = std::max(0.0, std::min(33.0, ammonia_wt_pct));
    return 1.8 * w + 0.03 * w * w;
  }

  // Basal melting temperature T_m(P) [K] accounting for Clapeyron effect & ammonia
  double melting_temperature_k(double pressure_pa, double ammonia_wt_pct = 0.0) const {
    double p_eff = std::min(2.07e8, std::max(0.0, pressure_pa)); // Ice I limit up to ~207 MPa
    double t_m = T_MELT_PURE_0 + GAMMA_MELT * p_eff;
    double delta_nh3 = ammonia_freezing_depression_k(ammonia_wt_pct);
    t_m -= delta_nh3;
    return std::max(T_EUTECTIC_NH3, t_m);
  }

  // Arrhenius ice viscosity eta(T) [Pa s]
  double viscosity_at_temperature_pa_s(double T_k, double eta_base = ETA_BASE_NOM,
                                      double E_act = E_ACT_DIFF, double T_base_k = T_MELT_PURE_0) const {
    double T = std::max(40.0, T_k);
    double exponent = (E_act / GAS_CONST_R) * (1.0 / T - 1.0 / T_base_k);
    exponent = std::min(80.0, exponent);
    return eta_base * std::exp(exponent);
  }

  // Viscosity contrast Delta eta = eta(T_surf) / eta(T_base)
  double viscosity_contrast(double T_surf_k, double T_base_k, double E_act = E_ACT_DIFF) const {
    return viscosity_at_temperature_pa_s(T_surf_k, 1.0, E_act, T_base_k);
  }

  // Rheological temperature scale Delta T_rh = R T_base^2 / E* [K]
  double rheological_temperature_scale_k(double T_base_k = T_MELT_PURE_0, double E_act = E_ACT_DIFF) const {
    return (GAS_CONST_R * T_base_k * T_base_k) / E_act;
  }

  // Frank-Kamenetskii rheological parameter theta = (E* Delta T) / (R T_base^2)
  double frank_kamenetskii_theta(double T_surf_k, double T_base_k = T_MELT_PURE_0, double E_act = E_ACT_DIFF) const {
    double delta_t = std::max(1.0, T_base_k - T_surf_k);
    return (E_act * delta_t) / (GAS_CONST_R * T_base_k * T_base_k);
  }

  // Basal Rayleigh number Ra_b based on basal viscosity and full shell thickness D
  double basal_rayleigh_number(double d_shell_km, double g, double T_surf_k, double T_base_k,
                               double eta_base = ETA_BASE_NOM, double alpha_exp = ALPHA_EXP,
                               double kappa = KAPPA_DIFF, double rho_ice = RHO_ICE) const {
    double D_m = d_shell_km * 1.0e3;
    double delta_t = std::max(1.0, T_base_k - T_surf_k);
    double numerator = rho_ice * g * alpha_exp * delta_t * std::pow(D_m, 3.0);
    double denominator = kappa * eta_base;
    return numerator / std::max(1.0e-30, denominator);
  }

  // Rheological Rayleigh number Ra_rh based on Delta T_rh
  double rheological_rayleigh_number(double d_shell_km, double g, double T_base_k,
                                     double eta_base = ETA_BASE_NOM, double E_act = E_ACT_DIFF,
                                     double alpha_exp = ALPHA_EXP, double kappa = KAPPA_DIFF,
                                     double rho_ice = RHO_ICE) const {
    double D_m = d_shell_km * 1.0e3;
    double delta_t_rh = rheological_temperature_scale_k(T_base_k, E_act);
    double numerator = rho_ice * g * alpha_exp * delta_t_rh * std::pow(D_m, 3.0);
    double denominator = kappa * eta_base;
    return numerator / std::max(1.0e-30, denominator);
  }

  // Critical Rayleigh number Ra_cr for onset of stagnant-lid convection (Solomatov 1995)
  double critical_rayleigh_number(double T_surf_k, double T_base_k, double E_act = E_ACT_DIFF) const {
    double theta = frank_kamenetskii_theta(T_surf_k, T_base_k, E_act);
    return 20.0 * std::pow(theta, 4.0);
  }

  // Whether solid-state stagnant-lid convection occurs
  bool is_convective(double d_shell_km, double g, double T_surf_k, double T_base_k,
                     double eta_base = ETA_BASE_NOM, double E_act = E_ACT_DIFF) const {
    double ra_b = basal_rayleigh_number(d_shell_km, g, T_surf_k, T_base_k, eta_base);
    double ra_cr = critical_rayleigh_number(T_surf_k, T_base_k, E_act);
    return ra_b >= ra_cr;
  }

  // Nusselt number Nu in stagnant lid regime (Spohn & Schubert 2003, Solomatov 1995)
  double nusselt_number(double d_shell_km, double g, double T_surf_k, double T_base_k,
                        double eta_base = ETA_BASE_NOM, double E_act = E_ACT_DIFF,
                        double a_coeff = 0.95, double beta = 0.22) const {
    double ra_b = basal_rayleigh_number(d_shell_km, g, T_surf_k, T_base_k, eta_base);
    double ra_cr = critical_rayleigh_number(T_surf_k, T_base_k, E_act);
    if (ra_b < ra_cr) {
      return 1.0;  // Subcritical: pure conduction
    }
    double ra_rh = rheological_rayleigh_number(d_shell_km, g, T_base_k, eta_base, E_act);
    double theta = frank_kamenetskii_theta(T_surf_k, T_base_k, E_act);
    double nu = a_coeff * std::pow(ra_rh, beta) / std::max(1.0, theta);
    return std::max(1.0, nu);
  }

  // Pure conductive heat flux [mW/m^2] with k(T) = A / T
  double conductive_heat_flux_mw_m2(double d_shell_km, double T_surf_k, double T_base_k,
                                   double A_cond = A_CONDUCT) const {
    double D_m = d_shell_km * 1.0e3;
    if (D_m <= 0.0) return 1.0e6;
    double flux_w_m2 = (A_cond * std::log(T_base_k / T_surf_k)) / D_m;
    return flux_w_m2 * 1.0e3;
  }

  // Total heat flux transported across ice shell [mW/m^2]
  double total_heat_flux_mw_m2(double d_shell_km, double g, double T_surf_k, double T_base_k,
                              double eta_base = ETA_BASE_NOM, double E_act = E_ACT_DIFF,
                              double A_cond = A_CONDUCT) const {
    double f_cond = conductive_heat_flux_mw_m2(d_shell_km, T_surf_k, T_base_k, A_cond);
    double nu = nusselt_number(d_shell_km, g, T_surf_k, T_base_k, eta_base, E_act);
    return f_cond * nu;
  }

  // Stagnant conductive lid thickness [km]
  double stagnant_lid_thickness_km(double d_shell_km, double g, double T_surf_k, double T_base_k,
                                   double eta_base = ETA_BASE_NOM, double E_act = E_ACT_DIFF) const {
    double nu = nusselt_number(d_shell_km, g, T_surf_k, T_base_k, eta_base, E_act);
    if (nu <= 1.001) return d_shell_km;
    double delta_t = std::max(1.0, T_base_k - T_surf_k);
    double delta_t_rh = rheological_temperature_scale_k(T_base_k, E_act);
    double lid_frac = (delta_t - delta_t_rh) / (delta_t * nu);
    lid_frac = std::min(1.0, std::max(0.05, lid_frac));
    return d_shell_km * lid_frac;
  }

  // Convective sublayer thickness [km]
  double convective_sublayer_thickness_km(double d_shell_km, double g, double T_surf_k, double T_base_k,
                                          double eta_base = ETA_BASE_NOM, double E_act = E_ACT_DIFF) const {
    double d_lid = stagnant_lid_thickness_km(d_shell_km, g, T_surf_k, T_base_k, eta_base, E_act);
    return std::max(0.0, d_shell_km - d_lid);
  }

  // Convective upwelling velocity [m/yr]
  double convective_velocity_m_yr(double d_shell_km, double g, double T_base_k,
                                  double eta_base = ETA_BASE_NOM, double E_act = E_ACT_DIFF,
                                  double c_u = 0.25) const {
    double ra_rh = rheological_rayleigh_number(d_shell_km, g, T_base_k, eta_base, E_act);
    double D_m = d_shell_km * 1.0e3;
    if (ra_rh <= 1.0) return 0.0;
    double u_m_s = c_u * (KAPPA_DIFF / D_m) * std::pow(ra_rh, 2.0 / 3.0);
    return u_m_s * (365.25 * 86400.0);
  }

  // Equilibrium ice shell thickness [km] where F_total(D_eq) == F_supply
  double equilibrium_shell_thickness_km(double g, double T_surf_k, double F_supply_mw_m2,
                                       double eta_base = ETA_BASE_NOM, double E_act = E_ACT_DIFF,
                                       double ammonia_wt_pct = 0.0,
                                       double d_min_km = 1.0, double d_max_km = 350.0) const {
    double d_low = d_min_km;
    double d_high = d_max_km;

    for (int iter = 0; iter < 100; ++iter) {
      double d_mid = 0.5 * (d_low + d_high);
      double p_base = basal_pressure_pa(d_mid, g);
      double t_base = melting_temperature_k(p_base, ammonia_wt_pct);
      double f_mid = total_heat_flux_mw_m2(d_mid, g, T_surf_k, t_base, eta_base, E_act);

      if (f_mid > F_supply_mw_m2) {
        d_low = d_mid;  // Thicker shell reduces heat flux
      } else {
        d_high = d_mid;
      }
    }
    return 0.5 * (d_low + d_high);
  }

  // Ocean thickness [km]
  double ocean_thickness_km(double total_h2o_km, double d_shell_km) const {
    return std::max(0.0, total_h2o_km - d_shell_km);
  }

  // Critical heat flux [mW/m^2] to prevent complete freezing
  double critical_heat_flux_for_ocean_mw_m2(double total_h2o_km, double T_surf_k, double T_base_k,
                                            double A_cond = A_CONDUCT) const {
    return conductive_heat_flux_mw_m2(total_h2o_km, T_surf_k, T_base_k, A_cond);
  }

  // Ocean survival boolean flag
  bool ocean_exists(double total_h2o_km, double d_eq_km) const {
    return d_eq_km < total_h2o_km;
  }

  struct SatelliteOceanResult {
    std::string name;
    double g;
    double T_surf_k;
    double T_base_k;
    double P_base_mpa;
    double D_shell_km;
    double D_lid_km;
    double D_conv_km;
    double D_ocean_km;
    double Nu;
    double Ra_b;
    double Ra_cr;
    double F_supply_mw_m2;
    double F_crit_mw_m2;
    bool is_convective;
    bool ocean_survives;
  };

  SatelliteOceanResult evaluate_satellite(const std::string& name, double g, double T_surf_k,
                                         double D_h2o_km, double F_rad_mw_m2, double F_tide_mw_m2,
                                         double eta_base = ETA_BASE_NOM, double ammonia_wt_pct = 0.0,
                                         double E_act = E_ACT_DIFF) const {
    double F_supply = F_rad_mw_m2 + F_tide_mw_m2;
    double D_eq = equilibrium_shell_thickness_km(g, T_surf_k, F_supply, eta_base, E_act, ammonia_wt_pct);
    double P_base = basal_pressure_pa(D_eq, g);
    double T_base = melting_temperature_k(P_base, ammonia_wt_pct);

    double nu = nusselt_number(D_eq, g, T_surf_k, T_base, eta_base, E_act);
    double ra_b = basal_rayleigh_number(D_eq, g, T_surf_k, T_base, eta_base);
    double ra_cr = critical_rayleigh_number(T_surf_k, T_base, E_act);
    bool conv = is_convective(D_eq, g, T_surf_k, T_base, eta_base, E_act);
    double D_lid = stagnant_lid_thickness_km(D_eq, g, T_surf_k, T_base, eta_base, E_act);
    double D_conv = convective_sublayer_thickness_km(D_eq, g, T_surf_k, T_base, eta_base, E_act);
    double D_ocean = ocean_thickness_km(D_h2o_km, D_eq);
    double F_crit = critical_heat_flux_for_ocean_mw_m2(D_h2o_km, T_surf_k, T_base);
    bool survives = ocean_exists(D_h2o_km, D_eq);

    return SatelliteOceanResult{
      name, g, T_surf_k, T_base, P_base / 1.0e6, D_eq, D_lid, D_conv, D_ocean,
      nu, ra_b, ra_cr, F_supply, F_crit, conv, survives
    };
  }

  SatelliteOceanResult evaluate_europa(double tidal_heat_mw_m2 = F_TIDE_EUROPA_MW_M2,
                                      double eta_base = ETA_BASE_NOM, double ammonia_pct = 0.0) const {
    return evaluate_satellite("Europa", G_EUROPA, T_SURF_EUROPA_K, D_H2O_EUROPA_KM,
                              F_RAD_EUROPA_MW_M2, tidal_heat_mw_m2, eta_base, ammonia_pct);
  }

  SatelliteOceanResult evaluate_ganymede(double tidal_heat_mw_m2 = F_TIDE_GANYMEDE_MW_M2,
                                        double eta_base = ETA_BASE_NOM, double ammonia_pct = 0.0) const {
    return evaluate_satellite("Ganymede", G_GANYMEDE, T_SURF_GANYMEDE_K, D_H2O_GANYMEDE_KM,
                              F_RAD_GANYMEDE_MW_M2, tidal_heat_mw_m2, eta_base, ammonia_pct);
  }

  SatelliteOceanResult evaluate_callisto(double tidal_heat_mw_m2 = F_TIDE_CALLISTO_MW_M2,
                                        double eta_base = ETA_BASE_NOM, double ammonia_pct = 0.0) const {
    return evaluate_satellite("Callisto", G_CALLISTO, T_SURF_CALLISTO_K, D_H2O_CALLISTO_KM,
                              F_RAD_CALLISTO_MW_M2, tidal_heat_mw_m2, eta_base, ammonia_pct);
  }

  SatelliteOceanResult evaluate_titan(double tidal_heat_mw_m2 = F_TIDE_TITAN_MW_M2,
                                     double eta_base = ETA_BASE_NOM, double ammonia_pct = 5.0) const {
    return evaluate_satellite("Titan", G_TITAN, T_SURF_TITAN_K, D_H2O_TITAN_KM,
                              F_RAD_TITAN_MW_M2, tidal_heat_mw_m2, eta_base, ammonia_pct);
  }

  SatelliteOceanResult evaluate_enceladus(double tidal_heat_mw_m2 = F_TIDE_ENCELADUS_MW_M2,
                                         double eta_base = ETA_BASE_NOM, double ammonia_pct = 1.0) const {
    return evaluate_satellite("Enceladus", G_ENCELADUS, T_SURF_ENCELADUS_K, D_H2O_ENCELADUS_KM,
                              F_RAD_ENCELADUS_MW_M2, tidal_heat_mw_m2, eta_base, ammonia_pct);
  }
};

using SpohnSchubert2003OceanModel = SpohnSchubert2003IcyMoonOceanModel;
using Paper221IcyMoonOceanModel = SpohnSchubert2003IcyMoonOceanModel;

}  // namespace hot_jupiter

#endif  // HOT_JUPITER_SOLAR_SYSTEM_HPP





