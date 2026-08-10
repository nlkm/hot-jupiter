"""
Verification script for Line et al. (2015) ApJ 807, 183.
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


def verify_line2015():
    ref_rows = load_csv("replications/line_2015/reference_data.csv")

    # Figure 1: Mass-Metallicity Relation
    ref_fig1_data = np.array([r for r in ref_rows if len(r) == 4])
    ref_mp = ref_fig1_data[:, 0]
    ref_z = ref_fig1_data[:, 1]

    sim_z_data = load_csv("replications/line_2015/sim_mass_metallicity.csv")
    sim_z = np.array(sim_z_data)

    sim_interp_z = np.interp(np.log10(ref_mp), np.log10(sim_z[:, 0]), sim_z[:,
                                                                            1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_z - ref_z)**2) / np.sum(
        (ref_z - np.mean(ref_z))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_z[:, 0], sim_z[:, 1], 'r-', lw=2.5, label='hot_jupiter Model')
    ax.errorbar(ref_mp,
                ref_z,
                yerr=[ref_z - ref_fig1_data[:, 3], ref_fig1_data[:, 2] - ref_z],
                fmt='ko',
                capsize=4,
                ms=6,
                label='Line et al. (2015) Sample')

    ax.set_xscale('log')
    ax.set_xlabel(r"Planetary Mass $M_p$ [$M_{\mathrm{Jup}}$]", fontsize=12)
    ax.set_ylabel(r"Atmospheric Metallicity $[M/H]$ [dex]", fontsize=12)
    ax.set_title(
        "Line et al. (2015) Figure 1: Hot Jupiter Mass-Metallicity Trend",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/line_2015/fig1_mass_metallicity.png", dpi=300)
    plt.close(fig)

    # Figure 2: C/O Ratio Distribution
    ref_fig2_data = np.array([r for r in ref_rows if len(r) == 2])
    ref_co_center = ref_fig2_data[:, 0]
    ref_co_count = ref_fig2_data[:, 1]

    sim_co_data = load_csv("replications/line_2015/sim_co_distribution.csv")
    sim_co = np.array(sim_co_data)

    sim_interp_co = np.interp(ref_co_center, sim_co[:, 0], sim_co[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_co - ref_co_count)**2) / np.sum(
        (ref_co_count - np.mean(ref_co_count))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_co[:, 0],
            sim_co[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter Gaussian Fit')
    ax.bar(ref_co_center,
           ref_co_count,
           width=0.15,
           alpha=0.5,
           color='#34495e',
           label='Line et al. (2015) Sample Bins')

    ax.set_xlabel("C/O Ratio", fontsize=12)
    ax.set_ylabel("Number of Planets", fontsize=12)
    ax.set_title(
        "Line et al. (2015) Figure 2: C/O Ratio Population Distribution",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/line_2015/fig2_co_distribution.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Mass-Metallicity R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 C/O Distribution R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Line et al. (2015) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_line2015()
