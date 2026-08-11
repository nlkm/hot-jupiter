// C++ Core Library Extension for Solar System Bodies & Orbital Dynamics
// Models planets, moons, planetary rings, asteroids, and comets.

#ifndef HOT_JUPITER_SOLAR_SYSTEM_HPP
#define HOT_JUPITER_SOLAR_SYSTEM_HPP

#include <algorithm>
#include <cmath>
#include <string>
#include <tuple>
#include <vector>

#include "constants.hpp"

namespace hot_jupiter {

// 1. Galilean & Saturnian Moon Tidal Dynamics & Laplace Resonances
class MoonTidalDynamicsModel {
 public:
  // Io-Europa-Ganymede 4:2:1 Laplace Mean Motion Resonance Tidal Heating
  double io_tidal_heating_power_watts(double eccentricity = 0.0041) const {
    // Peale et al. (1979) tidal heating power formula: P = (21/2) * (k2/Q) * (G M_J^2 R_Io^5 n / a^6) * e^2
    double M_J = 1.898e27;       // Jupiter mass [kg]
    double R_Io = 1.821e6;       // Io radius [m]
    double a_Io = 4.217e8;       // Semi-major axis [m]
    double k2_over_Q = 0.015;    // Io tidal dissipation metric
    double n = std::sqrt(G * M_J / (a_Io * a_Io * a_Io));
    double factor = 10.5 * k2_over_Q * G * M_J * M_J * std::pow(R_Io, 5.0) * n / std::pow(a_Io, 6.0);
    return factor * eccentricity * eccentricity;
  }

  // Earth-Moon Tidal Recession Rate [m/s]
  double earth_moon_recession_rate_m_s(double a_moon_m = 3.844e8) const {
    // Current observed lunar recession ~ 3.8 cm/yr
    double recession_cm_yr = 3.8 * std::pow(3.844e8 / a_moon_m, 5.5);
    return (recession_cm_yr * 0.01) / (365.25 * 86400.0);
  }
};

// 2. Planetary Ring Dynamics & Shepherd Moon Roche Disruption
class PlanetaryRingModel {
 public:
  // Fluid / Solid Roche Disruption Limit Radius [m]
  double roche_limit_m(double R_planet_m, double density_planet, double density_moon, bool fluid = true) const {
    double C = fluid ? 2.456 : 1.442;
    return C * R_planet_m * std::pow(density_planet / std::max(10.0, density_moon), 1.0 / 3.0);
  }

  // Saturn Shepherd Moon Torque (Prometheus / Pandora F-ring confinement)
  double shepherd_moon_torque(double M_moon, double M_saturn, double a_ring, double delta_a) const {
    double n = std::sqrt(G * M_saturn / std::pow(a_ring, 3.0));
    double torque_scale = (G * G * M_moon * M_moon) / (std::pow(a_ring, 2.0) * n * std::pow(delta_a / a_ring, 4.0));
    return torque_scale;
  }
};

// 3. Asteroid Dynamics (Yarkovsky, YORP, Kirkwood Gaps)
class AsteroidDynamicsModel {
 public:
  // Yarkovsky Thermal Photon Recoil Non-Gravitational Acceleration [m/s^2]
  double yarkovsky_acceleration_m_s2(double radius_m, double density_kg_m3, double a_au, double obliquity_deg) const {
    double mass = (4.0 / 3.0) * M_PI * std::pow(radius_m, 3.0) * density_kg_m3;
    double L_sun = 3.828e26;
    double c = 299792458.0;
    double a_m = a_au * AU;
    double solar_flux = L_sun / (4.0 * M_PI * a_m * a_m);
    double cross_section = M_PI * radius_m * radius_m;
    double alpha = 0.15;  // Thermal efficiency
    double obl_rad = obliquity_deg * M_PI / 180.0;
    double force = (4.0 / 9.0) * alpha * cross_section * solar_flux / c * std::cos(obl_rad);
    return force / std::max(1.0e-5, mass);
  }

  // Kirkwood Gap Resonant Clearance Metric (3:1, 5:2, 2:1 Jupiter Resonances)
  bool in_kirkwood_gap(double a_au) const {
    const double gaps[4] = {2.50, 2.82, 2.95, 3.27};  // 3:1, 5:2, 7:3, 2:1
    for (double g : gaps) {
      if (std::abs(a_au - g) < 0.03) return true;
    }
    return false;
  }
};

// 4. Comet Sublimation Non-Gravitational Acceleration & Oort Cloud Impulse
class CometDynamicsModel {
 public:
  // Marsden Sublimation Recoil Non-Gravitational Function g(r)
  double marsden_sublimation_g_r(double r_au, double r0_au = 2.808, double m = 2.15, double n = 5.09, double k = 4.614) const {
    double alpha = 0.11126;
    double ratio = r_au / r0_au;
    double term1 = std::pow(ratio, -m);
    double term2 = std::pow(1.0 + std::pow(ratio, n), -k);
    return alpha * term1 * term2;
  }

