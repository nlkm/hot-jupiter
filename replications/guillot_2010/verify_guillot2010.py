"""
Quantitative verification and plot generator for Guillot (2010) A&A 520, A27.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/guillot_2010")


def plot_fig1_temperature_optical_depth():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_guillot_tau.csv",
                             delimiter=",",
                             skip_header=1)
    tau, gamma, t_atm = sim_data[:, 0], sim_data[:, 1], sim_data[:, 2]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=6)
    ref_tau, ref_t = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    for g in np.unique(gamma):
        mask = gamma == g
        ax.plot(tau[mask], t_atm[mask], label=f"$\\gamma = {g}$")

    ax.plot(ref_tau,
            ref_t,
            "ro",
            label="Guillot (2010) Reference Points ($\\gamma=0.1$)")

    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xlabel("Optical Depth $\\tau$", fontsize=11)
    ax.set_ylabel("Atmospheric Temperature $T$ [K]", fontsize=11)
    ax.set_title("Guillot (2010) Fig 1: Double-Gray Temperature Profiles",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_temperature_tau.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_tp_profile():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_guillot_tp.csv",
                             delimiter=",",
                             skip_header=1,
                             dtype=str)
    names = sim_data[:, 0]
    p_bar = sim_data[:, 1].astype(float)
    t_atm = sim_data[:, 2].astype(float)

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=12,
                             max_rows=6)
    ref_p, ref_t = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    for name in np.unique(names):
        mask = names == name
        ax.plot(t_atm[mask], p_bar[mask], lw=2, label=name)

    ax.plot(ref_t, ref_p, "ro", label="HD 209458b Reference Points")

    ax.set_yscale("log")
    ax.invert_yaxis()
    ax.set_xlabel("Temperature $T$ [K]", fontsize=11)
    ax.set_ylabel("Pressure $P$ [bar]", fontsize=11)
    ax.set_title("Guillot (2010) Fig 2: Atmospheric T-P Profiles", fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_tp_profile.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_guillot2010():
    print("=== Quantitative Verification: Guillot (2010) ===")
    plot_fig1_temperature_optical_depth()
    plot_fig2_tp_profile()

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=6)
    ref_tau, ref_t = ref_data[:, 0], ref_data[:, 1]

    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_guillot_tau.csv",
                             delimiter=",",
                             skip_header=1)
    gamma = sim_data[:, 1]
    mask_g01 = gamma == 0.1
    sim_tau_g01 = sim_data[mask_g01, 0]
    sim_t_g01 = sim_data[mask_g01, 2]

    calc_t = np.interp(ref_tau, sim_tau_g01, sim_t_g01)
    ss_res = np.sum((ref_t - calc_t)**2)
    ss_tot = np.sum((ref_t - np.mean(ref_t))**2)
    r2_score = 1.0 - (ss_res / ss_tot)
    rmse = np.sqrt(np.mean((ref_t - calc_t)**2))

    print(
        f"--> Double-Gray Temperature Profile R^2 Score: {r2_score:.4f} ({r2_score:.2%})"
    )
    print(f"--> Root Mean Square Error:                    {rmse:.2f} K")
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print("✅ Guillot (2010) Verification PASSED!")


if __name__ == "__main__":
    verify_guillot2010()
