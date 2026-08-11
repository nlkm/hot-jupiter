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

    # Figure 1: Gas-phase Fe Abundance (first 6 data rows)
    ref_fig1_data = np.array(ref_rows[:6])
    ref_teq1 = ref_fig1_data[:, 0]
    ref_xfe = ref_fig1_data[:, 1]

    sim_fe_data = load_csv("replications/parmentier_2018/sim_fe_abundance.csv")
    sim_fe = np.array(sim_fe_data)
    sim_fe = sim_fe[np.argsort(sim_fe[:, 0])]

    sim_interp_xfe = np.interp(ref_teq1, sim_fe[:, 0], sim_fe[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_xfe - ref_xfe)**2) / np.sum(
        (ref_xfe - np.mean(ref_xfe))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_fe[:, 0],
            sim_fe[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter Gas-Phase Fe Model')
    ax.plot(ref_teq1,
            ref_xfe,
            'ko',
            ms=7,
            label='Parmentier et al. (2018) Cold Trap')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Gas-Phase Fe Abundance $\log_{10} X_{\mathrm{Fe}}$",
                  fontsize=12)
    ax.set_title(
        "Parmentier et al. (2018) Figure 1: Nightside Condensate Cold-Trapping",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/parmentier_2018/fig1_fe_cold_trap.png", dpi=300)
    plt.close(fig)

    # Figure 2: Phase Curve Amplitude Ratio (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[6:])
    ref_teq2 = ref_fig2_data[:, 0]
    ref_amp = ref_fig2_data[:, 1]

    sim_amp_data = load_csv(
        "replications/parmentier_2018/sim_phase_amplitude.csv")
    sim_amp = np.array(sim_amp_data)
    sim_amp = sim_amp[np.argsort(sim_amp[:, 0])]

    sim_interp_amp = np.interp(ref_teq2, sim_amp[:, 0], sim_amp[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_amp - ref_amp)**2) / np.sum(
        (ref_amp - np.mean(ref_amp))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_amp[:, 0],
            sim_amp[:, 1],
            'r-',
            lw=2.5,
            label=r'hot_jupiter Model $A_{\mathrm{opt}}/A_{\mathrm{ir}}$')
    ax.plot(ref_teq2,
            ref_amp,
            'ko',
            ms=7,
            label='Parmentier et al. (2018) Data')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(
        r"Phase Curve Amplitude Ratio $A_{\mathrm{opt}}/A_{\mathrm{ir}}$",
        fontsize=12)
    ax.set_title(
        "Parmentier et al. (2018) Figure 2: Optical-to-Infrared Phase Ratio",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/parmentier_2018/fig2_phase_ratio.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Fe Cold Trap R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)")
    print(
        f"--> Fig 2 Phase Amplitude Ratio R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Parmentier et al. (2018) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_parmentier2018()
