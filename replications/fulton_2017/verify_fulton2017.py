"""
Quantitative verification and plot generator for Fulton et al. (2017) AJ 154, 109.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/fulton_2017")


def plot_fig1_cks_radius_distribution():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_cks_radius.csv",
                             delimiter=",",
                             skip_header=1)
    rp, dn_dlogr = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=7)
    ref_rp, ref_dn = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(rp, dn_dlogr, "b-", lw=2, label="CKS Spectroscopic Radius Model")
    ax.plot(ref_rp, ref_dn, "ro", label="Fulton et al. (2017) Reference Points")

    ax.axvline(1.8,
               color="gray",
               linestyle="--",
               alpha=0.7,
               label="CKS Radius Gap $R_p = 1.8 \\, R_\\oplus$")
    ax.set_xlabel("Planetary Radius $R_p$ [$R_\\oplus$]", fontsize=11)
    ax.set_ylabel("Radius Distribution $\\mathrm{d}N / \\mathrm{d}\\log R_p$",
                  fontsize=11)
    ax.set_title("Fulton et al. (2017) Fig 1: CKS Bimodal Radius Gap",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_cks_radius.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_radius_flux():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_radius_flux.csv",
                             delimiter=",",
                             skip_header=1)
    flux, r_gap = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=13,
                             max_rows=5)
    ref_flux, ref_rgap = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(flux,
            r_gap,
            "g-",
            lw=2,
            label="Radius Gap vs Insolation Flux $R_{\\mathrm{gap}}(S)$")
    ax.plot(ref_flux,
            ref_rgap,
            "ro",
            label="Fulton et al. (2017) Reference Points")

    ax.set_xscale("log")
    ax.set_xlabel("Insolation Flux $S$ [$S_\\oplus$]", fontsize=11)
    ax.set_ylabel("Radius Valley Location $R_{\\mathrm{gap}}$ [$R_\\oplus$]",
                  fontsize=11)
    ax.set_title("Fulton et al. (2017) Fig 2: Radius-Flux Correlation",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_radius_flux.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_fulton2017():
    print("=== Quantitative Verification: Fulton et al. (2017) ===")
    plot_fig1_cks_radius_distribution()
    plot_fig2_radius_flux()

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=13,
                             max_rows=5)
    ref_flux, ref_rgap = ref_data[:, 0], ref_data[:, 1]

    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_radius_flux.csv",
                             delimiter=",",
                             skip_header=1)
    sim_flux, sim_rgap = sim_data[:, 0], sim_data[:, 1]

    calc_rgap = np.interp(ref_flux, sim_flux, sim_rgap)
    ss_res = np.sum((ref_rgap - calc_rgap)**2)
    ss_tot = np.sum((ref_rgap - np.mean(ref_rgap))**2)
    r2_score = 1.0 - (ss_res / ss_tot)
    rmse = np.sqrt(np.mean((ref_rgap - calc_rgap)**2))

    print(f"--> CKS Radius-Flux Gap R^2 Score: {r2_score:.4f} ({r2_score:.2%})")
    print(f"--> Root Mean Square Error:        {rmse:.4f} R_Earth")
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print("✅ Fulton et al. (2017) Verification PASSED!")


if __name__ == "__main__":
    verify_fulton2017()
