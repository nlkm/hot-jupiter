// Copyright 2026 Antigravity Scientific Automation & Solar System Dynamics Replication Campaign
// Replication of Paper #235: O'Brien, Morbidelli, & Levison (2006)
// "Terrestrial Planet Formation with Strong Dynamical Friction"
// Icarus 184, 39-58 (2006)
// First-principles modeling of planetesimal aerodynamic gas drag, embryo dynamical friction,
// oligarchic growth rates, water delivery from hydrated outer reservoirs, and terrestrial architecture (CJS vs EJS vs EEJS).

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "cpp/include/constants.hpp"
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "=================================================================" << std::endl;
  std::cout << "  Paper #235 Replication: O'Brien et al. (2006) Icarus 184, 39  " << std::endl;
  std::cout << "  Terrestrial Planet Formation with Strong Dynamical Friction    " << std::endl;
  std::cout << "=================================================================" << std::endl;

  hot_jupiter::OBrien2006TerrestrialAccretionModel model;

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "Gas disk surface density Sigma_0 at 1 AU: " << hot_jupiter::OBrien2006TerrestrialAccretionModel::SIGMA_GAS_1AU_NOMINAL << " kg/m^2 (1700 g/cm^2)" << std::endl;
  std::cout << "Solid disk surface density Sigma_s at 1 AU: " << hot_jupiter::OBrien2006TerrestrialAccretionModel::SIGMA_SOLID_1AU_NOMINAL << " kg/m^2 (10 g/cm^2)" << std::endl;
  std::cout << "Gas disk dissipation timescale tau_gas:   " << hot_jupiter::OBrien2006TerrestrialAccretionModel::TAU_GAS_NOMINAL_MYR << " Myr" << std::endl;
  std::cout << "Nominal planetesimal radius R_p:          " << hot_jupiter::OBrien2006TerrestrialAccretionModel::PLANETESIMAL_RADIUS_NOM_KM << " km" << std::endl;
  std::cout << "Planetesimal / Embryo Bulk Density:       " << hot_jupiter::OBrien2006TerrestrialAccretionModel::RHO_PLANETESIMAL << " kg/m^3" << std::endl;
  std::cout << std::endl;

  // 1. Planetesimal Aerodynamic Gas Drag Damping Sweep
  std::ofstream csv_drag("replications_ss/paper_235/gas_drag_damping_sweep.csv");
  csv_drag << "a_au,e_p,r_p_km,t_myr,rho_gas_kg_m3,tau_e_drag_yr,tau_i_drag_yr,da_dt_au_myr,v_rel_m_s,a_drag_m_s2\n";

  const double radii_km[3] = {1.0, 10.0, 100.0};
  const double times_myr[3] = {0.0, 1.0, 3.0};

  for (double a = 0.4; a <= 4.05; a += 0.1) {
    for (double e = 0.01; e <= 0.21; e += 0.05) {
      for (double r_km : radii_km) {
        for (double t_myr : times_myr) {
          double rho_g = model.gas_midplane_density_kg_m3(a, t_myr);
          double tau_e = model.eccentricity_damping_timescale_yr(a, e, r_km, t_myr);
          double tau_i = model.inclination_damping_timescale_yr(a, e, r_km, t_myr);
          double da_dt = model.semi_major_axis_decay_rate_au_myr(a, e, r_km, t_myr);
          double v_rel = model.planetesimal_relative_velocity_m_s(a, e, 0.5 * e);
          double a_drag = model.gas_drag_acceleration_m_s2(a, e, r_km, t_myr, 0.5 * e);

          csv_drag << std::fixed << std::setprecision(2) << a << ","
                   << std::setprecision(3) << e << ","
                   << std::setprecision(1) << r_km << ","
                   << std::setprecision(2) << t_myr << ","
                   << std::scientific << std::setprecision(4)
                   << rho_g << "," << tau_e << "," << tau_i << "," << da_dt << ","
                   << std::fixed << std::setprecision(2) << v_rel << ","
                   << std::scientific << std::setprecision(4) << a_drag << "\n";
        }
      }
    }
  }
  csv_drag.close();
  std::cout << "✅ Saved replications_ss/paper_235/gas_drag_damping_sweep.csv" << std::endl;

  // 2. Embryo Accretion Kinetics, Gravitational Focusing, and Dynamical Friction Sweep
  std::ofstream csv_kinetics("replications_ss/paper_235/embryo_growth_kinetics.csv");
  csv_kinetics << "a_au,embryo_mass_mearth,e_p,fg_focusing,m_iso_mearth,dM_dt_mearth_yr,tau_growth_yr,tau_df_yr,r_phys_km,v_esc_km_s\n";

  const double embryo_masses[4] = {0.01, 0.05, 0.10, 0.50};
  const double planetesimal_eccs[4] = {0.005, 0.01, 0.02, 0.05};

  for (double a = 0.5; a <= 3.55; a += 0.1) {
    double m_iso = model.isolation_mass_mearth(a, 10.0, 10.0, 1.5);
    for (double m_emb : embryo_masses) {
      for (double e_p : planetesimal_eccs) {
        double fg = model.gravitational_focusing_factor(m_emb, a, e_p);
        double dm_dt = model.embryo_accretion_rate_mearth_yr(a, m_emb, 100.0 * std::pow(a, -1.5), e_p);
        double tau_growth = model.embryo_mass_doubling_time_yr(a, m_emb, 100.0 * std::pow(a, -1.5), e_p);
        double tau_df = model.dynamical_friction_eccentricity_damping_yr(a, m_emb, 100.0 * std::pow(a, -1.5), e_p);
        double r_phys_km = model.embryo_physical_radius_meters(m_emb) / 1.0e3;
        double v_esc_km_s = model.embryo_escape_velocity_m_s(m_emb) / 1.0e3;

        csv_kinetics << std::fixed << std::setprecision(2) << a << ","
                     << std::setprecision(3) << m_emb << ","
                     << std::setprecision(3) << e_p << ","
                     << std::setprecision(2) << fg << ","
                     << std::setprecision(4) << m_iso << ","
                     << std::scientific << std::setprecision(4)
                     << dm_dt << "," << tau_growth << "," << tau_df << ","
                     << std::fixed << std::setprecision(1) << r_phys_km << ","
                     << std::setprecision(2) << v_esc_km_s << "\n";
      }
    }
  }
  csv_kinetics.close();
  std::cout << "✅ Saved replications_ss/paper_235/embryo_growth_kinetics.csv" << std::endl;

  // 3. Water Delivery Profile & Reservoir Depletion
  std::ofstream csv_water("replications_ss/paper_235/water_delivery_radial_profile.csv");
  csv_water << "a_au,initial_water_frac,rho_midplane_kg_m3,e_eq_gas_drag,sub_keplerian_eta,v_keplerian_kms\n";

  for (double a = 0.4; a <= 4.0; a += 0.05) {
    double w_init = model.initial_water_mass_fraction(a);
    double rho_g = model.gas_midplane_density_kg_m3(a, 0.0);
    double e_eq = model.equilibrium_planetesimal_eccentricity(a, 0.05, 10.0, 0.0);
    double eta = model.sub_keplerian_eta(a);
    double v_k = model.keplerian_velocity_m_s(a) / 1.0e3;

    csv_water << std::fixed << std::setprecision(2) << a << ","
              << std::setprecision(5) << w_init << ","
              << std::scientific << std::setprecision(4) << rho_g << ","
              << std::fixed << std::setprecision(4) << e_eq << ","
              << std::scientific << std::setprecision(4) << eta << ","
              << std::fixed << std::setprecision(2) << v_k << "\n";
  }
  csv_water.close();
  std::cout << "✅ Saved replications_ss/paper_235/water_delivery_radial_profile.csv" << std::endl;

  // 4. Synthesized Planetary System Architectures (CJS, EJS, EEJS, No-DF Classic, Observed Solar System)
  std::ofstream csv_arch("replications_ss/paper_235/planetary_architectures_summary.csv");
  csv_arch << "scenario,planet_name,a_au,mass_mearth,eccentricity,inc_deg,water_fraction,water_oceans,formation_time_myr\n";

  const std::vector<std::string> scenarios = {"Solar_System", "EJS", "CJS", "EEJS", "No_DF_Classic"};

  for (const auto& sc : scenarios) {
    auto sys = model.get_system_architecture(sc);
    std::cout << "Scenario: " << sc << " | N = " << sys.num_planets
              << " | Total M = " << sys.total_mass_mearth << " M_E"
              << " | AMD = " << sys.angular_momentum_deficit
              << " | RMC = " << sys.radial_mass_concentration
              << " | Earth Water = " << sys.earth_analog_water_oceans << " oceans" << std::endl;

    for (const auto& p : sys.planets) {
      double oceans = (p.mass_mearth * hot_jupiter::OBrien2006TerrestrialAccretionModel::M_EARTH_KG * p.water_mass_fraction) /
                      hot_jupiter::OBrien2006TerrestrialAccretionModel::EARTH_OCEAN_MASS_KG;
      csv_arch << sc << ","
               << p.name << ","
               << std::fixed << std::setprecision(3) << p.semi_major_axis_au << ","
               << std::setprecision(4) << p.mass_mearth << ","
               << std::setprecision(4) << p.eccentricity << ","
               << std::setprecision(2) << p.inclination_deg << ","
               << std::scientific << std::setprecision(4) << p.water_mass_fraction << ","
               << std::fixed << std::setprecision(2) << oceans << ","
               << std::setprecision(1) << p.formation_time_myr << "\n";
    }
  }
  csv_arch.close();
  std::cout << "✅ Saved replications_ss/paper_235/planetary_architectures_summary.csv" << std::endl;

  // 5. Benchmark Comparison Table
  std::ofstream csv_bench("replications_ss/paper_235/benchmark_comparison_metrics.csv");
  csv_bench << "metric,unit,observed_solar_system,model_ejs,model_cjs,model_eejs,model_no_df,accuracy_pct\n";

  auto bench_table = model.evaluate_benchmark_metrics();
  for (const auto& row : bench_table) {
    csv_bench << "\"" << row.metric_name << "\","
              << row.unit << ","
              << std::fixed << std::setprecision(4) << row.observed_solar_system << ","
              << row.model_ejs << ","
              << row.model_cjs << ","
              << row.model_eejs << ","
              << row.model_no_df << ","
              << std::setprecision(2) << row.relative_accuracy_pct << "\n";
  }
  csv_bench.close();
  std::cout << "✅ Saved replications_ss/paper_235/benchmark_comparison_metrics.csv" << std::endl;

  std::cout << "\n=================================================================" << std::endl;
  std::cout << "  Paper #235 Numerical Engine Execution Successfully Completed! " << std::endl;
  std::cout << "=================================================================" << std::endl;

  return 0;
}
