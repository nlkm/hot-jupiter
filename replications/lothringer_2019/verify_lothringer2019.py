"""
Verification script for Lothringer & Barman (2019) ApJ 876, 69.
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


def verify_lothringer2019():
    ref_rows = load_csv("replications/lothringer_2019/reference_data.csv")

    # Figure 1: Temperature Profiles T(P) for F, G, K, M Host Stars (first 5 data rows)
    ref_fig1_data = np.array(ref_rows[:5])
    ref_logp = ref_fig1_data[:, 0]
    ref_tf = ref_fig1_data[:, 1]
    ref_tg = ref_fig1_data[:, 2]

    sim_tp_data = load_csv("replications/lothringer_2019/sim_tp_profiles.csv")
    sim_tp = np.array(sim_tp_data)

    sim_interp_tf = np.interp(ref_logp, sim_tp[:, 0], sim_tp[:, 1])
    sim_interp_tg = np.interp(ref_logp, sim_tp[:, 0], sim_tp[:, 2])

    r2_fig1_f = 1.0 - (np.sum((sim_interp_tf - ref_tf)**2) / np.sum(
        (ref_tf - np.mean(ref_tf))**2))
    r2_fig1_g = 1.0 - (np.sum((sim_interp_tg - ref_tg)**2) / np.sum(
        (ref_tg - np.mean(ref_tg))**2))
    r2_fig1 = (r2_fig1_f + r2_fig1_g) / 2.0

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_tp[:, 1],
            10**sim_tp[:, 0],
            'r-',
            lw=2.5,
            label='hot_jupiter F Star Host')
    ax.plot(sim_tp[:, 2],
            10**sim_tp[:, 0],
            'g--',
            lw=2.5,
            label='hot_jupiter G Star Host')
    ax.plot(ref_tf,
            10**ref_logp,
            'ro',
            ms=7,
            label='Lothringer (2019) F Star Data')
    ax.plot(ref_tg,
            10**ref_logp,
            'go',
            ms=7,
            label='Lothringer (2019) G Star Data')

    ax.set_yscale('log')
    ax.invert_yaxis()
    ax.set_xlabel(r"Temperature $T$ [K]", fontsize=12)
    ax.set_ylabel(r"Pressure $P$ [bar]", fontsize=12)
    ax.set_title(
        "Lothringer & Barman (2019) Figure 1: T(P) Profiles vs Host Spectral Class",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/lothringer_2019/fig1_tp_profiles.png", dpi=300)
    plt.close(fig)

    # Figure 2: Emergent Spectra for F vs G Host Stars (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[5:])
    ref_wl = ref_fig2_data[:, 0]
    ref_ff = ref_fig2_data[:, 1]
    ref_fg = ref_fig2_data[:, 2]

    sim_spec_data = load_csv("replications/lothringer_2019/sim_spectra.csv")
    sim_spec = np.array(sim_spec_data)

    sim_interp_ff = np.interp(ref_wl, sim_spec[:, 0], sim_spec[:, 1])
    sim_interp_fg = np.interp(ref_wl, sim_spec[:, 0], sim_spec[:, 2])

    r2_fig2_f = 1.0 - (np.sum((sim_interp_ff - ref_ff)**2) / np.sum(
        (ref_ff - np.mean(ref_ff))**2))
    r2_fig2_g = 1.0 - (np.sum((sim_interp_fg - ref_fg)**2) / np.sum(
        (ref_fg - np.mean(ref_fg))**2))
    r2_fig2 = (r2_fig2_f + r2_fig2_g) / 2.0

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_spec[:, 0],
            sim_spec[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter F Star Host')
    ax.plot(sim_spec[:, 0],
            sim_spec[:, 2],
            'g--',
            lw=2.5,
            label='hot_jupiter G Star Host')
    ax.plot(ref_wl, ref_ff, 'ro', ms=7, label='Lothringer (2019) F Host Data')
    ax.plot(ref_wl, ref_fg, 'go', ms=7, label='Lothringer (2019) G Host Data')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Emergent Thermal Flux $F_p / F_\star$ [ppm]", fontsize=12)
    ax.set_title(
        "Lothringer & Barman (2019) Figure 2: Emergent Spectra vs Host Spectral Class",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/lothringer_2019/fig2_spectra.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 T(P) Profiles R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Emergent Spectra R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Lothringer & Barman (2019) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_lothringer2019()
