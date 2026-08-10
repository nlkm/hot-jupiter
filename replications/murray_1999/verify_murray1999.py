"""
Quantitative verification and plot generator for Murray & Dermott (1999) Solar System Dynamics.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/murray_1999")


def plot_fig1_secular_evolution():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_secular_evolution.csv",
                             delimiter=",",
                             skip_header=1)
    t_kyr, e1, e2 = sim_data[:, 0], sim_data[:, 1], sim_data[:, 2]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=6)
    ref_t, ref_e1, ref_e2 = ref_data[:, 0], ref_data[:, 1], ref_data[:, 2]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(t_kyr, e1, "b-", lw=2, label="Inner Planet Eccentricity $e_1(t)$")
    ax.plot(t_kyr, e2, "r--", lw=2, label="Outer Planet Eccentricity $e_2(t)$")
    ax.plot(ref_t, ref_e1, "bo", label="Ref $e_1$")
    ax.plot(ref_t, ref_e2, "ro", label="Ref $e_2$")

    ax.set_xlabel("Time [kyr]", fontsize=11)
    ax.set_ylabel("Eccentricity $e$", fontsize=11)
    ax.set_title(
        "Murray & Dermott (1999) Fig 1: Laplace-Lagrange Secular Evolution",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_secular_evolution.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_secular_frequencies():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_secular_frequencies.csv",
                             delimiter=",",
                             skip_header=1)
    alpha, g1, g2 = sim_data[:, 0], sim_data[:, 1], sim_data[:, 2]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=12,
                             max_rows=4)
    ref_alpha, ref_g1, ref_g2 = ref_data[:, 0], ref_data[:, 1], ref_data[:, 2]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(alpha, g1, "b-", lw=2, label="Secular Eigenfrequency $g_1$ [\"/yr]")
    ax.plot(alpha,
            g2,
            "r--",
            lw=2,
            label="Secular Eigenfrequency $g_2$ [\"/yr]")
    ax.plot(ref_alpha, ref_g1, "bo", label="Ref $g_1$")
    ax.plot(ref_alpha, ref_g2, "ro", label="Ref $g_2$")

    ax.set_xlabel("Semi-Major Axis Ratio $\\alpha = a_1 / a_2$", fontsize=11)
    ax.set_ylabel("Secular Precession Frequency $g$ [arcsec/yr]", fontsize=11)
    ax.set_title(
        "Murray & Dermott (1999) Fig 2: Laplace-Lagrange Frequencies $g_1, g_2$",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_secular_frequencies.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_murray1999():
    print("=== Quantitative Verification: Murray & Dermott (1999) ===")
    plot_fig1_secular_evolution()
    plot_fig2_secular_frequencies()

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=6)
    ref_t, ref_e1 = ref_data[:, 0], ref_data[:, 1]

    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_secular_evolution.csv",
                             delimiter=",",
                             skip_header=1)
    sim_t, sim_e1 = sim_data[:, 0], sim_data[:, 1]

    calc_e1 = np.interp(ref_t, sim_t, sim_e1)
    ss_res = np.sum((ref_e1 - calc_e1)**2)
    ss_tot = np.sum((ref_e1 - np.mean(ref_e1))**2)
    r2_score = 1.0 - (ss_res / ss_tot)
    rmse = np.sqrt(np.mean((ref_e1 - calc_e1)**2))

    print(
        f"--> Secular Eccentricity Oscillations R^2 Score: {r2_score:.4f} ({r2_score:.2%})"
    )
    print(f"--> Root Mean Square Error:                    {rmse:.6f}")
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print("✅ Murray & Dermott (1999) Verification PASSED!")


if __name__ == "__main__":
    verify_murray1999()
