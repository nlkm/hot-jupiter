"""
Verification script for Showman et al. (2015) ApJ 801, 95.
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


def verify_showman2015():
    ref_rows = load_csv("replications/showman_2015/reference_data.csv")

    # Figure 1: Hotspot Phase Shift (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_tau = ref_fig1_data[:, 0]
    ref_shift = ref_fig1_data[:, 1]

    sim_shift_data = load_csv("replications/showman_2015/sim_phase_shift.csv")
    sim_shift = np.array(sim_shift_data)

    sim_interp_shift = np.interp(np.log10(ref_tau), np.log10(sim_shift[:, 0]),
                                 sim_shift[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_shift - ref_shift)**2) / np.sum(
        (ref_shift - np.mean(ref_shift))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_shift[:, 0],
            sim_shift[:, 1],
            'b-',
            lw=2.5,
            label='hot_jupiter 3D Wave Model')
    ax.plot(ref_tau, ref_shift, 'ko', ms=7, label='Showman et al. (2015) GCM')

    ax.set_xscale('log')
    ax.set_xlabel(r"Radiative Timescale $\tau_{\mathrm{rad}}$ [days]",
                  fontsize=12)
    ax.set_ylabel(
        r"Hotspot Eastward Offset $\Delta \phi_{\mathrm{hotspot}}$ [deg]",
        fontsize=12)
    ax.set_title(
        "Showman et al. (2015) Figure 1: Thermal Hotspot Eastward Offset",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/showman_2015/fig1_phase_shift.png", dpi=300)
    plt.close(fig)

    # Figure 2: Day-Night Temperature Contrast (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_press = ref_fig2_data[:, 0]
    ref_dt = ref_fig2_data[:, 1]

    sim_dt_data = load_csv("replications/showman_2015/sim_temp_contrast.csv")
    sim_dt = np.array(sim_dt_data)

    sim_interp_dt = np.interp(np.log10(ref_press), np.log10(sim_dt[:, 0]),
                              sim_dt[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_dt - ref_dt)**2) / np.sum(
        (ref_dt - np.mean(ref_dt))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_dt[:, 1],
            sim_dt[:, 0],
            'r-',
            lw=2.5,
            label=r'hot_jupiter $\Delta T_{\mathrm{day-night}}$')
    ax.plot(ref_dt, ref_press, 'ko', ms=7, label='Showman et al. (2015) GCM')

    ax.set_yscale('log')
    ax.invert_yaxis()
    ax.set_xlabel(
        r"Day-Night Temperature Difference $\Delta T_{\mathrm{day-night}}$ [K]",
        fontsize=12)
    ax.set_ylabel("Pressure $P$ [bar]", fontsize=12)
    ax.set_title(
        "Showman et al. (2015) Figure 2: Day-Night Thermal Contrast Profile",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/showman_2015/fig2_temp_contrast.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Hotspot Phase Shift R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Day-Night Temp Contrast R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Showman et al. (2015) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_showman2015()
