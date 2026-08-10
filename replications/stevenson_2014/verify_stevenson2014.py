"""
Verification script for Stevenson et al. (2014) Science 346, 838.
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


def verify_stevenson2014():
    ref_rows = load_csv("replications/stevenson_2014/reference_data.csv")

    # Cleanly split Figure 1 (first 10 rows) and Figure 2 (next 10 rows)
    ref_fig1_data = np.array(ref_rows[:10])
    ref_phase = ref_fig1_data[:, 0]
    ref_flux = ref_fig1_data[:, 1]

    sim_phase_data = load_csv("replications/stevenson_2014/sim_phase_curve.csv")
    sim_phase = np.array(sim_phase_data)

    sim_interp_flux = np.interp(ref_phase, sim_phase[:, 0], sim_phase[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_flux - ref_flux)**2) / np.sum(
        (ref_flux - np.mean(ref_flux))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_phase[:, 0],
            sim_phase[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter Phase Curve Model')
    ax.plot(ref_phase, ref_flux, 'ko', ms=7, label='Stevenson et al. (2014)')

    ax.set_xlabel(r"Orbital Phase $\phi$", fontsize=12)
    ax.set_ylabel(r"Secondary Eclipse Flux Ratio $(F_p/F_\star)$ [ppm]",
                  fontsize=12)
    ax.set_title(
        "Stevenson et al. (2014) Figure 1: WASP-43b Spectroscopic Phase Curve",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/stevenson_2014/fig1_phase_curve.png", dpi=300)
    plt.close(fig)

    # Figure 2: Brightness Temperature Profile vs Longitude
    ref_fig2_data = np.array(ref_rows[10:])
    ref_lon = ref_fig2_data[:, 0]
    ref_temp = ref_fig2_data[:, 1]

    sim_temp_data = load_csv(
        "replications/stevenson_2014/sim_temperature_profile.csv")
    sim_temp = np.array(sim_temp_data)

    sim_interp_temp = np.interp(ref_lon, sim_temp[:, 0], sim_temp[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_temp - ref_temp)**2) / np.sum(
        (ref_temp - np.mean(ref_temp))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_temp[:, 0],
            sim_temp[:, 1],
            'b-',
            lw=2.5,
            label='hot_jupiter Thermal Profile')
    ax.plot(ref_lon, ref_temp, 'ko', ms=7, label='Stevenson et al. (2014)')

    ax.set_xlabel(r"Longitude $\phi$ [deg]", fontsize=12)
    ax.set_ylabel(r"Brightness Temperature $T_b$ [K]", fontsize=12)
    ax.set_title(
        "Stevenson et al. (2014) Figure 2: Longitudinal Temperature Profile",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/stevenson_2014/fig2_temperature_profile.png",
                dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Spectroscopic Phase Curve R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Temperature Profile R^2 Score:        {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Stevenson et al. (2014) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_stevenson2014()
