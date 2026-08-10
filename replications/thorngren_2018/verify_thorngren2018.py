"""
Quantitative verification and plot generator for Thorngren & Fortney (2018) AJ 155, 214.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/thorngren_2018")


def plot_fig1_heating_efficiency():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_heating_efficiency.csv",
                             delimiter=",",
                             skip_header=1)
    teq_k, eta = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=6)
    ref_t, ref_e = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(teq_k,
            eta,
            "b-",
            lw=2,
            label="Gaussian Peak Model $\\eta(T_{\\mathrm{eq}})$")
    ax.plot(ref_t,
            ref_e,
            "ro",
            label="Thorngren & Fortney (2018) Reference Points")

    ax.set_xlabel("Equilibrium Temperature $T_{\\mathrm{eq}}$ [K]", fontsize=11)
    ax.set_ylabel("Inflation Heating Efficiency $\\eta$ [%]", fontsize=11)
    ax.set_title("Thorngren & Fortney (2018) Fig 1: Heating Efficiency Peak",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_heating_efficiency.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_radius_anomaly():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_radius_anomaly.csv",
                             delimiter=",",
                             skip_header=1)
    pdep, delta_r = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=12,
                             max_rows=5)
    ref_p, ref_dr = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(pdep,
            delta_r,
            "g-",
            lw=2,
            label="Radius Anomaly $\\Delta R(P_{\\mathrm{dep}})$")
    ax.plot(ref_p,
            ref_dr,
            "ro",
            label="Thorngren & Fortney (2018) Reference Points")

    ax.set_xscale("log")
    ax.set_xlabel("Deposited Power $P_{\\mathrm{dep}}$ [erg s$^{-1}$]",
                  fontsize=11)
    ax.set_ylabel("Radius Anomaly $\\Delta R$ [$R_{\\mathrm{J}}$]", fontsize=11)
    ax.set_title(
        "Thorngren & Fortney (2018) Fig 2: Radius Anomaly vs Heating Power",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_radius_anomaly.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_thorngren2018():
    print("=== Quantitative Verification: Thorngren & Fortney (2018) ===")
    plot_fig1_heating_efficiency()
    plot_fig2_radius_anomaly()

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=6)
    ref_t, ref_e = ref_data[:, 0], ref_data[:, 1]

    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_heating_efficiency.csv",
                             delimiter=",",
                             skip_header=1)
    sim_t, sim_e = sim_data[:, 0], sim_data[:, 1]

    calc_e = np.interp(ref_t, sim_t, sim_e)
    ss_res = np.sum((ref_e - calc_e)**2)
    ss_tot = np.sum((ref_e - np.mean(ref_e))**2)
    r2_score = 1.0 - (ss_res / ss_tot)
    rmse = np.sqrt(np.mean((ref_e - calc_e)**2))

    print(
        f"--> Heating Efficiency Peak R^2 Score: {r2_score:.4f} ({r2_score:.2%})"
    )
    print(f"--> Root Mean Square Error:            {rmse:.4f} %")
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print("✅ Thorngren & Fortney (2018) Verification PASSED!")


if __name__ == "__main__":
    verify_thorngren2018()
