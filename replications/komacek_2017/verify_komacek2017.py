"""
Verification script for Komacek et al. (2017) ApJ 835, 198.
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


def verify_komacek2017():
    ref_rows = load_csv("replications/komacek_2017/reference_data.csv")

    # Figure 1: Observed Phase Curve Amplitude A_obs (first 5 data rows)
    ref_fig1_data = np.array(ref_rows[:5])
    ref_teq = ref_fig1_data[:, 0]
    ref_a_obs = ref_fig1_data[:, 1]
    ref_a_weak = ref_fig1_data[:, 2]
    ref_a_strong = ref_fig1_data[:, 3]

    sim_amp_data = load_csv("replications/komacek_2017/sim_phase_amplitude.csv")
    sim_amp = np.array(sim_amp_data)

    sim_interp_amp = np.interp(ref_teq, sim_amp[:, 0], sim_amp[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_amp - ref_a_obs)**2) / np.sum(
        (ref_a_obs - np.mean(ref_a_obs))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_amp[:, 0],
            sim_amp[:, 1],
            'b-',
            lw=2.5,
            label='hot_jupiter Population Trend')
    ax.plot(ref_teq, ref_a_obs, 'ko', ms=7, label='Spitzer/Kepler Observations')
    ax.plot(ref_teq,
            ref_a_weak,
            'b--',
            alpha=0.7,
            label=r'Weak Drag GCM ($\tau_{\mathrm{drag}} = 10^7\,\mathrm{s}$)')
    ax.plot(
        ref_teq,
        ref_a_strong,
        'r:',
        alpha=0.7,
        label=r'Strong Drag GCM ($\tau_{\mathrm{drag}} = 10^4\,\mathrm{s}$)')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Observed Thermal Contrast Amplitude $A_{\mathrm{obs}}$",
                  fontsize=12)
    ax.set_title(
        "Komacek et al. (2017) Figure 1: Observed Hot Jupiter Phase Curves",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    plt.savefig("replications/komacek_2017/fig1_phase_amplitude.png", dpi=300)
    plt.close(fig)

    # Figure 2: Phase Curve Peak Eastward Offset (next 5 data rows)
    ref_fig2_data = np.array(ref_rows[5:])
    ref_teq_off = ref_fig2_data[:, 0]
    ref_offset = ref_fig2_data[:, 1]

    sim_off_data = load_csv("replications/komacek_2017/sim_phase_offset.csv")
    sim_off = np.array(sim_off_data)

    sim_interp_off = np.interp(ref_teq_off, sim_off[:, 0], sim_off[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_off - ref_offset)**2) / np.sum(
        (ref_offset - np.mean(ref_offset))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_off[:, 0],
            sim_off[:, 1],
            'g-',
            lw=2.5,
            label=r'hot_jupiter Offset Trend')
    ax.plot(ref_teq_off,
            ref_offset,
            'ko',
            ms=7,
            label='Komacek et al. (2017) Data')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(
        r"Hotspot Eastward Offset $\Delta \phi_{\mathrm{offset}}$ [deg]",
        fontsize=12)
    ax.set_title(
        "Komacek et al. (2017) Figure 2: Hotspot Eastward Offset vs Temperature",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/komacek_2017/fig2_phase_offset.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Observed Phase Amplitude R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Phase Peak Offset R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Komacek et al. (2017) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_komacek2017()
