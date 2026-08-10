"""
Quantitative verification and plot generator for Kozai (1962) AJ 67, 591.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/kozai_1962")


def plot_fig1_phase_space():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_phase_space.csv",
                             delimiter=",",
                             skip_header=1)
    omega_deg, e = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=7)
    ref_omega, ref_e = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(omega_deg,
            e,
            "b-",
            lw=2,
            label="Kozai Trajectory $e(\\omega)$ ($i_0=65^\\circ$)")
    ax.plot(ref_omega, ref_e, "ro", label="Kozai (1962) Reference Points")

    ax.set_xlabel("Argument of Periastron $\\omega$ [deg]", fontsize=11)
    ax.set_ylabel("Eccentricity $e$", fontsize=11)
    ax.set_title("Kozai (1962) Fig 1: Kozai-Lidov Phase Space Trajectory",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_phase_space.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_max_eccentricity():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_emax.csv",
                             delimiter=",",
                             skip_header=1)
    inc_deg, e_max = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=14,
                             max_rows=6)
    ref_inc, ref_emax = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(
        inc_deg,
        e_max,
        "g-",
        lw=2,
        label=
        "Maximum Eccentricity $e_{\\mathrm{max}} = \\sqrt{1 - \\frac{5}{3}\\cos^2 i_0}$"
    )
    ax.plot(ref_inc, ref_emax, "ro", label="Kozai (1962) Reference Points")

    ax.set_xlabel("Initial Inclination $i_0$ [deg]", fontsize=11)
    ax.set_ylabel("Maximum Eccentricity $e_{\\mathrm{max}}$", fontsize=11)
    ax.set_title("Kozai (1962) Fig 2: Peak Eccentricity vs Initial Inclination",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_max_eccentricity.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_kozai1962():
    print("=== Quantitative Verification: Kozai (1962) ===")
    plot_fig1_phase_space()
    plot_fig2_max_eccentricity()

    # Fig 1 Verification
    ref_data1 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=3,
                              max_rows=7)
    ref_omega, ref_e = ref_data1[:, 0], ref_data1[:, 1]

    sim_data1 = np.genfromtxt(REPLICATION_DIR / "sim_phase_space.csv",
                              delimiter=",",
                              skip_header=1)
    sim_omega, sim_e = sim_data1[:, 0], sim_data1[:, 1]

    calc_e = np.interp(ref_omega, sim_omega, sim_e)
    ss_res1 = np.sum((ref_e - calc_e)**2)
    ss_tot1 = np.sum((ref_e - np.mean(ref_e))**2)
    r2_fig1 = 1.0 - (ss_res1 / ss_tot1)

    # Fig 2 Verification
    ref_data2 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=14,
                              max_rows=6)
    ref_inc, ref_emax = ref_data2[:, 0], ref_data2[:, 1]

    sim_data2 = np.genfromtxt(REPLICATION_DIR / "sim_emax.csv",
                              delimiter=",",
                              skip_header=1)
    sim_inc, sim_emax = sim_data2[:, 0], sim_data2[:, 1]

    calc_emax = np.interp(ref_inc, sim_inc, sim_emax)
    ss_res2 = np.sum((ref_emax - calc_emax)**2)
    ss_tot2 = np.sum((ref_emax - np.mean(ref_emax))**2)
    r2_fig2 = 1.0 - (ss_res2 / ss_tot2)

    print(
        f"--> Fig 1 Phase Space R^2 Score:       {r2_fig1:.4f} ({r2_fig1:.2%})")
    print(
        f"--> Fig 2 Max Eccentricity R^2 Score:  {r2_fig2:.4f} ({r2_fig2:.2%})")
    assert r2_fig1 > 0.98, f"Fig 1 verification failed! R^2 = {r2_fig1:.4f} < 0.98"
    assert r2_fig2 > 0.98, f"Fig 2 verification failed! R^2 = {r2_fig2:.4f} < 0.98"
    print("✅ Kozai (1962) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_kozai1962()
