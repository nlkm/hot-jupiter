"""
Quantitative verification and plot generator for Thorngren et al. (2016) ApJ 831, 64.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/thorngren_2016")


def plot_fig1_mz_vs_mp():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_mz_vs_mp.csv",
                             delimiter=",",
                             skip_header=1)
    sim_mp, sim_mz = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=7)
    ref_mp, ref_mz = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_mp,
            sim_mz,
            "b-",
            lw=2,
            label="Replicated Model ($M_z \\propto M_p^{0.63}$)")
    ax.plot(ref_mp, ref_mz, "ro", label="Thorngren (2016) Reference Points")

    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xlabel("Planet Mass $M_p$ [$M_{\\mathrm{Jup}}$]", fontsize=11)
    ax.set_ylabel("Heavy-Element Core Mass $M_z$ [$M_{\\oplus}$]", fontsize=11)
    ax.set_title(
        "Thorngren et al. (2016) Fig 1: Heavy Element Mass vs Planet Mass",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_mz_vs_mp.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_zp_vs_mp():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_mz_vs_mp.csv",
                             delimiter=",",
                             skip_header=1)
    sim_mp, sim_zp = sim_data[:, 0], sim_data[:, 2]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_mp,
            sim_zp,
            "g-",
            lw=2,
            label="Heavy Element Mass Fraction $Z_p = M_z / M_p$")

    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xlabel("Planet Mass $M_p$ [$M_{\\mathrm{Jup}}$]", fontsize=11)
    ax.set_ylabel("Heavy Element Mass Fraction $Z_p$", fontsize=11)
    ax.set_title(
        "Thorngren et al. (2016) Fig 2: Heavy Element Mass Fraction vs Mass",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_zp_vs_mp.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig3_mz_vs_feh():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_mz_vs_feh.csv",
                             delimiter=",",
                             skip_header=1)
    sim_feh, sim_mz = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=13,
                             max_rows=5)
    ref_feh, ref_mz = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_feh,
            sim_mz,
            "b-",
            lw=2,
            label="Replicated Model ($M_z \\propto 10^{0.51 [Fe/H]}$)")
    ax.plot(ref_feh, ref_mz, "ro", label="Thorngren (2016) Reference Points")

    ax.set_xlabel("Stellar Metallicity $[\\mathrm{Fe/H}]$", fontsize=11)
    ax.set_ylabel("Heavy-Element Core Mass $M_z$ [$M_{\\oplus}$]", fontsize=11)
    ax.set_title(
        "Thorngren et al. (2016) Fig 3: Core Mass vs Stellar Metallicity",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig3_mz_vs_feh.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_thorngren2016():
    print("=== Quantitative Verification: Thorngren et al. (2016) ===")
    plot_fig1_mz_vs_mp()
    plot_fig2_zp_vs_mp()
    plot_fig3_mz_vs_feh()

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=7)
    ref_mp, ref_mz = ref_data[:, 0], ref_data[:, 1]

    calc_mz = 15.0 * (ref_mp**0.63)
    ss_res = np.sum((ref_mz - calc_mz)**2)
    ss_tot = np.sum((ref_mz - np.mean(ref_mz))**2)
    r2_score = 1.0 - (ss_res / ss_tot)
    rmse = np.sqrt(np.mean((ref_mz - calc_mz)**2))

    print(
        f"--> Heavy Element Scaling R^2 Score: {r2_score:.4f} ({r2_score:.2%})")
    print(f"--> Root Mean Square Error:         {rmse:.4f} M_earth")
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print("✅ Thorngren et al. (2016) Verification PASSED!")


if __name__ == "__main__":
    verify_thorngren2016()