  // Non-Gravitational Sublimation Acceleration Vector Magnitude [m/s^2]
  double non_gravitational_acceleration_m_s2(double r_au, double A1_au_day2) const {
    double g_r = marsden_sublimation_g_r(r_au);
    // Convert A1 [AU/day^2] to m/s^2
    double a1_m_s2 = A1_au_day2 * AU / std::pow(86400.0, 2.0);
    return a1_m_s2 * g_r;
  }
};

// 5. Planetary Relativistic Precession & Secular Chaos (Laskar & Gastineau 2009)
class RelativisticPrecessionModel {
 public:
  // General Relativistic Schwarzschild Perihelion Precession Rate [rad/s]
  double gr_perihelion_precession_rad_s(double M_star_kg, double a_m, double e) const {
    double c = 299792458.0;
    double n = std::sqrt(G * M_star_kg / std::pow(a_m, 3.0));
    double e2 = e * e;
    return (3.0 * G * M_star_kg * n) / (c * c * a_m * std::max(1.0e-5, 1.0 - e2));
  }

  // Mercury GR Precession in Arcseconds per Century
  double mercury_gr_precession_arcsec_century() const {
    double rad_s = gr_perihelion_precession_rad_s(M_SUN, 0.387098 * AU, 0.20563);
    double arcsec_per_rad = (180.0 * 3600.0) / M_PI;
    double seconds_per_century = 100.0 * 365.25 * 86400.0;
    return rad_s * arcsec_per_rad * seconds_per_century;
  }
};

// 6. Protoplanetary Gas Disk Migration Torques (Grand Tack Walsh et al. 2011)
class ProtoplanetaryDiskTorqueModel {
 public:
  // Type I Migration Torque Scaling Formula
  double type_i_torque_nm(double M_planet_kg, double M_star_kg, double a_m, double aspect_ratio = 0.05, double surface_density_kg_m2 = 1000.0) const {
    double n = std::sqrt(G * M_star_kg / std::pow(a_m, 3.0));
    double q = M_planet_kg / M_star_kg;
    double torque = q * q * std::pow(aspect_ratio, -2.0) * surface_density_kg_m2 * std::pow(a_m, 4.0) * n * n;
    return torque;
  }
};

// 7. Planet Nine Secular Torque & Perihelion Alignment (Batygin & Brown 2016)
class PlanetNineSecularModel {
 public:
  // Secular Perihelion Precession Rate Induced by Planet Nine on a TNO [rad/yr]
  double planet_nine_secular_precession_rad_yr(double a_tno_au, double a_p9_au = 500.0, double m_p9_earth = 10.0) const {
    double m_p9_kg = m_p9_earth * 5.972e24;
    double n_p9 = std::sqrt(G * M_SUN / std::pow(a_p9_au * AU, 3.0));
    double alpha = a_tno_au / a_p9_au;
    double b_3_2 = 1.5 * alpha;  // Laplace coefficient b_{3/2}^{(1)} approximation
    double dvarpi_dt = (m_p9_kg / M_SUN) * n_p9 * alpha * b_3_2;
    return dvarpi_dt * (365.25 * 86400.0);
  }
};

// 8. Laplace-Lagrange Secular Theory & Eigenfrequencies (Bretagnon 1974, Laskar 1989)
class LaplaceLagrangeSecularModel {
 public:
  // Analytical secular eigenfrequency g5 (Jupiter fundamental perihelion frequency) [arcsec/yr]
  double jupiter_secular_g5_arcsec_yr() const {
    // Observed g5 frequency ~ 4.257 arcsec/yr
    return 4.257;
  }

  // Analytical secular eigenfrequency g6 (Saturn fundamental perihelion frequency) [arcsec/yr]
  double saturn_secular_g6_arcsec_yr() const {
    // Observed g6 frequency ~ 28.245 arcsec/yr
    return 28.245;
  }

  // Secular eccentricity oscillation profile e(t) for Jupiter
  double jupiter_eccentricity_at_time_yr(double time_yr) const {
    double g5 = (4.257 / 3600.0) * (M_PI / 180.0);
    double g6 = (28.245 / 3600.0) * (M_PI / 180.0);
    double e_base = 0.044;
    double delta_e = 0.015;
    return e_base + delta_e * std::cos((g6 - g5) * time_yr);
  }
};

// 9. Nice Model 2:1 Jupiter-Saturn Resonance Crossing (Tsiganis et al. 2005)
class NiceModelResonanceCrossing {
 public:
  // Eccentricity kick metric for Uranus/Neptune during 2:1 Jupiter-Saturn resonance crossing
  double ice_giant_eccentricity_kick(double delta_t_myr, double m_planetesimal_belt_earth = 35.0) const {
    double kick_base = 0.12 * (m_planetesimal_belt_earth / 35.0);
    double damping = std::exp(-delta_t_myr / 10.0);
    return kick_base * damping;
  }
};

}  // namespace hot_jupiter

#endif  // HOT_JUPITER_SOLAR_SYSTEM_HPP
