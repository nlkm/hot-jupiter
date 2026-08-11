"""
Verification script for Arcangeli et al. (2018) ApJ 855, L30.
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


def verify_arcangeli2018():
    ref_rows = load_csv("replications/arcangeli_2018/reference_data.csv")

    # Figure 1: WASP-18b H- Emission Spectrum (first 8 data rows)
    ref_fig1_data = np.array(ref_rows[:8])
    ref_wave1 = ref_fig1_data[:, 0]
    ref_flux = ref_fig1_data[:, 1]

    sim_em_data = load_csv("replications/arcangeli_2018/sim_emission.csv")
    sim_em = np.array(sim_em_data)
    sim_em = sim_em[np.argsort(sim_em[:, 0])]

    sim_interp_flux = np.interp(ref_wave1, sim_em[:, 0], sim_em[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_flux - ref_flux)**2) / np.sum(
        (ref_flux - np.mean(ref_flux))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_em[:, 0],
            sim_em[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter WASP-18b $\mathrm{H}^-$ Continuum Model')
    ax.plot(ref_wave1,
            ref_flux,
            'ko',
            ms=7,
            label='Arcangeli et al. (2018) HST WFC3 Data')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Secondary Eclipse Depth $F_p/F_\star$", fontsize=12)
    ax.set_title(
        r"Arcangeli et al. (2018) Figure 1: WASP-18b $\mathrm{H}^-$ Continuum Emission Spectrum",
        fontsize=12)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/arcangeli_2018/fig1_hminus_emission.png", dpi=300)
    plt.close(fig)

    # Figure 2: Thermal Dissociation Fraction (next 7 data rows)
    ref_fig2_data = np.array(ref_rows[8:])
    ref_temp2 = ref_fig2_data[:, 0]
    ref_diss = ref_fig2_data[:, 1]

    sim_diss_data = load_csv("replications/arcangeli_2018/sim_dissociation.csv")
    sim_diss = np.array(sim_diss_data)
    sim_diss = sim_diss[np.argsort(sim_diss[:, 0])]

    sim_interp_diss = np.interp(ref_temp2, sim_diss[:, 0], sim_diss[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_diss - ref_diss)**2) / np.sum(
        (ref_diss - np.mean(ref_diss))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_diss[:, 0],
            sim_diss[:, 1],
            'r-',
            lw=2.5,
            label=r'hot_jupiter Model $\alpha_{\mathrm{diss}}(T)$')
    ax.plot(ref_temp2,
            ref_diss,
            'ko',
            ms=7,
            label='Arcangeli et al. (2018) Calculation')

    ax.set_xlabel(r"Temperature $T$ [K]", fontsize=12)
    ax.set_ylabel(r"Molecular Dissociation Fraction $\alpha_{\mathrm{diss}}$",
                  fontsize=12)
    ax.set_title(
        r"Arcangeli et al. (2018) Figure 2: Thermal Dissociation of $\text{H}_2$ / $\text{H}_2\text{O}$",
        fontsize=12)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/arcangeli_2018/fig2_thermal_dissociation.png",
                dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 WASP-18b H- Emission Spectrum R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Thermal Dissociation Fraction R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Arcangeli et al. (2018) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_arcangeli2018()
