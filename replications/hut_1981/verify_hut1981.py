"""
Quantitative verification and plot generator for Hut (1981) A&A 99, 126.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/hut_1981")


def plot_fig1_tidal_evolution():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_tidal_evolution.csv",
                             delimiter=",",
                             skip_header=1)
    t_myr, a_au, ecc = sim_data[:, 0], sim_data[:, 1], sim_data[:, 2]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=6)
    ref_t, ref_a, ref_e = ref_data[:, 0], ref_data[:, 1], ref_data[:, 2]

    fig, ax1 = plt.subplots(figsize=(7, 5))
    color = "tab:blue"
    ax1.set_xlabel("Time $t$ [Myr]", fontsize=11)
    ax1.set_ylabel("Semi-Major Axis $a$ [AU]", color=color, fontsize=11)
    ax1.plot(t_myr, a_au, color=color, lw=2, label="Calculated $a(t)$")
    ax1.plot(ref_t, ref_a, "bo", label="Ref $a(t)$")
    ax1.tick_params(axis="y", labelcolor=color)

    ax2 = ax1.twinx()
    color = "tab:red"
    ax2.set_ylabel("Eccentricity $e$", color=color, fontsize=11)
    ax2.plot(t_myr,
             ecc,
             color=color,
             lw=2,
             linestyle="--",
             label="Calculated $e(t)$")
    ax2.plot(ref_t, ref_e, "ro", label="Ref $e(t)$")
    ax2.tick_params(axis="y", labelcolor=color)

    plt.title("Hut (1981) Fig 1: Tidal Semi-Major Axis & Eccentricity Decay",
              fontsize=12)
    fig.tight_layout()
    path = REPLICATION_DIR / "fig1_tidal_evolution.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_pseudo_spin():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_pseudo_spin.csv",
                             delimiter=",",
                             skip_header=1)
    ecc, omega_ps = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=12,
                             max_rows=6)
    ref_e, ref_ops = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(ecc,
            omega_ps,
            "g-",
            lw=2,
            label="Hut (1981) Equilibrium Tidal Model")
    ax.plot(ref_e, ref_ops, "ro", label="Hut (1981) Reference Points")

    ax.set_xlabel("Eccentricity $e$", fontsize=11)
    ax.set_ylabel("Pseudo-Synchronous Spin Rate $\\Omega_{\\mathrm{ps}} / n$",
                  fontsize=11)
    ax.set_title("Hut (1981) Fig 2: Pseudo-Synchronous Rotation", fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_pseudo_spin.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_hut1981():
    print("=== Quantitative Verification: Hut (1981) ===")
    plot_fig1_tidal_evolution()
    plot_fig2_pseudo_spin()

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=12,
                             max_rows=6)
    ref_e, ref_ops = ref_data[:, 0], ref_data[:, 1]

    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_pseudo_spin.csv",
                             delimiter=",",
                             skip_header=1)
    sim_e, sim_ops = sim_data[:, 0], sim_data[:, 1]

    calc_ops = np.interp(ref_e, sim_e, sim_ops)
    ss_res = np.sum((ref_ops - calc_ops)**2)
    ss_tot = np.sum((ref_ops - np.mean(ref_ops))**2)
    r2_score = 1.0 - (ss_res / ss_tot)
    rmse = np.sqrt(np.mean((ref_ops - calc_ops)**2))

    print(
        f"--> Pseudo-Synchronous Spin R^2 Score: {r2_score:.4f} ({r2_score:.2%})"
    )
    print(f"--> Root Mean Square Error:            {rmse:.6f}")
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print("✅ Hut (1981) Verification PASSED!")


if __name__ == "__main__":
    verify_hut1981()
