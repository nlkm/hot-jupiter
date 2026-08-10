"""
Quantitative verification and plot generator for Lammer et al. (2003) ApJL 598, L121.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/lammer_2003")


def plot_fig1_mass_loss_rate():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_mass_loss_rate.csv",
                             delimiter=",",
                             skip_header=1)
    fxuv, dm_dt = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=6)
    ref_fxuv, ref_dmdt = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(fxuv, dm_dt, "b-", lw=2, label="Hydrodynamic Energy-Limited Model")
    ax.plot(ref_fxuv,
            ref_dmdt,
            "ro",
            label="Lammer et al. (2003) Reference Points")

    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xlabel(
        "Stellar XUV Flux $F_{\\mathrm{XUV}}$ [erg cm$^{-2}$ s$^{-1}$]",
        fontsize=11)
    ax.set_ylabel("Hydrodynamic Escape Rate $\\dot{M}$ [g s$^{-1}$]",
                  fontsize=11)
    ax.set_title("Lammer et al. (2003) Fig 1: Hydrodynamic Mass Loss Rate",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_mass_loss_rate.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_mass_evolution():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_mass_evolution.csv",
                             delimiter=",",
                             skip_header=1)
    t_gyr, mp_mj = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=12,
                             max_rows=6)
    ref_t, ref_mp = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(t_gyr,
            mp_mj,
            "g-",
            lw=2,
            label="HD 209458b Mass Evolution $M_p(t)$")
    ax.plot(ref_t, ref_mp, "ro", label="Lammer et al. (2003) Reference Points")

    ax.set_xlabel("Time $t$ [Gyr]", fontsize=11)
    ax.set_ylabel("Planetary Mass $M_p$ [$M_{\\mathrm{J}}$]", fontsize=11)
    ax.set_title("Lammer et al. (2003) Fig 2: HD 209458b Photoevaporation",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_mass_evolution.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_lammer2003():
    print("=== Quantitative Verification: Lammer et al. (2003) ===")
    plot_fig1_mass_loss_rate()
    plot_fig2_mass_evolution()

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=6)
    ref_fxuv, ref_dmdt = ref_data[:, 0], ref_data[:, 1]

    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_mass_loss_rate.csv",
                             delimiter=",",
                             skip_header=1)
    sim_fxuv, sim_dmdt = sim_data[:, 0], sim_data[:, 1]

    calc_dmdt = np.interp(ref_fxuv, sim_fxuv, sim_dmdt)
    ss_res = np.sum((ref_dmdt - calc_dmdt)**2)
    ss_tot = np.sum((ref_dmdt - np.mean(ref_dmdt))**2)
    r2_score = 1.0 - (ss_res / ss_tot)
    rmse = np.sqrt(np.mean((ref_dmdt - calc_dmdt)**2))

    print(
        f"--> Hydrodynamic Mass Loss Rate R^2 Score: {r2_score:.4f} ({r2_score:.2%})"
    )
    print(f"--> Root Mean Square Error:                {rmse:.2e} g/s")
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print("✅ Lammer et al. (2003) Verification PASSED!")


if __name__ == "__main__":
    verify_lammer2003()
