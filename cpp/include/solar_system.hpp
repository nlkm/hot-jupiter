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

}  // namespace hot_jupiter

#endif  // HOT_JUPITER_SOLAR_SYSTEM_HPP
