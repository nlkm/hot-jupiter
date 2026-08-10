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
                vals = [float(x) for x in row]
                data.append(vals)
            except ValueError:
                continue
    return data


def verify_line2014():
    ref_rows = load_csv("replications/line_2014/reference_data.csv")

    # Figure 1: WASP-43b T-P Profile (rows with 4 values)
    ref_fig1_data = np.array([r for r in ref_rows if len(r) == 4])
    ref_p = ref_fig1_data[:, 0]
    ref_t_med = ref_fig1_data[:, 1]

    sim_tp_data = load_csv("replications/line_2014/sim_wasp43b_tp.csv")
    sim_tp = np.array(sim_tp_data)

    sim_interp_t = np.interp(np.log10(ref_p), np.log10(sim_tp[:, 0]), sim_tp[:,
                                                                             1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_t - ref_t_med)**2) / np.sum(
        (ref_t_med - np.mean(ref_t_med))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_tp[:, 1], sim_tp[:, 0], 'r-', lw=2.5, label='hot_jupiter Model')
    ax.fill_betweenx(sim_tp[:, 0],
                     sim_tp[:, 3],
                     sim_tp[:, 2],
                     color='red',
                     alpha=0.2,
                     label=r'1-$\sigma$ Envelope')
    ax.plot(ref_t_med, ref_p, 'ko', ms=7, label='Line et al. (2014)')

    ax.set_yscale('log')
    ax.invert_yaxis()
    ax.set_xlabel("Temperature [K]", fontsize=12)
    ax.set_ylabel("Pressure [bar]", fontsize=12)
    ax.set_title("Line et al. (2014) Figure 1: WASP-43b T-P Retrieval Profile",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/line_2014/fig1_wasp43b_tp.png", dpi=300)
    plt.close(fig)

    # Figure 2: Eclipse Spectrum
    sim_spec_data = load_csv("replications/line_2014/sim_wasp43b_spectrum.csv")
    sim_spec = np.array(sim_spec_data)

    ref_fig2_wave = np.array([3.6, 4.5, 5.8, 8.0])
    ref_fig2_flux = np.array([0.32, 0.41, 0.48, 0.56])

    sim_interp_spec = np.interp(ref_fig2_wave, sim_spec[:, 0], sim_spec[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_spec - ref_fig2_flux)**2) / np.sum(
        (ref_fig2_flux - np.mean(ref_fig2_flux))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_spec[:, 0],
            sim_spec[:, 1],
            'r-',
            lw=2.5,
            label='hot_jupiter Model')
    ax.plot(ref_fig2_wave,
            ref_fig2_flux,
            'ko',
            ms=7,
            label='Line et al. (2014)')

    ax.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]", fontsize=12)
    ax.set_ylabel(r"$F_{\mathrm{planet}} / F_{\star}$ [%]", fontsize=12)
    ax.set_title("Line et al. (2014) Figure 2: WASP-43b Eclipse Spectrum",
                 fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/line_2014/fig2_wasp43b_spectrum.png", dpi=300)
    plt.close(fig)

    print(
        f"--> Fig 1 WASP-43b T-P Retrieval R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)"
    )
    print(
        f"--> Fig 2 WASP-43b Spectrum R^2 Score:     {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Line et al. (2014) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_line2014()
