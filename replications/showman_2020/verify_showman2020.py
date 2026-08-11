"""
Verification script for Showman et al. (2020) ApJ 891, 78.
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


def verify_showman2020():
    ref_rows = load_csv("replications/showman_2020/reference_data.csv")

    # Figure 1: Phase Curve Amplitude vs Teq (first 6 data rows)
    ref_fig1_data = np.array(ref_rows[:6])
    ref_teq = ref_fig1_data[:, 0]
    ref_aphase = ref_fig1_data[:, 1]

    sim_amp_data = load_csv("replications/showman_2020/sim_phase_amplitude.csv")
    sim_amp = np.array(sim_amp_data)

    sim_interp_amp = np.interp(ref_teq, sim_amp[:, 0], sim_amp[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_amp - ref_aphase)**2) / np.sum(
        (ref_aphase - np.mean(ref_aphase))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_amp[:, 0],
            sim_amp[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter Model $A_{\mathrm{phase}}$')
    ax.plot(ref_teq, ref_aphase, 'ko', ms=7, label='Showman et al. (2020) GCM')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Phase Curve Amplitude $A_{\mathrm{phase}}$", fontsize=12)
    ax.set_title("Showman et al. (2020) Figure 1: Phase Curve Amplitude",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/showman_2020/fig1_phase_amplitude.png", dpi=300)
    plt.close(fig)

    # Figure 2: Hotspot Offset vs Teq (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[6:])
    ref_teq2 = ref_fig2_data[:, 0]
    ref_offset = ref_fig2_data[:, 1]

    sim_off_data = load_csv("replications/showman_2020/sim_hotspot_offset.csv")
    sim_off = np.array(sim_off_data)

    sim_interp_off = np.interp(ref_teq2, sim_off[:, 0], sim_off[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_off - ref_offset)**2) / np.sum(
        (ref_offset - np.mean(ref_offset))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_off[:, 0],
            sim_off[:, 1],
            'r-',
            lw=2.5,
            label=r'hot_jupiter Model $\Delta \phi_{\mathrm{hotspot}}$')
    ax.plot(ref_teq2, ref_offset, 'ko', ms=7, label='Showman et al. (2020) GCM')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Hotspot Offset $\Delta \phi_{\mathrm{hotspot}}$ [deg]",
                  fontsize=12)
    ax.set_title("Showman et al. (2020) Figure 2: Hotspot Phase Offset",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/showman_2020/fig2_hotspot_offset.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Phase Curve Amplitude R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Hotspot Offset R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Showman et al. (2020) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_showman2020()
