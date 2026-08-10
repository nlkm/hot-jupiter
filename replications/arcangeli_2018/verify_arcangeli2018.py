"""
Verification script for Arcangeli et al. (2018) ApJL 855, L30.
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

    # Figure 1: WASP-18b Secondary Eclipse Spectrum (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_wave = ref_fig1_data[:, 0]
    ref_flux = ref_fig1_data[:, 1]
    ref_err = ref_fig1_data[:, 2]

    sim_spec_data = load_csv(
        "replications/arcangeli_2018/sim_secondary_eclipse.csv")
    sim_spec = np.array(sim_spec_data)

    sim_interp_flux = np.interp(ref_wave, sim_spec[:, 0], sim_spec[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_flux - ref_flux)**2) / np.sum(
        (ref_flux - np.mean(ref_flux))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_spec[:, 0],
            sim_spec[:, 1],
            'r-',
            lw=2.5,
            label=r'hot_jupiter $H^-$ Opacity Model')
    ax.errorbar(ref_wave,
                ref_flux,
                yerr=ref_err,
                fmt='ko',
                capsize=4,
                ms=6,
                label='Arcangeli et al. (2018) WASP-18b Data')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Secondary Eclipse Flux Ratio $(F_p/F_\star)$ [ppm]",
                  fontsize=12)
    ax.set_title(
        "Arcangeli et al. (2018) Figure 1: WASP-18b Secondary Eclipse Spectrum",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/arcangeli_2018/fig1_secondary_eclipse.png",
                dpi=300)
    plt.close(fig)

    # Figure 2: Hydrogen Dissociation Fraction vs Temperature (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_temp = ref_fig2_data[:, 0]
    ref_alpha = ref_fig2_data[:, 1]

    sim_alpha_data = load_csv(
        "replications/arcangeli_2018/sim_hydrogen_dissociation.csv")
    sim_alpha = np.array(sim_alpha_data)

    sim_interp_alpha = np.interp(ref_temp, sim_alpha[:, 0], sim_alpha[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_alpha - ref_alpha)**2) / np.sum(
        (ref_alpha - np.mean(ref_alpha))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_alpha[:, 0],
            sim_alpha[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter Dissociation Model ($P=10$ mbar)')
    ax.plot(ref_temp, ref_alpha, 'ko', ms=7, label='Arcangeli et al. (2018)')

    ax.set_xlabel(r"Temperature $T$ [K]", fontsize=12)
    ax.set_ylabel(r"Hydrogen Dissociation Fraction $\alpha_H$", fontsize=12)
    ax.set_title(
        "Arcangeli et al. (2018) Figure 2: Thermal Hydrogen Dissociation",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/arcangeli_2018/fig2_hydrogen_dissociation.png",
                dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Secondary Eclipse R^2 Score:        {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Hydrogen Dissociation R^2 Score:     {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Arcangeli et al. (2018) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_arcangeli2018()
