// Solver for Paper #14: Type I Protoplanetary Disk Gas Migration (Ward 1997, Walsh et al. 2011)
// Evaluates inward migration timescales for planetary cores in gas disks.

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "planet_formation.hpp"

int main() {
  std::cout << "=== Running Ward (1997) & Walsh et al. (2011) Type I Migration Solver ===" << std::endl;

  hot_jupiter::DiskMigrationModel migration_model;

  std::ofstream csv_file("replications_ss/paper_014/type1_migration_timescale.csv");
  csv_file << "mass_earth,tau_mig_1au_yr,tau_mig_5au_yr\n";

  // Planetary masses from 0.1 M_earth to 10.0 M_earth
  for (double m_earth = 0.1; m_earth <= 10.0; m_earth += 0.5) {
    double m_kg = m_earth * 5.972e24;
    double tau_1au = migration_model.type_i_migration_timescale_yr(m_kg, 1.0 * hot_jupiter::AU);
    double tau_5au = migration_model.type_i_migration_timescale_yr(m_kg, 5.0 * hot_jupiter::AU);

    csv_file << std::fixed << std::setprecision(4) << m_earth << "," << std::scientific << tau_1au << "," << tau_5au << "\n";
  }

  csv_file.close();
  std::cout << "✅ Generated replications_ss/paper_014/type1_migration_timescale.csv" << std::endl;
  return 0;
}
