"""
Verification script for Batalha et al. (2019) ApJ 878, 70.
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


def verify_batalha2019():
    ref_rows = load_csv("replications/batalha_2019/reference_data.csv")

    # Figure 1: JWST NIRSpec G395H Noise Precision (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_wave = ref_fig1_data[:, 0]
    ref_noise = ref_fig1_data[:, 1]

    sim_noise_data = load_csv(
        "replications/batalha_2019/sim_noise_precision.csv")
    sim_noise = np.array(sim_noise_data)

    sim_interp_noise = np.interp(ref_wave, sim_noise[:, 0], sim_noise[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_noise - ref_noise)**2) / np.sum(
        (ref_noise - np.mean(ref_noise))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_noise[:, 0],
            sim_noise[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter PandExo Model')
    ax.plot(ref_wave,
            ref_noise,
            'ko',
            ms=7,
            label='Batalha et al. (2019) Benchmarks')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Transit Depth Noise Precision $\sigma$ [ppm]", fontsize=12)
    ax.set_title(
        "Batalha et al. (2019) Figure 1: JWST NIRSpec G395H Noise Floor",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/batalha_2019/fig1_noise_precision.png", dpi=300)
    plt.close(fig)

    # Figure 2: SNR vs Host Star J Magnitude (next 7 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_mag = ref_fig2_data[:, 0]
    ref_snr = ref_fig2_data[:, 1]

    sim_snr_data = load_csv("replications/batalha_2019/sim_snr.csv")
    sim_snr = np.array(sim_snr_data)

    sim_interp_snr = np.interp(ref_mag, sim_snr[:, 0], sim_snr[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_snr - ref_snr)**2) / np.sum(
        (ref_snr - np.mean(ref_snr))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_snr[:, 0],
            sim_snr[:, 1],
            'b-',
            lw=2.5,
            label='hot_jupiter PandExo SNR')
    ax.plot(ref_mag,
            ref_snr,
            'ko',
            ms=7,
            label='Batalha et al. (2019) Benchmarks')

    ax.set_xlabel(r"Host Star $J$-band Magnitude", fontsize=12)
    ax.set_ylabel("Signal-to-Noise Ratio (SNR)", fontsize=12)
    ax.set_title("Batalha et al. (2019) Figure 2: SNR vs Stellar Brightness",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/batalha_2019/fig2_snr.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Noise Precision R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 SNR Scaling R^2 Score:      {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Batalha et al. (2019) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_batalha2019()
