"""
Verification script for Espinoza et al. (2019) MNRAS 482, 2065.
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


def verify_espinoza2019():
    ref_rows = load_csv("replications/espinoza_2019/reference_data.csv")

    # Figure 1: WASP-19b Optical Transmission Spectrum (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_wave = ref_fig1_data[:, 0]
    ref_depth = ref_fig1_data[:, 1]
    ref_err = ref_fig1_data[:, 2]

    sim_spec_data = load_csv(
        "replications/espinoza_2019/sim_transmission_spectrum.csv")
    sim_spec = np.array(sim_spec_data)

    sim_interp_depth = np.interp(ref_wave, sim_spec[:, 0], sim_spec[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_depth - ref_depth)**2) / np.sum(
        (ref_depth - np.mean(ref_depth))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_spec[:, 0],
            sim_spec[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter ACCESS Model')
    ax.errorbar(ref_wave,
                ref_depth,
                yerr=ref_err,
                fmt='ko',
                capsize=4,
                ms=6,
                label='Espinoza et al. (2019) Data')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [\%]", fontsize=12)
    ax.set_title(
        "Espinoza et al. (2019) Figure 1: WASP-19b Optical Spectrum with Na I",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/espinoza_2019/fig1_transmission_spectrum.png",
                dpi=300)
    plt.close(fig)

    # Figure 2: Sodium Abundance Posterior Distribution (next 7 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_log_xna = ref_fig2_data[:, 0]
    ref_prob = ref_fig2_data[:, 1]

    sim_prob_data = load_csv(
        "replications/espinoza_2019/sim_sodium_posterior.csv")
    sim_prob = np.array(sim_prob_data)

    sim_interp_prob = np.interp(ref_log_xna, sim_prob[:, 0], sim_prob[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_prob - ref_prob)**2) / np.sum(
        (ref_prob - np.mean(ref_prob))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_prob[:, 0],
            sim_prob[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter Posterior Density')
    ax.plot(ref_log_xna,
            ref_prob,
            'ko',
            ms=7,
            label='Espinoza et al. (2019) MCMC')

    ax.set_xlabel(r"Sodium Volume Mixing Ratio $\log_{10} X_{\mathrm{Na}}$",
                  fontsize=12)
    ax.set_ylabel("Posterior Probability Density $P$", fontsize=12)
    ax.set_title("Espinoza et al. (2019) Figure 2: Sodium Abundance Constraint",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/espinoza_2019/fig2_sodium_posterior.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Transmission Spectrum R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Sodium Posterior R^2 Score:       {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Espinoza et al. (2019) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_espinoza2019()
