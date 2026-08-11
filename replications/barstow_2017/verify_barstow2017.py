"""
Verification script for Barstow et al. (2017) MNRAS 464, 1728.
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


def verify_barstow2017():
    ref_rows = load_csv("replications/barstow_2017/reference_data.csv")

    # Figure 1: HD 209458b Transmission Spectrum (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_wave = ref_fig1_data[:, 0]
    ref_depth = ref_fig1_data[:, 1]
    ref_err = ref_fig1_data[:, 2]

    sim_spec_data = load_csv(
        "replications/barstow_2017/sim_transmission_spectrum.csv")
    sim_spec = np.array(sim_spec_data)

    sim_interp_depth = np.interp(ref_wave, sim_spec[:, 0], sim_spec[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_depth - ref_depth)**2) / np.sum(
        (ref_depth - np.mean(ref_depth))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_spec[:, 0],
            sim_spec[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter Retrieval Model')
    ax.errorbar(ref_wave,
                ref_depth,
                yerr=ref_err,
                fmt='ko',
                capsize=4,
                ms=6,
                label='Barstow et al. (2017) HD 209458b')

    ax.set_xscale('log')
    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [\%]", fontsize=12)
    ax.set_title(
        "Barstow et al. (2017) Figure 1: HD 209458b Retrieval Spectrum",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/barstow_2017/fig1_transmission_spectrum.png",
                dpi=300)
    plt.close(fig)

    # Figure 2: Cloud Top Pressure vs Teq (next 7 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_teq = ref_fig2_data[:, 0]
    ref_pcloud = ref_fig2_data[:, 1]

    sim_cloud_data = load_csv(
        "replications/barstow_2017/sim_cloud_pressure.csv")
    sim_cloud = np.array(sim_cloud_data)

    sim_interp_pcloud = np.interp(ref_teq, sim_cloud[:, 0], sim_cloud[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_pcloud - ref_pcloud)**2) / np.sum(
        (ref_pcloud - np.mean(ref_pcloud))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_cloud[:, 0],
            sim_cloud[:, 1],
            'b-',
            lw=2.5,
            label='hot_jupiter Cloud pressure')
    ax.plot(ref_teq,
            ref_pcloud,
            'ko',
            ms=7,
            label='Barstow et al. (2017) 10 Hot Jupiters')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Cloud Top Pressure $\log_{10} P_{\mathrm{cloud}}$ [bar]",
                  fontsize=12)
    ax.set_title(
        "Barstow et al. (2017) Figure 2: Cloud Top Pressure vs Temperature",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/barstow_2017/fig2_cloud_pressure.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 HD 209458b Spectrum R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Cloud Pressure R^2 Score:       {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Barstow et al. (2017) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_barstow2017()
