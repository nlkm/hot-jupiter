"""
Verification script for Line et al. (2021) Nature 598, 580.
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


def verify_line2021():
    ref_rows = load_csv("replications/line_2021/reference_data.csv")

    # Figure 1: High-Resolution CCF SNR vs Vsys (first 5 data rows)
    ref_fig1_data = np.array(ref_rows[:5])
    ref_vsys = ref_fig1_data[:, 0]
    ref_snr = ref_fig1_data[:, 1]

    sim_ccf_data = load_csv("replications/line_2021/sim_ccf_snr.csv")
    sim_ccf = np.array(sim_ccf_data)

    sim_interp_snr = np.interp(ref_vsys, sim_ccf[:, 0], sim_ccf[:, 1])

    r2_fig1 = 1.0 - (np.sum((sim_interp_snr - ref_snr)**2) / np.sum(
        (ref_snr - np.mean(ref_snr))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_ccf[:, 0],
            sim_ccf[:, 1],
            'b-',
            lw=2.5,
            label=r'hot_jupiter $\mathrm{H}_2\mathrm{O}+\mathrm{CO}$ CCF')
    ax.plot(ref_vsys,
            ref_snr,
            'ko',
            ms=7,
            label='Line et al. (2021) IGRINS Observations')

    ax.set_xlabel(r"System Velocity $v_{\mathrm{sys}}$ [km/s]", fontsize=12)
    ax.set_ylabel(r"Cross-Correlation $S/N$", fontsize=12)
    ax.set_title(
        "Line et al. (2021) Figure 1: WASP-77Ab High-Res Cross-Correlation Peak",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/line_2021/fig1_ccf_snr.png", dpi=300)
    plt.close(fig)

    # Figure 2: H2O Abundance Posterior PDF vs Log10 X_H2O (next 5 data rows)
    ref_fig2_data = np.array(ref_rows[5:])
    ref_logx = ref_fig2_data[:, 0]
    ref_pdf = ref_fig2_data[:, 1]

    sim_h2o_data = load_csv("replications/line_2021/sim_h2o_posterior.csv")
    sim_h2o = np.array(sim_h2o_data)

    sim_interp_pdf = np.interp(ref_logx, sim_h2o[:, 0], sim_h2o[:, 1])

    r2_fig2 = 1.0 - (np.sum((sim_interp_pdf - ref_pdf)**2) / np.sum(
        (ref_pdf - np.mean(ref_pdf))**2))

    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(
        sim_h2o[:, 0],
        sim_h2o[:, 1],
        'r-',
        lw=2.5,
        label=
        r'hot_jupiter Retrieved $\log_{10} X_{\mathrm{H}_2\mathrm{O}}$ Posterior'
    )
    ax.plot(ref_logx,
            ref_pdf,
            'ko',
            ms=7,
            label='Line et al. (2021) Solar C/O Posterior')

    ax.set_xlabel(r"$\log_{10} X_{\mathrm{H}_2\mathrm{O}}$ Volume Mixing Ratio",
                  fontsize=12)
    ax.set_ylabel(r"Posterior Probability Density", fontsize=12)
    ax.set_title(
        "Line et al. (2021) Figure 2: WASP-77Ab Water Abundance Posterior",
        fontsize=13)
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=11)
    plt.tight_layout()
    plt.savefig("replications/line_2021/fig2_h2o_posterior.png", dpi=300)
    plt.close(fig)

    print(f"--> Fig 1 CCF SNR R^2 Score: {r2_fig1:.4f} ({r2_fig1*100:.2f}%)")
    print(
        f"--> Fig 2 H2O Posterior R^2 Score: {r2_fig2:.4f} ({r2_fig2*100:.2f}%)"
    )

    assert r2_fig1 >= 0.98, f"Figure 1 R^2 score {r2_fig1} below target 0.98!"
    assert r2_fig2 >= 0.98, f"Figure 2 R^2 score {r2_fig2} below target 0.98!"
    print("✅ Line et al. (2021) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_line2021()
