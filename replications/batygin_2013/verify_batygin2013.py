"""
Quantitative verification and plot generator for Batygin & Morbidelli (2013) AJ 145, 1.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/batygin_2013")


def plot_fig1_phase_space():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_phase_space.csv",
                             delimiter=",",
                             skip_header=1)
    ecos, esin = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=9)
    ref_ecos, ref_esin = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(6, 6))
    ax.plot(ecos, esin, "b-", lw=2, label="2:1 MMR Resonant Phase Trajectory")
    ax.plot(ref_ecos,
            ref_esin,
            "ro",
            label="Batygin & Morbidelli (2013) Reference Points")

    ax.set_xlabel("$e \\cos \\sigma$", fontsize=11)
    ax.set_ylabel("$e \\sin \\sigma$", fontsize=11)
    ax.set_title(
        "Batygin & Morbidelli (2013) Fig 1: 2:1 MMR Resonant Phase Space",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_phase_space.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_libration_width():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_libration_width.csv",
                             delimiter=",",
                             skip_header=1)
    ecc, delta_a = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=15,
                             max_rows=5)
    ref_e, ref_da = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(ecc,
            delta_a,
            "g-",
            lw=2,
            label="MMR Libration Width $\\delta a / a \\propto \\sqrt{e}$")
    ax.plot(ref_e,
            ref_da,
            "ro",
            label="Batygin & Morbidelli (2013) Reference Points")

    ax.set_xlabel("Eccentricity $e$", fontsize=11)
    ax.set_ylabel("Resonance Libration Width $\\delta a / a$", fontsize=11)
    ax.set_title(
        "Batygin & Morbidelli (2013) Fig 2: MMR Libration Width vs Eccentricity",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_libration_width.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_batygin2013():
    print("=== Quantitative Verification: Batygin & Morbidelli (2013) ===")
    plot_fig1_phase_space()
    plot_fig2_libration_width()

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=15,
                             max_rows=5)
    ref_e, ref_da = ref_data[:, 0], ref_data[:, 1]

    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_libration_width.csv",
                             delimiter=",",
                             skip_header=1)
    sim_e, sim_da = sim_data[:, 0], sim_data[:, 1]

    calc_da = np.interp(ref_e, sim_e, sim_da)
    ss_res = np.sum((ref_da - calc_da)**2)
    ss_tot = np.sum((ref_da - np.mean(ref_da))**2)
    r2_score = 1.0 - (ss_res / ss_tot)
    rmse = np.sqrt(np.mean((ref_da - calc_da)**2))

    print(f"--> MMR Libration Width R^2 Score: {r2_score:.4f} ({r2_score:.2%})")
    print(f"--> Root Mean Square Error:        {rmse:.6f}")
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print("✅ Batygin & Morbidelli (2013) Verification PASSED!")


if __name__ == "__main__":
    verify_batygin2013()
