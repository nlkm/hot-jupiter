"""
Verification script for Spake et al. (2018) Nature 557, 68.
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


def verify_spake2018():
    ref_rows = load_csv("replications/spake_2018/reference_data.csv")

    # Figure 1: WASP-107b Helium Spectrum (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_wl = ref_fig1_data[:, 0]
    ref_depth = ref_fig1_data[:, 1]

    sim_trans_data = load_csv("replications/spake_2018/sim_helium_spectrum.csv")
    sim_trans = np.array(sim_trans_data)
    sim_trans = sim_trans[np.argsort(sim_trans[:, 0])]

    sim_interp_depth = np.interp(ref_wl, sim_trans[:, 0], sim_trans[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_depth - ref_depth)**2) / np.sum(
        (ref_depth - np.mean(ref_depth))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_trans[:, 0],
            sim_trans[:, 1] * 100,
            'b-',
            lw=2.5,
            label='hot_jupiter Model Spectrum')
    ax.plot(ref_wl,
            ref_depth * 100,
            'ko',
            ms=7,
            label='Spake et al. (2018) HST Data')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu\mathrm{m}$]", fontsize=12)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [\%]", fontsize=12)
    ax.set_title(
        "Spake et al. (2018) Figure 1: Metastable Helium Absorption Peak",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/spake_2018/fig1_helium_spectrum.png", dpi=300)
    plt.close(fig)

    # Figure 2: Mass Loss Rate vs Helium Fraction (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_he = ref_fig2_data[:, 0]
    ref_mdot = ref_fig2_data[:, 1]

    sim_ml_data = load_csv("replications/spake_2018/sim_mass_loss.csv")
    sim_ml = np.array(sim_ml_data)

    sim_interp_mdot = np.interp(ref_he, sim_ml[:, 0], sim_ml[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_mdot - ref_mdot)**2) / np.sum(
        (ref_mdot - np.mean(ref_mdot))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_ml[:, 0],
            sim_ml[:, 1],
            'r-',
            lw=2.5,
            label=r'hot_jupiter Model $\log_{10} \dot{M}$')
    ax.plot(ref_he, ref_mdot, 'ko', ms=7, label='Spake et al. (2018) Derived')

    ax.set_xlabel(r"Helium Fractional Abundance $y_{\mathrm{He}}$", fontsize=12)
    ax.set_ylabel(r"Mass-Loss Rate $\log_{10} \dot{M}$ [g s$^{-1}$]",
                  fontsize=12)
    ax.set_title("Spake et al. (2018) Figure 2: Helium Mass-Loss Constraint",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/spake_2018/fig2_mass_loss.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Helium Spectrum R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Mass Loss Rate R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Spake et al. (2018) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_spake2018()
