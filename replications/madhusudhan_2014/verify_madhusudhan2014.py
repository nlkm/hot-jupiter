"""
Verification script for Madhusudhan et al. (2014) Space Sci Rev 186, 269.
Replicates Figures 1 & 2 using hot_jupiter library and C++ solver data.
Calculates statistical R^2 agreement for all published figures.
"""

import csv

import matplotlib.pyplot as plt
import numpy as np


def load_csv(filename):
    data = []
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        for row in reader:
            if not row or row[0].startswith('#'):
                continue
            try:
                data.append([float(x) for x in row])
            except ValueError:
                continue
    return data


def verify_madhusudhan2014():
    ref_rows = load_csv("replications/madhusudhan_2014/reference_data.csv")

    # Figure 1: Abundance vs Temperature (Solar C/O=0.5)
    ref_fig1_data = np.array([r for r in ref_rows if len(r) == 5])
    ref_t = ref_fig1_data[:, 0]
    ref_h2o = ref_fig1_data[:, 1]

    sim_temp_data = load_csv(
        "replications/madhusudhan_2014/sim_temp_abundance.csv")
    sim_temp = np.array(sim_temp_data)

    sim_interp_h2o = np.interp(ref_t, sim_temp[:, 0], sim_temp[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_h2o - ref_h2o)**2) /
                     np.sum((ref_h2o - np.mean(ref_h2o))**2 + 1e-10))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_temp[:, 0],
            sim_temp[:, 1],
            'b-',
            lw=2.5,
            label=r'$H_2O$ (Model)')
    ax.plot(sim_temp[:, 0],
            sim_temp[:, 2],
            'g--',
            lw=2.0,
            label=r'$CO$ (Model)')
    ax.plot(sim_temp[:, 0],
            sim_temp[:, 3],
            'r-.',
            lw=2.0,
            label=r'$CH_4$ (Model)')
    ax.plot(ref_t, ref_h2o, 'ko', ms=7, label='Madhusudhan (2014) Ref')

    ax.set_xlabel("Temperature [K]", fontsize=12)
    ax.set_ylabel(r"Mixing Ratio $\log_{10}(X_i)$", fontsize=12)
    ax.set_title(
        "Madhusudhan et al. (2014) Figure 1: Equilibrium Abundance vs Temp",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/madhusudhan_2014/fig1_temp_abundance.png",
                dpi=300)
    plt.close(fig)

    # Figure 2: Water Mixing Ratio vs C/O Ratio
    ref_fig2_data = np.array([r for r in ref_rows if len(r) == 2])
    ref_co = ref_fig2_data[:, 0]
    ref_h2o_co = ref_fig2_data[:, 1]

    sim_co_data = load_csv("replications/madhusudhan_2014/sim_water_vs_co.csv")
    sim_co = np.array(sim_co_data)

    sim_interp_co_h2o = np.interp(ref_co, sim_co[:, 0], sim_co[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_co_h2o - ref_h2o_co)**2) / np.sum(
        (ref_h2o_co - np.mean(ref_h2o_co))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_co[:, 0], sim_co[:, 1], 'r-', lw=2.5, label='hot_jupiter Model')
    ax.plot(ref_co, ref_h2o_co, 'ko', ms=7, label='Madhusudhan et al. (2014)')

    ax.axvline(1.0, color='gray', linestyle=':', label='C/O = 1.0 Transition')
    ax.set_xlabel("C/O Ratio", fontsize=12)
    ax.set_ylabel(r"Water Abundance $\log_{10}(X_{H_2O})$", fontsize=12)
    ax.set_title(
        "Madhusudhan et al. (2014) Figure 2: Water Abundance vs C/O Ratio",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/madhusudhan_2014/fig2_water_vs_co.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Temp Abundance R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Water vs C/O R^2 Score:    {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Madhusudhan et al. (2014) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_madhusudhan2014()
