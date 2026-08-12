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

  std::cout << "✅ All Solar System Dynamics C++ Tests PASSED!" << std::endl;
  return 0;
}
