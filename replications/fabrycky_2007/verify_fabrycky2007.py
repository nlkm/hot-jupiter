"""
Quantitative verification and plot generator for Fabrycky & Tremaine (2007) ApJ 669, 1298.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/fabrycky_2007")


def plot_fig1_trajectory():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_trajectory.csv",
                             delimiter=",",
                             skip_header=1)
    t_myr, a_au, q_au = sim_data[:, 0], sim_data[:, 1], sim_data[:, 2]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=6)
    ref_t, ref_a, ref_q = ref_data[:, 0], ref_data[:, 1], ref_data[:, 2]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(t_myr, a_au, "b-", lw=2, label="Semi-Major Axis $a(t)$ [AU]")
    ax.plot(t_myr, q_au, "r--", lw=2, label="Pericenter Distance $q(t)$ [AU]")
    ax.plot(ref_t, ref_a, "bo", label="Ref $a$")
    ax.plot(ref_t, ref_q, "ro", label="Ref $q$")

    ax.set_xlabel("Time [Myr]", fontsize=11)
    ax.set_ylabel("Distance [AU]", fontsize=11)
    ax.set_title(
        "Fabrycky & Tremaine (2007) Fig 1: KCTF High-Eccentricity Migration",
        fontsize=12)
    ax.set_yscale("log")
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_trajectory.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_period_cdf():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_period_cdf.csv",
                             delimiter=",",
                             skip_header=1)
    p_days, cdf = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=12,
                             max_rows=5)
    ref_p, ref_cdf = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(p_days, cdf, "g-", lw=2, label="KCTF Final Period CDF $f(P)$")
    ax.plot(ref_p,
            ref_cdf,
            "ro",
            label="Fabrycky & Tremaine (2007) Reference Points")

    ax.set_xlabel("Final Orbital Period $P_f$ [days]", fontsize=11)
    ax.set_ylabel("Cumulative Fraction", fontsize=11)
    ax.set_title(
        "Fabrycky & Tremaine (2007) Fig 2: Hot Jupiter 3-Day Pile-up Distribution",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_period_cdf.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_fabrycky2007():
    print("=== Quantitative Verification: Fabrycky & Tremaine (2007) ===")
    plot_fig1_trajectory()
    plot_fig2_period_cdf()

    # Fig 1 Verification
    ref_data1 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=3,
                              max_rows=6)
    ref_t, ref_a = ref_data1[:, 0], ref_data1[:, 1]

    sim_data1 = np.genfromtxt(REPLICATION_DIR / "sim_trajectory.csv",
                              delimiter=",",
                              skip_header=1)
    sim_t, sim_a = sim_data1[:, 0], sim_data1[:, 1]

    calc_a = np.interp(ref_t, sim_t, sim_a)
    ss_res1 = np.sum((ref_a - calc_a)**2)
    ss_tot1 = np.sum((ref_a - np.mean(ref_a))**2)
    r2_fig1 = 1.0 - (ss_res1 / ss_tot1)

    # Fig 2 Verification
    ref_data2 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=12,
                              max_rows=5)
    ref_p, ref_cdf = ref_data2[:, 0], ref_data2[:, 1]

    sim_data2 = np.genfromtxt(REPLICATION_DIR / "sim_period_cdf.csv",
                              delimiter=",",
                              skip_header=1)
    sim_p, sim_cdf = sim_data2[:, 0], sim_data2[:, 1]

    calc_cdf = np.interp(ref_p, sim_p, sim_cdf)
    ss_res2 = np.sum((ref_cdf - calc_cdf)**2)
    ss_tot2 = np.sum((ref_cdf - np.mean(ref_cdf))**2)
    r2_fig2 = 1.0 - (ss_res2 / ss_tot2)

    print(f"--> Fig 1 Trajectory R^2 Score: {r2_fig1:.4f} ({r2_fig1:.2%})")
    print(f"--> Fig 2 Period CDF R^2 Score:  {r2_fig2:.4f} ({r2_fig2:.2%})")
    assert r2_fig1 > 0.98, f"Fig 1 verification failed! R^2 = {r2_fig1:.4f} < 0.98"
    assert r2_fig2 > 0.98, f"Fig 2 verification failed! R^2 = {r2_fig2:.4f} < 0.98"
    print("✅ Fabrycky & Tremaine (2007) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_fabrycky2007()
