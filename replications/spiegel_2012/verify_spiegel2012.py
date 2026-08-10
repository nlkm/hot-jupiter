"""
Quantitative verification and plot generator for Spiegel & Burrows (2012) ApJ 745, 174.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/spiegel_2012")


def plot_fig1_tp():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_tp.csv",
                             delimiter=",",
                             skip_header=1)
    p_bar, t_inv, t_non = sim_data[:, 0], sim_data[:, 1], sim_data[:, 2]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=6)
    ref_p, ref_t_inv, ref_t_non = ref_data[:, 0], ref_data[:, 1], ref_data[:, 2]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(t_inv, p_bar, "r-", lw=2, label="Inverted Atmosphere (TiO/VO)")
    ax.plot(t_non, p_bar, "b--", lw=2, label="Non-Inverted Atmosphere")
    ax.plot(ref_t_inv, ref_p, "ro", label="Ref Inverted")
    ax.plot(ref_t_non, ref_p, "bo", label="Ref Non-Inverted")

    ax.set_xlabel("Temperature $T$ [K]", fontsize=11)
    ax.set_ylabel("Pressure $P$ [bar]", fontsize=11)
    ax.set_title(
        "Spiegel & Burrows (2012) Fig 1: Atmospheric T-P Inversion Profiles",
        fontsize=12)
    ax.set_yscale("log")
    ax.invert_yaxis()
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_tp.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_spectrum():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_spectrum.csv",
                             delimiter=",",
                             skip_header=1)
    wave, f_inv, f_non = sim_data[:, 0], sim_data[:, 1], sim_data[:, 2]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=13,
                             max_rows=5)
    ref_wave, ref_f_inv, ref_f_non = ref_data[:, 0], ref_data[:, 1], ref_data[:,
                                                                              2]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(wave, f_inv, "r-", lw=2, label="Inverted Emission Spectrum")
    ax.plot(wave, f_non, "b--", lw=2, label="Non-Inverted Emission Spectrum")
    ax.plot(ref_wave, ref_f_inv, "ro", label="Ref Inverted Spectrum")
    ax.plot(ref_wave, ref_f_non, "bo", label="Ref Non-Inverted Spectrum")

    ax.set_xlabel("Wavelength $\\lambda$ [$\\mu$m]", fontsize=11)
    ax.set_ylabel("Emergent Flux $F_\\lambda$ [W m$^{-2}$ $\\mu$m$^{-1}$]",
                  fontsize=11)
    ax.set_title(
        "Spiegel & Burrows (2012) Fig 2: Emission Spectrum Emission vs Absorption",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_spectrum.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_spiegel2012():
    print("=== Quantitative Verification: Spiegel & Burrows (2012) ===")
    plot_fig1_tp()
    plot_fig2_spectrum()

    # Fig 1 Verification
    ref_data1 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=3,
                              max_rows=6)
    ref_p, ref_t_inv = ref_data1[:, 0], ref_data1[:, 1]

    sim_data1 = np.genfromtxt(REPLICATION_DIR / "sim_tp.csv",
                              delimiter=",",
                              skip_header=1)
    sim_p, sim_t_inv = sim_data1[:, 0], sim_data1[:, 1]

    calc_t_inv = np.interp(np.log10(ref_p), np.log10(sim_p), sim_t_inv)
    ss_res1 = np.sum((ref_t_inv - calc_t_inv)**2)
    ss_tot1 = np.sum((ref_t_inv - np.mean(ref_t_inv))**2)
    r2_fig1 = 1.0 - (ss_res1 / ss_tot1)

    # Fig 2 Verification
    ref_data2 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=13,
                              max_rows=5)
    ref_wave, ref_f_inv = ref_data2[:, 0], ref_data2[:, 1]

    sim_data2 = np.genfromtxt(REPLICATION_DIR / "sim_spectrum.csv",
                              delimiter=",",
                              skip_header=1)
    sim_wave, sim_f_inv = sim_data2[:, 0], sim_data2[:, 1]

    calc_f_inv = np.interp(ref_wave, sim_wave, sim_f_inv)
    ss_res2 = np.sum((ref_f_inv - calc_f_inv)**2)
    ss_tot2 = np.sum((ref_f_inv - np.mean(ref_f_inv))**2)
    r2_fig2 = 1.0 - (ss_res2 / ss_tot2)

    print(
        f"--> Fig 1 T-P Inversion R^2 Score:     {r2_fig1:.4f} ({r2_fig1:.2%})")
    print(
        f"--> Fig 2 Emission Spectrum R^2 Score: {r2_fig2:.4f} ({r2_fig2:.2%})")
    assert r2_fig1 > 0.98, f"Fig 1 verification failed! R^2 = {r2_fig1:.4f} < 0.98"
    assert r2_fig2 > 0.98, f"Fig 2 verification failed! R^2 = {r2_fig2:.4f} < 0.98"
    print("✅ Spiegel & Burrows (2012) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_spiegel2012()
