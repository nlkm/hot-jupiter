"""
Verification script for Baxter et al. (2021) A&A 648, A127.
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


def verify_baxter2021():
    ref_rows = load_csv("replications/baxter_2021/reference_data.csv")

    # Figure 1: 3.6 um Spitzer Secondary Eclipse Depth (first 6 data rows)
    ref_fig1_data = np.array(ref_rows[:6])
    ref_teq36 = ref_fig1_data[:, 0]
    ref_decl36 = ref_fig1_data[:, 1]

    sim_36_data = load_csv("replications/baxter_2021/sim_36um_eclipse.csv")
    sim_36 = np.array(sim_36_data)

    sim_interp_decl36 = np.interp(ref_teq36, sim_36[:, 0], sim_36[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_decl36 - ref_decl36)**2) / np.sum(
        (ref_decl36 - np.mean(ref_decl36))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_36[:, 0],
            sim_36[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter $3.6\,\mu\mathrm{m}$ Eclipse Depth')
    ax.plot(ref_teq36,
            ref_decl36,
            'ko',
            ms=7,
            label='Baxter et al. (2021) Spitzer Population')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Secondary Eclipse Depth $D_{\mathrm{ecl}}$ [ppm]",
                  fontsize=12)
    ax.set_title("Baxter et al. (2021) Figure 1: 3.6um Eclipse Depth vs Temp",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/baxter_2021/fig1_36um_eclipse.png", dpi=300)
    plt.close(fig)

    # Figure 2: 4.5 um Spitzer Secondary Eclipse Depth (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[6:])
    ref_teq45 = ref_fig2_data[:, 0]
    ref_decl45 = ref_fig2_data[:, 1]

    sim_45_data = load_csv("replications/baxter_2021/sim_45um_eclipse.csv")
    sim_45 = np.array(sim_45_data)

    sim_interp_decl45 = np.interp(ref_teq45, sim_45[:, 0], sim_45[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_decl45 - ref_decl45)**2) / np.sum(
        (ref_decl45 - np.mean(ref_decl45))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_45[:, 0],
            sim_45[:, 1],
            'r-',
            lw=2.5,
            label=r'hot_jupiter $4.5\,\mu\mathrm{m}$ Eclipse Depth')
    ax.plot(ref_teq45,
            ref_decl45,
            'ko',
            ms=7,
            label='Baxter et al. (2021) Spitzer Population')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Secondary Eclipse Depth $D_{\mathrm{ecl}}$ [ppm]",
                  fontsize=12)
    ax.set_title("Baxter et al. (2021) Figure 2: 4.5um Eclipse Depth vs Temp",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/baxter_2021/fig2_45um_eclipse.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 3.6um Eclipse Depth R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 4.5um Eclipse Depth R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Baxter et al. (2021) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_baxter2021()
