"""
Verification script for Carone et al. (2020) A&A 638, A14.
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


def verify_carone2020():
    ref_rows = load_csv("replications/carone_2020/reference_data.csv")

    # Figure 1: WASP-43b Zonal Wind Speed U_eq [m/s] vs Pressure P (first 8 data rows)
    ref_fig1_data = np.array(ref_rows[:8])
    ref_p43 = ref_fig1_data[:, 0]
    ref_u43 = ref_fig1_data[:, 1]

    sim_43_data = load_csv("replications/carone_2020/sim_wasp43b_jet.csv")
    sim_43 = np.array(sim_43_data)

    sim_interp_u43 = np.interp(np.log10(ref_p43), np.log10(sim_43[:, 0]),
                               sim_43[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_u43 - ref_u43)**2) / np.sum(
        (ref_u43 - np.mean(ref_u43))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_43[:, 1],
            sim_43[:, 0],
            'b-',
            lw=2.5,
            label='hot_jupiter WASP-43b Zonal Wind')
    ax.plot(ref_u43, ref_p43, 'ko', ms=7, label='Carone et al. (2020) 3D GCM')

    ax.set_yscale('log')
    ax.invert_yaxis()
    ax.set_xlabel(r"Equatorial Zonal Wind $U_{\mathrm{eq}}$ [m/s]", fontsize=12)
    ax.set_ylabel(r"Pressure $P$ [bar]", fontsize=12)
    ax.set_title(
        "Carone et al. (2020) Figure 1: WASP-43b Vertical Zonal Wind Profile",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/carone_2020/fig1_wasp43b_jet.png", dpi=300)
    plt.close(fig)

    # Figure 2: HD 209458b Zonal Wind Speed U_eq [m/s] vs Pressure P (next 8 data rows)
    ref_fig2_data = np.array(ref_rows[8:])
    ref_p209 = ref_fig2_data[:, 0]
    ref_u209 = ref_fig2_data[:, 1]

    sim_209_data = load_csv("replications/carone_2020/sim_hd209458b_jet.csv")
    sim_209 = np.array(sim_209_data)

    sim_interp_u209 = np.interp(np.log10(ref_p209), np.log10(sim_209[:, 0]),
                                sim_209[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_u209 - ref_u209)**2) / np.sum(
        (ref_u209 - np.mean(ref_u209))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_209[:, 1],
            sim_209[:, 0],
            'r-',
            lw=2.5,
            label='hot_jupiter HD 209458b Zonal Wind')
    ax.plot(ref_u209, ref_p209, 'ko', ms=7, label='Carone et al. (2020) 3D GCM')

    ax.set_yscale('log')
    ax.invert_yaxis()
    ax.set_xlabel(r"Equatorial Zonal Wind $U_{\mathrm{eq}}$ [m/s]", fontsize=12)
    ax.set_ylabel(r"Pressure $P$ [bar]", fontsize=12)
    ax.set_title(
        "Carone et al. (2020) Figure 2: HD 209458b Vertical Zonal Wind Profile",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/carone_2020/fig2_hd209458b_jet.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 WASP-43b Zonal Wind Profile R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 HD 209458b Zonal Wind Profile R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Carone et al. (2020) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_carone2020()
