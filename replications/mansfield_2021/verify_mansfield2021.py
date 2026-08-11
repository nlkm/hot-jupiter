"""
Verification script for Mansfield et al. (2021) Nature Astronomy 5, 1224.
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


def verify_mansfield2021():
    ref_rows = load_csv("replications/mansfield_2021/reference_data.csv")

    # Figure 1: Emission Flux Ratio Spectrum Fp/Fstar (first 7 data rows)
    ref_fig1_data = np.array(ref_rows[:7])
    ref_wl = ref_fig1_data[:, 0]
    ref_flux = ref_fig1_data[:, 1]

    sim_flux_data = load_csv(
        "replications/mansfield_2021/sim_emission_spectrum.csv")
    sim_flux = np.array(sim_flux_data)

    sim_interp_flux = np.interp(ref_wl, sim_flux[:, 0], sim_flux[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_flux - ref_flux)**2) / np.sum(
        (ref_flux - np.mean(ref_flux))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_flux[:, 0],
            sim_flux[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter Emission Spectrum')
    ax.plot(ref_wl,
            ref_flux,
            'ko',
            ms=7,
            label='Mansfield et al. (2021) HST WFC3')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu\mathrm{m}$]", fontsize=12)
    ax.set_ylabel(r"Secondary Eclipse $F_p/F_\star$ [ppm]", fontsize=12)
    ax.set_title(
        "Mansfield et al. (2021) Figure 1: WASP-33b Water Emission Spectrum",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/mansfield_2021/fig1_emission_spectrum.png",
                dpi=300)
    plt.close(fig)

    # Figure 2: Thermal Inversion Profile T(P) vs Pressure P (next 5 data rows)
    ref_fig2_data = np.array(ref_rows[7:])
    ref_p2 = ref_fig2_data[:, 0]
    ref_t2 = ref_fig2_data[:, 1]

    sim_tp_data = load_csv(
        "replications/mansfield_2021/sim_thermal_inversion.csv")
    sim_tp = np.array(sim_tp_data)

    sim_interp_t = np.interp(np.log10(ref_p2), np.log10(sim_tp[:, 0]),
                             sim_tp[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_t - ref_t2)**2) / np.sum(
        (ref_t2 - np.mean(ref_t2))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_tp[:, 1],
            sim_tp[:, 0],
            'r-',
            lw=2.5,
            label=r'hot_jupiter Thermal Inversion Profile')
    ax.plot(ref_t2,
            ref_p2,
            'ko',
            ms=7,
            label='Mansfield et al. (2021) Day-side $T(P)$')

    ax.set_yscale('log')
    ax.invert_yaxis()
    ax.set_xlabel(r"Temperature $T$ [K]", fontsize=12)
    ax.set_ylabel(r"Pressure $P$ [bar]", fontsize=12)
    ax.set_title(
        "Mansfield et al. (2021) Figure 2: WASP-33b Thermal Inversion Profile",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/mansfield_2021/fig2_thermal_inversion.png",
                dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 Emission Spectrum R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 Thermal Inversion R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Mansfield et al. (2021) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_mansfield2021()
