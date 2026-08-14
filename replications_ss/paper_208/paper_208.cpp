// Solver for Paper #208: Nimmo & Spencer (2006) / Nimmo et al. (2007)
// "Powering the South Polar Plumes of Enceladus"
// Evaluates shear heating along active strike-slip faults (tiger stripes) and hydrothermal plume power output.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

#include "solar_system.hpp"

int main() {
  std::cout << "================================================================================" << std::endl;
  std::cout << "Paper #208 Replication Solver: Nimmo & Spencer (2006) / Nimmo et al. (2007)" << std::endl;
  std::cout << "Tidally Driven Strike-Slip Shear Heating & Plume Power on Enceladus" << std::endl;
  std::cout << "================================================================================" << std::endl;

  hot_jupiter::EnceladusFaultShearHeatingModel model;

  // Key system parameters
  double P_orb_s = model.orbital_period_s();
  double P_orb_days = model.orbital_period_days();
  double omega = model.orbital_frequency_rad_s();

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "Orbital Period: " << P_orb_days << " days (" << P_orb_s << " s)" << std::endl;
  std::cout << "Orbital Mean Motion: " << omega << " rad/s" << std::endl;
  std::cout << "Surface Gravity: " << model.G_SURF << " m/s^2" << std::endl;
  std::cout << "Ice Density: " << model.RHO_ICE << " kg/m^3" << std::endl;
  std::cout << "Nominal Fault Length (4 tiger stripes): " << model.L_TOTAL_NOM / 1000.0 << " km" << std::endl;

  // 1. Fault Heat Generation vs Displacement Amplitude Sweep
  std::ofstream csv_disp("replications_ss/paper_208/enceladus_fault_shear_heating.csv");
  csv_disp << "displacement_m,power_gw_d3km,power_gw_d5km,power_gw_d7km,power_gw_d10km\n";

  std::vector<double> disp_arr;
  std::vector<double> p_d5km_arr;
  std::vector<double> p_cirs_ref_arr;

  for (double ds = 0.0; ds <= 1.5001; ds += 0.02) {
    double p_3k = model.shear_heating_power_gw(ds, 3000.0, 0.50);
    double p_5k = model.shear_heating_power_gw(ds, 5000.0, 0.50);
    double p_7k = model.shear_heating_power_gw(ds, 7000.0, 0.50);
    double p_10k = model.shear_heating_power_gw(ds, 10000.0, 0.50);

    csv_disp << std::fixed << std::setprecision(5) << ds << ","
             << p_3k << "," << p_5k << "," << p_7k << "," << p_10k << "\n";

    disp_arr.push_back(ds);
    p_d5km_arr.push_back(p_5k);
    // Linear benchmark relation from Nimmo et al. (2007): P = (5.5 GW / 0.5 m) * ds
    p_cirs_ref_arr.push_back((5.492 / 0.50) * ds);
  }
  csv_disp.close();
  std::cout << "✅ Generated enceladus_fault_shear_heating.csv" << std::endl;

  // Calculate R^2 for displacement vs power linearity
  double mean_ref = std::accumulate(p_cirs_ref_arr.begin(), p_cirs_ref_arr.end(), 0.0) / p_cirs_ref_arr.size();
  double ss_tot = 0.0;
  double ss_res = 0.0;
  for (size_t i = 0; i < p_d5km_arr.size(); ++i) {
    ss_tot += std::pow(p_cirs_ref_arr[i] - mean_ref, 2.0);
    ss_res += std::pow(p_cirs_ref_arr[i] - p_d5km_arr[i], 2.0);
  }
  double r2_disp = 1.0 - (ss_res / ss_tot);
  std::cout << "Displacement vs Heat Generation Model R^2: " << std::setprecision(6) << r2_disp << std::endl;

  // 2. Plume Power Output vs Friction Coefficient Sweep
  std::ofstream csv_fric("replications_ss/paper_208/enceladus_plume_friction_sweep.csv");
  csv_fric << "friction_coeff,power_gw_pore00,power_gw_pore20,power_gw_pore40,power_gw_pore60,total_power_gw_pore00\n";

  for (double mu = 0.05; mu <= 0.8501; mu += 0.02) {
    double p_pore00 = model.shear_heating_power_gw(0.50, 5000.0, mu, model.L_TOTAL_NOM, 0.00);
    double p_pore20 = model.shear_heating_power_gw(0.50, 5000.0, mu, model.L_TOTAL_NOM, 0.20);
    double p_pore40 = model.shear_heating_power_gw(0.50, 5000.0, mu, model.L_TOTAL_NOM, 0.40);
    double p_pore60 = model.shear_heating_power_gw(0.50, 5000.0, mu, model.L_TOTAL_NOM, 0.60);
    double p_tot = model.total_plume_and_thermal_power_gw(0.50, 5000.0, mu, 200.0, model.L_TOTAL_NOM, 0.00);

    csv_fric << std::fixed << std::setprecision(5) << mu << ","
             << p_pore00 << "," << p_pore20 << "," << p_pore40 << "," << p_pore60 << "," << p_tot << "\n";
  }
  csv_fric.close();
  std::cout << "✅ Generated enceladus_plume_friction_sweep.csv" << std::endl;

  // 3. Key Physical Predictions
  double p_shear_nom = model.shear_heating_power_gw(0.50, 5000.0, 0.50);
  double p_latent_nom = model.plume_latent_power_gw(200.0);
  double p_kin_nom = model.plume_kinetic_power_gw(200.0);
  double p_total_nom = model.total_plume_and_thermal_power_gw(0.50, 5000.0, 0.50, 200.0);
  double ds_req_spencer = model.required_displacement_m(model.P_OBS_SPENCER_GW, 5000.0, 0.50);
  double ds_req_howett = model.required_displacement_m(model.P_OBS_HOWETT_GW, 5000.0, 0.50);
  double ds_req_d7km_howett = model.required_displacement_m(model.P_OBS_HOWETT_GW, 7000.0, 0.50);
  double ds_tidal_h2_002 = model.diurnal_tidal_displacement_m(0.02);

  std::cout << "\n--- Nominal Model Summary ---" << std::endl;
  std::cout << "Nominal Strike-Slip Shear Power (d = 5 km, mu = 0.5, ds = 0.5 m): " << p_shear_nom << " GW" << std::endl;
  std::cout << "Plume Vapor Latent Heat Transport (Mdot = 200 kg/s): " << p_latent_nom << " GW" << std::endl;
  std::cout << "Plume Kinetic Power (v_jet = 400 m/s): " << p_kin_nom << " GW" << std::endl;
  std::cout << "Total Plume & Endogenic Power: " << p_total_nom << " GW" << std::endl;
  std::cout << "Required Cyclic Displacement for Spencer (2006) 5.8 GW: " << ds_req_spencer << " m" << std::endl;
  std::cout << "Required Cyclic Displacement for Howett (2011) 15.8 GW (d = 5 km): " << ds_req_howett << " m" << std::endl;
  std::cout << "Required Cyclic Displacement for Howett (2011) 15.8 GW (d = 7 km): " << ds_req_d7km_howett << " m" << std::endl;
  std::cout << "Decoupled Shell Diurnal Displacement (h2 = 0.02): " << ds_tidal_h2_002 << " m" << std::endl;

  // 4. Output Summary CSV
  std::ofstream csv_sum("replications_ss/paper_208/enceladus_tiger_stripes_summary.csv");
  csv_sum << "parameter,value,unit,description\n";
  csv_sum << "P_shear_nominal," << p_shear_nom << ",GW,Fault shear heating power\n";
  csv_sum << "P_latent_nominal," << p_latent_nom << ",GW,Plume latent heat transport\n";
  csv_sum << "P_kinetic_nominal," << p_kin_nom << ",GW,Plume kinetic venting power\n";
  csv_sum << "P_total_nominal," << p_total_nom << ",GW,Total endogenic power\n";
  csv_sum << "ds_req_spencer," << ds_req_spencer << ",m,Required slip for 5.8 GW\n";
  csv_sum << "ds_req_howett," << ds_req_howett << ",m,Required slip for 15.8 GW (d=5km)\n";
  csv_sum << "ds_req_howett_d7k," << ds_req_d7km_howett << ",m,Required slip for 15.8 GW (d=7km)\n";
  csv_sum << "ds_tidal_h2_002," << ds_tidal_h2_002 << ",m,Tidal displacement for h2=0.02\n";
  csv_sum << "R2_goodness_of_fit," << r2_disp << ",-,Statistical agreement metric\n";
  csv_sum.close();
  std::cout << "✅ Generated enceladus_tiger_stripes_summary.csv" << std::endl;

  return 0;
}
