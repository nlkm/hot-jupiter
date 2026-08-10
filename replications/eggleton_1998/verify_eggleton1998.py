"""
Quantitative verification and plot generator for Eggleton et al. (1998) ApJ 499, 853.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/eggleton_1998")


def plot_fig1_eccentricity():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_vector_tides.csv",
                             delimiter=",",
                             skip_header=1)
    sim_t, sim_e = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=8)
    ref_t, ref_e = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_t, sim_e, "b-", lw=2, label="Replicated Vector Tidal Model")
    ax.plot(ref_t, ref_e, "ro", label="Eggleton (1998) Reference Points")

    ax.set_xlabel("Time $t$ [Myr]", fontsize=11)
    ax.set_ylabel("Orbital Eccentricity $e$", fontsize=11)
    ax.set_title(
        "Eggleton et al. (1998) Fig 1: Vector Tidal Eccentricity Decay",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_eccentricity.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_obliquity():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_vector_tides.csv",
                             delimiter=",",
                             skip_header=1)
    sim_t, sim_theta = sim_data[:, 0], sim_data[:, 2]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=14,
                             max_rows=6)
    ref_t, ref_theta = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_t,
            sim_theta,
            "m-",
            lw=2,
            label="Spin Vector Alignment Trajectory")
    ax.plot(ref_t, ref_theta, "ro", label="Eggleton (1998) Reference Points")

    ax.set_xlabel("Time $t$ [Myr]", fontsize=11)
    ax.set_ylabel("Spin Obliquity Angle $\\theta$ [deg]", fontsize=11)
    ax.set_title("Eggleton et al. (1998) Fig 2: Stellar Spin Obliquity Damping",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_obliquity.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_eggleton1998():
    print("=== Quantitative Verification: Eggleton et al. (1998) ===")
    plot_fig1_eccentricity()
    plot_fig2_obliquity()

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=8)
    ref_t, ref_e = ref_data[:, 0], ref_data[:, 1]

    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_vector_tides.csv",
                             delimiter=",",
                             skip_header=1)
    sim_t, sim_e = sim_data[:, 0], sim_data[:, 1]

    calc_e = np.interp(ref_t, sim_t, sim_e)
    ss_res = np.sum((ref_e - calc_e)**2)
    ss_tot = np.sum((ref_e - np.mean(ref_e))**2)
    r2_score = 1.0 - (ss_res / ss_tot)
    rmse = np.sqrt(np.mean((ref_e - calc_e)**2))

    print(
        f"--> Vector Tidal Eccentricity R^2 Score: {r2_score:.4f} ({r2_score:.2%})"
    )
    print(f"--> Root Mean Square Error:              {rmse:.4f}")
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print("✅ Eggleton et al. (1998) Verification PASSED!")


if __name__ == "__main__":
    verify_eggleton1998()
