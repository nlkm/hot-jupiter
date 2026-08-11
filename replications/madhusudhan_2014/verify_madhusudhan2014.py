"""
Verification script for Madhusudhan et al. (2014) ApJ Letters 791, L9.
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

    # Figure 1: Water Abundance Posterior (first 6 data rows)
    ref_fig1_data = np.array(ref_rows[:6])
    ref_logx = ref_fig1_data[:, 0]
    ref_prob = ref_fig1_data[:, 1]

    sim_h2o_data = load_csv(
        "replications/madhusudhan_2014/sim_h2o_abundance.csv")
    sim_h2o = np.array(sim_h2o_data)

    sim_interp_prob = np.interp(ref_logx, sim_h2o[:, 0], sim_h2o[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_prob - ref_prob)**2) / np.sum(
        (ref_prob - np.mean(ref_prob))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_h2o[:, 0],
            sim_h2o[:, 1],
            'b-',
            lw=2.5,
            label='hot_jupiter Model Posterior')
    ax.plot(ref_logx,
            ref_prob,
            'ko',
            ms=7,
            label='Madhusudhan et al. (2014) Data')

    ax.set_xlabel(r"$\log_{10} X_{\mathrm{H}_2\mathrm{O}}$ Abundance",
                  fontsize=12)
    ax.set_ylabel(r"Probability Density", fontsize=12)
    ax.set_title(
        "Madhusudhan et al. (2014) Figure 1: HD 209458b Water Abundance Posterior",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/madhusudhan_2014/fig1_h2o_posterior.png", dpi=300)
    plt.close(fig)

    # Figure 2: C/O Ratio Posterior (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[6:])
    ref_co = ref_fig2_data[:, 0]
    ref_co_prob = ref_fig2_data[:, 1]

    sim_co_data = load_csv("replications/madhusudhan_2014/sim_co_ratio.csv")
    sim_co = np.array(sim_co_data)

    sim_interp_co_prob = np.interp(ref_co, sim_co[:, 0], sim_co[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_co_prob - ref_co_prob)**2) / np.sum(
        (ref_co_prob - np.mean(ref_co_prob))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_co[:, 0],
            sim_co[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter C/O Posterior')
    ax.plot(ref_co,
            ref_co_prob,
            'ko',
            ms=7,
            label='Madhusudhan et al. (2014) C/O')

    ax.set_xlabel(r"Carbon-to-Oxygen Ratio (C/O)", fontsize=12)
    ax.set_ylabel(r"Probability Density", fontsize=12)
    ax.set_title(
        "Madhusudhan et al. (2014) Figure 2: Hot Jupiter C/O Ratio Posterior",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/madhusudhan_2014/fig2_co_posterior.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 H2O Posterior R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 C/O Posterior R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Madhusudhan et al. (2014) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_madhusudhan2014()
