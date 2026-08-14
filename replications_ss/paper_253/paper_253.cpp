// Copyright 2026 Antigravity Scientific Automation & Solar System Dynamics Replication Campaign
// Replication of Paper #253: Dones, Weissman, Levison, & Duncan (2004)
// "Oort Cloud Formation and Dynamics"
// In Comets II (M. C. Festou, H. U. Keller, & H. A. Weaver, Eds.), University of Arizona Press, pp. 153-174.
// First-principles C++ simulation of giant planet planetesimal scattering efficiency,
// Safronov numbers, Galactic tide perihelion lifting, fate branching fractions,
// and Oort Cloud retention over 4.5 Gyr of Solar System history.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "cpp/include/constants.hpp"
#include "cpp/include/solar_system.hpp"

int main() {
  std::cout << "=================================================================" << std::endl;
  std::cout << "  Paper #253 Replication: Dones, Weissman, Levison, Duncan (2004)" << std::endl;
  std::cout << "  Oort Cloud Formation and Dynamics                             " << std::endl;
  std::cout << "  Comets II (Univ. of Arizona Press), pp. 153-174 (2004)        " << std::endl;
  std::cout << "=================================================================" << std::endl;

  hot_jupiter::Dones2004OortCloudModel model;

  // 1. Giant Planet Scattering Properties & Safronov Numbers
  std::cout << "\n--- 1. Planetary Scattering Parameters & Safronov Numbers ---" << std::endl;
  std::cout << std::fixed << std::setprecision(4);
  for (const char* planet : {"Jupiter", "Saturn", "Uranus", "Neptune"}) {
    double a_p = model.planet_semi_major_axis_au(planet);
    double m_p = model.planet_mass_kg(planet) / model.M_EARTH_KG;
    double theta = model.safronov_number(planet);
    double sigma_kick = model.rms_energy_kick_au_inv(planet);
    double n_enc_10k = model.encounters_to_reach_a(planet, 10000.0);
    double tau_clear = model.planetary_clearance_timescale_myr(planet);

    std::cout << std::setw(8) << planet
              << " | a = " << std::setw(6) << a_p << " AU"
              << " | M = " << std::setw(7) << m_p << " M_E"
              << " | Theta = " << std::setw(7) << theta
              << " | sigma_kick = " << std::scientific << std::setprecision(2) << sigma_kick << " AU^-1"
              << std::fixed << std::setprecision(1)
              << " | N_enc(10k) = " << std::setw(7) << n_enc_10k
              << " | tau_clear = " << std::setw(5) << tau_clear << " Myr"
              << std::endl;
  }

  // 2. Feeding Zone Fate Branching Fractions Sweep (CSV Export)
  std::ofstream csv_zones("replications_ss/paper_253/zone_fate_branching.csv");
  csv_zones << "planet_zone,a_inner_au,a_outer_au,primordial_mass_mearth,safronov_number,rms_energy_kick_au_inv,"
            << "f_ejection,f_oort_total,f_oort_inner,f_oort_outer,f_collision,f_scattered_disk,"
            << "m_ejected_mearth,m_oort_total_mearth,m_oort_inner_mearth,m_oort_outer_mearth\n";

  auto zone_fates = model.get_all_zone_fates(35.0);
  for (const auto& z : zone_fates) {
    std::string p_name = (z.planet_zone.find("Jupiter") != std::string::npos) ? "Jupiter" :
                         (z.planet_zone.find("Saturn") != std::string::npos) ? "Saturn" :
                         (z.planet_zone.find("Uranus") != std::string::npos) ? "Uranus" : "Neptune";
    double theta = model.safronov_number(p_name);
    double sig_kick = model.rms_energy_kick_au_inv(p_name);
    double m_ej = z.f_ejection * z.primordial_mass_mearth;
    double m_oc = z.f_oort_total * z.primordial_mass_mearth;
    double m_ioc = z.f_oort_inner * z.primordial_mass_mearth;
    double m_ooc = z.f_oort_outer * z.primordial_mass_mearth;

    csv_zones << "\"" << z.planet_zone << "\","
              << std::fixed << std::setprecision(1) << z.a_inner_au << ","
              << z.a_outer_au << ","
              << std::setprecision(2) << z.primordial_mass_mearth << ","
              << std::setprecision(4) << theta << ","
              << std::scientific << std::setprecision(4) << sig_kick << ","
              << std::fixed << std::setprecision(4) << z.f_ejection << ","
              << z.f_oort_total << ","
              << z.f_oort_inner << ","
              << z.f_oort_outer << ","
              << z.f_collision << ","
              << z.f_scattered_disk << ","
              << std::setprecision(3) << m_ej << ","
              << m_oc << ","
              << m_ioc << ","
              << m_ooc << "\n";
  }
  csv_zones.close();
  std::cout << "✅ Saved replications_ss/paper_253/zone_fate_branching.csv" << std::endl;

  // 3. Composite System-Wide Inventory (Dones et al. 2004 Multi-Planet Integration)
  auto inv = model.compute_inventory(35.0, 0.52);
  std::cout << "\n--- 2. Composite Multi-Planet Fate & Mass Inventory (M_disk = 35.0 M_Earth) ---" << std::endl;
  std::cout << std::fixed << std::setprecision(3);
  std::cout << "Total Primordial Disk Mass:    " << inv.total_disk_mass_mearth << " M_Earth" << std::endl;
  std::cout << "Total Ejected Mass (E > 0):    " << inv.m_ejected_mearth << " M_Earth (" << inv.f_ejection * 100.0 << "%)" << std::endl;
  std::cout << "Total Oort Cloud Trapped Mass: " << inv.m_oort_total_mearth << " M_Earth (" << inv.f_oort_total * 100.0 << "%)" << std::endl;
  std::cout << "  - Inner Oort Cloud (Hills):  " << inv.m_oort_inner_mearth << " M_Earth (" << inv.f_oort_inner * 100.0 << "%)" << std::endl;
  std::cout << "  - Outer Oort Cloud (Class):  " << inv.m_oort_outer_mearth << " M_Earth (" << inv.f_oort_outer * 100.0 << "%)" << std::endl;
  std::cout << "Physical Collisions (Sun/Pl):  " << inv.m_collision_mearth << " M_Earth (" << inv.f_collision * 100.0 << "%)" << std::endl;
  std::cout << "Scattered Disk / Kuiper Belt:  " << inv.m_scattered_disk_mearth << " M_Earth (" << inv.f_scattered_disk * 100.0 << "%)" << std::endl;
  std::cout << "Estimated Outer Comets (D>2km):" << std::scientific << inv.n_comets_outer_d_gt_2p3km << std::endl;
  std::cout << "Estimated Inner Comets (D>2km):" << std::scientific << inv.n_comets_inner_d_gt_2p3km << std::endl;
  std::cout << "Estimated Total Comets (D>2km):" << std::scientific << inv.n_comets_total_d_gt_2p3km << std::endl;

  // 4. Time Evolution of Oort Cloud Trapped Population (CSV Export)
  std::ofstream csv_time("replications_ss/paper_253/time_evolution_oort.csv");
  csv_time << "time_yr,time_myr,f_oort_trapped,f_ejected,f_remaining_scattered,m_oort_trapped_mearth\n";

  for (double log_t = 5.0; log_t <= 9.66; log_t += 0.05) {
    double t_yr = std::pow(10.0, log_t);
    double t_myr = t_yr / 1.0e6;
    double f_oc = model.oort_retention_fraction_at_time(t_yr);
    
    // Ejection evolution
    double f_ej = 0.875 * (1.0 - std::exp(-t_myr / 35.0));
    double f_scat = std::max(0.0, 1.0 - (f_oc + f_ej + 0.028));
    double m_oc = f_oc * 35.0;

    csv_time << std::scientific << std::setprecision(4) << t_yr << ","
             << std::fixed << std::setprecision(4) << t_myr << ","
             << f_oc << ","
             << f_ej << ","
             << f_scat << ","
             << m_oc << "\n";
  }
  csv_time.close();
  std::cout << "✅ Saved replications_ss/paper_253/time_evolution_oort.csv" << std::endl;

  // 5. Galactic Tide Perihelion Lifting & Semi-Major Axis Distribution (CSV Export)
  std::ofstream csv_tide("replications_ss/paper_253/galactic_tide_lifting.csv");
  csv_tide << "a_au,log10_a,dq_dt_tide_au_gyr,decoupling_timescale_gyr,trapping_probability,diff_mass_density_mearth_dex,inc_pdf_outer,inc_pdf_inner\n";

  for (double log_a = 1.5; log_a <= 5.1; log_a += 0.04) {
    double a_au = std::pow(10.0, log_a);
    double dq_dt = model.galactic_tide_perihelion_rate_au_gyr(a_au, 30.0, 45.0, 45.0);
    double tau_dec = model.galactic_tide_decoupling_timescale_gyr(a_au, 30.0, 36.0, 45.0, 45.0);
    double p_trap = model.oort_trapping_probability_vs_a(a_au);
    double dN_dlog = model.differential_semi_major_axis_density(a_au, 35.0);
    double pdf_out = model.inclination_pdf(45.0, "outer");
    double pdf_in = model.inclination_pdf(45.0, "inner");

    csv_tide << std::fixed << std::setprecision(2) << a_au << ","
             << std::setprecision(4) << log_a << ","
             << std::scientific << std::setprecision(4) << dq_dt << ","
             << tau_dec << ","
             << std::fixed << std::setprecision(4) << p_trap << ","
             << std::setprecision(4) << dN_dlog << ","
             << std::setprecision(5) << pdf_out << ","
             << std::setprecision(5) << pdf_in << "\n";
  }
  csv_tide.close();
  std::cout << "✅ Saved replications_ss/paper_253/galactic_tide_lifting.csv" << std::endl;

  // 6. Benchmark Validation against Dones et al. (2004) & Literature Datasets
  auto benchmarks = model.get_benchmark_catalog();
  std::ofstream csv_bench("replications_ss/paper_253/benchmark_validation.csv");
  csv_bench << "category,parameter_name,observed_reference,model_value,relative_error_pct,description\n";

  double ss_res = 0.0;
  double ss_tot = 0.0;
  double sum_obs = 0.0;

  for (const auto& b : benchmarks) {
    sum_obs += b.observed_or_dones2004_value;
  }
  double mean_obs = sum_obs / benchmarks.size();

  for (const auto& b : benchmarks) {
    double err_pct = std::abs(b.model_predicted_value - b.observed_or_dones2004_value) /
                     std::max(1e-6, std::abs(b.observed_or_dones2004_value)) * 100.0;
    ss_res += (b.model_predicted_value - b.observed_or_dones2004_value) *
              (b.model_predicted_value - b.observed_or_dones2004_value);
    ss_tot += (b.observed_or_dones2004_value - mean_obs) *
              (b.observed_or_dones2004_value - mean_obs);

    csv_bench << "\"" << b.category << "\",\""
              << b.parameter_name << "\","
              << std::fixed << std::setprecision(4) << b.observed_or_dones2004_value << ","
              << b.model_predicted_value << ","
              << std::setprecision(3) << err_pct << ",\""
              << b.description << "\"\n";
  }
  csv_bench.close();
  std::cout << "✅ Saved replications_ss/paper_253/benchmark_validation.csv" << std::endl;

  double r2 = (ss_tot > 0.0) ? (1.0 - ss_res / ss_tot) : 1.0;
  std::cout << "\n=================================================================" << std::endl;
  std::cout << "  REPLICATION BENCHMARK SUMMARY (Dones et al. 2004)              " << std::endl;
  std::cout << "  Total Benchmark Parameters Evaluated: " << benchmarks.size() << std::endl;
  std::cout << "  Coefficient of Determination R^2:    " << std::setprecision(6) << r2 << std::endl;
  std::cout << "  Target Threshold (R^2 >= 0.98):       " << (r2 >= 0.98 ? "PASSED ✅" : "FAILED ❌") << std::endl;
  std::cout << "=================================================================" << std::endl;

  return 0;
}
