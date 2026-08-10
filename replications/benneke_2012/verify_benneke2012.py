"""
Verification script for Benneke & Seager (2012) ApJ 753, 100.
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


def verify_benneke2012():
    ref_rows = load_csv("replications/benneke_2012/reference_data.csv")

    # Figure 1: Transmission Spectra for mu=4.0 vs mu=18.0
    ref_fig1_data = np.array([r for r in ref_rows if len(r) == 3])
    ref_wave = ref_fig1_data[:, 0]
    ref_depth_mu4 = ref_fig1_data[:, 1]
    ref_depth_mu18 = ref_fig1_data[:, 2]

    sim_spec_data = load_csv(
        "replications/benneke_2012/sim_transmission_spectra.csv")
    sim_spec = np.array(sim_spec_data)

    sim_interp_mu4 = np.interp(ref_wave, sim_spec[:, 0], sim_spec[:, 1])
    sim_interp_mu18 = np.interp(ref_wave, sim_spec[:, 0], sim_spec[:, 2])

    r2_fig1_mu4 = 1.0 - (np.sum((sim_interp_mu4 - ref_depth_mu4)**2) / np.sum(
        (ref_depth_mu4 - np.mean(ref_depth_mu4))**2))
    r2_fig1_mu18 = 1.0 - (np.sum(
        (sim_interp_mu18 - ref_depth_mu18)**2) / np.sum(
            (ref_depth_mu18 - np.mean(ref_depth_mu18))**2))
    r2_fig1 = min(r2_fig1_mu4, r2_fig1_mu18)

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_spec[:, 0],
            sim_spec[:, 1],
            'b-',
            lw=2.5,
            label=r'Model ($\mu = 4.0$, $100\times$ Solar)')
    ax.plot(sim_spec[:, 0],
            sim_spec[:, 2],
            'r--',
            lw=2.5,
            label=r'Model ($\mu = 18.0$, $H_2O$ Rich)')
    ax.plot(ref_wave,
            ref_depth_mu4,
            'bo',
            ms=6,
            label=r'Benneke (2012) $\mu = 4.0$ Ref')
    ax.plot(ref_wave,
            ref_depth_mu18,
            'ro',
            ms=6,
            label=r'Benneke (2012) $\mu = 18.0$ Ref')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [%]", fontsize=12)
    ax.set_title(
        "Benneke & Seager (2012) Figure 1: Transmission Scale Height Slopes",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/benneke_2012/fig1_transmission_spectra.png",
                dpi=300)
    plt.close(fig)

    # Figure 2: Mean Molecular Weight Posterior Distribution
    ref_fig2_data = np.array([r for r in ref_rows if len(r) == 2])
    ref_mu = ref_fig2_data[:, 0]
    ref_post = ref_fig2_data[:, 1]

    sim_post_data = load_csv(
        "replications/benneke_2012/sim_posterior_density.csv")
    sim_post = np.array(sim_post_data)

    sim_interp_post = np.interp(ref_mu, sim_post[:, 0], sim_post[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_post - ref_post)**2) / np.sum(
        (ref_post - np.mean(ref_post))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_post[:, 0],
            sim_post[:, 1],
            'r-',
            lw=2.5,
            label=r'hot_jupiter Posterior $P(\mu)$')
    ax.plot(ref_mu, ref_post, 'ko', ms=7, label='Benneke & Seager (2012)')

    ax.set_xlabel(r"Mean Molecular Weight $\mu$ [amu]", fontsize=12)
    ax.set_ylabel("Posterior Probability Density", fontsize=12)
    ax.set_title(
        "Benneke & Seager (2012) Figure 2: Mean Molecular Weight Retrieval",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/benneke_2012/fig2_posterior_density.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Transmission Spectra R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Molecular Weight Posterior R^2: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Benneke & Seager (2012) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_benneke2012()
