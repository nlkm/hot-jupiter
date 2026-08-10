"""
Verification script for Crossfield & Kreidberg (2017) AJ 154, 261.
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


def verify_crossfield2017():
    ref_rows = load_csv("replications/crossfield_2017/reference_data.csv")

    # Figure 1: Water Amplitude vs T_eq (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_teq = ref_fig1_data[:, 0]
    ref_amp_teq = ref_fig1_data[:, 1]

    sim_teq_data = load_csv("replications/crossfield_2017/sim_teq_trend.csv")
    sim_teq = np.array(sim_teq_data)

    sim_interp_amp_teq = np.interp(ref_teq, sim_teq[:, 0], sim_teq[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_amp_teq - ref_amp_teq)**2) / np.sum(
        (ref_amp_teq - np.mean(ref_amp_teq))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_teq[:, 0],
            sim_teq[:, 1],
            'r-',
            lw=2.5,
            label=r'hot_jupiter Sub-Jovian Model')
    ax.plot(ref_teq,
            ref_amp_teq,
            'ko',
            ms=7,
            label='Crossfield & Kreidberg (2017)')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Water Amplitude $A_{\mathrm{H2O}}$ [Scale Heights $H$]",
                  fontsize=12)
    ax.set_title(
        r"Crossfield & Kreidberg (2017) Figure 1: Water Feature vs $T_{\mathrm{eq}}$",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/crossfield_2017/fig1_teq_trend.png", dpi=300)
    plt.close(fig)

    # Figure 2: Water Amplitude vs Radius (next 7 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_radius = ref_fig2_data[:, 0]
    ref_amp_radius = ref_fig2_data[:, 1]

    sim_radius_data = load_csv(
        "replications/crossfield_2017/sim_radius_trend.csv")
    sim_radius = np.array(sim_radius_data)

    sim_interp_amp_radius = np.interp(ref_radius, sim_radius[:, 0],
                                      sim_radius[:, 1])

    r2_fig2 = 1.0 - (np.sum(
        (sim_interp_amp_radius - ref_amp_radius)**2) / np.sum(
            (ref_amp_radius - np.mean(ref_amp_radius))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_radius[:, 0],
            sim_radius[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter Sub-Jovian Model')
    ax.plot(ref_radius,
            ref_amp_radius,
            'ko',
            ms=7,
            label='Crossfield & Kreidberg (2017)')

    ax.set_xlabel(r"Planet Radius $R_p$ [$R_\oplus$]", fontsize=12)
    ax.set_ylabel(r"Water Amplitude $A_{\mathrm{H2O}}$ [Scale Heights $H$]",
                  fontsize=12)
    ax.set_title(
        "Crossfield & Kreidberg (2017) Figure 2: Water Feature vs Radius",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/crossfield_2017/fig2_radius_trend.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Water Amplitude vs T_eq R^2 Score:   {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Water Amplitude vs Radius R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print(
        "✅ Crossfield & Kreidberg (2017) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_crossfield2017()
