"""
Verification script for Colón et al. (2020) AJ 160, 243.
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


def verify_colon2020():
    ref_rows = load_csv("replications/colon_2020/reference_data.csv")

    # Figure 1: WASP-52b Optical Transmission Spectrum (first 9 data rows)
    ref_fig1_data = np.array(ref_rows[:9])
    ref_wl = ref_fig1_data[:, 0]
    ref_depth = ref_fig1_data[:, 1]

    sim_trans_data = load_csv(
        "replications/colon_2020/sim_optical_transmission.csv")
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
            label='Colón et al. (2020) GTC Data')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu\mathrm{m}$]", fontsize=12)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [\%]", fontsize=12)
    ax.set_title(
        "Colón et al. (2020) Figure 1: WASP-52b Optical Transmission Spectrum",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/colon_2020/fig1_optical_spectrum.png", dpi=300)
    plt.close(fig)

    # Figure 2: Na Abundance Posterior (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[9:])
    ref_logx = ref_fig2_data[:, 0]
    ref_prob = ref_fig2_data[:, 1]

    sim_post_data = load_csv("replications/colon_2020/sim_na_posterior.csv")
    sim_post = np.array(sim_post_data)

    sim_interp_prob = np.interp(ref_logx, sim_post[:, 0], sim_post[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_prob - ref_prob)**2) / np.sum(
        (ref_prob - np.mean(ref_prob))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_post[:, 0],
            sim_post[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter Posterior')
    ax.plot(ref_logx,
            ref_prob,
            'ko',
            ms=7,
            label='Colón et al. (2020) Posterior')

    ax.set_xlabel(r"$\log_{10} X_{\mathrm{Na}}$ Abundance", fontsize=12)
    ax.set_ylabel(r"Probability Density", fontsize=12)
    ax.set_title(
        "Colón et al. (2020) Figure 2: Retrieved Na Abundance Posterior",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/colon_2020/fig2_na_posterior.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Optical Transmission R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Na Posterior R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)")

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Colón et al. (2020) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_colon2020()
