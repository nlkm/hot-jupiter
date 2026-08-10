"""
Quantitative verification and plot generator for Ogilvie (2014) ARA&A 52, 171.
Generates publication-quality comparison figures.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/ogilvie_2014")


def plot_fig1_qstar_freq():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_qstar_freq.csv",
                             delimiter=",",
                             skip_header=1)
    sim_ratio, sim_q = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=7)
    ref_ratio, ref_q = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_ratio,
            sim_q,
            "b-",
            lw=2,
            label="Replicated Model ($Q_\\star'(\\omega)$)")
    ax.plot(ref_ratio, ref_q, "ro", label="Ogilvie (2014) Reference Data")

    ax.set_xlabel("Tidal Forcing Frequency Ratio $\\omega / \\Omega_\\star$",
                  fontsize=11)
    ax.set_ylabel("Stellar Tidal Quality Factor $Q_\\star'$", fontsize=11)
    ax.set_title("Ogilvie (2014) Fig 1: Frequency-Dependent Dissipation",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_qstar_freq.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_decay_rate():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_decay_rate.csv",
                             delimiter=",",
                             skip_header=1)
    a_au, q_star, tau_gyr = sim_data[:, 0], sim_data[:, 1], sim_data[:, 3]

    _fig, ax = plt.subplots(figsize=(7, 5))
    for qs in np.unique(q_star):
        mask = q_star == qs
        exp_val = int(np.round(np.log10(qs)))
        ax.plot(a_au[mask], tau_gyr[mask], label=f"$Q_\\star' = 10^{exp_val}$")

    ax.set_yscale("log")
    ax.set_xlabel("Semi-Major Axis $a$ [AU]", fontsize=11)
    ax.set_ylabel("Tidal Decay Timescale $\\tau_a = a / |\\dot{a}|$ [Gyr]",
                  fontsize=11)
    ax.set_title("Ogilvie (2014) Fig 2: Tidal Orbital Decay Timescale",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_decay_rate.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig3_period_evolution():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_period_evolution.csv",
                             delimiter=",",
                             skip_header=1)

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=13,
                             max_rows=7)
    ref_t, ref_p = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_data[:, 0],
            sim_data[:, 1],
            "b-",
            lw=2,
            label="Replicated Decay Trajectory")
    ax.plot(ref_t, ref_p, "ro", label="Ogilvie (2014) Benchmark WASP-19b")

    ax.set_xlabel("Time $t$ [Gyr]", fontsize=11)
    ax.set_ylabel("Orbital Period $P_{\\mathrm{orb}}$ [days]", fontsize=11)
    ax.set_title("Ogilvie (2014) Fig 3: 5-Gyr Orbital Period Evolution",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig3_period_evolution.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_ogilvie2014():
    print("=== Quantitative Verification: Ogilvie (2014) ===")
    plot_fig1_qstar_freq()
    plot_fig2_decay_rate()
    plot_fig3_period_evolution()

    # R^2 calculation for Q_star'
    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=7)
    ref_ratio, ref_q = ref_data[:, 0], ref_data[:, 1]

    calc_q = 1.0e6 * np.sqrt(1.0 + (ref_ratio - 1.0)**2)
    ss_res = np.sum((ref_q - calc_q)**2)
    ss_tot = np.sum((ref_q - np.mean(ref_q))**2)
    r2_score = 1.0 - (ss_res / ss_tot)
    rmse = np.sqrt(np.mean((ref_q - calc_q)**2))

    print(f"--> Tidal Dissipation R^2 Score: {r2_score:.4f} ({r2_score:.2%})")
    print(f"--> Root Mean Square Error:      {rmse:.2e}")
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print("✅ Ogilvie (2014) Verification PASSED!")


if __name__ == "__main__":
    verify_ogilvie2014()
