// C++ Standalone Replication Solver for Line et al. (2013) ApJ 775, 137
// Calls core library class hot_jupiter::LineRetrievalMultiGas from atmosphere.hpp.

#include <cmath>
#include <fstream>
#include <iostream>

#include "atmosphere.hpp"
#include "constants.hpp"

namespace hot_jupiter {

void run_chemical_retrieval_sweep(const std::string& output_csv) {
  LineRetrievalMultiGas model;
  std::ofstream out(output_csv);
  out << "molecule_index,log10_mixing_ratio_median,log10_mixing_ratio_upper,log10_mixing_ratio_lower\n";

  for (double mol_idx = 1.0; mol_idx <= 4.0; mol_idx += 1.0) {
    double med, up, low;
    model.abundance_posteriors(mol_idx, med, up, low);
    out << mol_idx << "," << med << "," << up << "," << low << "\n";
  }
  out.close();
  std::cout << "--> Wrote Line et al. (2013) Chemical Retrieval dataset to " << output_csv << std::endl;
}

void run_spectrum_retrieval_sweep(const std::string& output_csv) {
  LineRetrievalMultiGas model;
  std::ofstream out(output_csv);
  out << "wavelength_micron,flux_ratio_pct\n";

  for (double wave = 3.0; wave <= 9.0; wave += 0.1) {
    double ratio = model.eclipse_flux_ratio_pct(wave);
    out << wave << "," << ratio << "\n";
  }
  out.close();
  std::cout << "--> Wrote Line et al. (2013) Spectrum Retrieval dataset to " << output_csv << std::endl;
}

}  // namespace hot_jupiter

int main() {
  std::cout << "=== Line et al. (2013) C++ Chemical Retrieval Solver ===" << std::endl;
  hot_jupiter::run_chemical_retrieval_sweep("replications/line_2013/sim_abundance_retrieval.csv");
  hot_jupiter::run_spectrum_retrieval_sweep("replications/line_2013/sim_spectrum_retrieval.csv");
  std::cout << "✅ Line et al. (2013) C++ Datasets Generated Successfully!" << std::endl;
  return 0;
}
