// Copyright 2026 Antigravity Scientific Automation & Solar System Replication Campaign
// First-principles replication of Brasser, Duncan, & Levison (2006), Icarus 184, 59-82
// "Embedded star clusters and the formation of the Oort Cloud" - The Formation of the Sedna Sphere

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "cpp/include/constants.hpp"
#include "cpp/include/solar_system.hpp"

int main() {
  hot_jupiter::Brasser2006SednaSphereModel model;

  std::cout << "============================================================================" << std::endl;
  std::cout << "Paper #245: Brasser et al. (2006) Formation of the Sedna Sphere Solver" << std::endl;
  std::cout << "============================================================================" << std::endl;

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "Nominal Cluster Density rho_c:   " << model.RHO_CLUSTER_NOM_MSUN_PC3 << " M_sun / pc^3" << std::endl;
  std::cout << "Nominal Cluster Membership N_*:  " << model.N_STARS_CLUSTER_NOM << " stars" << std::endl;
  std::cout << "Nominal Core Radius R_c:         " << model.R_CLUSTER_CORE_PC_NOM << " pc" << std::endl;
  std::cout << "Cluster Dissolution Lifetime:    " << model.TAU_CLUSTER_LIFETIME_MYR_NOM << " Myr" << std::endl;
  std::cout << "Primordial Disk Mass M_disk:     " << model.M_DISK_PRIMORDIAL_MEARTH_NOM << " M_Earth" << std::endl;
  std::cout << "----------------------------------------------------------------------------" << std::endl;

  // 1. Landmark Detached TNOs & Required Flyby Energetics
  std::cout << "\n[1] Landmark Detached Trans-Neptunian Objects (Sedna Sphere Archetypes):" << std::endl;
  std::cout << std::setw(18) << "Object"
            << std::setw(12) << "a [AU]"
            << std::setw(12) << "q [AU]"
            << std::setw(12) << "e"
            << std::setw(12) << "i [deg]"
            << std::setw(18) << "Req. b (q0=30AU)"
            << std::setw(18) << "Delta v [m/s]"
            << std::endl;

  struct LandmarkObject {
    std::string name;
    double a;
    double q;
    double e;
    double inc;
  };

  std::vector<LandmarkObject> landmarks = {
      {"(90377) Sedna", model.A_SEDNA_AU, model.Q_SEDNA_AU, model.E_SEDNA, model.I_SEDNA_DEG},
      {"2012 VP113", model.A_2012VP113_AU, model.Q_2012VP113_AU, 1.0 - (model.Q_2012VP113_AU / model.A_2012VP113_AU), 24.05},
      {"541132 Leleākūhonua", model.A_LELEAKUHONUA_AU, model.Q_LELEAKUHONUA_AU, 1.0 - (model.Q_LELEAKUHONUA_AU / model.A_LELEAKUHONUA_AU), 11.66}
  };

  for (const auto& obj : landmarks) {
    double req_b = model.required_impact_parameter_au(obj.a, 30.0, obj.q);
    double r_a = 2.0 * obj.a - 30.0;
    double dv_kms = model.impulsive_velocity_kick_km_s(r_a, req_b);
    double dv_ms = dv_kms * 1000.0;

    std::cout << std::setw(18) << obj.name
              << std::setw(12) << std::setprecision(1) << obj.a
              << std::setw(12) << std::setprecision(1) << obj.q
              << std::setw(12) << std::setprecision(4) << obj.e
              << std::setw(12) << std::setprecision(2) << obj.inc
              << std::setw(18) << std::setprecision(1) << req_b
              << std::setw(18) << std::setprecision(2) << dv_ms
              << std::endl;
  }

  // 2. Export Cluster Stellar Encounter Kinematics CSV
  std::ofstream csv_enc("replications_ss/paper_245/cluster_stellar_encounters.csv");
  csv_enc << "impact_parameter_au,rate_per_myr,cumulative_encounters_30myr,cross_section_au2,b_pdf,v_rel_kms,v_pdf\n";

  double b_min_sample = 50.0;
  double b_max_sample = 5000.0;
  for (double b = 50.0; b <= 5000.0; b += 25.0) {
    double rate = model.encounter_rate_per_myr(b, 20000.0);
    double cum_enc = model.cumulative_encounters(b, 30.0, 20000.0);
    double sigma_au2 = model.encounter_cross_section_au2(b);
    double pdf_b = model.impact_parameter_pdf(b, b_min_sample, b_max_sample);
    double v_sample = (b / 5000.0) * 4.0; // sample v from 0 to 4 km/s
    double pdf_v = model.relative_velocity_maxwellian_pdf(v_sample);

    csv_enc << std::fixed << std::setprecision(1) << b << ","
            << std::scientific << std::setprecision(5) << rate << ","
            << std::setprecision(5) << cum_enc << ","
            << std::setprecision(5) << sigma_au2 << ","
            << std::setprecision(5) << pdf_b << ","
            << std::fixed << std::setprecision(3) << v_sample << ","
            << std::scientific << std::setprecision(5) << pdf_v << "\n";
  }
  csv_enc.close();
  std::cout << "✅ Saved replications_ss/paper_245/cluster_stellar_encounters.csv" << std::endl;

  // 3. Export Perihelion Lifting Tracks vs Impact Parameter CSV
  std::ofstream csv_peri("replications_ss/paper_245/perihelion_lifting_tracks.csv");
  csv_peri << "impact_parameter_au,q_final_a250_au,q_final_a500_au,q_final_a1000_au,q_final_a2000_au,q_final_a5000_au,dv_a500_ms\n";

  for (double b = 100.0; b <= 3000.0; b += 20.0) {
    double q_250 = model.mean_lifted_perihelion_au(250.0, 30.0, b);
    double q_500 = model.mean_lifted_perihelion_au(500.0, 30.0, b);
    double q_1000 = model.mean_lifted_perihelion_au(1000.0, 30.0, b);
    double q_2000 = model.mean_lifted_perihelion_au(2000.0, 30.0, b);
    double q_5000 = model.mean_lifted_perihelion_au(5000.0, 30.0, b);
    double r_a_500 = 2.0 * 500.0 - 30.0;
    double dv_ms = model.impulsive_velocity_kick_km_s(r_a_500, b) * 1000.0;

    csv_peri << std::fixed << std::setprecision(1) << b << ","
             << std::setprecision(3) << q_250 << ","
             << std::setprecision(3) << q_500 << ","
             << std::setprecision(3) << q_1000 << ","
             << std::setprecision(3) << q_2000 << ","
             << std::setprecision(3) << q_5000 << ","
             << std::setprecision(3) << dv_ms << "\n";
  }
  csv_peri.close();
  std::cout << "✅ Saved replications_ss/paper_245/perihelion_lifting_tracks.csv" << std::endl;

  // 4. Export Trapping Efficiency & Semi-Major Axis Mass Distribution CSV
  std::ofstream csv_trap("replications_ss/paper_245/semimajor_trapping_efficiency.csv");
  csv_trap << "semimajor_axis_au,log10_a,p_trap_inner,p_retain_outer,net_efficiency,dn_dlog_mearth,reservoir_regime\n";

  for (double log_a = 1.90; log_a <= 5.00; log_a += 0.02) {
    double a_au = std::pow(10.0, log_a);
    double p_trap = model.inner_oort_trapping_probability(a_au);
    double p_ret = model.outer_oort_retention_probability(a_au);
    double net_eff = model.net_oort_efficiency(a_au);
    double dn_dlog = model.differential_semi_major_axis_density(a_au);

    std::string regime = "Planetary_Scattering";
    if (a_au >= 100.0 && a_au < 1000.0) {
      regime = "Sedna_Transition_Zone";
    } else if (a_au >= 1000.0 && a_au < 15000.0) {
      regime = "Sedna_Sphere_Core_IOC";
    } else if (a_au >= 15000.0) {
      regime = "Outer_Oort_Stripping_Zone";
    }

    csv_trap << std::fixed << std::setprecision(2) << a_au << ","
             << std::setprecision(4) << log_a << ","
             << std::setprecision(5) << p_trap << ","
             << std::setprecision(5) << p_ret << ","
             << std::setprecision(5) << net_eff << ","
             << std::setprecision(5) << dn_dlog << ","
             << regime << "\n";
  }
  csv_trap.close();
  std::cout << "✅ Saved replications_ss/paper_245/semimajor_trapping_efficiency.csv" << std::endl;

  // 5. Cluster Density & Lifetime Parameter Sweep CSV
  std::ofstream csv_sweep("replications_ss/paper_245/cluster_density_lifetime_sweep.csv");
  csv_sweep << "log10_rho_c,rho_c_msun_pc3,b_min_au,m_ioc_10myr,m_ioc_30myr,m_ioc_50myr,m_ioc_100myr,m_ooc_30myr,mass_ratio_ioc_ooc,f_eject_30myr,sedna_pop_30myr\n";

  std::cout << "\n[2] Birth Cluster Density Sweep (M_disk = 30 M_Earth, tau_cluster = 30 Myr):" << std::endl;
  std::cout << std::setw(14) << "rho_c [M_sun/pc3]"
            << std::setw(14) << "b_min [AU]"
            << std::setw(14) << "M_IOC [M_E]"
            << std::setw(14) << "M_OOC [M_E]"
            << std::setw(14) << "Ratio IOC/OOC"
            << std::setw(14) << "f_Eject [%]"
            << std::setw(16) << "Sedna-sized Pop"
            << std::endl;

  for (double log_rho = 2.0; log_rho <= 5.3; log_rho += 0.1) {
    double rho_c = std::pow(10.0, log_rho);
    double n_pc3 = model.cluster_stellar_number_density_pc3(rho_c);
    double b_min = model.minimum_impact_parameter_au(30.0, n_pc3);

    double m_ioc_10 = model.inner_oort_mass_mearth(rho_c, 10.0);
    double m_ioc_30 = model.inner_oort_mass_mearth(rho_c, 30.0);
    double m_ioc_50 = model.inner_oort_mass_mearth(rho_c, 50.0);
    double m_ioc_100 = model.inner_oort_mass_mearth(rho_c, 100.0);

    double m_ooc_30 = model.outer_oort_mass_mearth(rho_c, 30.0);
    double ratio_30 = model.inner_to_outer_oort_mass_ratio(rho_c, 30.0);
    double f_eject_30 = model.interstellar_ejection_fraction(rho_c, 30.0);
    double sedna_pop = model.sedna_sized_population_estimate(rho_c, 30.0);

    if (std::abs(log_rho - 2.0) < 1e-4 || std::abs(log_rho - 3.0) < 1e-4 ||
        std::abs(log_rho - 4.0) < 1e-4 || std::abs(log_rho - 5.0) < 1e-4) {
      std::cout << std::setw(14) << std::scientific << std::setprecision(2) << rho_c << std::fixed
                << std::setw(14) << std::setprecision(1) << b_min
                << std::setw(14) << std::setprecision(2) << m_ioc_30
                << std::setw(14) << std::setprecision(2) << m_ooc_30
                << std::setw(14) << std::setprecision(2) << ratio_30
                << std::setw(14) << std::setprecision(1) << f_eject_30 * 100.0
                << std::setw(16) << std::setprecision(0) << sedna_pop
                << std::endl;
    }

    csv_sweep << std::fixed << std::setprecision(2) << log_rho << ","
              << std::scientific << std::setprecision(4) << rho_c << std::fixed << ","
              << std::setprecision(2) << b_min << ","
              << std::setprecision(3) << m_ioc_10 << ","
              << std::setprecision(3) << m_ioc_30 << ","
              << std::setprecision(3) << m_ioc_50 << ","
              << std::setprecision(3) << m_ioc_100 << ","
              << std::setprecision(3) << m_ooc_30 << ","
              << std::setprecision(3) << ratio_30 << ","
              << std::setprecision(4) << f_eject_30 << ","
              << std::setprecision(1) << sedna_pop << "\n";
  }
  csv_sweep.close();
  std::cout << "✅ Saved replications_ss/paper_245/cluster_density_lifetime_sweep.csv" << std::endl;

  // 6. Export Benchmark Comparison CSV
  std::ofstream csv_bm("replications_ss/paper_245/detached_tno_benchmarks.csv");
  csv_bm << "benchmark_name,a_au,q_obs_au,q_model_au,eccentricity,inclination_deg,b_req_au,p_trap_percent\n";

  for (const auto& obj : landmarks) {
    double req_b = model.required_impact_parameter_au(obj.a, 30.0, obj.q);
    double q_mod = model.mean_lifted_perihelion_au(obj.a, 30.0, req_b);
    double p_trap = model.inner_oort_trapping_probability(obj.a) * 100.0;

    csv_bm << obj.name << ","
           << std::fixed << std::setprecision(1) << obj.a << ","
           << std::setprecision(1) << obj.q << ","
           << std::setprecision(1) << q_mod << ","
           << std::setprecision(4) << obj.e << ","
           << std::setprecision(2) << obj.inc << ","
           << std::setprecision(1) << req_b << ","
           << std::setprecision(2) << p_trap << "\n";
  }
  csv_bm.close();
  std::cout << "✅ Saved replications_ss/paper_245/detached_tno_benchmarks.csv" << std::endl;

  std::cout << "\n============================================================================" << std::endl;
  std::cout << "✅ Paper #245 C++ Solver Completed Successfully." << std::endl;
  std::cout << "============================================================================" << std::endl;

  return 0;
}
