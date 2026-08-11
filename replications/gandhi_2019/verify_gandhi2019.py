"""
Verification script for Gandhi & Madhusudhan (2019) MNRAS 485, 5817.
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


def verify_gandhi2019():
    ref_rows = load_csv("replications/gandhi_2019/reference_data.csv")

    # Figure 1: Volume Mixing Ratios log10(X_H2O) and log10(X_CO) vs Teq (first 5 data rows)
    ref_fig1_data = np.array(ref_rows[:5])
    ref_teq = ref_fig1_data[:, 0]
    ref_xh2o = ref_fig1_data[:, 1]
    ref_xco = ref_fig1_data[:, 2]

    sim_abund_data = load_csv("replications/gandhi_2019/sim_abundances.csv")
    sim_abund = np.array(sim_abund_data)

    sim_interp_h2o = np.interp(ref_teq, sim_abund[:, 0], sim_abund[:, 1])
    sim_interp_co = np.interp(ref_teq, sim_abund[:, 0], sim_abund[:, 2])

    r2_fig1_h2o = 1.0 - (np.sum((sim_interp_h2o - ref_xh2o)**2) / np.sum(
        (ref_xh2o - np.mean(ref_xh2o))**2))
    r2_fig1_co = 1.0 - (np.sum((sim_interp_co - ref_xco)**2) / np.sum(
        (ref_xco - np.mean(ref_xco))**2))
    r2_fig1 = (r2_fig1_h2o + r2_fig1_co) / 2.0

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_abund[:, 0],
            sim_abund[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter $\log_{10} X_{\mathrm{H2O}}$')
    ax.plot(sim_abund[:, 0],
            sim_abund[:, 2],
            'r--',
            lw=2.5,
            label=r'hot_jupiter $\log_{10} X_{\mathrm{CO}}$')
    ax.plot(ref_teq, ref_xh2o, 'bo', ms=7, label='Gandhi (2019) H2O Retrieval')
    ax.plot(ref_teq, ref_xco, 'ro', ms=7, label='Gandhi (2019) CO Retrieval')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Volume Mixing Ratio $\log_{10} X_i$", fontsize=12)
    ax.set_title(
        "Gandhi & Madhusudhan (2019) Figure 1: Atmospheric Abundances vs Temperature",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/gandhi_2019/fig1_abundances.png", dpi=300)
    plt.close(fig)

    # Figure 2: Retrieved C/O Ratio vs Teq (next 5 data rows)
    ref_fig2_data = np.array(ref_rows[5:])
    ref_teq_co = ref_fig2_data[:, 0]
    ref_co = ref_fig2_data[:, 1]

    sim_co_data = load_csv("replications/gandhi_2019/sim_co_ratio.csv")
    sim_co = np.array(sim_co_data)

    sim_interp_co_ratio = np.interp(ref_teq_co, sim_co[:, 0], sim_co[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_co_ratio - ref_co)**2) / np.sum(
        (ref_co - np.mean(ref_co))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_co[:, 0],
            sim_co[:, 1],
            'k-',
            lw=2.5,
            label='hot_jupiter C/O Ratio Model')
    ax.plot(ref_teq_co,
            ref_co,
            'ko',
            ms=7,
            label='Gandhi & Madhusudhan (2019) Retrieval')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Carbon-to-Oxygen Ratio $\mathrm{C/O}$", fontsize=12)
    ax.set_title(
        "Gandhi & Madhusudhan (2019) Figure 2: Retrieved C/O Ratio vs Temperature",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/gandhi_2019/fig2_co_ratio.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Volume Mixing Ratios R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(f"--> Fig 2 C/O Ratio R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)")

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Gandhi & Madhusudhan (2019) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_gandhi2019()
