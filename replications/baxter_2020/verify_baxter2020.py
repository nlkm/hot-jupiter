"""
Verification script for Baxter et al. (2020) A&A 639, A36.
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


def verify_baxter2020():
    ref_rows = load_csv("replications/baxter_2020/reference_data.csv")

    # Figure 1: Dayside Brightness Temperature T_bright [K] vs Teq [K] (first 6 data rows)
    ref_fig1_data = np.array(ref_rows[:6])
    ref_teq = ref_fig1_data[:, 0]
    ref_tb36 = ref_fig1_data[:, 1]
    ref_tb45 = ref_fig1_data[:, 2]

    sim_tb_data = load_csv("replications/baxter_2020/sim_tbright.csv")
    sim_tb = np.array(sim_tb_data)

    sim_interp_tb36 = np.interp(ref_teq, sim_tb[:, 0], sim_tb[:, 1])
    sim_interp_tb45 = np.interp(ref_teq, sim_tb[:, 0], sim_tb[:, 2])

    r2_fig1_36 = 1.0 - (np.sum((sim_interp_tb36 - ref_tb36)**2) / np.sum(
        (ref_tb36 - np.mean(ref_tb36))**2))
    r2_fig1_45 = 1.0 - (np.sum((sim_interp_tb45 - ref_tb45)**2) / np.sum(
        (ref_tb45 - np.mean(ref_tb45))**2))
    r2_fig1 = (r2_fig1_36 + r2_fig1_45) / 2.0

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_tb[:, 0],
            sim_tb[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter 3.6 $\mu$m $T_{\mathrm{bright}}$')
    ax.plot(sim_tb[:, 0],
            sim_tb[:, 2],
            'r--',
            lw=2.5,
            label=r'hot_jupiter 4.5 $\mu$m $T_{\mathrm{bright}}$')
    ax.plot(ref_teq,
            ref_tb36,
            'bo',
            ms=7,
            label=r'Baxter et al. (2020) 3.6 $\mu$m Data')
    ax.plot(ref_teq,
            ref_tb45,
            'ro',
            ms=7,
            label=r'Baxter et al. (2020) 4.5 $\mu$m Data')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Brightness Temperature $T_{\mathrm{bright}}$ [K]",
                  fontsize=12)
    ax.set_title(
        "Baxter et al. (2020) Figure 1: Dayside Brightness Temperatures",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/baxter_2020/fig1_tbright.png", dpi=300)
    plt.close(fig)

    # Figure 2: Brightness Temperature Difference Delta T_bright vs Teq (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[6:])
    ref_teq_diff = ref_fig2_data[:, 0]
    ref_delta_tb = ref_fig2_data[:, 1]

    sim_dtb_data = load_csv("replications/baxter_2020/sim_delta_tbright.csv")
    sim_dtb = np.array(sim_dtb_data)

    sim_interp_dtb = np.interp(ref_teq_diff, sim_dtb[:, 0], sim_dtb[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_dtb - ref_delta_tb)**2) / np.sum(
        (ref_delta_tb - np.mean(ref_delta_tb))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_dtb[:, 0],
            sim_dtb[:, 1],
            'g-',
            lw=2.5,
            label=r'hot_jupiter $\Delta T_{\mathrm{bright}}$ Model')
    ax.plot(ref_teq_diff,
            ref_delta_tb,
            'ko',
            ms=7,
            label='Baxter et al. (2020) Observations')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"$\Delta T_{\mathrm{bright}} = T_{3.6} - T_{4.5}$ [K]",
                  fontsize=12)
    ax.set_title(
        "Baxter et al. (2020) Figure 2: Brightness Temperature Difference",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/baxter_2020/fig2_delta_tbright.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Brightness Temperatures R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Delta Brightness Temperature R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Baxter et al. (2020) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_baxter2020()
