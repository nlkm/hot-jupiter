"""
Quantitative verification and plot generator for Showman et al. (2009) ApJ 699, 564.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

REPLICATION_DIR = Path("replications/showman_2009")


def plot_fig1_temperature():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_temperature.csv",
                             delimiter=",",
                             skip_header=1)
    lon_deg, temp = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=3,
                             max_rows=8)
    ref_lon, ref_temp = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(lon_deg,
            temp,
            "r-",
            lw=2,
            label="100 mbar Temperature $T(\\lambda)$ [K]")
    ax.axvline(0.0,
               color="k",
               linestyle=":",
               alpha=0.5,
               label="Substellar Point (0°)")
    ax.axvline(30.0,
               color="orange",
               linestyle="--",
               alpha=0.8,
               label="Hotspot Offset (+30°)")
    ax.plot(ref_lon,
            ref_temp,
            "ro",
            label="Showman et al. (2009) Reference Points")

    ax.set_xlabel("Longitude $\\lambda$ [deg]", fontsize=11)
    ax.set_ylabel("Temperature $T$ [K]", fontsize=11)
    ax.set_title(
        "Showman et al. (2009) Fig 1: Day-Night Temperature Profile & Hotspot Shift",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig1_temperature.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def plot_fig2_zonal_wind():
    sim_data = np.genfromtxt(REPLICATION_DIR / "sim_zonal_wind.csv",
                             delimiter=",",
                             skip_header=1)
    lat_deg, u_ms = sim_data[:, 0], sim_data[:, 1]

    ref_data = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                             delimiter=",",
                             skip_header=15,
                             max_rows=9)
    ref_lat, ref_u = ref_data[:, 0], ref_data[:, 1]

    _fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(lat_deg,
            u_ms,
            "b-",
            lw=2,
            label="Superrotating Jet $\\bar{u}(\\phi)$ [m/s]")
    ax.plot(ref_lat,
            ref_u,
            "ro",
            label="Showman et al. (2009) Reference Points")

    ax.set_xlabel("Latitude $\\phi$ [deg]", fontsize=11)
    ax.set_ylabel("Zonal-Mean Zonal Wind $\\bar{u}$ [m/s]", fontsize=11)
    ax.set_title(
        "Showman et al. (2009) Fig 2: Equatorial Superrotating Jet Profile",
        fontsize=12)
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend(fontsize=10)
    plt.tight_layout()
    path = REPLICATION_DIR / "fig2_zonal_wind.png"
    plt.savefig(path, dpi=150)
    plt.close()
    print(f"--> Saved {path}")


def verify_showman2009():
    print("=== Quantitative Verification: Showman et al. (2009) ===")
    plot_fig1_temperature()
    plot_fig2_zonal_wind()

    # Fig 1 Verification
    ref_data1 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=3,
                              max_rows=8)
    ref_lon, ref_temp = ref_data1[:, 0], ref_data1[:, 1]

    sim_data1 = np.genfromtxt(REPLICATION_DIR / "sim_temperature.csv",
                              delimiter=",",
                              skip_header=1)
    sim_lon, sim_temp = sim_data1[:, 0], sim_data1[:, 1]

    calc_temp = np.interp(ref_lon, sim_lon, sim_temp)
    ss_res1 = np.sum((ref_temp - calc_temp)**2)
    ss_tot1 = np.sum((ref_temp - np.mean(ref_temp))**2)
    r2_fig1 = 1.0 - (ss_res1 / ss_tot1)

    # Fig 2 Verification
    ref_data2 = np.genfromtxt(REPLICATION_DIR / "reference_data.csv",
                              delimiter=",",
                              skip_header=15,
                              max_rows=9)
    ref_lat, ref_u = ref_data2[:, 0], ref_data2[:, 1]

    sim_data2 = np.genfromtxt(REPLICATION_DIR / "sim_zonal_wind.csv",
                              delimiter=",",
                              skip_header=1)
    sim_lat, sim_u = sim_data2[:, 0], sim_data2[:, 1]

    calc_u = np.interp(ref_lat, sim_lat, sim_u)
    ss_res2 = np.sum((ref_u - calc_u)**2)
    ss_tot2 = np.sum((ref_u - np.mean(ref_u))**2)
    r2_fig2 = 1.0 - (ss_res2 / ss_tot2)

    print(
        f"--> Fig 1 Temperature Profile R^2 Score: {r2_fig1:.4f} ({r2_fig1:.2%})"
    )
    print(
        f"--> Fig 2 Zonal Wind Profile R^2 Score:  {r2_fig2:.4f} ({r2_fig2:.2%})"
    )
    assert r2_fig1 > 0.98, f"Fig 1 verification failed! R^2 = {r2_fig1:.4f} < 0.98"
    assert r2_fig2 > 0.98, f"Fig 2 verification failed! R^2 = {r2_fig2:.4f} < 0.98"
    print("✅ Showman et al. (2009) Verification PASSED FOR ALL FIGURES!")


if __name__ == "__main__":
    verify_showman2009()
