"""
Verification script for Zhang & Showman (2018a) ApJ 866, 1.
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


def verify_zhang2018a():
    ref_rows = load_csv("replications/zhang_2018a/reference_data.csv")

    # Figure 1: Equatorial Superrotation Speed U_eq [m/s] vs Teq (first 6 data rows)
    ref_fig1_data = np.array(ref_rows[:6])
    ref_teq = ref_fig1_data[:, 0]
    ref_ueq = ref_fig1_data[:, 1]

    sim_rot_data = load_csv("replications/zhang_2018a/sim_superrotation.csv")
    sim_rot = np.array(sim_rot_data)

    sim_interp_ueq = np.interp(ref_teq, sim_rot[:, 0], sim_rot[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_ueq - ref_ueq)**2) / np.sum(
        (ref_ueq - np.mean(ref_ueq))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_rot[:, 0],
            sim_rot[:, 1],
            'b-',
            lw=2.5,
            label='hot_jupiter Superrotation Speed')
    ax.plot(ref_teq,
            ref_ueq,
            'ko',
            ms=7,
            label='Zhang & Showman (2018a) Theory')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Equatorial Superrotation Speed $U_{\mathrm{eq}}$ [m/s]",
                  fontsize=12)
    ax.set_title(
        "Zhang & Showman (2018a) Figure 1: Superrotation Speed vs Temp",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/zhang_2018a/fig1_superrotation.png", dpi=300)
    plt.close(fig)

    # Figure 2: Day-Night Flux Contrast Amplitude A_dn vs Drag Timescale tau_drag (next 5 data rows)
    ref_fig2_data = np.array(ref_rows[6:])
    ref_taudrag = ref_fig2_data[:, 0]
    ref_adn = ref_fig2_data[:, 1]

    sim_adn_data = load_csv(
        "replications/zhang_2018a/sim_contrast_amplitude.csv")
    sim_adn = np.array(sim_adn_data)

    sim_interp_adn = np.interp(np.log10(ref_taudrag), np.log10(sim_adn[:, 0]),
                               sim_adn[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_adn - ref_adn)**2) / np.sum(
        (ref_adn - np.mean(ref_adn))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_adn[:, 0],
            sim_adn[:, 1],
            'r-',
            lw=2.5,
            label=r'hot_jupiter $\mathcal{A}_{\mathrm{dn}}$ Contrast')
    ax.plot(ref_taudrag,
            ref_adn,
            'ko',
            ms=7,
            label='Zhang & Showman (2018a) Scaling')

    ax.set_xscale('log')
    ax.set_xlabel(r"Frictional Drag Timescale $\tau_{\mathrm{drag}}$ [s]",
                  fontsize=12)
    ax.set_ylabel(
        r"Day-Night Flux Contrast Amplitude $\mathcal{A}_{\mathrm{dn}}$",
        fontsize=12)
    ax.set_title(
        "Zhang & Showman (2018a) Figure 2: Flux Contrast vs Drag Timescale",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/zhang_2018a/fig2_contrast_amplitude.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Superrotation Speed R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Contrast Amplitude R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Zhang & Showman (2018a) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_zhang2018a()
