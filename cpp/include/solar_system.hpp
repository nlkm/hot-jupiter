// C++ Core Library Extension for Solar System Bodies & Orbital Dynamics
// Generalized First-Principles Models for Planetary, Lunar, Ring, Asteroid, and Comet Physics.

#ifndef HOT_JUPITER_SOLAR_SYSTEM_HPP
#define HOT_JUPITER_SOLAR_SYSTEM_HPP

#include <algorithm>
#include <cmath>
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

}  // namespace hot_jupiter

#endif  // HOT_JUPITER_SOLAR_SYSTEM_HPP
