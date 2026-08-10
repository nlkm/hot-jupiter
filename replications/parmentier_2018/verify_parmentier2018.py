"""
Verification script for Parmentier et al. (2018) A&A 617, A110.
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


def verify_parmentier2018():
    ref_rows = load_csv("replications/parmentier_2018/reference_data.csv")

    # Figure 1: Thermal Profile T(P) (first 6 data rows)
    ref_fig1_data = np.array(ref_rows[:6])
    ref_p_bar = ref_fig1_data[:, 0]
    ref_temp = ref_fig1_data[:, 1]

    sim_tp_data = load_csv(
        "replications/parmentier_2018/sim_thermal_profile.csv")
    sim_tp = np.array(sim_tp_data)

    sim_interp_temp = np.interp(np.log10(ref_p_bar), np.log10(sim_tp[:, 0]),
                                sim_tp[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_temp - ref_temp)**2) / np.sum(
        (ref_temp - np.mean(ref_temp))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_tp[:, 1],
            sim_tp[:, 0],
            'r-',
            lw=2.5,
            label=r'hot_jupiter Model ($T_{\mathrm{eq}}=2400$ K)')
    ax.plot(ref_temp, ref_p_bar, 'ko', ms=7, label='Parmentier et al. (2018)')

    ax.set_yscale('log')
    ax.invert_yaxis()
    ax.set_xlabel(r"Temperature $T$ [K]", fontsize=12)
    ax.set_ylabel(r"Pressure $P$ [bar]", fontsize=12)
    ax.set_title("Parmentier et al. (2018) Figure 1: Thermal Inversion Profile",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/parmentier_2018/fig1_thermal_profile.png",
                dpi=300)
    plt.close(fig)

    # Figure 2: Brightness Temperature Contrast Delta T_b (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[6:])
    ref_teq = ref_fig2_data[:, 0]
    ref_contrast = ref_fig2_data[:, 1]

    sim_contrast_data = load_csv(
        "replications/parmentier_2018/sim_contrast.csv")
    sim_contrast = np.array(sim_contrast_data)

    sim_interp_contrast = np.interp(ref_teq, sim_contrast[:, 0],
                                    sim_contrast[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_contrast - ref_contrast)**2) / np.sum(
        (ref_contrast - np.mean(ref_contrast))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_contrast[:, 0],
            sim_contrast[:, 1],
            'b-',
            lw=2.5,
            label='hot_jupiter Contrast Model')
    ax.plot(ref_teq, ref_contrast, 'ko', ms=7, label='Parmentier et al. (2018)')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Brightness Temp Contrast $\Delta T_b$ [K]", fontsize=12)
    ax.set_title("Parmentier et al. (2018) Figure 2: Emission Contrast Peak",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/parmentier_2018/fig2_brightness_contrast.png",
                dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Thermal Profile R^2 Score:    {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Brightness Contrast R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Parmentier et al. (2018) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_parmentier2018()
