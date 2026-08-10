"""
Quantitative verification and plot generator for Batygin & Stevenson (2010) ApJL 714, L238.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/batygin_2010")


def plot_fig1_conductivity():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_conductivity.csv",
                             delimiter=",",
                             skip_header=1)
    temp, sigma = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=8)
    ref_t, ref_sigma = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(temp,
            sigma,
            "b-",
            lw=2,
            label="Replicated Thermal Ionization Model")
    ax.plot(ref_t,
            ref_sigma,
            "ro",
            label="Batygin & Stevenson (2010) Reference Points")

    ax.set_yscale("log")
    ax.set_xlabel("Temperature $T$ [K]", fontsize=11)
    ax.set_ylabel("Electrical Conductivity $\\sigma_{\\mathrm{elec}}$ [S/m]",
                  fontsize=11)
    ax.set_title("Batygin & Stevenson (2010) Fig 1: Atmospheric Conductivity",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_conductivity.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_radius_inflation():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_inflation.csv",
                             delimiter=",",
                             skip_header=1)
    teq, rp_rj = sim_data[:, 0], sim_data[:, 2]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=14,
                             max_rows=7)
    ref_teq, ref_rp = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(teq,
            rp_rj,
            "g-",
            lw=2,
            label="Ohmic Inflation Profile $R_p(T_{\\mathrm{eq}})$")
    ax.plot(ref_teq,
            ref_rp,
            "ro",
            label="Batygin & Stevenson (2010) Reference Points")

    ax.set_xlabel("Equilibrium Temperature $T_{\\mathrm{eq}}$ [K]", fontsize=11)
    ax.set_ylabel("Planetary Radius $R_p$ [$R_{\\mathrm{J}}$]", fontsize=11)
    ax.set_title(
        "Batygin & Stevenson (2010) Fig 2: Ohmic Radius Inflation Peak",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_radius_inflation.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_batygin2010():
    print("=== Quantitative Verification: Batygin & Stevenson (2010) ===")
    plot_fig1_conductivity()
    plot_fig2_radius_inflation()

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=14,
                             max_rows=7)
    ref_teq, ref_rp = ref_data[:, 0], ref_data[:, 1]

    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_inflation.csv",
                             delimiter=",",
                             skip_header=1)
    sim_teq, sim_rp = sim_data[:, 0], sim_data[:, 2]

    calc_rp = np.interp(ref_teq, sim_teq, sim_rp)
    ss_res = np.sum((ref_rp - calc_rp)**2)
    ss_tot = np.sum((ref_rp - np.mean(ref_rp))**2)
    r2_score = 1.0 - (ss_res / ss_tot)
    rmse = np.sqrt(np.mean((ref_rp - calc_rp)**2))

    print(
        f"--> Ohmic Radius Inflation R^2 Score: {r2_score:.4f} ({r2_score:.2%})"
    )
    print(f"--> Root Mean Square Error:           {rmse:.4f} R_J")
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print("✅ Batygin & Stevenson (2010) Verification PASSED!")


if __name__ == "__main__":
    verify_batygin2010()
