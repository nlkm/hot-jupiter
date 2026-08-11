"""
Verification script for May et al. (2021) AJ 162, 158.
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


def verify_may2021():
    ref_rows = load_csv("replications/may_2021/reference_data.csv")

    # Figure 1: WASP-76b 4.5um Phase Curve (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_phase76 = ref_fig1_data[:, 0]
    ref_flux76 = ref_fig1_data[:, 1]

    sim_76_data = load_csv("replications/may_2021/sim_wasp76b_phase.csv")
    sim_76 = np.array(sim_76_data)

    sim_interp_flux76 = np.interp(ref_phase76, sim_76[:, 0], sim_76[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_flux76 - ref_flux76)**2) / np.sum(
        (ref_flux76 - np.mean(ref_flux76))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_76[:, 0],
            sim_76[:, 1] * 1e3,
            'b-',
            lw=2.5,
            label=r'hot_jupiter WASP-76b $4.5\,\mu\mathrm{m}$')
    ax.plot(ref_phase76,
            ref_flux76 * 1e3,
            'ko',
            ms=7,
            label='May et al. (2021) Spitzer Data')

    ax.set_xlabel(r"Orbital Phase $\phi$", fontsize=12)
    ax.set_ylabel(r"Phase Curve Flux Ratio $F_p / F_\star \times 10^3$",
                  fontsize=12)
    ax.set_title(
        "May et al. (2021) Figure 1: WASP-76b 4.5um Spitzer Phase Curve",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/may_2021/fig1_wasp76b_phase.png", dpi=300)
    plt.close(fig)

    # Figure 2: WASP-121b 4.5um Phase Curve (next 7 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_phase121 = ref_fig2_data[:, 0]
    ref_flux121 = ref_fig2_data[:, 1]

    sim_121_data = load_csv("replications/may_2021/sim_wasp121b_phase.csv")
    sim_121 = np.array(sim_121_data)

    sim_interp_flux121 = np.interp(ref_phase121, sim_121[:, 0], sim_121[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_flux121 - ref_flux121)**2) / np.sum(
        (ref_flux121 - np.mean(ref_flux121))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_121[:, 0],
            sim_121[:, 1] * 1e3,
            'r-',
            lw=2.5,
            label=r'hot_jupiter WASP-121b $4.5\,\mu\mathrm{m}$')
    ax.plot(ref_phase121,
            ref_flux121 * 1e3,
            'ko',
            ms=7,
            label='May et al. (2021) Spitzer Data')

    ax.set_xlabel(r"Orbital Phase $\phi$", fontsize=12)
    ax.set_ylabel(r"Phase Curve Flux Ratio $F_p / F_\star \times 10^3$",
                  fontsize=12)
    ax.set_title(
        "May et al. (2021) Figure 2: WASP-121b 4.5um Spitzer Phase Curve",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/may_2021/fig2_wasp121b_phase.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 WASP-76b Phase Curve R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 WASP-121b Phase Curve R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ May et al. (2021) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_may2021()
