"""
Quantitative verification and plot generator for Lubow & Shu (1975) ApJ 198, 383.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/lubow_1975")


def plot_fig1_trajectory():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_trajectory.csv",
                             delimiter=",",
                             skip_header=1)
    x, y = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=6)
    ref_x, ref_y = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(x, y, "b-", lw=2, label="L1 Gas Stream Trajectory $(x/d, y/d)$")
    ax.plot(ref_x, ref_y, "ro", label="Lubow & Shu (1975) Reference Points")

    ax.set_xlabel("Rotating Frame $x / d$", fontsize=11)
    ax.set_ylabel("Rotating Frame $y / d$", fontsize=11)
    ax.set_title(
        "Lubow & Shu (1975) Fig 1: L1 Gas Stream Deflection Trajectory",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_trajectory.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_mass_transfer():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_mass_transfer.csv",
                             delimiter=",",
                             skip_header=1)
    ratio, mdot = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=12,
                             max_rows=5)
    ref_ratio, ref_mdot = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(ratio, mdot, "g-", lw=2, label="1D Sonic L1 Nozzle Mdot [g/s]")
    ax.plot(ref_ratio,
            ref_mdot,
            "ro",
            label="Lubow & Shu (1975) Reference Points")

    ax.set_xlabel("Sound Speed Ratio $c_s / (\\Omega d)$", fontsize=11)
    ax.set_ylabel("Mass Transfer Rate $\\dot{M}_{L1}$ [g/s]", fontsize=11)
    ax.set_title(
        "Lubow & Shu (1975) Fig 2: L1 Mass Transfer Rate vs Sound Speed",
        fontsize=12)
    ax.set_yscale("log")
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_mass_transfer.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_lubow1975():
    print("=== Quantitative Verification: Lubow & Shu (1975) ===")
    plot_fig1_trajectory()
    plot_fig2_mass_transfer()

    # Fig 1 Verification
    ref_data1 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=3,
                              max_rows=6)
    ref_x, ref_y = ref_data1[:, 0], ref_data1[:, 1]

    sim_data1 = np.genfromtxt(REPLICATION_DIR / "sim_trajectory.csv",
                              delimiter=",",
                              skip_header=1)
    sim_x, sim_y = sim_data1[:, 0], sim_data1[:, 1]

    idx = np.argsort(sim_x)
    sx, sy = sim_x[idx], sim_y[idx]

    calc_y = np.interp(ref_x, sx, sy)
    ss_res1 = np.sum((ref_y - calc_y)**2)
    ss_tot1 = np.sum((ref_y - np.mean(ref_y))**2)
    r2_fig1 = 1.0 - (ss_res1 / ss_tot1)

    # Fig 2 Verification
    ref_data2 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=12,
                              max_rows=5)
    ref_ratio, ref_mdot = ref_data2[:, 0], ref_data2[:, 1]

    sim_data2 = np.genfromtxt(REPLICATION_DIR / "sim_mass_transfer.csv",
                              delimiter=",",
                              skip_header=1)
    sim_ratio, sim_mdot = sim_data2[:, 0], sim_data2[:, 1]

    calc_mdot = np.interp(ref_ratio, sim_ratio, sim_mdot)
    ss_res2 = np.sum((np.log10(ref_mdot) - np.log10(calc_mdot))**2)
    ss_tot2 = np.sum((np.log10(ref_mdot) - np.mean(np.log10(ref_mdot)))**2)
    r2_fig2 = 1.0 - (ss_res2 / ss_tot2)

    print(
        f"--> Fig 1 Stream Trajectory R^2 Score: {r2_fig1:.4f} ({r2_fig1:.2%})")
    print(
        f"--> Fig 2 Mass Transfer R^2 Score:     {r2_fig2:.4f} ({r2_fig2:.2%})")
    assert r2_fig1 > 0.98, f"Fig 1 verification failed! R^2 = {r2_fig1:.4f} < 0.98"
    assert r2_fig2 > 0.98, f"Fig 2 verification failed! R^2 = {r2_fig2:.4f} < 0.98"
    print("✅ Lubow & Shu (1975) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_lubow1975()
