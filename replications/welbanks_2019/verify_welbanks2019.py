"""
Verification script for Welbanks et al. (2019) ApJL 887, L20.
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


def verify_welbanks2019():
    ref_rows = load_csv("replications/welbanks_2019/reference_data.csv")

    # Figure 1: Atmospheric Water Abundance log10(X_H2O) vs Planet Mass (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_mass = ref_fig1_data[:, 0]
    ref_xh2o = ref_fig1_data[:, 1]

    sim_water_data = load_csv(
        "replications/welbanks_2019/sim_water_abundance.csv")
    sim_water = np.array(sim_water_data)

    sim_interp_xh2o = np.interp(np.log10(ref_mass), np.log10(sim_water[:, 0]),
                                sim_water[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_xh2o - ref_xh2o)**2) / np.sum(
        (ref_xh2o - np.mean(ref_xh2o))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_water[:, 0],
            sim_water[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter $\log_{10} X_{\mathrm{H2O}}$')
    ax.plot(ref_mass,
            ref_xh2o,
            'ko',
            ms=7,
            label='Welbanks et al. (2019) 19 Hot Jupiters')

    ax.set_xscale('log')
    ax.set_xlabel(r"Planetary Mass $M_p$ [$M_{\mathrm{Jup}}$]", fontsize=12)
    ax.set_ylabel(r"Water Abundance $\log_{10} X_{\mathrm{H2O}}$", fontsize=12)
    ax.set_title(
        "Welbanks et al. (2019) Figure 1: Water Abundance vs Planetary Mass",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/welbanks_2019/fig1_water_abundance.png", dpi=300)
    plt.close(fig)

    # Figure 2: Atmospheric Metallicity vs Planet Mass (next 7 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_mass_mh = ref_fig2_data[:, 0]
    ref_mh = ref_fig2_data[:, 1]

    sim_mh_data = load_csv(
        "replications/welbanks_2019/sim_mass_metallicity.csv")
    sim_mh = np.array(sim_mh_data)

    sim_interp_mh = np.interp(np.log10(ref_mass_mh), np.log10(sim_mh[:, 0]),
                              sim_mh[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_mh - ref_mh)**2) / np.sum(
        (ref_mh - np.mean(ref_mh))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_mh[:, 0],
            sim_mh[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter Mass-Metallicity Relation')
    ax.plot(ref_mass_mh,
            ref_mh,
            'ko',
            ms=7,
            label='Welbanks et al. (2019) Retrived Metallicity')

    ax.set_xscale('log')
    ax.set_yscale('log')
    ax.set_xlabel(r"Planetary Mass $M_p$ [$M_{\mathrm{Jup}}$]", fontsize=12)
    ax.set_ylabel(r"Atmospheric Metallicity $[\mathrm{M/H}]$ [$\times$ Solar]",
                  fontsize=12)
    ax.set_title(
        "Welbanks et al. (2019) Figure 2: Mass-Metallicity Scaling Trend",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/welbanks_2019/fig2_mass_metallicity.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Water Abundance R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Mass-Metallicity Trend R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Welbanks et al. (2019) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_welbanks2019()
