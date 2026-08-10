"""
Verification script for Knutson et al. (2014) ApJ 785, 126.
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


def verify_knutson2014():
    ref_rows = load_csv("replications/knutson_2014/reference_data.csv")

    # Figure 1: HD 97658b Transmission Spectrum (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_wave = ref_fig1_data[:, 0]
    ref_depth = ref_fig1_data[:, 1]
    ref_err = ref_fig1_data[:, 2]

    sim_spec_data = load_csv(
        "replications/knutson_2014/sim_transmission_spectrum.csv")
    sim_spec = np.array(sim_spec_data)

    sim_interp_depth = np.interp(ref_wave, sim_spec[:, 0], sim_spec[:, 1])

    rmse_fig1 = np.sqrt(np.mean((sim_interp_depth - ref_depth)**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_spec[:, 0],
            sim_spec[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter High Metallicity Model (0.570%)')
    ax.errorbar(ref_wave,
                ref_depth,
                yerr=ref_err,
                fmt='ko',
                capsize=4,
                ms=6,
                label='Knutson et al. (2014) WFC3 Data')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [%]", fontsize=12)
    ax.set_title(
        "Knutson et al. (2014) Figure 1: HD 97658b Transmission Spectrum",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/knutson_2014/fig1_transmission_spectrum.png",
                dpi=300)
    plt.close(fig)

    # Figure 2: Water Feature Amplitude vs Metallicity (next 7 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_dex = ref_fig2_data[:, 0]
    ref_h2o_amp = ref_fig2_data[:, 1]

    sim_amp_data = load_csv(
        "replications/knutson_2014/sim_metallicity_dampening.csv")
    sim_amp = np.array(sim_amp_data)

    sim_interp_amp = np.interp(ref_dex, sim_amp[:, 0], sim_amp[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_amp - ref_h2o_amp)**2) / np.sum(
        (ref_h2o_amp - np.mean(ref_h2o_amp))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_amp[:, 0],
            sim_amp[:, 1],
            'b-',
            lw=2.5,
            label='hot_jupiter Dampening Model')
    ax.plot(ref_dex, ref_h2o_amp, 'ko', ms=7, label='Knutson et al. (2014)')

    ax.set_xlabel(r"Atmospheric Metallicity $[M/H]$ [dex]", fontsize=12)
    ax.set_ylabel(r"$H_2O$ Feature Amplitude $\Delta(R_p/R_\star)^2$ [ppm]",
                  fontsize=12)
    ax.set_title(
        "Knutson et al. (2014) Figure 2: Water Amplitude vs Metallicity",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/knutson_2014/fig2_metallicity_dampening.png",
                dpi=300)
    plt.close(fig)

    print(f"--> Fig 1 Spectrum RMSE:                   {rmse_fig1:.6f} %")
    print(
        f"--> Fig 2 Metallicity Dampening R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert rmse_fig1 <= 0.01, f"Figure 1 RMSE {rmse_fig1} exceeds 0.01%!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Knutson et al. (2014) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_knutson2014()
