"""
Verification script for Line et al. (2016) AJ 152, 203.
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


def verify_line2016():
    ref_rows = load_csv("replications/line_2016/reference_data.csv")

    # Figure 1: WASP-12b Secondary Eclipse Spectrum (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_wave = ref_fig1_data[:, 0]
    ref_flux = ref_fig1_data[:, 1]
    ref_err = ref_fig1_data[:, 2]

    sim_spec_data = load_csv("replications/line_2016/sim_secondary_eclipse.csv")
    sim_spec = np.array(sim_spec_data)

    sim_interp_flux = np.interp(ref_wave, sim_spec[:, 0], sim_spec[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_flux - ref_flux)**2) / np.sum(
        (ref_flux - np.mean(ref_flux))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_spec[:, 0],
            sim_spec[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter Model')
    ax.errorbar(ref_wave,
                ref_flux,
                yerr=ref_err,
                fmt='ko',
                capsize=4,
                ms=6,
                label='Line et al. (2016) WASP-12b Data')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Secondary Eclipse Flux Ratio $(F_p/F_\star)$ [ppm]",
                  fontsize=12)
    ax.set_title(
        "Line et al. (2016) Figure 1: WASP-12b Secondary Eclipse Spectrum",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/line_2016/fig1_secondary_eclipse.png", dpi=300)
    plt.close(fig)

    # Figure 2: Water Abundance Posterior Distribution (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_log_x = ref_fig2_data[:, 0]
    ref_post = ref_fig2_data[:, 1]

    sim_post_data = load_csv("replications/line_2016/sim_water_posterior.csv")
    sim_post = np.array(sim_post_data)

    sim_interp_post = np.interp(ref_log_x, sim_post[:, 0], sim_post[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_post - ref_post)**2) / np.sum(
        (ref_post - np.mean(ref_post))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_post[:, 0],
            sim_post[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter Posterior $P(\log_{10} X_{\mathrm{H2O}})$')
    ax.plot(ref_log_x, ref_post, 'ko', ms=7, label='Line et al. (2016)')

    ax.set_xlabel(r"Water Volume Mixing Ratio $\log_{10}(X_{\mathrm{H2O}})$",
                  fontsize=12)
    ax.set_ylabel("Posterior Probability Density", fontsize=12)
    ax.set_title("Line et al. (2016) Figure 2: Water Abundance Posterior",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/line_2016/fig2_water_posterior.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Secondary Eclipse R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Water Posterior R^2 Score:    {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Line et al. (2016) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_line2016()
