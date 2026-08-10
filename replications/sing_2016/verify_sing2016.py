"""
Quantitative verification and plot generator for Sing et al. (2016) Nature 529, 59.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/sing_2016")


def plot_fig1_spectrum():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_spectrum.csv",
                             delimiter=",",
                             skip_header=1)
    wave, clear_ppm, cloudy_ppm = sim_data[:, 0], sim_data[:, 1], sim_data[:, 2]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=5)
    ref_wave, ref_clear, ref_cloudy = ref_data[:, 0], ref_data[:,
                                                               1], ref_data[:,
                                                                            2]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(wave,
            clear_ppm,
            "b-",
            lw=2,
            label="Clear Transmission Spectrum (WASP-31b)")
    ax.plot(wave,
            cloudy_ppm,
            "r--",
            lw=2,
            label="Cloudy Transmission Spectrum (WASP-12b)")
    ax.plot(ref_wave, ref_clear, "bo", label="Ref Clear")
    ax.plot(ref_wave, ref_cloudy, "ro", label="Ref Cloudy")

    ax.set_xlabel("Wavelength $\\lambda$ [$\\mu$m]", fontsize=11)
    ax.set_ylabel("Transit Depth $(R_p/R_\\star)^2$ [ppm]", fontsize=11)
    ax.set_title(
        "Sing et al. (2016) Fig 1: Clear vs Hazy/Cloudy Transmission Spectra",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_spectrum.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_water_h():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_water_h.csv",
                             delimiter=",",
                             skip_header=1)
    tau_index, water_h = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=12,
                             max_rows=5)
    ref_tau, ref_h = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(tau_index,
            water_h,
            "g-",
            lw=2,
            label="H$_2$O Amplitude $\\Delta H_{\\text{H2O}}$ [Scale Heights]")
    ax.plot(ref_tau, ref_h, "ro", label="Sing et al. (2016) Reference Points")

    ax.set_xlabel("Cloud Optical Depth Index $\\tau_{\\text{cloud}}$",
                  fontsize=11)
    ax.set_ylabel("Water Absorption Amplitude $\\Delta H_{\\text{H2O}}$ [$H$]",
                  fontsize=11)
    ax.set_title(
        "Sing et al. (2016) Fig 2: Water Feature Dampening by Cloud Opacity",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_water_h.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_sing2016():
    print("=== Quantitative Verification: Sing et al. (2016) ===")
    plot_fig1_spectrum()
    plot_fig2_water_h()

    # Fig 1 Verification
    ref_data1 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=3,
                              max_rows=5)
    ref_wave, ref_clear = ref_data1[:, 0], ref_data1[:, 1]

    sim_data1 = np.genfromtxt(REPLICATION_DIR / "sim_spectrum.csv",
                              delimiter=",",
                              skip_header=1)
    sim_wave, sim_clear = sim_data1[:, 0], sim_data1[:, 1]

    calc_clear = np.interp(ref_wave, sim_wave, sim_clear)
    ss_res1 = np.sum((ref_clear - calc_clear)**2)
    ss_tot1 = np.sum((ref_clear - np.mean(ref_clear))**2)
    r2_fig1 = 1.0 - (ss_res1 / ss_tot1)

    # Fig 2 Verification
    ref_data2 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=12,
                              max_rows=5)
    ref_tau, ref_h = ref_data2[:, 0], ref_data2[:, 1]

    sim_data2 = np.genfromtxt(REPLICATION_DIR / "sim_water_h.csv",
                              delimiter=",",
                              skip_header=1)
    sim_tau, sim_h = sim_data2[:, 0], sim_data2[:, 1]

    calc_h = np.interp(ref_tau, sim_tau, sim_h)
    ss_res2 = np.sum((ref_h - calc_h)**2)
    ss_tot2 = np.sum((ref_h - np.mean(ref_h))**2)
    r2_fig2 = 1.0 - (ss_res2 / ss_tot2)

    print(
        f"--> Fig 1 Transmission Spectra R^2 Score: {r2_fig1:.4f} ({r2_fig1:.2%})"
    )
    print(
        f"--> Fig 2 Water Feature Dampening R^2 Score: {r2_fig2:.4f} ({r2_fig2:.2%})"
    )
    assert r2_fig1 > 0.98, f"Fig 1 verification failed! R^2 = {r2_fig1:.4f} < 0.98"
    assert r2_fig2 > 0.98, f"Fig 2 verification failed! R^2 = {r2_fig2:.4f} < 0.98"
    print("✅ Sing et al. (2016) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_sing2016()
