"""
Verification script for Line et al. (2013) ApJ 775, 137.
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
        header = None
        for row in reader:
            if not row or row[0].startswith('#'):
                continue
            if header is None:
                header = row
                continue
            data.append([float(x) for x in row])
    data = np.array(data)
    return header, data


def verify_line2013():
    # Figure 1: Abundance Retrieval
    _, sim_abund = load_csv(
        "replications/line_2013/sim_abundance_retrieval.csv")

    # Ref values for Fig 1: H2O (-3.5), CO (-3.1), CO2 (-6.2), CH4 (-5.8)
    ref_fig1_med = np.array([-3.5, -3.1, -6.2, -5.8])
    sim_fig1_med = sim_abund[:, 1]

    r2_fig1 = 1.0 - (np.sum((sim_fig1_med - ref_fig1_med)**2) / np.sum(
        (ref_fig1_med - np.mean(ref_fig1_med))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    molecules = ['H2O', 'CO', 'CO2', 'CH4']
    x = np.arange(len(molecules))
    width = 0.35

    ax.bar(x - width / 2,
           ref_fig1_med,
           width,
           label='Line et al. (2013)',
           color='#34495e',
           alpha=0.8)
    ax.bar(x + width / 2,
           sim_fig1_med,
           width,
           label='hot_jupiter Library',
           color='#e74c3c',
           alpha=0.8)

    ax.set_xticks(x)
    ax.set_xticklabels(molecules, fontsize=12)
    ax.set_ylabel(r"Retrieved $\log_{10}(X_i)$", fontsize=12)
    ax.set_title("Line et al. (2013) Figure 1: Atmospheric Abundance Retrieval",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/line_2013/fig1_abundance_retrieval.png", dpi=300)
    plt.close(fig)

    # Figure 2: Eclipse Spectrum
    _, sim_spec = load_csv("replications/line_2013/sim_spectrum_retrieval.csv")

    ref_fig2_wave = np.array([3.6, 4.5, 5.8, 8.0])
    ref_fig2_flux = np.array([0.15, 0.22, 0.28, 0.34])

    # Interpolate simulation at reference wavelengths
    sim_interp = np.interp(ref_fig2_wave, sim_spec[:, 0], sim_spec[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp - ref_fig2_flux)**2) / np.sum(
        (ref_fig2_flux - np.mean(ref_fig2_flux))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_spec[:, 0],
            sim_spec[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter Model')
    ax.plot(ref_fig2_wave,
            ref_fig2_flux,
            'ko',
            ms=7,
            label='Line et al. (2013)')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"$F_{\mathrm{planet}} / F_{\star}$ [%]", fontsize=12)
    ax.set_title(
        "Line et al. (2013) Figure 2: Secondary Eclipse Spectrum Retrieval",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/line_2013/fig2_spectrum_retrieval.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Abundance Retrieval R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Spectrum Retrieval R^2 Score:  {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Line et al. (2013) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_line2013()
