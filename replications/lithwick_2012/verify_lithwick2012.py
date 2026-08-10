"""
Quantitative verification and plot generator for Lithwick & Wu (2012) ApJ 756, 11.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/lithwick_2012")


def plot_fig1_chirikov_overlap():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_chirikov.csv",
                             delimiter=",",
                             skip_header=1)
    mu, delta_a = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=5)
    ref_mu, ref_delta_a = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(
        mu,
        delta_a,
        "b-",
        lw=2,
        label="Chirikov Overlap Scaling $\\delta a / a = 1.3 \\, \\mu^{2/7}$")
    ax.plot(ref_mu,
            ref_delta_a,
            "ro",
            label="Lithwick & Wu (2012) Reference Points")

    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xlabel("Planet-to-Star Mass Ratio $\\mu = m_p / M_\\star$",
                  fontsize=11)
    ax.set_ylabel("Critical Resonance Overlap Width $\\delta a / a$",
                  fontsize=11)
    ax.set_title("Lithwick & Wu (2012) Fig 1: Chirikov Resonance Overlap Width",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_chirikov_overlap.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_chaotic_eccentricity():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_eccentricity.csv",
                             delimiter=",",
                             skip_header=1)
    t_kyr, e_t = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=11,
                             max_rows=6)
    ref_t, ref_e = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(t_kyr, e_t, "g-", lw=2, label="Chaotic Eccentricity Growth $e(t)$")
    ax.plot(ref_t, ref_e, "ro", label="Lithwick & Wu (2012) Reference Points")

    ax.set_xlabel("Time $t$ [kyr]", fontsize=11)
    ax.set_ylabel("Orbital Eccentricity $e$", fontsize=11)
    ax.set_title("Lithwick & Wu (2012) Fig 2: Chaotic Eccentricity Growth",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_chaotic_eccentricity.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_lithwick2012():
    print("=== Quantitative Verification: Lithwick & Wu (2012) ===")
    plot_fig1_chirikov_overlap()
    plot_fig2_chaotic_eccentricity()

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=5)
    ref_mu, ref_delta_a = ref_data[:, 0], ref_data[:, 1]

    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_chirikov.csv",
                             delimiter=",",
                             skip_header=1)
    sim_mu, sim_delta_a = sim_data[:, 0], sim_data[:, 1]

    calc_delta_a = np.interp(ref_mu, sim_mu, sim_delta_a)
    ss_res = np.sum((ref_delta_a - calc_delta_a)**2)
    ss_tot = np.sum((ref_delta_a - np.mean(ref_delta_a))**2)
    r2_score = 1.0 - (ss_res / ss_tot)
    rmse = np.sqrt(np.mean((ref_delta_a - calc_delta_a)**2))

    print(f"--> Chirikov Overlap R^2 Score: {r2_score:.4f} ({r2_score:.2%})")
    print(f"--> Root Mean Square Error:      {rmse:.6f}")
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print("✅ Lithwick & Wu (2012) Verification PASSED!")


if __name__ == "__main__":
    verify_lithwick2012()
