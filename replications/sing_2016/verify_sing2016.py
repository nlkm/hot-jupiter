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

    # Figure 1: WASP-39b Transmission Spectrum (first 9 data rows)
    ref_fig1_data = np.array(ref_rows[:9])
    ref_wave = ref_fig1_data[:, 0]
    ref_depth = ref_fig1_data[:, 1]
    ref_err = ref_fig1_data[:, 2]

    sim_spec_data = load_csv(
        "replications/sing_2016/sim_transmission_spectrum.csv")
    sim_spec = np.array(sim_spec_data)

    sim_interp_depth = np.interp(ref_wave, sim_spec[:, 0], sim_spec[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_depth - ref_depth)**2) / np.sum(
        (ref_depth - np.mean(ref_depth))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_spec[:, 0],
            sim_spec[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter WASP-39b Model')
    ax.errorbar(ref_wave,
                ref_depth,
                yerr=ref_err,
                fmt='ko',
                capsize=4,
                ms=6,
                label='Sing et al. (2016) Data')

    ax.set_xscale('log')
    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [\%]", fontsize=12)
    ax.set_title(
        "Sing et al. (2016) Figure 1: WASP-39b Clear Transmission Spectrum",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/sing_2016/fig1_transmission_spectrum.png",
                dpi=300)
    plt.close(fig)

    # Figure 2: Water Feature Scale Height Amplitude (next 10 data rows)
    ref_fig2_data = np.array(ref_rows[9:])
    ref_planet_idx = ref_fig2_data[:, 0]
    ref_water_amp = ref_fig2_data[:, 1]

    sim_amp_data = load_csv("replications/sing_2016/sim_water_amplitude.csv")
    sim_amp = np.array(sim_amp_data)

    sim_interp_amp = np.interp(ref_planet_idx, sim_amp[:, 0], sim_amp[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_amp - ref_water_amp)**2) / np.sum(
        (ref_water_amp - np.mean(ref_water_amp))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_amp[:, 0],
            sim_amp[:, 1],
            'b-',
            lw=2.5,
            label='hot_jupiter Clear-to-Cloudy Trend')
    ax.plot(ref_planet_idx,
            ref_water_amp,
            'ko',
            ms=7,
            label='Sing et al. (2016) 10 Hot Jupiters')

    ax.set_xlabel(r"Planet Index (Clear $\to$ Cloudy Continuum)", fontsize=12)
    ax.set_ylabel(r"Water Amplitude $\Delta N_H$ [Scale Heights $H$]",
                  fontsize=12)
    ax.set_title(
        "Sing et al. (2016) Figure 2: Clear-to-Cloudy Spectral Continuum",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/sing_2016/fig2_water_amplitude.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Transmission Spectrum R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Water Amplitude R^2 Score:       {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Sing et al. (2016) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_sing2016()
