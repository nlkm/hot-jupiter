"""
Verification script for Sing et al. (2016) Nature 529, 59.
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


def verify_sing2016():
    ref_rows = load_csv("replications/sing_2016/reference_data.csv")

    # Figure 1: WASP-12b Transmission Spectrum (first 9 data rows)
    ref_fig1_data = np.array(ref_rows[:9])
    ref_wl = ref_fig1_data[:, 0]
    ref_depth = ref_fig1_data[:, 1]

    sim_trans_data = load_csv("replications/sing_2016/sim_wasp12b_spectrum.csv")
    sim_trans = np.array(sim_trans_data)
    sim_trans = sim_trans[np.argsort(sim_trans[:, 0])]

    sim_interp_depth = np.interp(ref_wl, sim_trans[:, 0], sim_trans[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_depth - ref_depth)**2) / np.sum(
        (ref_depth - np.mean(ref_depth))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_trans[:, 0],
            sim_trans[:, 1] * 100,
            'b-',
            lw=2.5,
            label='hot_jupiter Model Spectrum')
    ax.plot(ref_wl,
            ref_depth * 100,
            'ko',
            ms=7,
            label='Sing et al. (2016) WASP-12b')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu\mathrm{m}$]", fontsize=12)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [\%]", fontsize=12)
    ax.set_title("Sing et al. (2016) Figure 1: WASP-12b Cloud-Muffled Spectrum",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/sing_2016/fig1_wasp12b_spectrum.png", dpi=300)
    plt.close(fig)

    # Figure 2: Water Feature Amplitude vs Teq (next 6 data rows)
    ref_fig2_data = np.array(ref_rows[9:])
    ref_teq = ref_fig2_data[:, 0]
    ref_amp = ref_fig2_data[:, 1]

    sim_amp_data = load_csv("replications/sing_2016/sim_water_amplitude.csv")
    sim_amp = np.array(sim_amp_data)

    sim_interp_amp = np.interp(ref_teq, sim_amp[:, 0], sim_amp[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_amp - ref_amp)**2) / np.sum(
        (ref_amp - np.mean(ref_amp))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(
        sim_amp[:, 0],
        sim_amp[:, 1] * 100,
        'r-',
        lw=2.5,
        label=r'hot_jupiter Model $\Delta(R_p/R_\star)^2_{1.4\mu\mathrm{m}}$')
    ax.plot(ref_teq,
            ref_amp * 100,
            'ko',
            ms=7,
            label='Sing et al. (2016) Survey')

    ax.set_xlabel(r"Equilibrium Temperature $T_{\mathrm{eq}}$ [K]", fontsize=12)
    ax.set_ylabel(
        r"Water Feature Amplitude $\Delta(R_p/R_\star)^2_{1.4\mu\mathrm{m}}$ [\%]",
        fontsize=12)
    ax.set_title("Sing et al. (2016) Figure 2: Water Feature Amplitude",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/sing_2016/fig2_water_amplitude.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 WASP-12b Spectrum R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Water Amplitude R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Sing et al. (2016) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_sing2016()
