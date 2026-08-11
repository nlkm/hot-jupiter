"""
Verification script for Showman & Kaspi (2013) ApJ 776, 85.
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


def verify_showman2013():
    ref_rows = load_csv("replications/showman_2013/reference_data.csv")

    # Figure 1: Zonal Jet Speed U_jet [m/s] vs Teq (first 6 data rows)
    ref_fig1_data = np.array(ref_rows[:6])
    ref_teq = ref_fig1_data[:, 0]
    ref_ujet = ref_fig1_data[:, 1]

    sim_jet_data = load_csv("replications/showman_2013/sim_zonal_jet.csv")
    sim_jet = np.array(sim_jet_data)

    sim_interp_ujet = np.interp(ref_teq, sim_jet[:, 0], sim_jet[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_ujet - ref_ujet)**2) / np.sum(
        (ref_ujet - np.mean(ref_ujet))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_jet[:, 0],
            sim_jet[:, 1],
            'b-',
            lw=2.5,
            label='hot_jupiter Zonal Jet Speed')
    ax.plot(ref_teq,
            ref_ujet,
            'ko',
            ms=7,
            label='Showman & Kaspi (2013) Theory')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Zonal Jet Speed $U_{\mathrm{jet}}$ [m/s]", fontsize=12)
    ax.set_title(
        "Showman & Kaspi (2013) Figure 1: Zonal Jet Speed vs Equilibrium Temperature",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/showman_2013/fig1_zonal_jet.png", dpi=300)
    plt.close(fig)

    # Figure 2: Normalized Rossby Deformation Radius L_D / a vs Rotation Period Prot (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[6:])
    ref_prot = ref_fig2_data[:, 0]
    ref_ld = ref_fig2_data[:, 1]

    sim_ld_data = load_csv("replications/showman_2013/sim_rossby_radius.csv")
    sim_ld = np.array(sim_ld_data)

    sim_interp_ld = np.interp(ref_prot, sim_ld[:, 0], sim_ld[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_ld - ref_ld)**2) / np.sum(
        (ref_ld - np.mean(ref_ld))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_ld[:, 0],
            sim_ld[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter Rossby Scale')
    ax.plot(ref_prot,
            ref_ld,
            'ko',
            ms=7,
            label='Showman & Kaspi (2013) Scaling')

    ax.set_xlabel(r"Rotation Period $P_{\mathrm{rot}}$ [days]", fontsize=12)
    ax.set_ylabel(r"Rossby Deformation Scale $L_D / a$", fontsize=12)
    ax.set_title(
        "Showman & Kaspi (2013) Figure 2: Rossby Radius vs Rotation Period",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/showman_2013/fig2_rossby_radius.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Zonal Jet Speed R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Rossby Deformation Radius R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Showman & Kaspi (2013) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_showman2013()
