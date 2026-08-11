"""
Verification script for Komacek & Showman (2016) ApJ 821, 16.
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


def verify_komacek2016():
    ref_rows = load_csv("replications/komacek_2016/reference_data.csv")

    # Figure 1: Thermal Contrast vs Teq (first 5 data rows)
    ref_fig1_data = np.array(ref_rows[:5])
    ref_teq = ref_fig1_data[:, 0]
    ref_aw = ref_fig1_data[:, 1]
    ref_am = ref_fig1_data[:, 2]
    ref_as = ref_fig1_data[:, 3]

    sim_teq_data = load_csv("replications/komacek_2016/sim_teq_contrast.csv")
    sim_teq = np.array(sim_teq_data)

    sim_interp_aw = np.interp(ref_teq, sim_teq[:, 0], sim_teq[:, 1])
    sim_interp_am = np.interp(ref_teq, sim_teq[:, 0], sim_teq[:, 2])
    sim_interp_as = np.interp(ref_teq, sim_teq[:, 0], sim_teq[:, 3])

    r2_aw = 1.0 - (np.sum((sim_interp_aw - ref_aw)**2) / np.sum(
        (ref_aw - np.mean(ref_aw))**2))
    r2_am = 1.0 - (np.sum((sim_interp_am - ref_am)**2) / np.sum(
        (ref_am - np.mean(ref_am))**2))
    r2_as = 1.0 - (np.sum((sim_interp_as - ref_as)**2) / np.sum(
        (ref_as - np.mean(ref_as))**2))
    r2_fig1 = (r2_aw + r2_am + r2_as) / 3.0

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_teq[:, 0],
            sim_teq[:, 1],
            'b-',
            lw=2.5,
            label=r'Weak Drag ($\gamma_{\mathrm{drag}} = 0.01$)')
    ax.plot(ref_teq, ref_aw, 'bo', ms=7)

    ax.plot(sim_teq[:, 0],
            sim_teq[:, 2],
            'g-',
            lw=2.5,
            label=r'Intermediate Drag ($\gamma_{\mathrm{drag}} = 1.0$)')
    ax.plot(ref_teq, ref_am, 'go', ms=7)

    ax.plot(sim_teq[:, 0],
            sim_teq[:, 3],
            'r-',
            lw=2.5,
            label=r'Strong Drag ($\gamma_{\mathrm{drag}} = 100$)')
    ax.plot(ref_teq, ref_as, 'ro', ms=7)

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(
        r"Fractional Day-Night Contrast $A = (T_{\mathrm{day}}-T_{\mathrm{night}})/(T_{\mathrm{day}}+T_{\mathrm{night}})$",
        fontsize=11)
    ax.set_title(
        "Komacek & Showman (2016) Figure 1: Day-Night Contrast vs Temperature",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    plt.savefig("replications/komacek_2016/fig1_teq_contrast.png", dpi=300)
    plt.close(fig)

    # Figure 2: Thermal Contrast vs Wave Drag Gamma (next 5 data rows)
    ref_fig2_data = np.array(ref_rows[5:])
    ref_gamma = ref_fig2_data[:, 0]
    ref_a = ref_fig2_data[:, 1]

    sim_g_data = load_csv("replications/komacek_2016/sim_gamma_drag.csv")
    sim_g = np.array(sim_g_data)

    sim_interp_a = np.interp(np.log10(ref_gamma), np.log10(sim_g[:, 0]),
                             sim_g[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_a - ref_a)**2) / np.sum(
        (ref_a - np.mean(ref_a))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(
        sim_g[:, 0],
        sim_g[:, 1],
        'purple',
        lw=2.5,
        label=r'hot_jupiter Scaling Theory ($T_{\mathrm{eq}} = 2000\mathrm{K}$)'
    )
    ax.plot(ref_gamma, ref_a, 'ko', ms=7, label='Komacek & Showman (2016) GCM')

    ax.set_xscale('log')
    ax.set_xlabel(r"Dimensionless Wave Drag Parameter $\gamma_{\mathrm{drag}}$",
                  fontsize=12)
    ax.set_ylabel("Fractional Thermal Contrast $A$", fontsize=12)
    ax.set_title(
        "Komacek & Showman (2016) Figure 2: Day-Night Contrast vs Wave Drag",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/komacek_2016/fig2_gamma_drag.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Teq Contrast R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)")
    print(
        f"--> Fig 2 Wave Drag Contrast R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Komacek & Showman (2016) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_komacek2016()
