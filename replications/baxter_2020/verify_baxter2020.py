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

    # Figure 1: Thermal Inversion Difference Delta T_inv (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_teq1 = ref_fig1_data[:, 0]
    ref_dt = ref_fig1_data[:, 1]

    sim_inv_data = load_csv("replications/baxter_2020/sim_inversion.csv")
    sim_inv = np.array(sim_inv_data)
    sim_inv = sim_inv[np.argsort(sim_inv[:, 0])]

    sim_interp_dt = np.interp(ref_teq1, sim_inv[:, 0], sim_inv[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_dt - ref_dt)**2) / np.sum(
        (ref_dt - np.mean(ref_dt))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_inv[:, 0],
            sim_inv[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter Model $\Delta T_{\mathrm{inv}}$')
    ax.plot(ref_teq1,
            ref_dt,
            'ko',
            ms=7,
            label='Baxter et al. (2020) Spitzer Population')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(
        r"Thermal Inversion $\Delta T_{\mathrm{inv}} = T_{4.5} - T_{3.6}$ [K]",
        fontsize=12)
    ax.set_title(
        r"Baxter et al. (2020) Figure 1: Onset of Thermal Inversions ($T_{\mathrm{eq}} \gtrsim 2200\,\text{K}$)",
        fontsize=12)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/baxter_2020/fig1_thermal_inversion.png", dpi=300)
    plt.close(fig)

    # Figure 2: Dayside 3.6 um Brightness Temperature T_3.6 (next 7 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_teq2 = ref_fig2_data[:, 0]
    ref_tb = ref_fig2_data[:, 1]

    sim_tb_data = load_csv("replications/baxter_2020/sim_tbright_36.csv")
    sim_tb = np.array(sim_tb_data)
    sim_tb = sim_tb[np.argsort(sim_tb[:, 0])]

    sim_interp_tb = np.interp(ref_teq2, sim_tb[:, 0], sim_tb[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_tb - ref_tb)**2) / np.sum(
        (ref_tb - np.mean(ref_tb))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_tb[:, 0],
            sim_tb[:, 1],
            'r-',
            lw=2.5,
            label=r'hot_jupiter Model $T_{3.6}(T_{\mathrm{eq}})$')
    ax.plot(ref_teq2,
            ref_tb,
            'ko',
            ms=7,
            label='Baxter et al. (2020) Population Data')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Dayside $3.6\,\mu\mathrm{m}$ Brightness Temp $T_{3.6}$ [K]",
                  fontsize=12)
    ax.set_title(
        r"Baxter et al. (2020) Figure 2: Dayside Brightness Temperature Trend",
        fontsize=12)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/baxter_2020/fig2_brightness_temp_36.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Thermal Inversion Onset R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Dayside Brightness Temp R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Baxter et al. (2020) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_baxter2020()
