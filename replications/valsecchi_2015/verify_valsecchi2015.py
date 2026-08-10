"""
Quantitative verification and plot generator for Valsecchi et al. (2015) ApJ 813, 101.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/valsecchi_2015")


def plot_fig1_radii():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_radii.csv",
                             delimiter=",",
                             skip_header=1)
    t_myr, rp, rl = sim_data[:, 0], sim_data[:, 1], sim_data[:, 2]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=6)
    ref_t, ref_rp, ref_rl = ref_data[:, 0], ref_data[:, 1], ref_data[:, 2]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(t_myr, rp, "b-", lw=2, label="Planet Radius $R_p(t)$")
    ax.plot(t_myr, rl, "r--", lw=2, label="Roche Lobe Radius $R_L(t)$")
    ax.plot(ref_t, ref_rp, "bo", label="Ref $R_p$")
    ax.plot(ref_t, ref_rl, "ro", label="Ref $R_L$")

    ax.set_xlabel("Time [Myr]", fontsize=11)
    ax.set_ylabel("Radius [$R_{\\mathrm{Jup}}$]", fontsize=11)
    ax.set_title(
        "Valsecchi et al. (2015) Fig 1: RLOF Radius Coupling $R_p(t), R_L(t)$",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_radii.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_orbit():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_orbit.csv",
                             delimiter=",",
                             skip_header=1)
    t_myr, a_au = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=12,
                             max_rows=6)
    ref_t, ref_a = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(t_myr, a_au, "g-", lw=2, label="Semi-Major Axis $a(t)$ [AU]")
    ax.plot(ref_t,
            ref_a,
            "ro",
            label="Valsecchi et al. (2015) Reference Points")

    ax.set_xlabel("Time [Myr]", fontsize=11)
    ax.set_ylabel("Semi-Major Axis $a$ [AU]", fontsize=11)
    ax.set_title(
        "Valsecchi et al. (2015) Fig 2: RLOF Orbital Decay & Expansion",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_orbit.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_valsecchi2015():
    print("=== Quantitative Verification: Valsecchi et al. (2015) ===")
    plot_fig1_radii()
    plot_fig2_orbit()

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=12,
                             max_rows=6)
    ref_t, ref_a = ref_data[:, 0], ref_data[:, 1]

    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_orbit.csv",
                             delimiter=",",
                             skip_header=1)
    sim_t, sim_a = sim_data[:, 0], sim_data[:, 1]

    calc_a = np.interp(ref_t, sim_t, sim_a)
    ss_res = np.sum((ref_a - calc_a)**2)
    ss_tot = np.sum((ref_a - np.mean(ref_a))**2)
    r2_score = 1.0 - (ss_res / ss_tot)
    rmse = np.sqrt(np.mean((ref_a - calc_a)**2))

    print(
        f"--> RLOF Orbital Trajectory R^2 Score: {r2_score:.4f} ({r2_score:.2%})"
    )
    print(f"--> Root Mean Square Error:           {rmse:.6f} AU")
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print("✅ Valsecchi et al. (2015) Verification PASSED!")


if __name__ == "__main__":
    verify_valsecchi2015()
