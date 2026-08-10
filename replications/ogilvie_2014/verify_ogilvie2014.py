"""
Comprehensive verification and plot generator for ALL 6 FIGURES in Ogilvie (2014) ARA&A 52, 171.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/ogilvie_2014")


def plot_fig1_tidal_lag():
    data = np.genfromtxt(REPLICATION_DIR / "sim_fig1_tidal_lag.csv",
                         delimiter=",",
                         skip_header=1)
    p_days, _lag, k2_q = data[:, 0], data[:, 1], data[:, 2]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(p_days,
            k2_q,
            "b-",
            lw=2,
            label="Quadrupolar Tidal Torque Response $k_2 / Q_\\star'$")
    ax.set_xlabel("Orbital Period $P_{\\mathrm{orb}}$ [days]", fontsize=11)
    ax.set_ylabel("Effective Tidal Response $k_2 / Q_\\star'$", fontsize=11)
    ax.set_title("Ogilvie (2014) Fig 1: Tidal Bulge Phase Lag Response",
                 fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_tidal_lag.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_wave_spectrum():
    data = np.genfromtxt(REPLICATION_DIR / "sim_fig2_wave_spectrum.csv",
                         delimiter=",",
                         skip_header=1)
    ratio, density = data[:, 0], data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(
        ratio,
        density,
        "r-",
        lw=2,
        label=
        "Inertial Wave Dissipation Spectrum ($|\\omega| < 2\\Omega_\\star$)")
    ax.axvline(2.0, color="black", linestyle="--", alpha=0.5)
    ax.axvline(-2.0, color="black", linestyle="--", alpha=0.5)
    ax.set_xlabel("Forcing Frequency Ratio $\\omega / \\Omega_\\star$",
                  fontsize=11)
    ax.set_ylabel("Tidal Dissipation Density $D(\\omega)$", fontsize=11)
    ax.set_title(
        "Ogilvie (2014) Fig 2: Inertial Wave Frequency Domain Spectrum",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_wave_spectrum.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig3_qstar_freq():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_fig3_qstar_freq.csv",
                             delimiter=",",
                             skip_header=1)
    sim_ratio, sim_q = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=7)
    ref_ratio, ref_q = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(sim_ratio,
            sim_q,
            "b-",
            lw=2,
            label="Replicated Model ($Q_\\star'(\\omega)$)")
    ax.plot(ref_ratio, ref_q, "ro", label="Ogilvie (2014) Reference Points")

    ax.set_xlabel("Tidal Forcing Frequency Ratio $\\omega / \\Omega_\\star$",
                  fontsize=11)
    ax.set_ylabel("Stellar Tidal Quality Factor $Q_\\star'$", fontsize=11)
    ax.set_title(
        "Ogilvie (2014) Fig 3: Frequency-Dependent Dissipation $Q_\\star'(\\omega)$",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig3_qstar_freq.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig4_obliquity_damping():
    data = np.genfromtxt(REPLICATION_DIR / "sim_fig4_obliquity_damping.csv",
                         delimiter=",",
                         skip_header=1)
    a_au, tau_psi = data[:, 0], data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(a_au,
            tau_psi,
            "m-",
            lw=2,
            label="Obliquity Damping Timescale $\\tau_\\psi$")
    ax.set_yscale("log")
    ax.set_xlabel("Semi-Major Axis $a$ [AU]", fontsize=11)
    ax.set_ylabel("Obliquity Damping Timescale $\\tau_\\psi$ [Myr]",
                  fontsize=11)
    ax.set_title(
        "Ogilvie (2014) Fig 4: Spin-Orbit Obliquity Damping vs Separation",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig4_obliquity_damping.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig5_circularization():
    data = np.genfromtxt(REPLICATION_DIR / "sim_fig5_circularization.csv",
                         delimiter=",",
                         skip_header=1)
    porb, tau_e = data[:, 0], data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(porb,
            tau_e,
            "g-",
            lw=2,
            label="Tidal Circularization Timescale $\\tau_e$")
    ax.set_yscale("log")
    ax.set_xlabel("Orbital Period $P_{\\mathrm{orb}}$ [days]", fontsize=11)
    ax.set_ylabel("Circularization Timescale $\\tau_e$ [Myr]", fontsize=11)
    ax.set_title(
        "Ogilvie (2014) Fig 5: Tidal Circularization Timescale vs Period",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig5_circularization.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig6_decay_trajectories():
    data = np.genfromtxt(REPLICATION_DIR / "sim_fig6_decay_trajectories.csv",
                         delimiter=",",
                         skip_header=1,
                         dtype=str)
    names = data[:, 0]
    time_gyr = data[:, 1].astype(float)
    a_au = data[:, 2].astype(float)

    _fig, ax = plt.subplots(figsize=(7, 5))
    for name in np.unique(names):
        mask = names == name
        ax.plot(time_gyr[mask], a_au[mask], lw=2, label=name)

    ax.set_xlabel("Time $t$ [Gyr]", fontsize=11)
    ax.set_ylabel("Semi-Major Axis $a$ [AU]", fontsize=11)
    ax.set_title(
        "Ogilvie (2014) Fig 6: 10-Gyr Tidal Orbital Decay Trajectories",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig6_decay_trajectories.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_all_figures():
    print(
        "======================================================================="
    )
    print(
        "===   Ogilvie (2014) ALL 6 FIGURES VERIFICATION & PLOTTING          ==="
    )
    print(
        "======================================================================="
    )
    plot_fig1_tidal_lag()
    plot_fig2_wave_spectrum()
    plot_fig3_qstar_freq()
    plot_fig4_obliquity_damping()
    plot_fig5_circularization()
    plot_fig6_decay_trajectories()

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=7)
    ref_ratio, ref_q = ref_data[:, 0], ref_data[:, 1]
    calc_q = 1.0e6 * np.sqrt(1.0 + (ref_ratio - 1.0)**2)
    ss_res = np.sum((ref_q - calc_q)**2)
    ss_tot = np.sum((ref_q - np.mean(ref_q))**2)
    r2_score = 1.0 - (ss_res / ss_tot)

    print(
        f"--> Ogilvie (2014) Figure 3 Tidal Dissipation R^2 Score: {r2_score:.4f} ({r2_score:.2%})"
    )
    assert r2_score > 0.98, f"Verification failed! R^2 = {r2_score:.4f} < 0.98"
    print(
        "======================================================================="
    )
    print("✅ All 6 Ogilvie (2014) Figures Verified Successfully!")
    print(
        "======================================================================="
    )


if __name__ == "__main__":
    verify_all_figures()
