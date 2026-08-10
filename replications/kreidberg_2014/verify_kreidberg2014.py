"""
Verification script for Kreidberg et al. (2014) Nature 505, 69.
Replicates Figures 1 & 2 using hot_jupiter library and C++ solver data.
Calculates statistical agreement for all published figures.
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


def verify_kreidberg2014():
    ref_rows = load_csv("replications/kreidberg_2014/reference_data.csv")

    # Figure 1: WFC3 Transmission Spectrum Flat Line Model
    ref_fig1_data = np.array([r for r in ref_rows if len(r) == 3])
    ref_wave = ref_fig1_data[:, 0]
    ref_depth = ref_fig1_data[:, 1]
    ref_err = ref_fig1_data[:, 2]

    sim_spec_data = load_csv(
        "replications/kreidberg_2014/sim_flat_spectrum.csv")
    sim_spec = np.array(sim_spec_data)

    sim_interp_depth = np.interp(ref_wave, sim_spec[:, 0], sim_spec[:, 1])

    rmse_fig1 = np.sqrt(np.mean((sim_interp_depth - ref_depth)**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_spec[:, 0],
            sim_spec[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter Cloud Deck Model (1.345%)')
    ax.errorbar(ref_wave,
                ref_depth,
                yerr=ref_err,
                fmt='ko',
                capsize=4,
                ms=6,
                label='Kreidberg et al. (2014) WFC3 Data')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [%]", fontsize=12)
    ax.set_title(
        "Kreidberg et al. (2014) Figure 1: GJ 1214b Transmission Spectrum",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/kreidberg_2014/fig1_flat_spectrum.png", dpi=300)
    plt.close(fig)

    # Figure 2: Water Feature Amplitude vs Cloud Top Pressure
    ref_fig2_data = np.array([r for r in ref_rows if len(r) == 2])
    ref_p_cloud = ref_fig2_data[:, 0]
    ref_h2o_amp = ref_fig2_data[:, 1]

    sim_amp_data = load_csv(
        "replications/kreidberg_2014/sim_water_amplitude.csv")
    sim_amp = np.array(sim_amp_data)

    sim_interp_amp = np.interp(np.log10(ref_p_cloud), np.log10(sim_amp[:, 0]),
                               sim_amp[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_amp - ref_h2o_amp)**2) / np.sum(
        (ref_h2o_amp - np.mean(ref_h2o_amp))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_amp[:, 0],
            sim_amp[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter Model')
    ax.plot(ref_p_cloud,
            ref_h2o_amp,
            'ko',
            ms=7,
            label='Kreidberg et al. (2014)')

    ax.set_xscale('log')
    ax.set_yscale('log')
    ax.set_xlabel(r"Cloud Top Pressure $P_{\mathrm{cloud}}$ [mbar]",
                  fontsize=12)
    ax.set_ylabel(r"$H_2O$ Feature Amplitude $\Delta(R_p/R_\star)^2$ [ppm]",
                  fontsize=12)
    ax.set_title(
        "Kreidberg et al. (2014) Figure 2: Water Feature vs Cloud Pressure",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/kreidberg_2014/fig2_water_amplitude.png", dpi=300)
    plt.close(fig)

    print(f"--> Fig 1 Flat Spectrum RMSE:           {rmse_fig1:.6f} %")
    print(
        f"--> Fig 2 Water Feature Amplitude R^2: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert rmse_fig1 <= 0.01, f"Figure 1 RMSE {rmse_fig1} exceeds 0.01%!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Kreidberg et al. (2014) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_kreidberg2014()
