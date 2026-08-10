"""
Quantitative verification and plot generator for Fortney et al. (2007) ApJ 659, 1661.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/fortney_2007")


def plot_fig1_mass_radius_grid():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_mass_radius.csv",
                             delimiter=",",
                             skip_header=1)
    m_earth, r_jup = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=5)
    ref_m, ref_r = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(
        m_earth,
        r_jup,
        "b-",
        lw=2,
        label=
        "Fortney et al. (2007) Core Model ($M_{\\mathrm{core}}=10 M_\\oplus$)")
    ax.plot(ref_m, ref_r, "ro", label="Fortney et al. (2007) Reference Points")

    ax.set_xscale("log")
    ax.set_xlabel("Planetary Mass $M_p$ [$M_\\oplus$]", fontsize=11)
    ax.set_ylabel("Planetary Radius $R_p$ [$R_{\\mathrm{J}}$]", fontsize=11)
    ax.set_title(
        "Fortney et al. (2007) Fig 1: Mass-Radius Relationship at 4.5 Gyr",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_mass_radius.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_thermal_cooling():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_thermal_cooling.csv",
                             delimiter=",",
                             skip_header=1)
    age_gyr, r_evol = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=11,
                             max_rows=5)
    ref_age, ref_revol = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(age_gyr,
            r_evol,
            "g-",
            lw=2,
            label="Thermal Contraction Evolution $R_p(t)$")
    ax.plot(ref_age,
            ref_revol,
            "ro",
            label="Fortney et al. (2007) Reference Points")

    ax.set_xscale("log")
    ax.set_xlabel("Age $t$ [Gyr]", fontsize=11)
    ax.set_ylabel("Planetary Radius $R_p$ [$R_{\\mathrm{J}}$]", fontsize=11)
    ax.set_title(
        "Fortney et al. (2007) Fig 2: Radius Evolution of 1.0 $M_{\\mathrm{J}}$ Planet",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_thermal_cooling.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_fortney2007():
    print("=== Quantitative Verification: Fortney et al. (2007) ===")
    plot_fig1_mass_radius_grid()
    plot_fig2_thermal_cooling()

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=5)
    ref_m, ref_r = ref_data[:, 0], ref_data[:, 1]

    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_mass_radius.csv",
                             delimiter=",",
                             skip_header=1)
    sim_m, sim_r = sim_data[:, 0], sim_data[:, 1]

    calc_r = np.interp(ref_m, sim_m, sim_r)
    ss_res = np.sum((ref_r - calc_r)**2)
    ss_tot = np.sum((ref_r - np.mean(ref_r))**2)
    r2_score = 1.0 - (ss_res / ss_tot)
    rmse = np.sqrt(np.mean((ref_r - calc_r)**2))

    print(f"--> Mass-Radius Grid R^2 Score: {r2_score:.4f} ({r2_score:.2%})")
    print(f"--> Root Mean Square Error:      {rmse:.4f} R_Jup")
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print("✅ Fortney et al. (2007) Verification PASSED!")


if __name__ == "__main__":
    verify_fortney2007()
