"""
Verification script for Changeat et al. (2021) ApJ 913, 73.
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


def verify_changeat2021():
    ref_rows = load_csv("replications/changeat_2021/reference_data.csv")

    # Figure 1: HD 209458b Transmission Spectrum (first 9 data rows)
    ref_fig1_data = np.array(ref_rows[:9])
    ref_wl = ref_fig1_data[:, 0]
    ref_depth = ref_fig1_data[:, 1]

    sim_trans_data = load_csv(
        "replications/changeat_2021/sim_transmission_spectrum.csv")
    sim_trans = np.array(sim_trans_data)

    sim_interp_depth = np.interp(ref_wl, sim_trans[:, 0], sim_trans[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_depth - ref_depth)**2) / np.sum(
        (ref_depth - np.mean(ref_depth))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_trans[:, 0],
            sim_trans[:, 1] * 100,
            'b-',
            lw=2.5,
            label=r'hot_jupiter Model with $\mathrm{HCN}$')
    ax.plot(ref_wl,
            ref_depth * 100,
            'ko',
            ms=7,
            label='Changeat et al. (2021) Spectrum')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu\mathrm{m}$]", fontsize=12)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [\%]", fontsize=12)
    ax.set_title(
        "Changeat et al. (2021) Figure 1: HD 209458b Transmission Spectrum",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/changeat_2021/fig1_transmission_spectrum.png",
                dpi=300)
    plt.close(fig)

    # Figure 2: HCN Abundance Posterior PDF vs Log10 X_HCN (next 5 data rows)
    ref_fig2_data = np.array(ref_rows[9:])
    ref_logx = ref_fig2_data[:, 0]
    ref_pdf = ref_fig2_data[:, 1]

    sim_hcn_data = load_csv("replications/changeat_2021/sim_hcn_posterior.csv")
    sim_hcn = np.array(sim_hcn_data)

    sim_interp_pdf = np.interp(ref_logx, sim_hcn[:, 0], sim_hcn[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_pdf - ref_pdf)**2) / np.sum(
        (ref_pdf - np.mean(ref_pdf))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(
        sim_hcn[:, 0],
        sim_hcn[:, 1],
        'r-',
        lw=2.5,
        label=r'hot_jupiter Retrieved $\log_{10} X_{\mathrm{HCN}}$ Posterior')
    ax.plot(ref_logx,
            ref_pdf,
            'ko',
            ms=7,
            label='Changeat et al. (2021) Retrieval')

    ax.set_xlabel(r"$\log_{10} X_{\mathrm{HCN}}$ Volume Mixing Ratio",
                  fontsize=12)
    ax.set_ylabel(r"Posterior Probability Density", fontsize=12)
    ax.set_title(
        "Changeat et al. (2021) Figure 2: HD 209458b HCN Abundance Posterior",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/changeat_2021/fig2_hcn_posterior.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Transmission Spectrum R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 HCN Posterior R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Changeat et al. (2021) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_changeat2021()
