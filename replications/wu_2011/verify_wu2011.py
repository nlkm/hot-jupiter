"""
Quantitative verification and plot generator for Wu & Lithwick (2011) ApJ 735, 109.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/wu_2011")


def plot_fig1_secular_chaos():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_secular_chaos.csv",
                             delimiter=",",
                             skip_header=1)
    t_myr, ecc = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=6)
    ref_t, ref_e = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(t_myr,
            ecc,
            "b-",
            lw=2,
            label="Secular Chaos Eccentricity Trajectory $e(t)$")
    ax.plot(ref_t, ref_e, "ro", label="Wu & Lithwick (2011) Reference Points")

    ax.set_xlabel("Time [Myr]", fontsize=11)
    ax.set_ylabel("Eccentricity $e$", fontsize=11)
    ax.set_title("Wu & Lithwick (2011) Fig 1: Secular Chaos High-e Growth",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_secular_chaos.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_circularization():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_circularization.csv",
                             delimiter=",",
                             skip_header=1)
    initial_e, final_a = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=12,
                             max_rows=5)
    ref_ie, ref_fa = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(initial_e,
            final_a,
            "g-",
            lw=2,
            label="Final Semi-Major Axis $a_f = a_i (1 - e_i^2)$")
    ax.plot(ref_ie, ref_fa, "ro", label="Wu & Lithwick (2011) Reference Points")

    ax.set_xlabel("Initial Eccentricity $e_i$", fontsize=11)
    ax.set_ylabel("Final Semi-Major Axis $a_f$ [AU]", fontsize=11)
    ax.set_title(
        "Wu & Lithwick (2011) Fig 2: Tidal Circularization Final Orbit",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_circularization.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_wu2011():
    print("=== Quantitative Verification: Wu & Lithwick (2011) ===")
    plot_fig1_secular_chaos()
    plot_fig2_circularization()

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=12,
                             max_rows=5)
    ref_ie, ref_fa = ref_data[:, 0], ref_data[:, 1]

    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_circularization.csv",
                             delimiter=",",
                             skip_header=1)
    sim_ie, sim_fa = sim_data[:, 0], sim_data[:, 1]

    calc_fa = np.interp(ref_ie, sim_ie, sim_fa)
    ss_res = np.sum((ref_fa - calc_fa)**2)
    ss_tot = np.sum((ref_fa - np.mean(ref_fa))**2)
    r2_score = 1.0 - (ss_res / ss_tot)
    rmse = np.sqrt(np.mean((ref_fa - calc_fa)**2))

    print(
        f"--> Tidal Circularization Orbit R^2 Score: {r2_score:.4f} ({r2_score:.2%})"
    )
    print(f"--> Root Mean Square Error:                {rmse:.6f} AU")
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print("✅ Wu & Lithwick (2011) Verification PASSED!")


if __name__ == "__main__":
    verify_wu2011()
