"""
Verification script for Fortney et al. (2020) AJ 160, 288.
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


def verify_fortney2020():
    ref_rows = load_csv("replications/fortney_2020/reference_data.csv")

    # Figure 1: H2 Dissociation Fraction vs Pressure P (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_p1 = ref_fig1_data[:, 0]
    ref_alpha1 = ref_fig1_data[:, 1]

    sim_alpha_data = load_csv(
        "replications/fortney_2020/sim_h2_dissociation.csv")
    sim_alpha = np.array(sim_alpha_data)

    sim_interp_alpha = np.interp(np.log10(ref_p1), np.log10(sim_alpha[:, 0]),
                                 sim_alpha[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_alpha - ref_alpha1)**2) / np.sum(
        (ref_alpha1 - np.mean(ref_alpha1))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_alpha[:, 0],
            sim_alpha[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter Model ($T=3000\,\mathrm{K}$)')
    ax.plot(ref_p1,
            ref_alpha1,
            'ko',
            ms=7,
            label='Fortney et al. (2020) Dissociation')

    ax.set_xscale('log')
    ax.set_xlabel(r"Atmospheric Pressure $P$ [bar]", fontsize=12)
    ax.set_ylabel(
        r"$\mathrm{H}_2$ Dissociation Fraction $\alpha_{\mathrm{dissoc}}$",
        fontsize=12)
    ax.set_title(
        "Fortney et al. (2020) Figure 1: H2 Dissociation Fraction vs Pressure",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/fortney_2020/fig1_h2_dissociation.png", dpi=300)
    plt.close(fig)

    # Figure 2: Temperature Profile T(P) vs Pressure P (next 8 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_p2 = ref_fig2_data[:, 0]
    ref_t2 = ref_fig2_data[:, 1]

    sim_tp_data = load_csv("replications/fortney_2020/sim_thermal_profile.csv")
    sim_tp = np.array(sim_tp_data)

    sim_interp_t = np.interp(np.log10(ref_p2), np.log10(sim_tp[:, 0]),
                             sim_tp[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_t - ref_t2)**2) / np.sum(
        (ref_t2 - np.mean(ref_t2))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_tp[:, 1],
            sim_tp[:, 0],
            'r-',
            lw=2.5,
            label=r'hot_jupiter $T(P)$ with $\mathrm{H}^-$ Opacity')
    ax.plot(ref_t2,
            ref_p2,
            'ko',
            ms=7,
            label='Fortney et al. (2020) Climate Model')

    ax.set_yscale('log')
    ax.invert_yaxis()
    ax.set_xlabel(r"Temperature $T$ [K]", fontsize=12)
    ax.set_ylabel(r"Pressure $P$ [bar]", fontsize=12)
    ax.set_title(
        "Fortney et al. (2020) Figure 2: Ultra-Hot Jupiter Thermal Profile",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/fortney_2020/fig2_thermal_profile.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 H2 Dissociation R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 T(P) Profile R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)")

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Fortney et al. (2020) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_fortney2020()
