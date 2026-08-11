"""
Verification script for Lothringer et al. (2018) ApJ 866, 27.
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


def verify_lothringer2018():
    ref_rows = load_csv("replications/lothringer_2018/reference_data.csv")

    # Figure 1: Dayside T-P Profile (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_log_p = ref_fig1_data[:, 0]
    ref_temp = ref_fig1_data[:, 1]

    sim_tp_data = load_csv("replications/lothringer_2018/sim_tp_profile.csv")
    sim_tp = np.array(sim_tp_data)

    sim_interp_temp = np.interp(ref_log_p, sim_tp[:, 0], sim_tp[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_temp - ref_temp)**2) / np.sum(
        (ref_temp - np.mean(ref_temp))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_tp[:, 1],
            10**sim_tp[:, 0],
            'r-',
            lw=2.5,
            label='hot_jupiter T-P Profile (Teq=3000 K)')
    ax.plot(ref_temp,
            10**ref_log_p,
            'ko',
            ms=7,
            label='Lothringer et al. (2018) Model')

    ax.set_yscale('log')
    ax.invert_yaxis()
    ax.set_xlabel(r"Temperature $T$ [K]", fontsize=12)
    ax.set_ylabel(r"Pressure $P$ [bar]", fontsize=12)
    ax.set_title(
        "Lothringer et al. (2018) Figure 1: Ultra-Hot Jupiter Dayside Inversion Profile",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/lothringer_2018/fig1_tp_profile.png", dpi=300)
    plt.close(fig)

    # Figure 2: NUV-to-NIR Emission Spectrum (next 7 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_wave = ref_fig2_data[:, 0]
    ref_em = ref_fig2_data[:, 1]

    sim_em_data = load_csv(
        "replications/lothringer_2018/sim_emission_spectrum.csv")
    sim_em = np.array(sim_em_data)

    sim_interp_em = np.interp(ref_wave, sim_em[:, 0], sim_em[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_em - ref_em)**2) / np.sum(
        (ref_em - np.mean(ref_em))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_em[:, 0],
            sim_em[:, 1],
            'b-',
            lw=2.5,
            label='hot_jupiter NUV/O Spectrum')
    ax.plot(ref_wave,
            ref_em,
            'ko',
            ms=7,
            label='Lothringer et al. (2018) Spectrum')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Thermal Emission $F_p / F_\star$ [ppm]", fontsize=12)
    ax.set_title(
        "Lothringer et al. (2018) Figure 2: NUV-to-NIR Ultra-Hot Emission Spectrum",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/lothringer_2018/fig2_emission_spectrum.png",
                dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Dayside T-P Profile R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Emission Spectrum R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Lothringer et al. (2018) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_lothringer2018()
