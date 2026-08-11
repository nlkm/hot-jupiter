"""
Verification script for Kreidberg et al. (2018) AJ 156, 17.
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


def verify_kreidberg2018():
    ref_rows = load_csv("replications/kreidberg_2018/reference_data.csv")

    # Figure 1: WASP-103b Phase Curve (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_phase1 = ref_fig1_data[:, 0]
    ref_flux = ref_fig1_data[:, 1]

    sim_pc_data = load_csv("replications/kreidberg_2018/sim_phase_curve.csv")
    sim_pc = np.array(sim_pc_data)
    sim_pc = sim_pc[np.argsort(sim_pc[:, 0])]

    sim_interp_flux = np.interp(ref_phase1, sim_pc[:, 0], sim_pc[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_flux - ref_flux)**2) / np.sum(
        (ref_flux - np.mean(ref_flux))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_pc[:, 0],
            sim_pc[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter WASP-103b Phase Curve Model')
    ax.plot(ref_phase1,
            ref_flux,
            'ko',
            ms=7,
            label='Kreidberg et al. (2018) HST WFC3 Data')

    ax.set_xlabel(r"Orbital Phase $\phi$", fontsize=12)
    ax.set_ylabel(r"Thermal Emission Flux Ratio $F_p/F_\star$", fontsize=12)
    ax.set_title(
        r"Kreidberg et al. (2018) Figure 1: WASP-103b Phase-Resolved Emission",
        fontsize=12)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/kreidberg_2018/fig1_phase_curve.png", dpi=300)
    plt.close(fig)

    # Figure 2: Longitudinal Temperature Profile (next 7 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_lon2 = ref_fig2_data[:, 0]
    ref_temp = ref_fig2_data[:, 1]

    sim_temp_data = load_csv("replications/kreidberg_2018/sim_lon_temp.csv")
    sim_temp = np.array(sim_temp_data)
    sim_temp = sim_temp[np.argsort(sim_temp[:, 0])]

    sim_interp_temp = np.interp(ref_lon2, sim_temp[:, 0], sim_temp[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_temp - ref_temp)**2) / np.sum(
        (ref_temp - np.mean(ref_temp))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_temp[:, 0],
            sim_temp[:, 1],
            'r-',
            lw=2.5,
            label=r'hot_jupiter Model $T(\mathrm{longitude})$')
    ax.plot(ref_lon2,
            ref_temp,
            'ko',
            ms=7,
            label='Kreidberg et al. (2018) Retrieval')

    ax.set_xlabel(r"Longitude [deg]", fontsize=12)
    ax.set_ylabel(r"Brightness Temperature $T_{\mathrm{bright}}$ [K]",
                  fontsize=12)
    ax.set_title(
        r"Kreidberg et al. (2018) Figure 2: WASP-103b Longitudinal Temperature Profile",
        fontsize=12)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/kreidberg_2018/fig2_lon_temperature.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 WASP-103b Phase Curve R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Longitudinal Temperature R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Kreidberg et al. (2018) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_kreidberg2018()
