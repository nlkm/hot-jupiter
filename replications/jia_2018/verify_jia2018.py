"""
Quantitative verification and plot generator for Jia & Spruit (2018) MNRAS 476, 1765.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/jia_2018")


def plot_fig1_envelope():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_envelope.csv",
                             delimiter=",",
                             skip_header=1)
    rp, f_env = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=6)
    ref_rp, ref_f = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(rp,
            f_env,
            "b-",
            lw=2,
            label="Envelope Mass Fraction $f_{\\mathrm{env}}(R_p)$")
    ax.plot(ref_rp, ref_f, "ro", label="Jia & Spruit (2018) Reference Points")

    ax.set_xlabel("Planetary Radius $R_p$ [$R_{\\mathrm{Jup}}$]", fontsize=11)
    ax.set_ylabel(
        "Envelope Mass Fraction $f_{\\mathrm{env}} = M_{\\mathrm{env}} / M_p$",
        fontsize=11)
    ax.set_title("Jia & Spruit (2018) Fig 1: Envelope Mass Fraction vs Radius",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_envelope.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_stripping():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_stripping.csv",
                             delimiter=",",
                             skip_header=1)
    fc, mdot = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=12,
                             max_rows=5)
    ref_fc, ref_mdot = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(fc, mdot, "g-", lw=2, label="RLOF Stripping Mdot [g/s]")
    ax.plot(ref_fc,
            ref_mdot,
            "ro",
            label="Jia & Spruit (2018) Reference Points")

    ax.set_xlabel("Core Mass Fraction $M_c / M_p$", fontsize=11)
    ax.set_ylabel("Mass Loss Rate $\\dot{M}_{RLOF}$ [g/s]", fontsize=11)
    ax.set_title("Jia & Spruit (2018) Fig 2: RLOF Envelope Stripping Rate",
                 fontsize=12)
    ax.set_yscale("log")
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_stripping.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_jia2018():
    print("=== Quantitative Verification: Jia & Spruit (2018) ===")
    plot_fig1_envelope()
    plot_fig2_stripping()

    # Verify Figure 1: Envelope Mass Fraction
    ref_data1 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=3,
                              max_rows=6)
    ref_rp, ref_f = ref_data1[:, 0], ref_data1[:, 1]

    sim_data1 = np.genfromtxt(REPLICATION_DIR / "sim_envelope.csv",
                              delimiter=",",
                              skip_header=1)
    sim_rp, sim_f = sim_data1[:, 0], sim_data1[:, 1]

    calc_f = np.interp(ref_rp, sim_rp, sim_f)
    ss_res1 = np.sum((ref_f - calc_f)**2)
    ss_tot1 = np.sum((ref_f - np.mean(ref_f))**2)
    r2_fig1 = 1.0 - (ss_res1 / ss_tot1)

    # Verify Figure 2: RLOF Envelope Stripping Rate
    ref_data2 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=12,
                              max_rows=5)
    ref_fc, ref_mdot = ref_data2[:, 0], ref_data2[:, 1]

    sim_data2 = np.genfromtxt(REPLICATION_DIR / "sim_stripping.csv",
                              delimiter=",",
                              skip_header=1)
    sim_fc, sim_mdot = sim_data2[:, 0], sim_data2[:, 1]

    calc_mdot = np.interp(ref_fc, sim_fc, sim_mdot)
    ss_res2 = np.sum((np.log10(ref_mdot) - np.log10(calc_mdot))**2)
    ss_tot2 = np.sum((np.log10(ref_mdot) - np.mean(np.log10(ref_mdot)))**2)
    r2_fig2 = 1.0 - (ss_res2 / ss_tot2)

    print(
        f"--> Fig 1 Envelope Fraction R^2 Score: {r2_fig1:.4f} ({r2_fig1:.2%})")
    print(
        f"--> Fig 2 Stripping Mdot R^2 Score:     {r2_fig2:.4f} ({r2_fig2:.2%})"
    )
    assert r2_fig1 > 0.98, f"Fig 1 verification failed! R^2 = {r2_fig1:.4f} < 0.98"
    assert r2_fig2 > 0.98, f"Fig 2 verification failed! R^2 = {r2_fig2:.4f} < 0.98"
    print("✅ Jia & Spruit (2018) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_jia2018()
