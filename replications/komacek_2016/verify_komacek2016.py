"""
Quantitative verification and plot generator for Komacek & Showman (2016) ApJ 821, 16.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/komacek_2016")


def plot_fig1_contrast():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_contrast.csv",
                             delimiter=",",
                             skip_header=1)
    teq, contrast = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=7)
    ref_teq, ref_contrast = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(
        teq,
        contrast,
        "r-",
        lw=2,
        label=
        "Day-Night Fractional Contrast $\\Delta T / \\Delta T_{\\text{eq}}$")
    ax.plot(ref_teq,
            ref_contrast,
            "ro",
            label="Komacek & Showman (2016) Reference Points")

    ax.set_xlabel("Equilibrium Temperature $T_{\\text{eq}}$ [K]", fontsize=11)
    ax.set_ylabel("Day-Night Contrast $\\Delta T / \\Delta T_{\\text{eq}}$",
                  fontsize=11)
    ax.set_title(
        "Komacek & Showman (2016) Fig 1: Day-Night Contrast vs Temperature",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_contrast.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_zonal_wind():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_zonal_wind.csv",
                             delimiter=",",
                             skip_header=1)
    tau_drag, u_ms = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=14,
                             max_rows=5)
    ref_tau, ref_u = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(tau_drag, u_ms, "b-", lw=2, label="Zonal Wind Speed $U$ [m/s]")
    ax.plot(ref_tau,
            ref_u,
            "ro",
            label="Komacek & Showman (2016) Reference Points")

    ax.set_xlabel("Drag Timescale $\\tau_{\\text{drag}}$ [s]", fontsize=11)
    ax.set_ylabel("Equatorial Zonal Wind $U$ [m/s]", fontsize=11)
    ax.set_title("Komacek & Showman (2016) Fig 2: Zonal Wind vs Drag Timescale",
                 fontsize=12)
    ax.set_xscale("log")
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_zonal_wind.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_komacek2016():
    print("=== Quantitative Verification: Komacek & Showman (2016) ===")
    plot_fig1_contrast()
    plot_fig2_zonal_wind()

    # Fig 1 Verification
    ref_data1 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=3,
                              max_rows=7)
    ref_teq, ref_contrast = ref_data1[:, 0], ref_data1[:, 1]

    sim_data1 = np.genfromtxt(REPLICATION_DIR / "sim_contrast.csv",
                              delimiter=",",
                              skip_header=1)
    sim_teq, sim_contrast = sim_data1[:, 0], sim_data1[:, 1]

    calc_contrast = np.interp(ref_teq, sim_teq, sim_contrast)
    ss_res1 = np.sum((ref_contrast - calc_contrast)**2)
    ss_tot1 = np.sum((ref_contrast - np.mean(ref_contrast))**2)
    r2_fig1 = 1.0 - (ss_res1 / ss_tot1)

    # Fig 2 Verification
    ref_data2 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=14,
                              max_rows=5)
    ref_tau, ref_u = ref_data2[:, 0], ref_data2[:, 1]

    sim_data2 = np.genfromtxt(REPLICATION_DIR / "sim_zonal_wind.csv",
                              delimiter=",",
                              skip_header=1)
    sim_tau, sim_u = sim_data2[:, 0], sim_data2[:, 1]

    calc_u = np.interp(np.log10(ref_tau), np.log10(sim_tau), sim_u)
    ss_res2 = np.sum((ref_u - calc_u)**2)
    ss_tot2 = np.sum((ref_u - np.mean(ref_u))**2)
    r2_fig2 = 1.0 - (ss_res2 / ss_tot2)

    print(
        f"--> Fig 1 Day-Night Contrast R^2 Score: {r2_fig1:.4f} ({r2_fig1:.2%})"
    )
    print(
        f"--> Fig 2 Zonal Wind Speed R^2 Score:  {r2_fig2:.4f} ({r2_fig2:.2%})")
    assert r2_fig1 > 0.98, f"Fig 1 verification failed! R^2 = {r2_fig1:.4f} < 0.98"
    assert r2_fig2 > 0.98, f"Fig 2 verification failed! R^2 = {r2_fig2:.4f} < 0.98"
    print("✅ Komacek & Showman (2016) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_komacek2016()
