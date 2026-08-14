// Copyright 2026 Antigravity Scientific Automation & Solar System Replication Campaign
// First-principles replication of Walsh et al. (2011, 2012), Levison et al. (2008), Brasser et al. (2010, 2012), Dones et al. (2004, 2015)
// Populating the Kuiper Belt and Oort Cloud during Planetary Migration

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "cpp/include/constants.hpp"
#include "cpp/include/solar_system.hpp"

int main() {
  hot_jupiter::PlanetesimalMigrationScatteringModel model;

  std::cout << "============================================================================" << std::endl;
  std::cout << "Paper #230: Walsh et al. (2012) Planetesimal Scattering & Oort Cloud Solver" << std::endl;
  std::cout << "============================================================================" << std::endl;

  std::cout << std::fixed << std::setprecision(4);
  std::cout << "Initial Planetesimal Disk Mass:  " << hot_jupiter::PlanetesimalMigrationScatteringModel::M_DISK_PRIMORDIAL_MEARTH << " M_Earth" << std::endl;
  std::cout << "Nominal Migration Timescale:     " << hot_jupiter::PlanetesimalMigrationScatteringModel::TAU_MIG_NOMINAL_MYR << " Myr" << std::endl;
  std::cout << "Local Galactic Tide Density:     " << hot_jupiter::PlanetesimalMigrationScatteringModel::RHO_GALACTIC_TIDE << " M_sun / pc^3" << std::endl;
  std::cout << "----------------------------------------------------------------------------" << std::endl;

  // 1. Planetary Scattering Energetics & Safronov Parameters
  std::cout << "\n[1] Giant Planet Dynamical Scattering Characteristics:" << std::endl;
  std::cout << std::setw(12) << "Planet"
            << std::setw(14) << "Mass [M_J]"
            << std::setw(14) << "a_init [AU]"
            << std::setw(14) << "a_final [AU]"
            << std::setw(14) << "Safronov Theta"
            << std::setw(18) << "RMS d(1/a) [AU^-1]"
            << std::endl;

  std::vector<std::pair<std::string, std::pair<double, double>>> planets = {
      {"Jupiter", {model.A_JUPITER_INIT_AU, model.A_JUPITER_FINAL_AU}},
      {"Saturn", {model.A_SATURN_INIT_AU, model.A_SATURN_FINAL_AU}},
      {"Uranus", {model.A_URANUS_INIT_AU, model.A_URANUS_FINAL_AU}},
      {"Neptune", {model.A_NEPTUNE_INIT_AU, model.A_NEPTUNE_FINAL_AU}}};

  for (const auto& p : planets) {
    std::string name = p.first;
    double a_init = p.second.first;
    double a_final = p.second.second;
    double mass_mj = model.planet_mass_kg(name) / model.M_JUPITER;
    double theta = model.safronov_number(name, a_final);
    double rms_kick = model.rms_energy_kick_au_inv(name, a_final);

    std::cout << std::setw(12) << name
              << std::setw(14) << std::setprecision(4) << mass_mj
              << std::setw(14) << std::setprecision(3) << a_init
              << std::setw(14) << std::setprecision(3) << a_final
              << std::setw(14) << std::setprecision(2) << theta
              << std::setw(18) << std::scientific << std::setprecision(3) << rms_kick << std::fixed
              << std::endl;
  }

  // 2. Migration Tracks CSV Export
  std::ofstream csv_mig("replications_ss/paper_230/planetary_migration_tracks.csv");
  csv_mig << "time_myr,a_jupiter_au,a_saturn_au,a_uranus_au,a_neptune_au,neptune_kuiper_edge_au\n";
  for (double t = 0.0; t <= 50.0; t += 0.25) {
    double a_j = model.planet_semi_major_axis_au("Jupiter", t);
    double a_s = model.planet_semi_major_axis_au("Saturn", t);
    double a_u = model.planet_semi_major_axis_au("Uranus", t);
    double a_n = model.planet_semi_major_axis_au("Neptune", t);
    double edge_kb = a_n * 1.6; // Approximate 2:1 resonance outer boundary
    csv_mig << std::fixed << std::setprecision(2) << t << ","
            << std::setprecision(4) << a_j << "," << a_s << ","
            << a_u << "," << a_n << "," << edge_kb << "\n";
  }
  csv_mig.close();
  std::cout << "✅ Saved replications_ss/paper_230/planetary_migration_tracks.csv" << std::endl;

  // 3. Oort Cloud Capture Efficiency & Galactic Tide Perihelion Lifting
  std::ofstream csv_oort("replications_ss/paper_230/oort_capture_efficiency.csv");
  csv_oort << "semimajor_axis_au,log10_a,dq_dt_au_gyr,capture_prob,is_decoupled_flag\n";

  for (double log_a = 2.0; log_a <= 5.2; log_a += 0.02) {
    double a_au = std::pow(10.0, log_a);
    double dq_dt = model.galactic_tide_perihelion_rate_au_gyr(a_au);
    double p_cap = model.oort_capture_probability(a_au);
    bool decoupled = (dq_dt >= 5.0 && a_au <= 60000.0);

    csv_oort << std::fixed << std::setprecision(1) << a_au << ","
             << std::setprecision(4) << log_a << ","
             << std::setprecision(4) << dq_dt << ","
             << std::setprecision(5) << p_cap << ","
             << (decoupled ? 1 : 0) << "\n";
  }
  csv_oort.close();
  std::cout << "✅ Saved replications_ss/paper_230/oort_capture_efficiency.csv" << std::endl;

  // 4. Semi-major Axis Distribution CSV Export
  std::ofstream csv_dist("replications_ss/paper_230/semimajor_axis_distribution.csv");
  csv_dist << "semimajor_axis_au,log10_a,dn_dloga_mearth,dn_da_norm,reservoir_zone\n";

  for (double log_a = 1.48; log_a <= 5.08; log_a += 0.02) {
    double a_au = std::pow(10.0, log_a);
    double dn_dlog = model.differential_semi_major_axis_density(a_au);
    double dn_da = dn_dlog / (a_au * std::log(10.0));
    std::string zone = "Scattered_Disk";
    if (a_au >= 1000.0 && a_au < 20000.0) {
      zone = "Inner_Oort_Cloud";
    } else if (a_au >= 20000.0) {
      zone = "Outer_Oort_Cloud";
    }

    csv_dist << std::fixed << std::setprecision(2) << a_au << ","
             << std::setprecision(4) << log_a << ","
             << std::setprecision(5) << dn_dlog << ","
             << std::scientific << std::setprecision(5) << dn_da << std::fixed << ","
             << zone << "\n";
  }
  csv_dist.close();
  std::cout << "✅ Saved replications_ss/paper_230/semimajor_axis_distribution.csv" << std::endl;

  // 5. Migration Timescale Parameter Sweep
  std::ofstream csv_tau("replications_ss/paper_230/migration_timescale_sweep.csv");
  csv_tau << "tau_mig_myr,f_ejection,f_oort_total,f_oort_inner,f_oort_outer,f_kuiper,f_resonant,f_collision,f_asteroid,m_oort_mearth,m_kuiper_mearth\n";

  std::cout << "\n[2] Migration Timescale Parameter Sweep (M_disk = 30 M_Earth):" << std::endl;
  std::cout << std::setw(12) << "tau [Myr]"
            << std::setw(14) << "f_Eject [%]"
            << std::setw(14) << "f_Oort [%]"
            << std::setw(14) << "f_Kuiper [%]"
            << std::setw(14) << "f_Resonant [%]"
            << std::setw(16) << "M_Oort [M_E]"
            << std::setw(16) << "M_KB [M_E]"
            << std::endl;

  for (double tau = 1.0; tau <= 40.0; tau += 1.0) {
    auto f = model.planetesimal_fate_fractions(tau, 30.0);
    auto m = model.reservoir_mass_inventories(tau, 30.0);

    if (static_cast<int>(tau) % 5 == 0 || tau == 1.0 || tau == 10.0) {
      std::cout << std::setw(12) << std::setprecision(1) << tau
                << std::setw(14) << std::setprecision(2) << f.f_ejection * 100.0
                << std::setw(14) << std::setprecision(2) << f.f_oort_total * 100.0
                << std::setw(14) << std::setprecision(2) << f.f_kuiper_belt * 100.0
                << std::setw(14) << std::setprecision(3) << f.f_resonant * 100.0
                << std::setw(16) << std::setprecision(3) << m.m_oort_total
                << std::setw(16) << std::setprecision(3) << m.m_kuiper_scattered
                << std::endl;
    }

    csv_tau << std::fixed << std::setprecision(1) << tau << ","
            << std::setprecision(5) << f.f_ejection << ","
            << f.f_oort_total << "," << f.f_oort_inner << "," << f.f_oort_outer << ","
            << f.f_kuiper_belt << "," << f.f_resonant << ","
            << f.f_collision << "," << f.f_asteroid_belt << ","
            << m.m_oort_total << "," << m.m_kuiper_scattered << "\n";
  }
  csv_tau.close();
  std::cout << "✅ Saved replications_ss/paper_230/migration_timescale_sweep.csv" << std::endl;

  // 6. Resonance Trapping Efficiency Sweep
  std::ofstream csv_res("replications_ss/paper_230/resonance_trapping_sweep.csv");
  csv_res << "initial_eccentricity,p_trap_3_2,p_trap_2_1,p_trap_5_3,p_trap_7_4\n";

  for (double e = 0.001; e <= 0.25; e += 0.002) {
    double p_32 = model.kuiper_resonance_capture_probability(e, 10.0, "3:2");
    double p_21 = model.kuiper_resonance_capture_probability(e, 10.0, "2:1");
    double p_53 = model.kuiper_resonance_capture_probability(e, 10.0, "5:3");
    double p_74 = model.kuiper_resonance_capture_probability(e, 10.0, "7:4");

    csv_res << std::fixed << std::setprecision(4) << e << ","
            << std::setprecision(5) << p_32 << "," << p_21 << ","
            << p_53 << "," << p_74 << "\n";
  }
  csv_res.close();
  std::cout << "✅ Saved replications_ss/paper_230/resonance_trapping_sweep.csv" << std::endl;

  std::cout << "\n============================================================================" << std::endl;
  std::cout << "  Paper #230 Replication Solver Completed Successfully!                   " << std::endl;
  std::cout << "============================================================================" << std::endl;

  return 0;
}
