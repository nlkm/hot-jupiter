// C++ Unit Test for Solar System Bodies & Orbital Dynamics Library

#include <cassert>
#include <cmath>
#include <iostream>

#include "solar_system.hpp"

int main() {
  std::cout << "=== Running Solar System Dynamics C++ Tests ===" << std::endl;

  hot_jupiter::MoonTidalDynamicsModel moon_model;
  double P_io = moon_model.io_tidal_heating_power_watts();
  std::cout << "--> Io Tidal Heating Power: " << P_io / 1.0e12 << " TW" << std::endl;
  assert(P_io > 1.0e13 && "Io tidal heating power out of expected range!");

  double rate_recession = moon_model.earth_moon_recession_rate_m_s();
  double cm_per_yr = rate_recession * 100.0 * 365.25 * 86400.0;
  std::cout << "--> Earth-Moon Recession Rate: " << cm_per_yr << " cm/yr" << std::endl;
  assert(std::abs(cm_per_yr - 3.8) < 0.5 && "Lunar recession rate should be ~3.8 cm/yr!");

  hot_jupiter::PlanetaryRingModel ring_model;
  double r_saturn_roche = ring_model.roche_limit_m(6.0268e7, 687.0, 1000.0, true);
  std::cout << "--> Saturn Fluid Roche Limit: " << r_saturn_roche / 1.0e6 << " 10^3 km" << std::endl;
  assert(r_saturn_roche > 1.0e8 && "Saturn Roche limit out of range!");

  hot_jupiter::AsteroidDynamicsModel asteroid_model;
  double a_yark = asteroid_model.yarkovsky_acceleration_m_s2(500.0, 2000.0, 2.5, 30.0);
  std::cout << "--> Yarkovsky Acceleration (500m asteroid at 2.5 AU): " << a_yark << " m/s^2" << std::endl;
  assert(a_yark > 1.0e-15 && "Yarkovsky acceleration should be positive!");
  assert(asteroid_model.in_kirkwood_gap(2.50) && "2.50 AU should be in 3:1 Kirkwood gap!");

  hot_jupiter::CometDynamicsModel comet_model;
  double g_1au = comet_model.marsden_sublimation_g_r(1.0);
  std::cout << "--> Marsden Comet Sublimation g(1 AU): " << g_1au << std::endl;
  assert(g_1au > 0.05 && "Marsden sublimation g(r) at 1 AU out of range!");

  hot_jupiter::RelativisticPrecessionModel gr_model;
  double merc_gr = gr_model.mercury_gr_precession_arcsec_century();
  std::cout << "--> Mercury GR Perihelion Precession: " << merc_gr << " arcsec/century" << std::endl;
  assert(std::abs(merc_gr - 43.0) < 3.0 && "Mercury GR precession should be ~43 arcsec/century!");

  hot_jupiter::PlanetNineSecularModel p9_model;
  double p9_prec = p9_model.planet_nine_secular_precession_rad_yr(250.0);
  std::cout << "--> Planet Nine TNO Precession (250 AU): " << p9_prec << " rad/yr" << std::endl;
  assert(p9_prec > 1.0e-10 && "Planet Nine secular precession should be positive!");

  hot_jupiter::LaplaceLagrangeSecularModel ll_model;
  double g5 = ll_model.jupiter_secular_g5_arcsec_yr();
  double g6 = ll_model.saturn_secular_g6_arcsec_yr();
  std::cout << "--> Secular Eigenfrequencies: g5 = " << g5 << ", g6 = " << g6 << " arcsec/yr" << std::endl;
  assert(std::abs(g5 - 4.257) < 0.01 && "g5 frequency mismatch!");
  assert(std::abs(g6 - 28.245) < 0.01 && "g6 frequency mismatch!");

  hot_jupiter::NiceModelResonanceCrossing nice_model;
  double kick = nice_model.ice_giant_eccentricity_kick(0.0, 35.0);
  std::cout << "--> Nice Model Ice Giant Eccentricity Kick: " << kick << std::endl;
  assert(kick > 0.10 && "Nice Model eccentricity kick out of range!");

  hot_jupiter::SeasonalYarkovskyModel seasonal_model;
  double drift = seasonal_model.seasonal_drift_rate_au_myr(500.0, 2000.0, 2.5, 90.0);
  std::cout << "--> Seasonal Yarkovsky Drift Rate (90 deg obl): " << drift << " AU/Myr" << std::endl;
  assert(drift < 0.0 && "Seasonal Yarkovsky drift rate should be negative!");

  hot_jupiter::SaturnRingLindbladResonanceModel lindblad_model;
  double torque = lindblad_model.lindblad_resonance_torque_nm(1.4e17, 1.3935e8);
  std::cout << "--> Lindblad Ring Torque: " << torque << " N m" << std::endl;
  assert(torque > 1.0e8 && "Lindblad torque should be positive!");

  hot_jupiter::CetoPhorcysBinaryModel ceto_model;
  double ceto_p = ceto_model.orbital_period_days();
  double ceto_rho = ceto_model.system_bulk_density_kg_m3();
  std::cout << "--> Ceto-Phorcys Orbital Period: " << ceto_p << " days, Density: " << ceto_rho << " kg/m^3" << std::endl;
  assert(std::abs(ceto_p - 9.554) < 0.05 && "Ceto orbital period mismatch!");
  assert(std::abs(ceto_rho - 1370.0) < 50.0 && "Ceto density mismatch!");

  hot_jupiter::AltjiraBinaryModel altjira_model;
  double altjira_p = altjira_model.orbital_period_days();
  double altjira_rho = altjira_model.system_bulk_density_kg_m3();
  std::cout << "--> Altjira Orbital Period: " << altjira_p << " days, Density: " << altjira_rho << " kg/m^3" << std::endl;
  assert(std::abs(altjira_p - 139.6) < 2.0 && "Altjira orbital period mismatch!");
  assert(std::abs(altjira_rho - 510.0) < 30.0 && "Altjira density mismatch!");

  hot_jupiter::SilaNunamBinaryModel sila_model;
  double sila_p = sila_model.orbital_period_days();
  double sila_rho = sila_model.system_bulk_density_kg_m3();
  std::cout << "--> Sila-Nunam Orbital Period: " << sila_p << " days, Density: " << sila_rho << " kg/m^3" << std::endl;
  assert(std::abs(sila_p - 12.51) < 0.1 && "Sila-Nunam orbital period mismatch!");
  assert(std::abs(sila_rho - 720.0) < 30.0 && "Sila-Nunam density mismatch!");

  hot_jupiter::TeharonhiawakoBinaryModel teh_model;
  double teh_p = teh_model.orbital_period_days();
  double teh_rho = teh_model.system_bulk_density_kg_m3();
  std::cout << "--> Teharonhiawako Orbital Period: " << teh_p << " days, Density: " << teh_rho << " kg/m^3" << std::endl;
  assert(std::abs(teh_p - 828.7) < 5.0 && "Teharonhiawako orbital period mismatch!");
  assert(std::abs(teh_rho - 620.0) < 30.0 && "Teharonhiawako density mismatch!");

  hot_jupiter::KS38BinaryModel ks38_model;
  double ks38_p = ks38_model.orbital_period_days();
  double ks38_rho = ks38_model.system_bulk_density_kg_m3();
  std::cout << "--> 2000 KS38 Orbital Period: " << ks38_p << " days, Density: " << ks38_rho << " kg/m^3" << std::endl;
  assert(std::abs(ks38_p - 450.0) < 3.0 && "2000 KS38 orbital period mismatch!");
  assert(std::abs(ks38_rho - 375.0) < 30.0 && "2000 KS38 density mismatch!");

  hot_jupiter::OJ67BinaryModel oj67_model;
  double oj67_p = oj67_model.orbital_period_days();
  double oj67_rho = oj67_model.system_bulk_density_kg_m3();
  std::cout << "--> 2000 OJ67 Orbital Period: " << oj67_p << " days, Density: " << oj67_rho << " kg/m^3" << std::endl;
  assert(std::abs(oj67_p - 380.0) < 10.0 && "2000 OJ67 orbital period mismatch!");
  assert(std::abs(oj67_rho - 450.0) < 30.0 && "2000 OJ67 density mismatch!");

  hot_jupiter::EG138BinaryModel eg138_model;
  double eg138_p = eg138_model.orbital_period_days();
  double eg138_rho = eg138_model.system_bulk_density_kg_m3();
  std::cout << "--> 2000 EG138 Orbital Period: " << eg138_p << " days, Density: " << eg138_rho << " kg/m^3" << std::endl;
  assert(std::abs(eg138_p - 360.0) < 45.0 && "2000 EG138 orbital period mismatch!");
  assert(std::abs(eg138_rho - 450.0) < 30.0 && "2000 EG138 density mismatch!");

  hot_jupiter::YN81BinaryModel yn81_model;
  double yn81_p = yn81_model.orbital_period_days();
  double yn81_rho = yn81_model.system_bulk_density_kg_m3();
  std::cout << "--> 2000 YN81 Orbital Period: " << yn81_p << " days, Density: " << yn81_rho << " kg/m^3" << std::endl;
  assert(std::abs(yn81_p - 410.0) < 30.0 && "2000 YN81 orbital period mismatch!");
  assert(std::abs(yn81_rho - 470.0) < 30.0 && "2000 YN81 density mismatch!");

  hot_jupiter::WC19BinaryModel wc19_model;
  double wc19_p = wc19_model.orbital_period_days();
  double wc19_rho = wc19_model.system_bulk_density_kg_m3();
  std::cout << "--> 2002 WC19 Orbital Period: " << wc19_p << " days, Density: " << wc19_rho << " kg/m^3" << std::endl;
  assert(std::abs(wc19_p - 8.40) < 0.1 && "2002 WC19 orbital period mismatch!");
  assert(std::abs(wc19_rho - 638.0) < 30.0 && "2002 WC19 density mismatch!");

  hot_jupiter::KP76BinaryModel kp76_model;
  double kp76_p = kp76_model.orbital_period_days();
  double kp76_rho = kp76_model.system_bulk_density_kg_m3();
  std::cout << "--> 2001 KP76 Orbital Period: " << kp76_p << " days, Density: " << kp76_rho << " kg/m^3" << std::endl;
  assert(std::abs(kp76_p - 240.0) < 30.0 && "2001 KP76 orbital period mismatch!");
  assert(std::abs(kp76_rho - 460.0) < 30.0 && "2001 KP76 density mismatch!");

  hot_jupiter::FB128BinaryModel fb128_model;
  double fb128_p = fb128_model.orbital_period_days();
  double fb128_rho = fb128_model.system_bulk_density_kg_m3();
  std::cout << "--> 2003 FB128 Orbital Period: " << fb128_p << " days, Density: " << fb128_rho << " kg/m^3" << std::endl;
  assert(std::abs(fb128_p - 1660.0) < 10.0 && "2003 FB128 orbital period mismatch!");
  assert(std::abs(fb128_rho - 498.0) < 30.0 && "2003 FB128 density mismatch!");

  hot_jupiter::RN43BinaryModel rn43_model;
  double rn43_p = rn43_model.orbital_period_days();
  double rn43_rho = rn43_model.system_bulk_density_kg_m3();
  std::cout << "--> 2005 RN43 Orbital Period: " << rn43_p << " days, Density: " << rn43_rho << " kg/m^3" << std::endl;
  assert(std::abs(rn43_p - 14.80) < 1.0 && "2005 RN43 orbital period mismatch!");
  assert(std::abs(rn43_rho - 635.0) < 30.0 && "2005 RN43 density mismatch!");

  hot_jupiter::PD149BinaryModel pd149_model;
  double pd149_p = pd149_model.orbital_period_days();
  double pd149_rho = pd149_model.system_bulk_density_kg_m3();
  std::cout << "--> 2002 PD149 Orbital Period: " << pd149_p << " days, Density: " << pd149_rho << " kg/m^3" << std::endl;
  assert(std::abs(pd149_p - 1260.0) < 10.0 && "2002 PD149 orbital period mismatch!");
  assert(std::abs(pd149_rho - 340.0) < 30.0 && "2002 PD149 density mismatch!");

  hot_jupiter::GZ31BinaryModel gz31_model;
  double gz31_p = gz31_model.orbital_period_days();
  double gz31_rho = gz31_model.system_bulk_density_kg_m3();
  std::cout << "--> 2002 GZ31 Orbital Period: " << gz31_p << " days, Density: " << gz31_rho << " kg/m^3" << std::endl;
  assert(std::abs(gz31_p - 1010.0) < 10.0 && "2002 GZ31 orbital period mismatch!");
  assert(std::abs(gz31_rho - 238.0) < 30.0 && "2002 GZ31 density mismatch!");

  hot_jupiter::AZ84BinaryModel az84_model;
  double az84_p = az84_model.orbital_period_days();
  double az84_rho = az84_model.system_bulk_density_kg_m3();
  std::cout << "--> 2003 AZ84 Orbital Period: " << az84_p << " days, Density: " << az84_rho << " kg/m^3" << std::endl;
  assert(std::abs(az84_p - 12.25) < 1.5 && "2003 AZ84 orbital period mismatch!");
  assert(std::abs(az84_rho - 870.0) < 30.0 && "2003 AZ84 density mismatch!");

  hot_jupiter::VT130BinaryModel vt130_model;
  double vt130_p = vt130_model.orbital_period_days();
  double vt130_rho = vt130_model.system_bulk_density_kg_m3();
  std::cout << "--> 2002 VT130 Orbital Period: " << vt130_p << " days, Density: " << vt130_rho << " kg/m^3" << std::endl;
  assert(std::abs(vt130_p - 1060.0) < 10.0 && "2002 VT130 orbital period mismatch!");
  assert(std::abs(vt130_rho - 126.0) < 30.0 && "2002 VT130 density mismatch!");

  hot_jupiter::QY90BinaryModel qy90_model;
  double qy90_p = qy90_model.orbital_period_days();
  double qy90_rho = qy90_model.system_bulk_density_kg_m3();
  std::cout << "--> 2003 QY90 Orbital Period: " << qy90_p << " days, Density: " << qy90_rho << " kg/m^3" << std::endl;
  assert(std::abs(qy90_p - 320.0) < 30.0 && "2003 QY90 orbital period mismatch!");
  assert(std::abs(qy90_rho - 740.0) < 30.0 && "2003 QY90 density mismatch!");

  hot_jupiter::JA132BinaryModel ja132_model;
  double ja132_p = ja132_model.orbital_period_days();
  double ja132_rho = ja132_model.system_bulk_density_kg_m3();
  std::cout << "--> 1999 JA132 Orbital Period: " << ja132_p << " days, Density: " << ja132_rho << " kg/m^3" << std::endl;
  assert(std::abs(ja132_p - 515.0) < 10.0 && "1999 JA132 orbital period mismatch!");
  assert(std::abs(ja132_rho - 224.0) < 50.0 && "1999 JA132 density mismatch!");

  hot_jupiter::FM185BinaryModel fm185_model;
  double fm185_p = fm185_model.orbital_period_days();
  double fm185_rho = fm185_model.system_bulk_density_kg_m3();
  std::cout << "--> 2001 FM185 Orbital Period: " << fm185_p << " days, Density: " << fm185_rho << " kg/m^3" << std::endl;
  assert(std::abs(fm185_p - 310.0) < 10.0 && "2001 FM185 orbital period mismatch!");
  assert(std::abs(fm185_rho - 395.0) < 50.0 && "2001 FM185 density mismatch!");

  hot_jupiter::OJ67TNOBinaryModel oj67_tno_model;
  double oj67_tno_p = oj67_tno_model.orbital_period_days();
  double oj67_tno_rho = oj67_tno_model.system_bulk_density_kg_m3();
  std::cout << "--> 2000 OJ67 TNO Orbital Period: " << oj67_tno_p << " days, Density: " << oj67_tno_rho << " kg/m^3" << std::endl;
  assert(std::abs(oj67_tno_p - 1005.0) < 10.0 && "2000 OJ67 TNO orbital period mismatch!");
  assert(std::abs(oj67_tno_rho - 566.0) < 30.0 && "2000 OJ67 TNO density mismatch!");

  hot_jupiter::QuaoarWeywotBinaryModel quaoar_model;
  double quaoar_p = quaoar_model.orbital_period_days();
  double quaoar_rho = quaoar_model.system_bulk_density_kg_m3();
  std::cout << "--> Quaoar / Weywot Orbital Period: " << quaoar_p << " days, Density: " << quaoar_rho << " kg/m^3" << std::endl;
  assert(std::abs(quaoar_p - 12.438) < 0.5 && "Quaoar / Weywot orbital period mismatch!");
  assert(std::abs(quaoar_rho - 1640.0) < 50.0 && "Quaoar / Weywot density mismatch!");

  hot_jupiter::UX10BinaryModel ux10_model;
  double ux10_p = ux10_model.orbital_period_days();
  double ux10_rho = ux10_model.system_bulk_density_kg_m3();
  std::cout << "--> 2004 UX10 Orbital Period: " << ux10_p << " days, Density: " << ux10_rho << " kg/m^3" << std::endl;
  assert(std::abs(ux10_p - 122.0) < 5.0 && "2004 UX10 orbital period mismatch!");
  assert(std::abs(ux10_rho - 1164.0) < 50.0 && "2004 UX10 density mismatch!");

  hot_jupiter::QY297BinaryModel qy297_model;
  double qy297_p = qy297_model.orbital_period_days();
  double qy297_rho = qy297_model.system_bulk_density_kg_m3();
  std::cout << "--> 2001 QY297 Orbital Period: " << qy297_p << " days, Density: " << qy297_rho << " kg/m^3" << std::endl;
  assert(std::abs(qy297_p - 138.1) < 5.0 && "2001 QY297 orbital period mismatch!");
  assert(std::abs(qy297_rho - 471.0) < 30.0 && "2001 QY297 density mismatch!");

  hot_jupiter::CA101BinaryModel ca101_model;
  double ca101_p = ca101_model.orbital_period_days();
  double ca101_rho = ca101_model.system_bulk_density_kg_m3();
  std::cout << "--> 2000 CA101 Orbital Period: " << ca101_p << " days, Density: " << ca101_rho << " kg/m^3" << std::endl;
  assert(std::abs(ca101_p - 345.0) < 10.0 && "2000 CA101 orbital period mismatch!");
  assert(std::abs(ca101_rho - 613.0) < 40.0 && "2000 CA101 density mismatch!");

  hot_jupiter::UQ18BinaryModel uq18_model;
  double uq18_p = uq18_model.orbital_period_days();
  double uq18_rho = uq18_model.system_bulk_density_kg_m3();
  std::cout << "--> 2001 UQ18 Orbital Period: " << uq18_p << " days, Density: " << uq18_rho << " kg/m^3" << std::endl;
  assert(std::abs(uq18_p - 165.0) < 5.0 && "2001 UQ18 orbital period mismatch!");
  assert(std::abs(uq18_rho - 398.0) < 30.0 && "2001 UQ18 density mismatch!");

  hot_jupiter::SaturnRingResonanceAnalysisModel saturn_ring_model;
  double r_mimas21 = saturn_ring_model.inner_lindblad_resonance_km(185539.0, 2, 1);
  double r_janus76 = saturn_ring_model.inner_lindblad_resonance_km(151460.0, 7, 6);
  double r_fring = saturn_ring_model.shepherd_torque_balance_km();
  std::cout << "--> Saturn Ring Resonances: Mimas 2:1 = " << r_mimas21 << " km, Janus 7:6 = " << r_janus76 << " km, F-Ring = " << r_fring << " km" << std::endl;
  assert(std::abs(r_mimas21 - 117580.0) < 1000.0 && "Mimas 2:1 ILR mismatch!");
  assert(std::abs(r_janus76 - 136770.0) < 500.0 && "Janus 7:6 ILR mismatch!");
  assert(std::abs(r_fring - 140220.0) < 500.0 && "F-ring shepherd torque balance mismatch!");

  hot_jupiter::EnceladusTidalAnalysisModel enceladus_model;
  double p_diss_gw = enceladus_model.tidal_dissipation_power_gw();
  double q_cond_gw = enceladus_model.conductive_heat_flux_gw(20.0);
  std::cout << "--> Enceladus Tidal Analysis: Dissipation Power = " << p_diss_gw << " GW, Conductive Heat Loss = " << q_cond_gw << " GW" << std::endl;
  assert(std::abs(p_diss_gw - 15.8) < 1.0 && "Enceladus tidal dissipation power mismatch!");
  assert(std::abs(q_cond_gw - 29.3) < 2.0 && "Enceladus conductive heat loss mismatch!");

  hot_jupiter::IoLaplaceTidalAnalysisModel io_model;
  double io_power_tw = io_model.io_tidal_power_tw();
  double io_flux_w_m2 = io_model.surface_heat_flux_w_m2(io_power_tw);
  std::cout << "--> Io Laplace Tidal Analysis: Power = " << io_power_tw << " TW, Heat Flux = " << io_flux_w_m2 << " W/m^2" << std::endl;
  assert(std::abs(io_power_tw - 105.0) < 1.0 && "Io tidal dissipation power mismatch!");
  assert(std::abs(io_flux_w_m2 - 2.52) < 0.1 && "Io surface heat flux mismatch!");

  hot_jupiter::JupiterJunoGravityAnalysisModel jupiter_gravity;
  double j2_val = jupiter_gravity.j2_harmonic_1e6();
  double j4_val = jupiter_gravity.j4_harmonic_1e6();
  double j6_val = jupiter_gravity.j6_harmonic_1e6();
  std::cout << "--> Jupiter Juno Gravity Analysis: J2 = " << j2_val << ", J4 = " << j4_val << ", J6 = " << j6_val << std::endl;
  assert(std::abs(j2_val - 14696.57) < 50.0 && "Jupiter J2 harmonic mismatch!");
  assert(std::abs(j4_val - (-586.61)) < 5.0 && "Jupiter J4 harmonic mismatch!");
  assert(std::abs(j6_val - 34.20) < 1.0 && "Jupiter J6 harmonic mismatch!");

  hot_jupiter::SaturnCassiniGravityAnalysisModel saturn_gravity;
  double sat_j2 = saturn_gravity.j2_harmonic_1e6();
  double sat_j4 = saturn_gravity.j4_harmonic_1e6();
  double sat_j6 = saturn_gravity.j6_harmonic_1e6();
  std::cout << "--> Saturn Cassini Gravity Analysis: J2 = " << sat_j2 << ", J4 = " << sat_j4 << ", J6 = " << sat_j6 << std::endl;
  assert(std::abs(sat_j2 - 16290.71) < 50.0 && "Saturn J2 harmonic mismatch!");
  assert(std::abs(sat_j4 - (-935.83)) < 5.0 && "Saturn J4 harmonic mismatch!");
  assert(std::abs(sat_j6 - 86.14) < 1.0 && "Saturn J6 harmonic mismatch!");

  hot_jupiter::MercuryRelativisticPrecessionModel mercury_obs_model;
  double merc_obs_gr_val = mercury_obs_model.gr_precession_arcsec_century();
  double merc_obs_j2_val = mercury_obs_model.j2_sun_precession_arcsec_century();
  std::cout << "--> Mercury Relativistic Precession: GR = " << merc_obs_gr_val << ", Solar J2 = " << merc_obs_j2_val << std::endl;
  assert(std::abs(merc_obs_gr_val - 42.982) < 0.1 && "Mercury GR precession mismatch!");
  assert(std::abs(merc_obs_j2_val - 0.0286) < 0.01 && "Mercury Solar J2 precession mismatch!");

  hot_jupiter::BennuYarkovskyModel bennu_test_model;
  double bennu_drift = bennu_test_model.yarkovsky_drift_m_yr();
  std::cout << "--> Bennu Yarkovsky Drift: Rate = " << bennu_drift << " m/yr" << std::endl;
  assert(std::abs(bennu_drift - (-284.0)) < 5.0 && "Bennu Yarkovsky drift rate mismatch!");

  hot_jupiter::RyuguYarkovskyModel ryugu_test_model;
  double ryugu_drift = ryugu_test_model.yarkovsky_drift_m_yr();
  std::cout << "--> Ryugu Yarkovsky Drift: Rate = " << ryugu_drift << " m/yr" << std::endl;
  assert(std::abs(ryugu_drift - (-215.0)) < 5.0 && "Ryugu Yarkovsky drift rate mismatch!");

  hot_jupiter::Comet67POutgassingModel comet_test_model;
  double a1_val = comet_test_model.radial_acceleration_AU_day2(1.0);
  std::cout << "--> Comet 67P Outgassing: A1 * g(1 AU) = " << a1_val << " AU/day^2" << std::endl;
  assert(std::abs(a1_val - 3.25e-8) < 1.0e-9 && "Comet 67P outgassing acceleration mismatch!");

  hot_jupiter::PlanetNineSecularModel p9_test_model;
  double p9_angle = p9_test_model.secular_perihelion_clustering_deg();
  std::cout << "--> Planet Nine Secular: Angle = " << p9_angle << " deg" << std::endl;
  assert(std::abs(p9_angle - 180.0) < 5.0 && "Planet Nine secular perihelion clustering mismatch!");

  hot_jupiter::PlutoCharonMutualModel pc_test_model;
  double pc_period = pc_test_model.orbital_period_days();
  std::cout << "--> Pluto-Charon Mutual: Period = " << pc_period << " days" << std::endl;
  assert(std::abs(pc_period - 6.38723) < 0.001 && "Pluto-Charon period mismatch!");

  hot_jupiter::ErisDysnomiaModel ed_test_model;
  double ed_period = ed_test_model.orbital_period_days();
  std::cout << "--> Eris-Dysnomia Mutual: Period = " << ed_period << " days" << std::endl;
  assert(std::abs(ed_period - 15.7232) < 0.01 && "Eris-Dysnomia period mismatch!");

  hot_jupiter::HaumeaEllipsoidRingModel h_test_model;
  double r_ring = h_test_model.ring_3to1_resonance_radius_km();
  std::cout << "--> Haumea Ellipsoid & Ring: Ring Radius = " << r_ring << " km" << std::endl;
  assert(std::abs(r_ring - 2287.3) < 10.0 && "Haumea ring radius mismatch!");

  hot_jupiter::HD209458bPhotoevaporationModel photo_test_model;
  double photo_mdot = photo_test_model.mass_loss_rate_g_s();
  std::cout << "--> HD 209458b Photoevaporation: Mass Loss = " << photo_mdot << " g/s" << std::endl;
  assert(std::abs(photo_mdot - 5.0e10) < 1.0e10 && "HD 209458b mass loss rate mismatch!");

  hot_jupiter::HD189733bMassLossModel hd189_test_model;
  double hd189_mdot_flare = hd189_test_model.flare_mass_loss_rate_g_s();
  std::cout << "--> HD 189733b Flare Mass Loss = " << hd189_mdot_flare << " g/s" << std::endl;
  assert(std::abs(hd189_mdot_flare - 4.5e11) < 1.0e11 && "HD 189733b flare mass loss rate mismatch!");

  hot_jupiter::GJ436bHydrogenCloudModel gj436_test_model;
  double gj436_mdot = gj436_test_model.mass_loss_rate_g_s();
  std::cout << "--> GJ 436b Cloud Mass Loss = " << gj436_mdot << " g/s" << std::endl;
  assert(std::abs(gj436_mdot - 2.2e10) < 5.0e9 && "GJ 436b cloud mass loss rate mismatch!");

  hot_jupiter::WASP12bTidalDecayModel wasp12_test_model;
  double wasp12_pdot = wasp12_test_model.period_decay_rate_ms_yr();
  std::cout << "--> WASP-12b Tidal Decay Rate = " << wasp12_pdot << " ms/year" << std::endl;
  assert(std::abs(wasp12_pdot - (-29.0)) < 2.0 && "WASP-12b period decay rate mismatch!");

  hot_jupiter::WASP43bTidalCircularizationModel wasp43_test_model;
  double wasp43_tau_e = wasp43_test_model.circularization_timescale_myr();
  std::cout << "--> WASP-43b Circularization Timescale = " << wasp43_tau_e << " Myr" << std::endl;
  assert(std::abs(wasp43_tau_e - 7.5) < 1.0 && "WASP-43b circularization timescale mismatch!");

  hot_jupiter::TRAPPIST1ResonantChainModel trap_test_model;
  double trap_ttv = trap_test_model.ttv_chopping_amplitude_minutes();
  std::cout << "--> TRAPPIST-1d TTV Chopping Amplitude = " << trap_ttv << " minutes" << std::endl;
  assert(std::abs(trap_ttv - 38.5) < 2.0 && "TRAPPIST-1 TTV chopping amplitude mismatch!");

  hot_jupiter::Kepler223ResonantChainModel kep_test_model;
  double kep_ttv = kep_test_model.ttv_chopping_amplitude_minutes();
  std::cout << "--> Kepler-223b TTV Chopping Amplitude = " << kep_ttv << " minutes" << std::endl;
  assert(std::abs(kep_ttv - 14.2) < 1.0 && "Kepler-223 TTV chopping amplitude mismatch!");

  hot_jupiter::KELT9bUltraHotThermosphereModel kelt_test_model;
  double kelt_depth = kelt_test_model.halpha_excess_depth_percent();
  std::cout << "--> KELT-9b H-alpha Excess Depth = " << kelt_depth << " %" << std::endl;
  assert(std::abs(kelt_depth - 1.15) < 0.2 && "KELT-9b H-alpha absorption depth mismatch!");

  hot_jupiter::HATP11bHeliumEscapeModel hat_test_model;
  double hat_depth = hat_test_model.hei_10830_excess_depth_percent();
  std::cout << "--> HAT-P-11b He I 10830A Excess Depth = " << hat_depth << " %" << std::endl;
  assert(std::abs(hat_depth - 1.08) < 0.2 && "HAT-P-11b helium absorption depth mismatch!");

  hot_jupiter::TOI560bSubNeptuneEscapeModel toi_test_model;
  double toi_depth = toi_test_model.hei_10830_excess_depth_percent();
  std::cout << "--> TOI-560b He I 10830A Excess Depth = " << toi_depth << " %" << std::endl;
  assert(std::abs(toi_depth - 0.68) < 0.2 && "TOI-560b helium absorption depth mismatch!");

  hot_jupiter::WASP121bDeformabilityRLOFModel wasp_test_model;
  double fe_ii_depth = wasp_test_model.nuv_fe_ii_excess_depth_percent();
  std::cout << "--> WASP-121b Fe II NUV Excess Depth = " << fe_ii_depth << " %" << std::endl;
  assert(std::abs(fe_ii_depth - 0.85) < 0.2 && "WASP-121b Fe II absorption depth mismatch!");

  hot_jupiter::LTT9779bUltraHotNeptuneModel ltt_test_model;
  double ltt_albedo = ltt_test_model.geometric_albedo();
  std::cout << "--> LTT 9779b Geometric Albedo A_g = " << ltt_albedo << std::endl;
  assert(std::abs(ltt_albedo - 0.80) < 0.1 && "LTT 9779b albedo mismatch!");

  hot_jupiter::PlanetNinePositionPredictionEngine p9_pred_test_model;
  double p9_ra = p9_pred_test_model.predicted_ra_deg();
  double p9_mu = p9_pred_test_model.proper_motion_arcsec_yr();
  std::cout << "--> Planet Nine Position Prediction: Peak RA = " << p9_ra << " deg, Proper Motion = " << p9_mu << " arcsec/yr" << std::endl;
  assert(std::abs(p9_ra - 55.55) < 1.0 && "Planet Nine RA mismatch!");
  assert(std::abs(p9_mu - 109.3) < 5.0 && "Planet Nine proper motion mismatch!");

  hot_jupiter::OjakangasStevenson1989EnceladusModel ojakangas_model;
  double ojakangas_p_tide = ojakangas_model.tidal_dissipation_power_gw(0.0107);
  double ojakangas_q_cond = ojakangas_model.conductive_heat_loss_gw(20.0);
  double ojakangas_omega_m_nom = ojakangas_model.maxwell_relaxation_frequency_rad_s(1.0e13);
  double ojakangas_im_k2_nom = ojakangas_model.dissipation_love_number_im_k2(ojakangas_omega_m_nom);
  std::cout << "--> Ojakangas & Stevenson (1989) Ice Shell: Tidal Power = " << ojakangas_p_tide
            << " GW, Cond Heat Loss (20km) = " << ojakangas_q_cond
            << " GW, Maxwell Freq = " << ojakangas_omega_m_nom
            << " rad/s, Im(k2) = " << ojakangas_im_k2_nom << std::endl;
  assert(std::abs(ojakangas_p_tide - 15.88) < 0.5 && "Ojakangas & Stevenson tidal power mismatch!");
  assert(std::abs(ojakangas_q_cond - 29.27) < 1.0 && "Ojakangas & Stevenson conductive loss mismatch!");
  assert(ojakangas_im_k2_nom > 0.001 && "Ojakangas & Stevenson Im(k2) mismatch!");

  hot_jupiter::EuropaViscoelasticTidalModel europa_model;
  double europa_p_tw = europa_model.total_tidal_power_tw(20000.0, 1.0);
  double europa_flux_mw_m2 = europa_model.surface_heat_flux_mw_m2(20000.0, 1.0);
  double europa_k2_over_q = europa_model.effective_k2_over_q(20000.0, 1.0);
  std::cout << "--> Europa Viscoelastic Tidal Heating: Total Power = " << europa_p_tw
            << " TW, Surface Flux = " << europa_flux_mw_m2
            << " mW/m^2, Im(k2) = " << europa_k2_over_q << std::endl;
  assert(europa_p_tw > 1.0 && europa_p_tw < 5.0 && "Europa tidal power out of expected range!");
  assert(europa_flux_mw_m2 > 30.0 && europa_flux_mw_m2 < 150.0 && "Europa surface heat flux out of range!");
  assert(europa_k2_over_q > 0.001 && europa_k2_over_q < 0.010 && "Europa Im(k2) out of range!");

  hot_jupiter::NiceModelResonantCrossingAnalyticalModel nice_bm_model;
  double w1 = nice_bm_model.resonance_frequency_width_1(0.048);
  double w2 = nice_bm_model.resonance_frequency_width_2(0.054);
  double chirikov_s = nice_bm_model.chirikov_overlap_parameter(0.048, 0.054);
  double e_s_crit = nice_bm_model.critical_saturn_eccentricity_overlap(0.048);
  std::cout << "--> Batygin & Morbidelli (2011) 2:1 Crossing: w1 = " << w1
            << " rad/s, w2 = " << w2 << " rad/s, Chirikov S = " << chirikov_s
            << ", e_S critical = " << e_s_crit << std::endl;
  assert(w1 > 0.0 && w2 > 0.0 && "Resonance widths should be positive!");
  assert(chirikov_s > 1.0 && "Jupiter-Saturn 2:1 crossing should be in Chirikov overlap regime (S > 1)!");
  assert(e_s_crit >= 0.0 && "Critical Saturn eccentricity should be non-negative!");

  hot_jupiter::Gomes2005LateHeavyBombardmentModel lhb_model;
  double t_inst = lhb_model.instability_delay_myr(1.5);
  double r_res = lhb_model.resonance_crossing_semi_major_axis_ratio();
  double p_rat = lhb_model.period_ratio(8.18, 5.45);
  double fg_earth = lhb_model.gravitational_focusing_factor(11.186, 15.0);
  double fg_moon = lhb_model.gravitational_focusing_factor(2.380, 15.0);
  assert(fg_earth > fg_moon && "Earth gravitational focusing factor must exceed Moon!");
  double mass_ratio_earth_moon = lhb_model.relative_impact_mass_ratio_vs_moon(6.371e6, 11.186, 15.0);
  double m_moon_tot = lhb_model.cumulative_mass_delivered_kg("Moon", 1100.0);
  double basins_tot = lhb_model.cumulative_lunar_basins(1100.0);

  std::cout << "--> Gomes et al. (2005) LHB: Delay = " << t_inst
            << " Myr, Res Ratio = " << r_res
            << ", Init Period Ratio = " << p_rat
            << ", Earth/Moon Impact Mass Ratio = " << mass_ratio_earth_moon
            << ", Moon Total Mass Delivered = " << m_moon_tot / 1.0e18 << " x 10^18 kg"
            << ", Basins Formed = " << basins_tot << std::endl;

  assert(t_inst > 600.0 && t_inst < 1000.0 && "LHB instability delay out of expected range!");
  assert(std::abs(r_res - 1.5874) < 0.001 && "2:1 resonance semi-major axis ratio mismatch!");
  assert(mass_ratio_earth_moon > 18.0 && mass_ratio_earth_moon < 25.0 && "Earth-to-Moon impact mass ratio out of range!");
  assert(m_moon_tot > 5.0e18 && m_moon_tot < 8.0e18 && "Delivered lunar mass out of expected LHB range!");
  hot_jupiter::Morbidelli2010TerrestrialAccretionModel terr_model;
  double sig_mmsn_1au = terr_model.surface_density_mmsn(1.0);
  double sig_gt_08au = terr_model.surface_density_grand_tack(0.8);
  assert(sig_gt_08au > 0.0 && "Grand tack surface density at 0.8 AU must be positive!");
  double m_iso_1au = terr_model.isolation_mass_mearth(1.0, sig_mmsn_1au);
  auto gt_res = terr_model.simulate_terrestrial_accretion(
      hot_jupiter::Morbidelli2010TerrestrialAccretionModel::DiskModelType::GRAND_TACK, 42);
  auto mmsn_res = terr_model.simulate_terrestrial_accretion(
      hot_jupiter::Morbidelli2010TerrestrialAccretionModel::DiskModelType::CLASSICAL_MMSN, 42);

  std::cout << "--> Morbidelli et al. (2010, 2012) Terrestrial Accretion: Grand Tack M_Mars = "
            << gt_res.mars_mass_mearth << " M_E, M_Earth = " << gt_res.earth_mass_mearth
            << " M_E, Water = " << gt_res.earth_water_oceans << " oceans, AMD = " << gt_res.amd
            << ", RMC = " << gt_res.rmc << ", R^2 = " << gt_res.r_squared_architecture << std::endl;

  assert(sig_mmsn_1au > 0.25 && sig_mmsn_1au < 0.50 && "MMSN surface density at 1 AU mismatch!");
  assert(m_iso_1au > 0.03 && m_iso_1au < 0.20 && "Isolation mass at 1 AU out of range!");
  assert(gt_res.mars_mass_mearth < 0.20 && "Grand Tack should produce small Mars!");
  assert(mmsn_res.mars_mass_mearth > 0.60 && "Classical MMSN should suffer from Mars Problem!");
  assert(gt_res.earth_water_oceans >= 1.0 && "Grand Tack should deliver water to Earth!");
  assert(gt_res.amd < 0.0035 && "Grand Tack should reproduce low AMD!");
  assert(gt_res.r_squared_architecture >= 0.98 && "Grand Tack architecture match R^2 should exceed 0.98!");

  // Paper #231: Brasser et al. (2012) Trojan Capture Model Verification
  hot_jupiter::Brasser2012TrojanCaptureModel trojan_model;
  double p_lib_j = trojan_model.trojan_libration_period_yr();
  double p_cap_in = trojan_model.capture_efficiency(1.0, 0.06, 35.0, true);
  double m_trojan = trojan_model.captured_trojan_mass_earth(1.0, 35.0, 0.06, 0.35, true);
  double r_asym = trojan_model.l4_l5_asymmetry_ratio(1.0, 0.04, true);
  double sat_surv_4gyr = trojan_model.saturn_trojan_survival_fraction(4.0, 1.0);
  double p_lib_s = trojan_model.saturn_trojan_libration_period_yr();

  std::cout << "--> Brasser et al. (2012) Trojan Capture: P_lib(Jup) = " << p_lib_j
            << " yr, P_cap = " << p_cap_in * 100.0 << "%, M_Trojan = " << m_trojan
            << " M_E, L4/L5 Ratio = " << r_asym << ", Saturn Trojan 4-Gyr Surv = " << sat_surv_4gyr << std::endl;

  assert(std::abs(p_lib_j - 147.9) < 2.0 && "Jupiter Trojan libration period should be ~147.9 yr!");
  assert(std::abs(p_lib_s - 675.3) < 5.0 && "Saturn Trojan libration period should be ~675.3 yr!");
  assert(p_cap_in > 1.0e-4 && p_cap_in < 5.0e-4 && "Capture efficiency should be in nominal range!");
  assert(m_trojan > 1.0e-4 && m_trojan < 1.0e-2 && "Captured Trojan mass should be in expected disk-scaled range!");
  // Paper #256: Morbidelli et al. (2008) Dynamical Evolution Model Verification
  hot_jupiter::Morbidelli2008PlanetaryEvolutionModel morb_model;
  double mu_j = morb_model.M_JUPITER_KG / morb_model.M_SUN_KG;
  double e_crit_21 = morb_model.critical_eccentricity(2, 1, mu_j);
  double p_cap_low = morb_model.adiabatic_capture_probability(0.01, e_crit_21);
  double p_cap_high = morb_model.adiabatic_capture_probability(0.20, e_crit_21);
  double s_chirikov = morb_model.chirikov_overlap_parameter(2.8, 0.15, 5.2, mu_j);
  double d_a_diff = morb_model.semi_major_axis_diffusion_coefficient_au2_yr(2.8, 0.15, 5.2, mu_j);
  double t_inst_morb = morb_model.instability_timescale_yr(5.0);

  std::cout << "--> Morbidelli et al. (2008) Planetary Dynamics: e_crit(2:1) = " << e_crit_21
            << ", P_cap(e=0.01) = " << p_cap_low * 100.0 << "%, P_cap(e=0.20) = " << p_cap_high * 100.0
            << "%, Chirikov S = " << s_chirikov << ", D_a = " << d_a_diff
            << " AU^2/yr, T_inst(5 R_H) = " << t_inst_morb << " yr" << std::endl;

  assert(e_crit_21 > 0.05 && e_crit_21 < 0.20 && "Critical eccentricity for 2:1 MMR out of range!");
  assert(std::abs(p_cap_low - 1.0) < 1.0e-5 && "Adiabatic capture probability for e0 <= e_crit must be exactly 100%!");
  assert(p_cap_high > 0.10 && p_cap_high < 0.90 && "Capture probability for e0 > e_crit should be in expected range!");
  assert(s_chirikov > 0.10 && "Chirikov parameter should be positive!");
  assert(d_a_diff > 0.0 && "Diffusion coefficient must be positive!");
  assert(t_inst_morb > 1.0e3 && t_inst_morb < 1.0e6 && "Instability timescale at 5 Hill radii should be ~ 10^4 - 10^5 yr!");

  // Paper #250: Shankman et al. (2017) OSSOS High-q TNO Model Verification
  hot_jupiter::Shankman2017OSSOSModel ossos_model;
  auto ossos_metrics = ossos_model.evaluate_validation_metrics();
  auto ossos_cat = ossos_model.get_ossos_characterized_sample();
  double m_r_test = ossos_model.apparent_magnitude(6.42, 41.0);
  double eta_test = ossos_model.detection_efficiency(m_r_test);
  double rate_test = ossos_model.rate_of_motion_arcsec_hr(41.0);
  double q_pdf_val = ossos_model.perihelion_pdf(40.0);
  double varpi_pdf_val = ossos_model.directional_bias_varpi_pdf(253.3);

  std::cout << "--> Shankman et al. (2017) OSSOS: m_r(GP136) = " << m_r_test
            << ", eta = " << eta_test << ", rate = " << rate_test
            << " \"/hr, q_pdf(40AU) = " << q_pdf_val
            << ", varpi_pdf(253.3) = " << varpi_pdf_val
            << ", Mean R^2 = " << ossos_metrics.mean_r_squared
            << ", Kuiper p-val = " << ossos_metrics.kuiper_p_val_uniform << std::endl;

  assert(ossos_cat.size() == 8 && "OSSOS characterized sample should have 8 objects!");
  assert(m_r_test > 22.0 && m_r_test < 25.0 && "Apparent magnitude for GP136 out of expected range!");
  assert(eta_test > 0.50 && "Detection efficiency for GP136 should be high!");
  assert(rate_test > 1.0 && rate_test < 5.0 && "Rate of motion should be ~2-4 arcsec/hr!");
  assert(ossos_metrics.mean_r_squared >= 0.98 && "OSSOS validation mean R^2 must be >= 0.98!");
  assert(ossos_metrics.kuiper_p_val_uniform > 0.05 && "Kuiper test must show uniform population consistent (p > 0.05)!");

  // Observational Papers #31-#35 Models Verification
  hot_jupiter::TitanMethaneAtmosphereModel titan_model;
  assert(std::abs(titan_model.surface_pressure_bar() - 1.47) < 0.05);
  assert(std::abs(titan_model.superrotation_jet_speed_m_s() - 120.0) < 5.0);

  hot_jupiter::EnceladusPlumeHydrothermalModel enc_model;
  assert(std::abs(enc_model.south_polar_heat_power_gw() - 5.8) < 0.5);
  assert(std::abs(enc_model.plume_mass_loss_kg_s() - 200.0) < 10.0);

  hot_jupiter::TOI849bStrippedCoreModel toi_model;
  assert(std::abs(toi_model.planet_mass_mearth() - 39.1) < 1.0);
  assert(std::abs(toi_model.bulk_density_g_cm3() - 5.50) < 0.2);

  hot_jupiter::ProximaCentauribFlareHabitabilityModel prox_model;
  assert(std::abs(prox_model.semimajor_axis_au() - 0.0485) < 0.005);
  assert(std::abs(prox_model.stellar_flux_relative() - 0.65) < 0.05);

  hot_jupiter::TritonRetrogradeCaptureModel tri_model;
  assert(std::abs(tri_model.retrograde_inclination_deg() - 156.8) < 1.0);
  assert(std::abs(tri_model.circularization_timescale_myr() - 100.0) < 10.0);

  // Observational Papers #36-#40 Models Verification
  hot_jupiter::K218bHyceanAtmosphereModel k218_model;
  assert(std::abs(k218_model.planet_mass_mearth() - 8.63) < 0.1);
  assert(std::abs(k218_model.methane_volume_mixing_ratio() - 0.01) < 0.005);

  hot_jupiter::EnceladusCDASaltFractionationModel cda_model;
  assert(std::abs(cda_model.sodium_salt_mass_fraction() - 0.015) < 0.005);
  assert(std::abs(cda_model.dust_mass_production_rate_kg_s() - 5.0) < 0.5);

  hot_jupiter::WASP76bIronRainModel wasp76_model;
  assert(std::abs(wasp76_model.dayside_temp_k() - 2500.0) < 50.0);
  assert(std::abs(wasp76_model.evening_terminator_fe_absorption_percent() - 0.45) < 0.05);

  hot_jupiter::Kepler11CompactResonantModel k11_model;
  assert(k11_model.number_of_planets() == 6);
  assert(std::abs(k11_model.ttv_amplitude_minutes() - 24.5) < 1.0);

  hot_jupiter::BorisovInterstellarCometModel borisov_model;
  assert(std::abs(borisov_model.orbital_eccentricity() - 3.36) < 0.1);
  assert(std::abs(borisov_model.co_to_water_ratio() - 1.45) < 0.1);

  // Observational Papers #41-#45 Models Verification
  hot_jupiter::Trappist1eHabitabilityAtmosphereModel trap_model;
  assert(std::abs(trap_model.planet_mass_mearth() - 0.692) < 0.05);
  assert(std::abs(trap_model.incident_flux_relative() - 0.662) < 0.05);

  hot_jupiter::NeptuneGreatDarkSpotModel neptune_spot_model;
  assert(std::abs(neptune_spot_model.zonal_wind_speed_m_s() - (-400.0)) < 10.0);
  assert(std::abs(neptune_spot_model.vortex_drift_speed_m_s() - 15.0) < 2.0);

  hot_jupiter::BennuParticleEjectionModel bennu_ej_model;
  assert(std::abs(bennu_ej_model.particle_ejection_velocity_m_s() - 0.50) < 0.05);
  assert(std::abs(bennu_ej_model.mean_particle_radius_cm() - 1.5) < 0.2);

  hot_jupiter::LHS3844bBareRockModel lhs_model;
  assert(std::abs(lhs_model.dayside_temp_k() - 1040.0) < 20.0);
  assert(std::abs(lhs_model.heat_redistribution_efficiency() - 0.0) < 0.01);

  hot_jupiter::SaturnRingSpokesModel spokes_model;
  assert(std::abs(spokes_model.dust_grain_radius_um() - 0.60) < 0.05);
  assert(std::abs(spokes_model.electrostatic_potential_volts() - (-15.0)) < 1.0);

  // Observational Papers #46-#50 Models Verification
  hot_jupiter::GJ1214bAerosolHazeModel gj1214_model;
  assert(std::abs(gj1214_model.planet_mass_mearth() - 8.17) < 0.1);
  assert(std::abs(gj1214_model.metallicity_solar_factor() - 500.0) < 10.0);

  hot_jupiter::CeresAhunaMonsCryovolcanismModel ahuna_model;
  assert(std::abs(ahuna_model.dome_height_km() - 4.0) < 0.2);
  assert(std::abs(ahuna_model.sodium_carbonate_mass_fraction() - 0.20) < 0.02);

  hot_jupiter::PlutoSputnikPlanitiaConvectionModel pluto_sput_model;
  assert(std::abs(pluto_sput_model.cell_diameter_km() - 30.0) < 2.0);
  assert(std::abs(pluto_sput_model.nitrogen_ice_thickness_km() - 6.0) < 0.5);

  hot_jupiter::WASP107bPuffyNeptuneModel wasp107_model;
  assert(std::abs(wasp107_model.planet_mass_mearth() - 30.5) < 0.5);
  assert(std::abs(wasp107_model.bulk_density_g_cm3() - 0.13) < 0.02);

  hot_jupiter::CharonTectonicFreezingModel charon_model;
  assert(std::abs(charon_model.volumetric_expansion_fraction() - 0.07) < 0.01);
  assert(std::abs(charon_model.canyon_chasma_depth_km() - 8.0) < 0.5);

  std::cout << "✅ All Solar System Dynamics C++ Tests PASSED!" << std::endl;
  return 0;
}






