// ============================================================================
// Paper Replication #209: Spencer et al. (2006) Science 311, 1401-1405
// "Cassini Encounters Enceladus: South Polar Terrain Heat Flow and Thermal Radiation Budget"
//
// First-principles C++ Solver for Enceladus South Polar Terrain (SPT) Heat Flow,
// Tiger Stripe Cryovolcanic Fissure Emission, and Viscoelastic Tidal Budget.
// ============================================================================

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "solar_system.hpp"

struct LatGridPoint {
  double latitude_deg;
  double solar_flux_absorbed_w_m2;
  double t_passive_k;
  double flux_passive_w_m2;
  double t_cirs_obs_k;
  double flux_cirs_obs_w_m2;
  double q_endogenic_w_m2;
  double q_endogenic_mw_m2;
};

struct AreaSweepPoint {
  double area_stripes_km2;
  double power_120k_gw;
  double power_135k_gw;
  double power_140k_gw;
  double power_145k_gw;
  double power_155k_gw;
};

int main(int argc, char** argv) {
  std::cout << "====================================================================\n";
  std::cout << " Spencer et al. (2006) Cassini CIRS Enceladus Heat Flow Solver (#209)\n";
  std::cout << "====================================================================\n\n";

  hot_jupiter::Spencer2006EnceladusHeatFlowModel model;

  // 1. Latitude Grid Evaluation (-90 deg S to +90 deg N)
  std::vector<LatGridPoint> lat_profile;
  const double subsolar_lat = -22.8; // southern summer encounter

  for (double lat = -90.0; lat <= 90.0; lat += 1.0) {
    LatGridPoint pt;
    pt.latitude_deg = lat;
    pt.solar_flux_absorbed_w_m2 = model.absorbed_solar_flux_w_m2(lat, subsolar_lat);
    pt.t_passive_k = model.passive_equilibrium_temp_k(lat, subsolar_lat);
    pt.flux_passive_w_m2 = model.passive_emitted_flux_w_m2(lat, subsolar_lat);
    pt.t_cirs_obs_k = model.cirs_observed_temp_k(lat, subsolar_lat);
    pt.flux_cirs_obs_w_m2 = model.cirs_observed_flux_w_m2(lat, subsolar_lat);
    pt.q_endogenic_w_m2 = model.endogenic_heat_flux_w_m2(lat, subsolar_lat);
    pt.q_endogenic_mw_m2 = pt.q_endogenic_w_m2 * 1000.0;
    lat_profile.push_back(pt);
  }

  // Write latitude profile to CSV
  std::string lat_csv_path = "replications_ss/paper_209/cirs_heat_flux_vs_latitude.csv";
  std::ofstream lat_csv(lat_csv_path);
  if (!lat_csv.is_open()) {
    // Fallback if running from within directory
    lat_csv_path = "cirs_heat_flux_vs_latitude.csv";
    lat_csv.open(lat_csv_path);
  }
  lat_csv << "latitude_deg,solar_absorbed_w_m2,t_passive_k,flux_passive_w_m2,t_cirs_obs_k,flux_cirs_obs_w_m2,q_endogenic_w_m2,q_endogenic_mw_m2\n";
  for (const auto& pt : lat_profile) {
    lat_csv << std::fixed << std::setprecision(4)
            << pt.latitude_deg << ","
            << pt.solar_flux_absorbed_w_m2 << ","
            << pt.t_passive_k << ","
            << pt.flux_passive_w_m2 << ","
            << pt.t_cirs_obs_k << ","
            << pt.flux_cirs_obs_w_m2 << ","
            << pt.q_endogenic_w_m2 << ","
            << pt.q_endogenic_mw_m2 << "\n";
  }
  lat_csv.close();
  std::cout << "✅ Latitude profile written to: " << lat_csv_path << "\n";

  // 2. Tiger Stripe Surface Area & Temperature Parameter Sweep
  std::vector<AreaSweepPoint> area_sweep;
  for (double area_km2 = 20.0; area_km2 <= 300.0; area_km2 += 5.0) {
    AreaSweepPoint pt;
    pt.area_stripes_km2 = area_km2;
    pt.power_120k_gw = model.radiated_power_gw(area_km2, 120.0, 1500.0, 85.0, 72.0);
    pt.power_135k_gw = model.radiated_power_gw(area_km2, 135.0, 1500.0, 85.0, 72.0);
    pt.power_140k_gw = model.radiated_power_gw(area_km2, 140.0, 1500.0, 85.0, 72.0);
    pt.power_145k_gw = model.radiated_power_gw(area_km2, 145.0, 1500.0, 85.0, 72.0);
    pt.power_155k_gw = model.radiated_power_gw(area_km2, 155.0, 1500.0, 85.0, 72.0);
    area_sweep.push_back(pt);
  }

  std::string area_csv_path = "replications_ss/paper_209/power_vs_tiger_stripe_area.csv";
  std::ofstream area_csv(area_csv_path);
  if (!area_csv.is_open()) {
    area_csv_path = "power_vs_tiger_stripe_area.csv";
    area_csv.open(area_csv_path);
  }
  area_csv << "area_stripes_km2,power_120k_gw,power_135k_gw,power_140k_gw,power_145k_gw,power_155k_gw\n";
  for (const auto& pt : area_sweep) {
    area_csv << std::fixed << std::setprecision(4)
             << pt.area_stripes_km2 << ","
             << pt.power_120k_gw << ","
             << pt.power_135k_gw << ","
             << pt.power_140k_gw << ","
             << pt.power_145k_gw << ","
             << pt.power_155k_gw << "\n";
  }
  area_csv.close();
  std::cout << "✅ Area parameter sweep written to: " << area_csv_path << "\n";

  // 3. Thermal Radiation Budget Integration
  double p_spt_integrated_gw = model.integrated_spt_endogenic_power_gw(-65.0);
  double p_nominal_model_gw = model.radiated_power_gw(125.0, 140.0, 1500.0, 85.0, 72.0);
  double p_radio_gw = model.radiogenic_power_gw();
  double p_tide_nominal_gw = model.tidal_dissipation_power_gw(0.00100);
  double req_k2q = model.required_k2_over_q(p_nominal_model_gw);
  double spt_area_km2 = model.spt_surface_area_m2(-65.0) / 1.0e6;
  double avg_spt_flux_mw_m2 = (p_nominal_model_gw * 1.0e12) / (spt_area_km2 * 1.0e6);

  std::string budget_csv_path = "replications_ss/paper_209/enceladus_thermal_budget.csv";
  std::ofstream budget_csv(budget_csv_path);
  if (!budget_csv.is_open()) {
    budget_csv_path = "enceladus_thermal_budget.csv";
    budget_csv.open(budget_csv_path);
  }
  budget_csv << "metric,value,unit,reference_observation\n";
  budget_csv << "spt_surface_area," << spt_area_km2 << ",km^2,37430 km^2 poleward of 65S\n";
  budget_csv << "cirs_measured_endogenic_power," << p_nominal_model_gw << ",GW,5.8 +/- 1.9 GW (Spencer 2006)\n";
  budget_csv << "integrated_spt_power," << p_spt_integrated_gw << ",GW,5.82 GW\n";
  budget_csv << "spt_average_heat_flux," << avg_spt_flux_mw_m2 << ",mW/m^2,155 mW/m^2\n";
  budget_csv << "core_radiogenic_power," << p_radio_gw << ",GW,0.301 GW (Chondritic core)\n";
  budget_csv << "tidal_heating_power," << p_tide_nominal_gw << ",GW,5.52 GW (k2/Q = 1.0e-3)\n";
  budget_csv << "required_k2_over_q," << req_k2q << ",dimensionless,1.00e-3\n";
  budget_csv << "tiger_stripes_length," << 500.0 << ",km,4 main Sulci\n";
  budget_csv << "tiger_stripes_active_area," << 125.0 << ",km^2,100-250 km^2\n";
  budget_csv << "tiger_stripes_peak_temp," << 140.0 << ",K,135-145 K\n";
  budget_csv.close();
  std::cout << "✅ Thermal budget written to: " << budget_csv_path << "\n\n";

  // Summary Metrics Output
  std::cout << "--- REPLICATION METRICS SUMMARY ---\n";
  std::cout << "• SPT Surface Area (poleward of 65°S): " << spt_area_km2 << " km^2\n";
  std::cout << "• Passive Equilibrium South Pole Temp: " << model.passive_equilibrium_temp_k(-90.0) << " K\n";
  std::cout << "• CIRS South Pole Effective Temp:      " << model.cirs_observed_temp_k(-90.0) << " K\n";
  std::cout << "• Peak Endogenic Heat Flux:           " << model.endogenic_heat_flux_w_m2(-90.0) * 1000.0 << " mW/m^2\n";
  std::cout << "• Total Endogenic Radiated Power:     " << p_nominal_model_gw << " GW (Observed: 5.8 ± 1.9 GW)\n";
  std::cout << "• Integrated Latitude Profile Power:   " << p_spt_integrated_gw << " GW\n";
  std::cout << "• Radiogenic Silicate Core Power:     " << p_radio_gw << " GW\n";
  std::cout << "• Viscoelastic Tidal Power:           " << p_tide_nominal_gw << " GW\n";
  std::cout << "• Required Dissipation Factor k2/Q:   " << req_k2q << "\n";
  std::cout << "====================================================================\n";

  return 0;
}
