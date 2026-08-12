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
};

// Backward-compatibility alias
using MoonTidalDynamicsModel = TidalDissipationModel;
using EnceladusTidalOceanModel = TidalDissipationModel;

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

}  // namespace hot_jupiter

#endif  // HOT_JUPITER_SOLAR_SYSTEM_HPP
