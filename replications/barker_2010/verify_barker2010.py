"""
Quantitative verification and plot generator for Barker & Ogilvie (2010) MNRAS 404, 1849.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/barker_2010")


def plot_fig1_inclination():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_inclination.csv",
                             delimiter=",",
                             skip_header=1)
    sim_t, sim_inc = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=9)
    ref_t, ref_inc = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_t,
            sim_inc,
            "b-",
            lw=2,
            label="Replicated Internal Wave Tidal Model")
    ax.plot(ref_t,
            ref_inc,
            "ro",
            label="Barker & Ogilvie (2010) Reference Points")

    ax.set_xlabel("Time $t$ [Gyr]", fontsize=11)
    ax.set_ylabel("Stellar Inclination $i$ [deg]", fontsize=11)
    ax.set_title("Barker & Ogilvie (2010) Fig 1: Inclination Damping",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_inclination.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_a_vs_inc():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_inclination.csv",
                             delimiter=",",
                             skip_header=1)
    sim_inc, sim_a = sim_data[:, 1], sim_data[:, 2]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=15,
                             max_rows=7)
    ref_inc, ref_a = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_inc,
            sim_a,
            "g-",
            lw=2,
            label="Coupled Semi-Major Axis Evolution $a(i)$")
    ax.plot(ref_inc,
            ref_a,
            "ro",
            label="Barker & Ogilvie (2010) Reference Points")

    ax.set_xlabel("Stellar Inclination $i$ [deg]", fontsize=11)
    ax.set_ylabel("Semi-Major Axis $a$ [AU]", fontsize=11)
    ax.set_title(
        "Barker & Ogilvie (2010) Fig 2: Semi-Major Axis vs Inclination",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_a_vs_inc.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_barker2010():
    print("=== Quantitative Verification: Barker & Ogilvie (2010) ===")
    plot_fig1_inclination()
    plot_fig2_a_vs_inc()

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=9)
    ref_t, ref_inc = ref_data[:, 0], ref_data[:, 1]

    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_inclination.csv",
                             delimiter=",",
                             skip_header=1)
    sim_t, sim_inc = sim_data[:, 0], sim_data[:, 1]

    calc_inc = np.interp(ref_t, sim_t, sim_inc)
    ss_res = np.sum((ref_inc - calc_inc)**2)
    ss_tot = np.sum((ref_inc - np.mean(ref_inc))**2)
    r2_score = 1.0 - (ss_res / ss_tot)
    rmse = np.sqrt(np.mean((ref_inc - calc_inc)**2))

    print(f"--> Inclination Damping R^2 Score: {r2_score:.4f} ({r2_score:.2%})")
    print(f"--> Root Mean Square Error:        {rmse:.4f} deg")
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print("✅ Barker & Ogilvie (2010) Verification PASSED!")


if __name__ == "__main__":
    verify_barker2010()
