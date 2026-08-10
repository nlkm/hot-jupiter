"""
Quantitative verification and plot generator for Madhusudhan & Seager (2009) ApJ 707, 24.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/madhusudhan_2009")


def plot_fig1_tp_retrieval():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_tp_retrieval.csv",
                             delimiter=",",
                             skip_header=1)
    p_bar, t_med, t_up, t_low = sim_data[:,
                                         0], sim_data[:,
                                                      1], sim_data[:,
                                                                   2], sim_data[:,
                                                                                3]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=5)
    ref_p, ref_t_med = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(t_med,
            p_bar,
            "r-",
            lw=2,
            label="Retrieved Median T-P Profile (HD 189733b)")
    ax.fill_betweenx(p_bar,
                     t_low,
                     t_up,
                     color="r",
                     alpha=0.2,
                     label="1-$\\sigma$ Retrieval Confidence Envelope")
    ax.plot(ref_t_med,
            ref_p,
            "ro",
            label="Madhusudhan & Seager (2009) Reference Points")

    ax.set_xlabel("Temperature $T$ [K]", fontsize=11)
    ax.set_ylabel("Pressure $P$ [bar]", fontsize=11)
    ax.set_title(
        "Madhusudhan & Seager (2009) Fig 1: Atmospheric T-P Retrieval Envelope",
        fontsize=12)
    ax.set_yscale("log")
    ax.invert_yaxis()
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_tp_retrieval.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_secondary_eclipse():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_spectrum_retrieval.csv",
                             delimiter=",",
                             skip_header=1)
    wave, flux_ratio = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=12,
                             max_rows=5)
    ref_wave, ref_ratio = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(wave,
            flux_ratio,
            "b-",
            lw=2,
            label="Retrieved Secondary Eclipse Spectrum $F_p/F_\\star$")
    ax.plot(ref_wave, ref_ratio, "ro", label="Spitzer IRAC Data Points")

    ax.set_xlabel("Wavelength $\\lambda$ [$\\mu$m]", fontsize=11)
    ax.set_ylabel("Planet-to-Star Flux Ratio $F_p / F_\\star$ [\\%]",
                  fontsize=11)
    ax.set_title(
        "Madhusudhan & Seager (2009) Fig 2: Secondary Eclipse Spectrum",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_secondary_eclipse.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_madhusudhan2009():
    print("=== Quantitative Verification: Madhusudhan & Seager (2009) ===")
    plot_fig1_tp_retrieval()
    plot_fig2_secondary_eclipse()

    # Fig 1 Verification
    ref_data1 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=3,
                              max_rows=5)
    ref_p, ref_t_med = ref_data1[:, 0], ref_data1[:, 1]

    sim_data1 = np.genfromtxt(REPLICATION_DIR / "sim_tp_retrieval.csv",
                              delimiter=",",
                              skip_header=1)
    sim_p, sim_t_med = sim_data1[:, 0], sim_data1[:, 1]

    calc_t_med = np.interp(np.log10(ref_p), np.log10(sim_p), sim_t_med)
    ss_res1 = np.sum((ref_t_med - calc_t_med)**2)
    ss_tot1 = np.sum((ref_t_med - np.mean(ref_t_med))**2)
    r2_fig1 = 1.0 - (ss_res1 / ss_tot1)

    # Fig 2 Verification
    ref_data2 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=12,
                              max_rows=5)
    ref_wave, ref_ratio = ref_data2[:, 0], ref_data2[:, 1]

    sim_data2 = np.genfromtxt(REPLICATION_DIR / "sim_spectrum_retrieval.csv",
                              delimiter=",",
                              skip_header=1)
    sim_wave, sim_ratio = sim_data2[:, 0], sim_data2[:, 1]

    calc_ratio = np.interp(ref_wave, sim_wave, sim_ratio)
    ss_res2 = np.sum((ref_ratio - calc_ratio)**2)
    ss_tot2 = np.sum((ref_ratio - np.mean(ref_ratio))**2)
    r2_fig2 = 1.0 - (ss_res2 / ss_tot2)

    print(
        f"--> Fig 1 T-P Retrieval Envelope R^2 Score: {r2_fig1:.4f} ({r2_fig1:.2%})"
    )
    print(
        f"--> Fig 2 Secondary Eclipse Spectrum R^2 Score: {r2_fig2:.4f} ({r2_fig2:.2%})"
    )
    assert r2_fig1 > 0.98, f"Fig 1 verification failed! R^2 = {r2_fig1:.4f} < 0.98"
    assert r2_fig2 > 0.98, f"Fig 2 verification failed! R^2 = {r2_fig2:.4f} < 0.98"
    print("✅ Madhusudhan & Seager (2009) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_madhusudhan2009()
