"""
Quantitative verification and plot generator for Naoz et al. (2011) Nature 473, 187.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/naoz_2011")


def plot_fig1_flip():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_flip.csv",
                             delimiter=",",
                             skip_header=1)
    t_myr, inc_deg = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=7)
    ref_t, ref_inc = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(t_myr, inc_deg, "b-", lw=2, label="Inclination $i(t)$ [deg]")
    ax.axhline(90.0,
               color="k",
               linestyle="--",
               alpha=0.7,
               label="Retrograde Flip Boundary ($i=90^\\circ$)")
    ax.plot(ref_t, ref_inc, "ro", label="Naoz et al. (2011) Reference Points")

    ax.set_xlabel("Time [Myr]", fontsize=11)
    ax.set_ylabel("Inclination $i$ [deg]", fontsize=11)
    ax.set_title("Naoz et al. (2011) Fig 1: EKL Retrograde Orbit Flip",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_flip.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_dist():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_dist.csv",
                             delimiter=",",
                             skip_header=1)
    bins, frac = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=13,
                             max_rows=6)
    ref_bins, ref_frac = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.bar(bins,
           frac,
           width=25,
           alpha=0.6,
           color="g",
           edgecolor="k",
           label="EKL Model Fraction")
    ax.plot(ref_bins,
            ref_frac,
            "ro",
            label="Naoz et al. (2011) Reference Points")

    ax.set_xlabel("Inclination $i$ [deg]", fontsize=11)
    ax.set_ylabel("Fraction of Hot Jupiters", fontsize=11)
    ax.set_title(
        "Naoz et al. (2011) Fig 2: Prograde and Retrograde Distribution",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_dist.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_naoz2011():
    print("=== Quantitative Verification: Naoz et al. (2011) ===")
    plot_fig1_flip()
    plot_fig2_dist()

    # Fig 1 Verification
    ref_data1 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=3,
                              max_rows=7)
    ref_t, ref_inc = ref_data1[:, 0], ref_data1[:, 1]

    sim_data1 = np.genfromtxt(REPLICATION_DIR / "sim_flip.csv",
                              delimiter=",",
                              skip_header=1)
    sim_t, sim_inc = sim_data1[:, 0], sim_data1[:, 1]

    calc_inc = np.interp(ref_t, sim_t, sim_inc)
    ss_res1 = np.sum((ref_inc - calc_inc)**2)
    ss_tot1 = np.sum((ref_inc - np.mean(ref_inc))**2)
    r2_fig1 = 1.0 - (ss_res1 / ss_tot1)

    # Fig 2 Verification
    ref_data2 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=13,
                              max_rows=6)
    ref_bins, ref_frac = ref_data2[:, 0], ref_data2[:, 1]

    sim_data2 = np.genfromtxt(REPLICATION_DIR / "sim_dist.csv",
                              delimiter=",",
                              skip_header=1)
    sim_bins, sim_frac = sim_data2[:, 0], sim_data2[:, 1]

    calc_frac = np.interp(ref_bins, sim_bins, sim_frac)
    ss_res2 = np.sum((ref_frac - calc_frac)**2)
    ss_tot2 = np.sum((ref_frac - np.mean(ref_frac))**2)
    r2_fig2 = 1.0 - (ss_res2 / ss_tot2)

    print(f"--> Fig 1 EKL Flip R^2 Score:        {r2_fig1:.4f} ({r2_fig1:.2%})")
    print(
        f"--> Fig 2 Inclination Dist R^2 Score: {r2_fig2:.4f} ({r2_fig2:.2%})")
    assert r2_fig1 > 0.98, f"Fig 1 verification failed! R^2 = {r2_fig1:.4f} < 0.98"
    assert r2_fig2 > 0.98, f"Fig 2 verification failed! R^2 = {r2_fig2:.4f} < 0.98"
    print("✅ Naoz et al. (2011) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_naoz2011()
