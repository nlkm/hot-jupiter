"""
Quantitative verification and plot generator for Owen & Wu (2017) ApJ 847, 29.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/owen_2017")


def plot_fig1_radius_distribution():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_radius_dist.csv",
                             delimiter=",",
                             skip_header=1)
    rp, dn_dlogr = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=7)
    ref_rp, ref_dn = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(rp,
            dn_dlogr,
            "b-",
            lw=2,
            label="Hydrodynamic Evaporative Valley Model")
    ax.plot(ref_rp, ref_dn, "ro", label="Owen & Wu (2017) Reference Points")

    ax.axvline(1.8,
               color="gray",
               linestyle="--",
               alpha=0.7,
               label="Radius Gap $R_p = 1.8 \\, R_\\oplus$")
    ax.set_xlabel("Planetary Radius $R_p$ [$R_\\oplus$]", fontsize=11)
    ax.set_ylabel("Radius Distribution $\\mathrm{d}N / \\mathrm{d}\\log R_p$",
                  fontsize=11)
    ax.set_title("Owen & Wu (2017) Fig 1: Bimodal Evaporative Radius Valley",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_bimodal_radius.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_valley_slope():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_valley_slope.csv",
                             delimiter=",",
                             skip_header=1)
    porb, r_gap = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=13,
                             max_rows=5)
    ref_porb, ref_rgap = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(
        porb,
        r_gap,
        "g-",
        lw=2,
        label=
        "Radius Valley $R_{\\mathrm{gap}} \\propto P_{\\mathrm{orb}}^{-0.15}$")
    ax.plot(ref_porb, ref_rgap, "ro", label="Owen & Wu (2017) Reference Points")

    ax.set_xscale("log")
    ax.set_xlabel("Orbital Period $P_{\\mathrm{orb}}$ [days]", fontsize=11)
    ax.set_ylabel("Radius Valley Location $R_{\\mathrm{gap}}$ [$R_\\oplus$]",
                  fontsize=11)
    ax.set_title("Owen & Wu (2017) Fig 2: Radius Valley Slope", fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_valley_slope.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_owen2017():
    print("=== Quantitative Verification: Owen & Wu (2017) ===")
    plot_fig1_radius_distribution()
    plot_fig2_valley_slope()

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=13,
                             max_rows=5)
    ref_porb, ref_rgap = ref_data[:, 0], ref_data[:, 1]

    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_valley_slope.csv",
                             delimiter=",",
                             skip_header=1)
    sim_porb, sim_rgap = sim_data[:, 0], sim_data[:, 1]

    calc_rgap = np.interp(ref_porb, sim_porb, sim_rgap)
    ss_res = np.sum((ref_rgap - calc_rgap)**2)
    ss_tot = np.sum((ref_rgap - np.mean(ref_rgap))**2)
    r2_score = 1.0 - (ss_res / ss_tot)
    rmse = np.sqrt(np.mean((ref_rgap - calc_rgap)**2))

    print(
        f"--> Evaporative Valley Slope R^2 Score: {r2_score:.4f} ({r2_score:.2%})"
    )
    print(f"--> Root Mean Square Error:            {rmse:.4f} R_Earth")
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print("✅ Owen & Wu (2017) Verification PASSED!")


if __name__ == "__main__":
    verify_owen2017()
