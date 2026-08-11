"""
Verification script for Pinhas et al. (2019) MNRAS 482, 1485.
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


def verify_pinhas2019():
    ref_rows = load_csv("replications/pinhas_2019/reference_data.csv")

    # Figure 1: WASP-31b Transmission Spectrum (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_wave = ref_fig1_data[:, 0]
    ref_depth = ref_fig1_data[:, 1]
    ref_err = ref_fig1_data[:, 2]

    sim_spec_data = load_csv(
        "replications/pinhas_2019/sim_transmission_spectrum.csv")
    sim_spec = np.array(sim_spec_data)

    sim_interp_depth = np.interp(ref_wave, sim_spec[:, 0], sim_spec[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_depth - ref_depth)**2) / np.sum(
        (ref_depth - np.mean(ref_depth))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_spec[:, 0],
            sim_spec[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter H2O Model')
    ax.errorbar(ref_wave,
                ref_depth,
                yerr=ref_err,
                fmt='ko',
                capsize=4,
                ms=6,
                label='Pinhas et al. (2019) WASP-31b')

    ax.set_xscale('log')
    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [\%]", fontsize=12)
    ax.set_title("Pinhas et al. (2019) Figure 1: WASP-31b H2O Retrieval Fit",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/pinhas_2019/fig1_transmission_spectrum.png",
                dpi=300)
    plt.close(fig)

    # Figure 2: Water Abundance vs Teq (next 7 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_teq = ref_fig2_data[:, 0]
    ref_h2o = ref_fig2_data[:, 1]

    sim_h2o_data = load_csv("replications/pinhas_2019/sim_water_abundance.csv")
    sim_h2o = np.array(sim_h2o_data)

    sim_interp_h2o = np.interp(ref_teq, sim_h2o[:, 0], sim_h2o[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_h2o - ref_h2o)**2) / np.sum(
        (ref_h2o - np.mean(ref_h2o))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_h2o[:, 0],
            sim_h2o[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter $\log_{10} X_{\mathrm{H_2O}}$')
    ax.plot(ref_teq,
            ref_h2o,
            'ko',
            ms=7,
            label='Pinhas et al. (2019) 10 Hot Jupiters')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Water Mixing Ratio $\log_{10} X_{\mathrm{H_2O}}$",
                  fontsize=12)
    ax.set_title(
        "Pinhas et al. (2019) Figure 2: H2O Abundance vs Equilibrium Temperature",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/pinhas_2019/fig2_water_abundance.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 WASP-31b Spectrum R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Water Abundance R^2 Score:    {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Pinhas et al. (2019) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_pinhas2019()
