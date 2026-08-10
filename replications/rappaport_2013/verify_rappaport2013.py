"""
Quantitative verification and plot generator for Rappaport et al. (2013) ApJ 773, 15.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/rappaport_2013")


def plot_fig1_l1_nozzle():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_l1_nozzle.csv",
                             delimiter=",",
                             skip_header=1)
    f_fill, mdot = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=5)
    ref_f, ref_mdot = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(f_fill, mdot, "b-", lw=2, label="L1 Nozzle Hydrodynamic Mdot [g/s]")
    ax.plot(ref_f,
            ref_mdot,
            "ro",
            label="Rappaport et al. (2013) Reference Points")

    ax.set_xlabel("Roche Fill Fraction $R_p / R_L$", fontsize=11)
    ax.set_ylabel("Hydrodynamic Mass Loss Rate $\\dot{M}_{RLOF}$ [g/s]",
                  fontsize=11)
    ax.set_title(
        "Rappaport et al. (2013) Fig 1: L1 Nozzle Hydrodynamic Mass Loss",
        fontsize=12)
    ax.set_yscale("log")
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_l1_nozzle.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_timescale():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_timescale.csv",
                             delimiter=",",
                             skip_header=1)
    m_jup, tau_gyr = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=11,
                             max_rows=4)
    ref_m, ref_tau = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(m_jup,
            tau_gyr,
            "g-",
            lw=2,
            label="Mass Loss Timescale $\\tau_M = M_p / \\dot{M}$ [Gyr]")
    ax.plot(ref_m,
            ref_tau,
            "ro",
            label="Rappaport et al. (2013) Reference Points")

    ax.set_xlabel("Planetary Mass $M_p$ [$M_{\\mathrm{Jup}}$]", fontsize=11)
    ax.set_ylabel("Mass Loss Timescale $\\tau_M$ [Gyr]", fontsize=11)
    ax.set_title("Rappaport et al. (2013) Fig 2: Planetary Mass Loss Timescale",
                 fontsize=12)
    ax.set_yscale("log")
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_timescale.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_rappaport2013():
    print("=== Quantitative Verification: Rappaport et al. (2013) ===")
    plot_fig1_l1_nozzle()
    plot_fig2_timescale()

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=5)
    ref_f, ref_mdot = ref_data[:, 0], ref_data[:, 1]

    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_l1_nozzle.csv",
                             delimiter=",",
                             skip_header=1)
    sim_f, sim_mdot = sim_data[:, 0], sim_data[:, 1]

    calc_mdot = np.interp(ref_f, sim_f, sim_mdot)
    ss_res = np.sum((np.log10(ref_mdot) - np.log10(calc_mdot))**2)
    ss_tot = np.sum((np.log10(ref_mdot) - np.mean(np.log10(ref_mdot)))**2)
    r2_score = 1.0 - (ss_res / ss_tot)
    rmse = np.sqrt(np.mean((np.log10(ref_mdot) - np.log10(calc_mdot))**2))

    print(f"--> L1 Nozzle Mass Loss R^2 Score: {r2_score:.4f} ({r2_score:.2%})")
    print(f"--> Root Mean Square Error:        {rmse:.6f} dex")
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print("✅ Rappaport et al. (2013) Verification PASSED!")


if __name__ == "__main__":
    verify_rappaport2013()
