"""
Verification script for Fisher & Heng (2018) MNRAS 481, 4698.
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


def verify_fisher2018():
    ref_rows = load_csv("replications/fisher_2018/reference_data.csv")

    # Figure 1: WASP-12b Analytical Retrieval Spectrum (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_wave = ref_fig1_data[:, 0]
    ref_depth = ref_fig1_data[:, 1]
    ref_err = ref_fig1_data[:, 2]

    sim_spec_data = load_csv(
        "replications/fisher_2018/sim_transmission_spectrum.csv")
    sim_spec = np.array(sim_spec_data)

    sim_interp_depth = np.interp(ref_wave, sim_spec[:, 0], sim_spec[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_depth - ref_depth)**2) / np.sum(
        (ref_depth - np.mean(ref_depth))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_spec[:, 0],
            sim_spec[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter Analytical Retrieval')
    ax.errorbar(ref_wave,
                ref_depth,
                yerr=ref_err,
                fmt='ko',
                capsize=4,
                ms=6,
                label='Fisher & Heng (2018) WASP-12b')

    ax.set_xscale('log')
    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [\%]", fontsize=12)
    ax.set_title("Fisher & Heng (2018) Figure 1: WASP-12b Analytical Retrieval",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/fisher_2018/fig1_transmission_spectrum.png",
                dpi=300)
    plt.close(fig)

    # Figure 2: Scattering Index gamma vs Teq (next 7 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_teq = ref_fig2_data[:, 0]
    ref_gamma = ref_fig2_data[:, 1]

    sim_gamma_data = load_csv(
        "replications/fisher_2018/sim_scattering_index.csv")
    sim_gamma = np.array(sim_gamma_data)

    sim_interp_gamma = np.interp(ref_teq, sim_gamma[:, 0], sim_gamma[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_gamma - ref_gamma)**2) / np.sum(
        (ref_gamma - np.mean(ref_gamma))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_gamma[:, 0],
            sim_gamma[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter Scattering Slope $\gamma$')
    ax.plot(ref_teq,
            ref_gamma,
            'ko',
            ms=7,
            label='Fisher & Heng (2018) 38 Hot Jupiters')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Scattering Index $\gamma = -d\ln\kappa / d\ln\lambda$",
                  fontsize=12)
    ax.set_title(
        "Fisher & Heng (2018) Figure 2: Optical Scattering Index vs Temperature",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/fisher_2018/fig2_scattering_index.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 WASP-12b Spectrum R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Scattering Index R^2 Score:    {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Fisher & Heng (2018) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_fisher2018()
