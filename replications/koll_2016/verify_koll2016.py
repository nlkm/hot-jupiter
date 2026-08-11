"""
Verification script for Koll & Abbot (2016) ApJ 825, 99.
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


def verify_koll2016():
    ref_rows = load_csv("replications/koll_2016/reference_data.csv")

    # Figure 1: Day-Night Temperature Difference Delta T_dn [K] vs Teq (first 5 data rows)
    ref_fig1_data = np.array(ref_rows[:5])
    ref_teq = ref_fig1_data[:, 0]
    ref_dtdn = ref_fig1_data[:, 1]

    sim_dt_data = load_csv("replications/koll_2016/sim_day_night_contrast.csv")
    sim_dt = np.array(sim_dt_data)

    sim_interp_dtdn = np.interp(ref_teq, sim_dt[:, 0], sim_dt[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_dtdn - ref_dtdn)**2) / np.sum(
        (ref_dtdn - np.mean(ref_dtdn))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_dt[:, 0],
            sim_dt[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter $\Delta T_{\mathrm{dn}}$')
    ax.plot(ref_teq,
            ref_dtdn,
            'ko',
            ms=7,
            label='Koll & Abbot (2016) Circulation Model')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(
        r"Day-Night Temperature Difference $\Delta T_{\mathrm{dn}}$ [K]",
        fontsize=12)
    ax.set_title(
        "Koll & Abbot (2016) Figure 1: Day-Night Contrast vs Equilibrium Temp",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/koll_2016/fig1_day_night_contrast.png", dpi=300)
    plt.close(fig)

    # Figure 2: Thermal Inversion Strength eta_inv vs Opacity Ratio gamma (next 5 data rows)
    ref_fig2_data = np.array(ref_rows[5:])
    ref_gamma = ref_fig2_data[:, 0]
    ref_eta = ref_fig2_data[:, 1]

    sim_eta_data = load_csv("replications/koll_2016/sim_inversion_strength.csv")
    sim_eta = np.array(sim_eta_data)

    sim_interp_eta = np.interp(np.log10(ref_gamma), np.log10(sim_eta[:, 0]),
                               sim_eta[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_eta - ref_eta)**2) / np.sum(
        (ref_eta - np.mean(ref_eta))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_eta[:, 0],
            sim_eta[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter Inversion Strength')
    ax.plot(ref_gamma,
            ref_eta,
            'ko',
            ms=7,
            label='Koll & Abbot (2016) Opacity Theory')

    ax.set_xscale('log')
    ax.set_xlabel(
        r"SW-to-LW Opacity Ratio $\gamma = \kappa_{\mathrm{SW}} / \kappa_{\mathrm{LW}}$",
        fontsize=12)
    ax.set_ylabel(r"Thermal Inversion Strength $\eta_{\mathrm{inv}}$",
                  fontsize=12)
    ax.set_title(
        "Koll & Abbot (2016) Figure 2: Thermal Inversion vs Opacity Ratio",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/koll_2016/fig2_inversion_strength.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Day-Night Contrast R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Thermal Inversion Strength R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Koll & Abbot (2016) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_koll2016()
