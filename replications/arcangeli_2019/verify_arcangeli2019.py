"""
Verification script for Arcangeli et al. (2019) A&A 625, A136.
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


def verify_arcangeli2019():
    ref_rows = load_csv("replications/arcangeli_2019/reference_data.csv")

    # Figure 1: WASP-18b Dayside Spectrum (first 10 data rows)
    ref_fig1_data = np.array(ref_rows[:10])
    ref_wl = ref_fig1_data[:, 0]
    ref_flux = ref_fig1_data[:, 1]

    sim_ds_data = load_csv(
        "replications/arcangeli_2019/sim_dayside_spectrum.csv")
    sim_ds = np.array(sim_ds_data)

    sim_interp_flux = np.interp(ref_wl, sim_ds[:, 0], sim_ds[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_flux - ref_flux)**2) / np.sum(
        (ref_flux - np.mean(ref_flux))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_ds[:, 0],
            sim_ds[:, 1],
            'b-',
            lw=2.5,
            label='hot_jupiter Dayside Spectrum')
    ax.plot(ref_wl,
            ref_flux,
            'ko',
            ms=7,
            label='Arcangeli et al. (2019) HST Data')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Secondary Eclipse Depth $F_p / F_\star$ [ppm]", fontsize=12)
    ax.set_title(
        "Arcangeli et al. (2019) Figure 1: WASP-18b Dayside HST Spectrum",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/arcangeli_2019/fig1_dayside_spectrum.png",
                dpi=300)
    plt.close(fig)

    # Figure 2: Day vs Night Emission Spectra (next 5 data rows)
    ref_fig2_data = np.array(ref_rows[10:])
    ref_wl_dn = ref_fig2_data[:, 0]
    ref_fday = ref_fig2_data[:, 1]
    ref_fnight = ref_fig2_data[:, 2]

    sim_dn_data = load_csv(
        "replications/arcangeli_2019/sim_daynight_spectrum.csv")
    sim_dn = np.array(sim_dn_data)

    sim_interp_fday = np.interp(ref_wl_dn, sim_dn[:, 0], sim_dn[:, 1])
    sim_interp_fnight = np.interp(ref_wl_dn, sim_dn[:, 0], sim_dn[:, 2])

    r2_fig2_day = 1.0 - (np.sum((sim_interp_fday - ref_fday)**2) / np.sum(
        (ref_fday - np.mean(ref_fday))**2))
    r2_fig2_night = 1.0 - (np.sum((sim_interp_fnight - ref_fnight)**2) / np.sum(
        (ref_fnight - np.mean(ref_fnight))**2))
    r2_fig2 = (r2_fig2_day + r2_fig2_night) / 2.0

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_dn[:, 0],
            sim_dn[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter Dayside (2900 K)')
    ax.plot(sim_dn[:, 0],
            sim_dn[:, 2],
            'b--',
            lw=2.5,
            label='hot_jupiter Nightside (1500 K)')
    ax.plot(ref_wl_dn,
            ref_fday,
            'ro',
            ms=7,
            label='Arcangeli (2019) Dayside Data')
    ax.plot(ref_wl_dn,
            ref_fnight,
            'bo',
            ms=7,
            label='Arcangeli (2019) Nightside Data')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Thermal Emission $F_p / F_\star$ [ppm]", fontsize=12)
    ax.set_title(
        "Arcangeli et al. (2019) Figure 2: WASP-18b Day-Night Emission Comparison",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/arcangeli_2019/fig2_daynight_spectrum.png",
                dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Dayside HST Spectrum R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Day-Night Emission Comparison R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Arcangeli et al. (2019) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_arcangeli2019()
