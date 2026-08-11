"""
Verification script for Line et al. (2014) ApJ 783, 70.
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


def verify_line2014():
    ref_rows = load_csv("replications/line_2014/reference_data.csv")

    # Figure 1: Emission Spectrum Fp/Fstar (first 6 data rows)
    ref_fig1_data = np.array(ref_rows[:6])
    ref_wl = ref_fig1_data[:, 0]
    ref_flux = ref_fig1_data[:, 1]

    sim_spec_data = load_csv("replications/line_2014/sim_emission_spectrum.csv")
    sim_spec = np.array(sim_spec_data)
    sim_spec = sim_spec[np.argsort(sim_spec[:, 0])]

    sim_interp_flux = np.interp(ref_wl, sim_spec[:, 0], sim_spec[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_flux - ref_flux)**2) / np.sum(
        (ref_flux - np.mean(ref_flux))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_spec[:, 0],
            sim_spec[:, 1] * 100,
            'b-',
            lw=2.5,
            label='hot_jupiter Model Emission Spectrum')
    ax.plot(ref_wl,
            ref_flux * 100,
            'ko',
            ms=7,
            label='Line et al. (2014) HD 189733b')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu\mathrm{m}$]", fontsize=12)
    ax.set_ylabel(r"Secondary Eclipse Flux Ratio $F_p/F_\star$ [\%]",
                  fontsize=12)
    ax.set_title(
        "Line et al. (2014) Figure 1: HD 189733b Emission Spectrum Retrieval",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/line_2014/fig1_emission_spectrum.png", dpi=300)
    plt.close(fig)

    # Figure 2: T(P) Profile Retrieval (next 7 data rows)
    ref_fig2_data = np.array(ref_rows[6:])
    ref_p = ref_fig2_data[:, 0]
    ref_t = ref_fig2_data[:, 1]
    ref_logp = np.log10(ref_p)

    sim_tp_data = load_csv("replications/line_2014/sim_tp_profile.csv")
    sim_tp = np.array(sim_tp_data)
    sim_logp = np.log10(sim_tp[:, 0])

    sim_interp_t = np.interp(ref_logp, sim_logp, sim_tp[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_t - ref_t)**2) / np.sum(
        (ref_t - np.mean(ref_t))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_tp[:, 1],
            sim_tp[:, 0],
            'r-',
            lw=2.5,
            label='hot_jupiter Retrieved T(P)')
    ax.plot(ref_t, ref_p, 'ko', ms=7, label='Line et al. (2014) T(P) Retrieval')

    ax.set_yscale('log')
    ax.invert_yaxis()
    ax.set_xlabel(r"Temperature $T$ [K]", fontsize=12)
    ax.set_ylabel(r"Pressure $P$ [bar]", fontsize=12)
    ax.set_title("Line et al. (2014) Figure 2: Retrieved Thermal Profile T(P)",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/line_2014/fig2_tp_profile.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Emission Spectrum R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 T(P) Profile R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)")

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Line et al. (2014) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_line2014()
