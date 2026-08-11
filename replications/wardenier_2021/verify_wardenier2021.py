"""
Verification script for Wardenier et al. (2021) MNRAS 506, 1258.
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


def verify_wardenier2021():
    ref_rows = load_csv("replications/wardenier_2021/reference_data.csv")

    # Figure 1: Evening Limb Transmission Spectrum (first 9 data rows)
    ref_fig1_data = np.array(ref_rows[:9])
    ref_wl = ref_fig1_data[:, 0]
    ref_depth_eve = ref_fig1_data[:, 1]

    sim_trans_data = load_csv(
        "replications/wardenier_2021/sim_limb_transmission.csv")
    sim_trans = np.array(sim_trans_data)

    sim_interp_depth = np.interp(ref_wl, sim_trans[:, 0], sim_trans[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_depth - ref_depth_eve)**2) / np.sum(
        (ref_depth_eve - np.mean(ref_depth_eve))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_trans[:, 0],
            sim_trans[:, 1] * 100,
            'b-',
            lw=2.5,
            label='hot_jupiter Evening Limb Spectrum')
    ax.plot(ref_wl,
            ref_depth_eve * 100,
            'ko',
            ms=7,
            label='Wardenier et al. (2021) Evening Limb')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu\mathrm{m}$]", fontsize=12)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [\%]", fontsize=12)
    ax.set_title(
        "Wardenier et al. (2021) Figure 1: Evening Limb Transmission Spectrum",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/wardenier_2021/fig1_limb_transmission.png",
                dpi=300)
    plt.close(fig)

    # Figure 2: Evening Limb Thermal Profile T(P) vs Pressure P (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[9:])
    ref_p2 = ref_fig2_data[:, 0]
    ref_t_eve = ref_fig2_data[:, 2]

    sim_tp_data = load_csv("replications/wardenier_2021/sim_limb_tp.csv")
    sim_tp = np.array(sim_tp_data)

    sim_interp_t = np.interp(np.log10(ref_p2), np.log10(sim_tp[:, 0]),
                             sim_tp[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_t - ref_t_eve)**2) / np.sum(
        (ref_t_eve - np.mean(ref_t_eve))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_tp[:, 1],
            sim_tp[:, 0],
            'r-',
            lw=2.5,
            label='hot_jupiter Evening Limb $T(P)$')
    ax.plot(ref_t_eve,
            ref_p2,
            'ko',
            ms=7,
            label='Wardenier et al. (2021) Evening Limb')

    ax.set_yscale('log')
    ax.invert_yaxis()
    ax.set_xlabel(r"Temperature $T$ [K]", fontsize=12)
    ax.set_ylabel(r"Pressure $P$ [bar]", fontsize=12)
    ax.set_title(
        "Wardenier et al. (2021) Figure 2: Evening Limb Thermal Profile",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/wardenier_2021/fig2_limb_tp.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Evening Limb Transmission R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Evening Limb T(P) R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Wardenier et al. (2021) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_wardenier2021()
