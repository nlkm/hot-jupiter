"""
End-to-End Incremental Population Study: Analyzing Radius Distribution Progression across 6 Physical Stages.
"""

import os
import matplotlib.pyplot as plt

from hot_jupiter.population import PopulationSimulator, get_curated_hot_jupiter_catalog


def main():
    print("=== Incremental Hot Jupiter Population Synthesis Study ===")

    catalog = get_curated_hot_jupiter_catalog()
    print(f"Loaded catalog of {len(catalog)} Hot Jupiter exoplanet systems.\n")

    simulator = PopulationSimulator(catalog=catalog, k2_over_Q=2.0e-5)
    result = simulator.run_incremental_simulation()

    print("Stage Summary Table:")
    print("-" * 85)
    print(f"{'Model Stage':<42} | {'Mean Radius [R_Jup]':<20} | {'KS Stat D':<10} | {'p-value':<8}")
    print("-" * 85)

    for stage_key, stats in result.stage_results.items():
        print(f"{stats.name:<42} | {stats.mean_R:.2f} +/- {stats.std_R:.2f}         | {stats.ks_stat:.3f}      | {stats.p_value:.4f}")

    print("-" * 85)
    print(f"{'Observed Catalog':<42} | {result.R_obs_jup.mean():.2f} +/- {result.R_obs_jup.std():.2f}         | --         | --")
    print("-" * 85)

    output_dir = "outputs"
    os.makedirs(output_dir, exist_ok=True)
    fig_path = os.path.join(output_dir, "hot_jupiter_incremental_ks_comparison.pdf")
    fig = simulator.plot_incremental_stages(result, savepath=fig_path)
    plt.close(fig)

    print(f"\nPlot saved to {fig_path}.")


if __name__ == "__main__":
    main()
