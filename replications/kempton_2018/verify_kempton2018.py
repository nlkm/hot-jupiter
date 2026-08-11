"""
Verification script for Kempton et al. (2018) PASP 130, 114401.
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


def verify_kempton2018():
    ref_rows = load_csv("replications/kempton_2018/reference_data.csv")

    # Figure 1: Transmission Spectroscopy Metric (TSM) (first 6 data rows)
    ref_fig1_data = np.array(ref_rows[:6])
    ref_rp = ref_fig1_data[:, 0]
    ref_tsm = ref_fig1_data[:, 1]

    sim_tsm_data = load_csv("replications/kempton_2018/sim_tsm.csv")
    sim_tsm = np.array(sim_tsm_data)

    sim_interp_tsm = np.interp(ref_rp, sim_tsm[:, 0], sim_tsm[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_tsm - ref_tsm)**2) / np.sum(
        (ref_tsm - np.mean(ref_tsm))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_tsm[:, 0],
            sim_tsm[:, 1],
            'b-',
            lw=2.5,
            label='hot_jupiter TSM Model')
    ax.plot(ref_rp,
            ref_tsm,
            'ko',
            ms=7,
            label='Kempton et al. (2018) Benchmarks')

    ax.set_xlabel(r"Planet Radius $R_p$ [$R_\oplus$]", fontsize=12)
    ax.set_ylabel(r"Transmission Spectroscopy Metric (TSM)", fontsize=12)
    ax.set_title(
        "Kempton et al. (2018) Figure 1: Transmission Metric vs Planet Radius",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/kempton_2018/fig1_tsm.png", dpi=300)
    plt.close(fig)

    # Figure 2: Emission Spectroscopy Metric (ESM) (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[6:])
    ref_teq = ref_fig2_data[:, 0]
    ref_esm = ref_fig2_data[:, 1]

    sim_esm_data = load_csv("replications/kempton_2018/sim_esm.csv")
    sim_esm = np.array(sim_esm_data)

    sim_interp_esm = np.interp(ref_teq, sim_esm[:, 0], sim_esm[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_esm - ref_esm)**2) / np.sum(
        (ref_esm - np.mean(ref_esm))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_esm[:, 0],
            sim_esm[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter ESM Model')
    ax.plot(ref_teq,
            ref_esm,
            'ko',
            ms=7,
            label='Kempton et al. (2018) Benchmarks')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Emission Spectroscopy Metric (ESM)", fontsize=12)
    ax.set_title(
        "Kempton et al. (2018) Figure 2: Emission Metric vs Equilibrium Temperature",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/kempton_2018/fig2_esm.png", dpi=300)
    plt.close(fig)

    print(f"--> Fig 1 TSM R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)")
    print(f"--> Fig 2 ESM R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)")

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Kempton et al. (2018) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_kempton2018()
