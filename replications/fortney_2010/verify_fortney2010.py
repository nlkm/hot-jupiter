"""
Verification script for Fortney et al. (2010) ApJ 709, 1396.
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


def calc_r2(sim, ref):
    ss_tot = np.sum((ref - np.mean(ref))**2)
    ss_res = np.sum((sim - ref)**2)
    if ss_tot == 0:
        return 1.0 - np.mean(np.abs(sim - ref))
    return 1.0 - (ss_res / ss_tot)


def verify_fortney2010():
    ref_rows = load_csv("replications/fortney_2010/reference_data.csv")

    # Figure 1: Metallicity Grid (first 6 data rows)
    ref_fig1_data = np.array(ref_rows[:6])
    ref_wave = ref_fig1_data[:, 0]
    ref_d1 = ref_fig1_data[:, 1]
    ref_d10 = ref_fig1_data[:, 2]
    ref_d30 = ref_fig1_data[:, 3]

    sim_met_data = load_csv(
        "replications/fortney_2010/sim_metallicity_grid.csv")
    sim_met = np.array(sim_met_data)

    sim_interp_d1 = np.interp(ref_wave, sim_met[:, 0], sim_met[:, 1])
    sim_interp_d10 = np.interp(ref_wave, sim_met[:, 0], sim_met[:, 2])
    sim_interp_d30 = np.interp(ref_wave, sim_met[:, 0], sim_met[:, 3])

    r2_d1 = calc_r2(sim_interp_d1, ref_d1)
    r2_d10 = calc_r2(sim_interp_d10, ref_d10)
    r2_d30 = calc_r2(sim_interp_d30, ref_d30)
    r2_fig1 = (r2_d1 + r2_d10 + r2_d30) / 3.0

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_met[:, 0],
            sim_met[:, 1],
            'b-',
            lw=2,
            label=r'$1\times$ Solar Metallicity')
    ax.plot(ref_wave, ref_d1, 'bo', ms=5)

    ax.plot(sim_met[:, 0],
            sim_met[:, 2],
            'g-',
            lw=2,
            label=r'$10\times$ Solar Metallicity')
    ax.plot(ref_wave, ref_d10, 'go', ms=5)

    ax.plot(sim_met[:, 0],
            sim_met[:, 3],
            'r-',
            lw=2,
            label=r'$30\times$ Solar Metallicity')
    ax.plot(ref_wave, ref_d30, 'ro', ms=5)

    ax.set_xscale('log')
    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [\%]", fontsize=12)
    ax.set_title(
        "Fortney et al. (2010) Figure 1: Metallicity-Dependent Transmission Spectra",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/fortney_2010/fig1_metallicity_grid.png", dpi=300)
    plt.close(fig)

    # Figure 2: Cloud Deck Grid (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[6:])
    ref_wave_c = ref_fig2_data[:, 0]
    ref_d_clear = ref_fig2_data[:, 1]
    ref_d_10mbar = ref_fig2_data[:, 2]
    ref_d_1mbar = ref_fig2_data[:, 3]

    sim_cloud_data = load_csv("replications/fortney_2010/sim_cloud_grid.csv")
    sim_cloud = np.array(sim_cloud_data)

    sim_interp_d_clear = np.interp(ref_wave_c, sim_cloud[:, 0], sim_cloud[:, 1])
    sim_interp_d_10mbar = np.interp(ref_wave_c, sim_cloud[:, 0], sim_cloud[:,
                                                                           2])
    sim_interp_d_1mbar = np.interp(ref_wave_c, sim_cloud[:, 0], sim_cloud[:, 3])

    r2_clear = calc_r2(sim_interp_d_clear, ref_d_clear)
    r2_10mbar = calc_r2(sim_interp_d_10mbar, ref_d_10mbar)
    r2_1mbar = calc_r2(sim_interp_d_1mbar, ref_d_1mbar)
    r2_fig2 = (r2_clear + r2_10mbar + r2_1mbar) / 3.0

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_cloud[:, 0],
            sim_cloud[:, 1],
            'k-',
            lw=2,
            label='Clear Atmosphere')
    ax.plot(ref_wave_c, ref_d_clear, 'ko', ms=5)

    ax.plot(sim_cloud[:, 0],
            sim_cloud[:, 2],
            'b--',
            lw=2,
            label=r'$P_{\mathrm{cloud}} = 10\ \mathrm{mbar}$')
    ax.plot(ref_wave_c, ref_d_10mbar, 'bo', ms=5)

    ax.plot(sim_cloud[:, 0],
            sim_cloud[:, 3],
            'r:',
            lw=2,
            label=r'$P_{\mathrm{cloud}} = 1\ \mathrm{mbar}$')
    ax.plot(ref_wave_c, ref_d_1mbar, 'ro', ms=5)

    ax.set_xscale('log')
    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [\%]", fontsize=12)
    ax.set_title(
        "Fortney et al. (2010) Figure 2: Cloud Top Pressure Transmission Spectra",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/fortney_2010/fig2_cloud_grid.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Metallicity Grid R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Cloud Deck Grid R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Fortney et al. (2010) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_fortney2010()
