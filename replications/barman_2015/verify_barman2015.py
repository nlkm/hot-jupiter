"""
Verification script for Barman et al. (2015) ApJ 804, 61.
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


def verify_barman2015():
    ref_rows = load_csv("replications/barman_2015/reference_data.csv")

    # Figure 1: CCF S/N vs V_K (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_vk = ref_fig1_data[:, 0]
    ref_sn1 = ref_fig1_data[:, 1]

    sim_vk_data = load_csv("replications/barman_2015/sim_vk_sweep.csv")
    sim_vk = np.array(sim_vk_data)
    sim_vk = sim_vk[np.argsort(sim_vk[:, 0])]

    sim_interp_sn1 = np.interp(ref_vk, sim_vk[:, 0], sim_vk[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_sn1 - ref_sn1)**2) / np.sum(
        (ref_sn1 - np.mean(ref_sn1))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_vk[:, 0],
            sim_vk[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter Model $CCF$ S/N')
    ax.plot(ref_vk,
            ref_sn1,
            'ko',
            ms=7,
            label='Barman et al. (2015) HD 209458b')

    ax.set_xlabel(r"Orbital Velocity $K_p$ [km s$^{-1}$]", fontsize=12)
    ax.set_ylabel(r"Cross-Correlation Significance [S/N]", fontsize=12)
    ax.set_title(
        "Barman et al. (2015) Figure 1: High-Resolution CCF S/N vs Orbital Velocity",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/barman_2015/fig1_ccf_vk.png", dpi=300)
    plt.close(fig)

    # Figure 2: CCF S/N vs V_sys (next 7 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_vsys = ref_fig2_data[:, 0]
    ref_sn2 = ref_fig2_data[:, 1]

    sim_vsys_data = load_csv("replications/barman_2015/sim_vsys_sweep.csv")
    sim_vsys = np.array(sim_vsys_data)
    sim_vsys = sim_vsys[np.argsort(sim_vsys[:, 0])]

    sim_interp_sn2 = np.interp(ref_vsys, sim_vsys[:, 0], sim_vsys[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_sn2 - ref_sn2)**2) / np.sum(
        (ref_sn2 - np.mean(ref_sn2))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_vsys[:, 0],
            sim_vsys[:, 1],
            'r-',
            lw=2.5,
            label=r'hot_jupiter Model $CCF$ Slice')
    ax.plot(ref_vsys, ref_sn2, 'ko', ms=7, label='Barman et al. (2015) Slice')

    ax.set_xlabel(r"Systemic Velocity Offset $V_{\mathrm{sys}}$ [km s$^{-1}$]",
                  fontsize=12)
    ax.set_ylabel(r"Cross-Correlation Significance [S/N]", fontsize=12)
    ax.set_title(
        "Barman et al. (2015) Figure 2: 1D Doppler CCF Slice vs Systemic Velocity",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/barman_2015/fig2_ccf_vsys.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 CCF S/N vs V_K R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 CCF S/N vs V_sys R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Barman et al. (2015) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_barman2015()
