"""
Verification script for Wakeford et al. (2017) Science 356, 1150.
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


def verify_wakeford2017():
    ref_rows = load_csv("replications/wakeford_2017/reference_data.csv")

    # Figure 1: HAT-P-26b Transmission Spectrum (first 8 data rows)
    ref_fig1_data = np.array(ref_rows[:8])
    ref_wave = ref_fig1_data[:, 0]
    ref_depth = ref_fig1_data[:, 1]
    ref_err = ref_fig1_data[:, 2]

    sim_spec_data = load_csv(
        "replications/wakeford_2017/sim_transmission_spectrum.csv")
    sim_spec = np.array(sim_spec_data)

    sim_interp_depth = np.interp(ref_wave, sim_spec[:, 0], sim_spec[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_depth - ref_depth)**2) / np.sum(
        (ref_depth - np.mean(ref_depth))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_spec[:, 0],
            sim_spec[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter HAT-P-26b Model')
    ax.errorbar(ref_wave,
                ref_depth,
                yerr=ref_err,
                fmt='ko',
                capsize=4,
                ms=6,
                label='Wakeford et al. (2017) Data')

    ax.set_xscale('log')
    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [ppm]", fontsize=12)
    ax.set_title(
        "Wakeford et al. (2017) Figure 1: HAT-P-26b Transmission Spectrum",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/wakeford_2017/fig1_transmission_spectrum.png",
                dpi=300)
    plt.close(fig)

    # Figure 2: Mass-Metallicity Relation (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[8:])
    ref_mass = ref_fig2_data[:, 0]
    ref_z = ref_fig2_data[:, 1]

    sim_z_data = load_csv("replications/wakeford_2017/sim_mass_metallicity.csv")
    sim_z = np.array(sim_z_data)

    sim_interp_z = np.interp(np.log10(ref_mass), np.log10(sim_z[:, 0]),
                             sim_z[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_z - ref_z)**2) / np.sum(
        (ref_z - np.mean(ref_z))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_z[:, 0],
            sim_z[:, 1],
            'b-',
            lw=2.5,
            label='hot_jupiter Mass-Metallicity Scaling')
    ax.plot(ref_mass, ref_z, 'ko', ms=7, label='Wakeford et al. (2017) Planets')

    ax.set_xscale('log')
    ax.set_xlabel(r"Planet Mass $M_p$ [$M_\oplus$]", fontsize=12)
    ax.set_ylabel(r"Metallicity $\log_{10} [M/H]$", fontsize=12)
    ax.set_title("Wakeford et al. (2017) Figure 2: Mass-Metallicity Trend",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/wakeford_2017/fig2_mass_metallicity.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Transmission Spectrum R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Mass-Metallicity R^2 Score:      {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Wakeford et al. (2017) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_wakeford2017()
