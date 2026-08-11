"""
Verification script for Brogi et al. (2016) ApJ 817, 106.
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


def verify_brogi2016():
    ref_rows = load_csv("replications/brogi_2016/reference_data.csv")

    # Figure 1: Wind Blueshift CCF Profile (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_voff = ref_fig1_data[:, 0]
    ref_sn1 = ref_fig1_data[:, 1]

    sim_wind_data = load_csv("replications/brogi_2016/sim_wind_sweep.csv")
    sim_wind = np.array(sim_wind_data)
    sim_wind = sim_wind[np.argsort(sim_wind[:, 0])]

    sim_interp_sn1 = np.interp(ref_voff, sim_wind[:, 0], sim_wind[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_sn1 - ref_sn1)**2) / np.sum(
        (ref_sn1 - np.mean(ref_sn1))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_wind[:, 0],
            sim_wind[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter Model $CCF$ Wind Profile')
    ax.plot(ref_voff,
            ref_sn1,
            'ko',
            ms=7,
            label='Brogi et al. (2016) CRIRES Data')

    ax.set_xlabel(r"Velocity Offset $V_{\mathrm{offset}}$ [km s$^{-1}$]",
                  fontsize=12)
    ax.set_ylabel(r"Cross-Correlation Significance [S/N]", fontsize=12)
    ax.set_title(
        r"Brogi et al. (2016) Figure 1: Day-to-Night Jetstream Wind ($v_{\mathrm{wind}} = -1.9$ km s$^{-1}$)",
        fontsize=12)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/brogi_2016/fig1_wind_blueshift.png", dpi=300)
    plt.close(fig)

    # Figure 2: Rotational Broadening CCF Profile (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_vrot = ref_fig2_data[:, 0]
    ref_sn2 = ref_fig2_data[:, 1]

    sim_rot_data = load_csv("replications/brogi_2016/sim_rot_sweep.csv")
    sim_rot = np.array(sim_rot_data)
    sim_rot = sim_rot[np.argsort(sim_rot[:, 0])]

    sim_interp_sn2 = np.interp(ref_vrot, sim_rot[:, 0], sim_rot[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_sn2 - ref_sn2)**2) / np.sum(
        (ref_sn2 - np.mean(ref_sn2))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_rot[:, 0],
            sim_rot[:, 1],
            'r-',
            lw=2.5,
            label=r'hot_jupiter Rotational Broadening')
    ax.plot(ref_vrot, ref_sn2, 'ko', ms=7, label='Brogi et al. (2016) Data')

    ax.set_xlabel(r"Rotational Velocity $v_{\mathrm{rot}}\sin i$ [km s$^{-1}$]",
                  fontsize=12)
    ax.set_ylabel(r"Cross-Correlation Significance [S/N]", fontsize=12)
    ax.set_title(
        r"Brogi et al. (2016) Figure 2: Planetary Rotation ($v_{\mathrm{rot}}\sin i = 3.4$ km s$^{-1}$)",
        fontsize=12)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/brogi_2016/fig2_rotational_broadening.png",
                dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Wind Blueshift R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Rotational Broadening R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Brogi et al. (2016) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_brogi2016()
