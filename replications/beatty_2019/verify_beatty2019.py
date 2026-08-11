"""
Verification script for Beatty et al. (2019) AJ 158, 166.
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


def verify_beatty2019():
    ref_rows = load_csv("replications/beatty_2019/reference_data.csv")

    # Figure 1: KELT-1b Phase Curve (first 11 data rows)
    ref_fig1_data = np.array(ref_rows[:11])
    ref_phase1 = ref_fig1_data[:, 0]
    ref_flux = ref_fig1_data[:, 1]

    sim_pc_data = load_csv("replications/beatty_2019/sim_phase_curve.csv")
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
            label=r'hot_jupiter KELT-1b Phase Curve Model')
    ax.plot(ref_phase1,
            ref_flux,
            'ko',
            ms=7,
            label='Beatty et al. (2019) Spitzer Data')

    ax.set_xlabel(r"Orbital Phase $\phi$", fontsize=12)
    ax.set_ylabel(r"Secondary Eclipse Depth [ppm]", fontsize=12)
    ax.set_title(
        r"Beatty et al. (2019) Figure 1: KELT-1b Spitzer $3.6\,\mu\mathrm{m}$ Phase Curve",
        fontsize=12)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/beatty_2019/fig1_phase_curve.png", dpi=300)
    plt.close(fig)

    # Figure 2: Recirculation Efficiency (next 7 data rows)
    ref_fig2_data = np.array(ref_rows[11:])
    ref_teq2 = ref_fig2_data[:, 0]
    ref_eps = ref_fig2_data[:, 1]

    sim_rec_data = load_csv("replications/beatty_2019/sim_recirculation.csv")
    sim_rec = np.array(sim_rec_data)
    sim_rec = sim_rec[np.argsort(sim_rec[:, 0])]

    sim_interp_eps = np.interp(ref_teq2, sim_rec[:, 0], sim_rec[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_eps - ref_eps)**2) / np.sum(
        (ref_eps - np.mean(ref_eps))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_rec[:, 0],
            sim_rec[:, 1],
            'r-',
            lw=2.5,
            label=r'hot_jupiter Model $\varepsilon(T_{\mathrm{eq}})$')
    ax.plot(ref_teq2,
            ref_eps,
            'ko',
            ms=7,
            label='Beatty et al. (2019) Benchmark')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(r"Heat Recirculation Efficiency $\varepsilon$", fontsize=12)
    ax.set_title(
        r"Beatty et al. (2019) Figure 2: Recirculation Efficiency vs $T_{\mathrm{eq}}$",
        fontsize=12)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/beatty_2019/fig2_recirculation_efficiency.png",
                dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 KELT-1b Phase Curve R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Recirculation Efficiency R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Beatty et al. (2019) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_beatty2019()
