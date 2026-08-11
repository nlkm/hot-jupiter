"""
Verification script for Kreidberg et al. (2014) Nature 505, 69.
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


def verify_kreidberg2014():
    ref_rows = load_csv("replications/kreidberg_2014/reference_data.csv")

    # Figure 1: GJ 1214b Transmission Spectrum (first 8 data rows)
    ref_fig1_data = np.array(ref_rows[:8])
    ref_wl = ref_fig1_data[:, 0]
    ref_depth = ref_fig1_data[:, 1]

    sim_trans_data = load_csv(
        "replications/kreidberg_2014/sim_gj1214b_spectrum.csv")
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
            label='Kreidberg et al. (2014) WFC3 Data')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu\mathrm{m}$]", fontsize=12)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [\%]", fontsize=12)
    ax.set_title(
        "Kreidberg et al. (2014) Figure 1: GJ 1214b Cloud Transmission Spectrum",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/kreidberg_2014/fig1_gj1214b_spectrum.png",
                dpi=300)
    plt.close(fig)

    # Figure 2: Chi2/dof Rejection vs Cloud Pressure (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[8:])
    ref_logp = np.log10(ref_fig2_data[:, 0])
    ref_chi2 = ref_fig2_data[:, 1]

    sim_chi2_data = load_csv("replications/kreidberg_2014/sim_chi2_dof.csv")
    sim_chi2 = np.array(sim_chi2_data)

    sim_interp_chi2 = np.interp(ref_logp, sim_chi2[:, 0], sim_chi2[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_chi2 - ref_chi2)**2) / np.sum(
        (ref_chi2 - np.mean(ref_chi2))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_chi2[:, 0],
            sim_chi2[:, 1],
            'r-',
            lw=2.5,
            label=r'hot_jupiter Model $\chi^2/\mathrm{dof}$')
    ax.plot(ref_logp,
            ref_chi2,
            'ko',
            ms=7,
            label=r'Kreidberg et al. (2014) $\chi^2/\mathrm{dof}$')

    ax.set_xlabel(r"$\log_{10} P_{\mathrm{cloud}}$ [bar]", fontsize=12)
    ax.set_ylabel(r"Model Rejection Significance $\chi^2/\mathrm{dof}$",
                  fontsize=12)
    ax.set_title(
        "Kreidberg et al. (2014) Figure 2: Rejection vs Cloud Top Pressure",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/kreidberg_2014/fig2_chi2_rejection.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 GJ 1214b Spectrum R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Chi2 Rejection R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Kreidberg et al. (2014) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_kreidberg2014()
