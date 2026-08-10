"""
Quantitative verification and plot generator for Parmentier et al. (2016) A&A 596, A33.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/parmentier_2016")


def plot_fig1_condensation():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_condensation.csv",
                             delimiter=",",
                             skip_header=1)
    p_bar, t_mgsio3, t_mns = sim_data[:, 0], sim_data[:, 1], sim_data[:, 2]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=5)
    ref_p, ref_t_mgsio3, ref_t_mns = ref_data[:, 0], ref_data[:, 1], ref_data[:,
                                                                              2]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(t_mgsio3,
            p_bar,
            "r-",
            lw=2,
            label="MgSiO$_3$ Condensation $T_{\\text{cond}}(P)$")
    ax.plot(t_mns,
            p_bar,
            "b--",
            lw=2,
            label="MnS Condensation $T_{\\text{cond}}(P)$")
    ax.plot(ref_t_mgsio3, ref_p, "ro", label="Ref MgSiO$_3$")
    ax.plot(ref_t_mns, ref_p, "bo", label="Ref MnS")

    ax.set_xlabel("Condensation Temperature $T_{\\text{cond}}$ [K]",
                  fontsize=11)
    ax.set_ylabel("Pressure $P$ [bar]", fontsize=11)
    ax.set_title(
        "Parmentier et al. (2016) Fig 1: Cloud Condensation Boundaries",
        fontsize=12)
    ax.set_yscale("log")
    ax.invert_yaxis()
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_condensation.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_cloud_tau():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_cloud_tau.csv",
                             delimiter=",",
                             skip_header=1)
    teq, tau_cloud = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=12,
                             max_rows=5)
    ref_teq, ref_tau = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(teq,
            tau_cloud,
            "g-",
            lw=2,
            label="Cloud Optical Depth $\\tau_{\\text{cloud}}$")
    ax.plot(ref_teq,
            ref_tau,
            "ro",
            label="Parmentier et al. (2016) Reference Points")

    ax.set_xlabel("Equilibrium Temperature $T_{\\text{eq}}$ [K]", fontsize=11)
    ax.set_ylabel("Cloud Optical Depth $\\tau_{\\text{cloud}}$", fontsize=11)
    ax.set_title(
        "Parmentier et al. (2016) Fig 2: Clear-to-Cloudy Transition near 1600 K",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_cloud_tau.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_parmentier2016():
    print("=== Quantitative Verification: Parmentier et al. (2016) ===")
    plot_fig1_condensation()
    plot_fig2_cloud_tau()

    # Fig 1 Verification
    ref_data1 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=3,
                              max_rows=5)
    ref_p, ref_t_mgsio3 = ref_data1[:, 0], ref_data1[:, 1]

    sim_data1 = np.genfromtxt(REPLICATION_DIR / "sim_condensation.csv",
                              delimiter=",",
                              skip_header=1)
    sim_p, sim_t_mgsio3 = sim_data1[:, 0], sim_data1[:, 1]

    calc_t_mgsio3 = np.interp(np.log10(ref_p), np.log10(sim_p), sim_t_mgsio3)
    ss_res1 = np.sum((ref_t_mgsio3 - calc_t_mgsio3)**2)
    ss_tot1 = np.sum((ref_t_mgsio3 - np.mean(ref_t_mgsio3))**2)
    r2_fig1 = 1.0 - (ss_res1 / ss_tot1)

    # Fig 2 Verification
    ref_data2 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=12,
                              max_rows=5)
    ref_teq, ref_tau = ref_data2[:, 0], ref_data2[:, 1]

    sim_data2 = np.genfromtxt(REPLICATION_DIR / "sim_cloud_tau.csv",
                              delimiter=",",
                              skip_header=1)
    sim_teq, sim_tau = sim_data2[:, 0], sim_data2[:, 1]

    calc_tau = np.interp(ref_teq, sim_teq, sim_tau)
    ss_res2 = np.sum((ref_tau - calc_tau)**2)
    ss_tot2 = np.sum((ref_tau - np.mean(ref_tau))**2)
    r2_fig2 = 1.0 - (ss_res2 / ss_tot2)

    print(
        f"--> Fig 1 Condensation Boundaries R^2 Score: {r2_fig1:.4f} ({r2_fig1:.2%})"
    )
    print(
        f"--> Fig 2 Cloud Optical Depth R^2 Score:     {r2_fig2:.4f} ({r2_fig2:.2%})"
    )
    assert r2_fig1 > 0.98, f"Fig 1 verification failed! R^2 = {r2_fig1:.4f} < 0.98"
    assert r2_fig2 > 0.98, f"Fig 2 verification failed! R^2 = {r2_fig2:.4f} < 0.98"
    print("✅ Parmentier et al. (2016) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_parmentier2016()
