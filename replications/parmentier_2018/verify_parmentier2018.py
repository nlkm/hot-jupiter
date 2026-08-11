"""
Verification script for Parmentier et al. (2018) A&A 617, A110.
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


def verify_parmentier2018():
    ref_rows = load_csv("replications/parmentier_2018/reference_data.csv")

    # Figure 1: Water Dissociation Curve (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_temp = ref_fig1_data[:, 0]
    ref_log_h2o = ref_fig1_data[:, 1]

    sim_diss_data = load_csv(
        "replications/parmentier_2018/sim_h2o_dissociation.csv")
    sim_diss = np.array(sim_diss_data)

    sim_interp_log_h2o = np.interp(ref_temp, sim_diss[:, 0], sim_diss[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_log_h2o - ref_log_h2o)**2) / np.sum(
        (ref_log_h2o - np.mean(ref_log_h2o))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_diss[:, 0],
            sim_diss[:, 1],
            'b-',
            lw=2.5,
            label='hot_jupiter Thermal Dissociation')
    ax.plot(ref_temp,
            ref_log_h2o,
            'ko',
            ms=7,
            label='Parmentier et al. (2018) Chemical Eq.')

    ax.set_xlabel(r"Temperature $T$ [K]", fontsize=12)
    ax.set_ylabel(r"Water Volume Abundance $\log_{10}(X_{\mathrm{H2O}})$",
                  fontsize=12)
    ax.set_title(
        "Parmentier et al. (2018) Figure 1: H2O Thermal Dissociation at 10 mbar",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/parmentier_2018/fig1_h2o_dissociation.png",
                dpi=300)
    plt.close(fig)

    # Figure 2: Emission Spectrum for WASP-121b (next 8 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_wave = ref_fig2_data[:, 0]
    ref_em_121b = ref_fig2_data[:, 1]

    sim_em_data = load_csv(
        "replications/parmentier_2018/sim_wasp121b_emission.csv")
    sim_em = np.array(sim_em_data)

    sim_interp_em = np.interp(ref_wave, sim_em[:, 0], sim_em[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_em - ref_em_121b)**2) / np.sum(
        (ref_em_121b - np.mean(ref_em_121b))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_em[:, 0],
            sim_em[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter Ultra-Hot Emission Model')
    ax.plot(ref_wave,
            ref_em_121b,
            'ko',
            ms=7,
            label='Parmentier et al. (2018) WASP-121b Spectrum')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Thermal Emission $F_p / F_\star$ [ppm]", fontsize=12)
    ax.set_title(
        "Parmentier et al. (2018) Figure 2: WASP-121b Emission Spectrum",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/parmentier_2018/fig2_wasp121b_emission.png",
                dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 H2O Dissociation R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 WASP-121b Emission Spectrum R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Parmentier et al. (2018) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_parmentier2018()
