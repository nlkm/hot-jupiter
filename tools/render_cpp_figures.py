"""
Render high-resolution publication-quality vector figures from C++ generated CSV simulation data.
Uses standard Python csv module and matplotlib.
"""

import os
import csv
import matplotlib.pyplot as plt

def read_csv_columns(filepath):
    data = {}
    with open(filepath, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            for k, v in row.items():
                if k not in data:
                    data[k] = []
                data[k].append(float(v))
    return data

def main():
    os.makedirs("outputs", exist_ok=True)
    os.makedirs("paper/figures", exist_ok=True)

    print("Rendering publication-quality vector PDF figures from C++ simulation data...")

    # 1. Jupiter Cooling Track Plot
    if os.path.exists("outputs/jupiter_cooling_track.csv"):
        df = read_csv_columns("outputs/jupiter_cooling_track.csv")
        fig, axes = plt.subplots(2, 2, figsize=(8, 7), sharex=True)

        axes[0, 0].plot(df['t_gyr'], df['R_p_Rjup'], color='#1f77b4', lw=2)
        axes[0, 0].axhline(1.000, color='#d62728', ls='--', lw=1.2, label='Observed (1.000 R_Jup)')
        axes[0, 0].set_ylabel(r"Radius $R_p$ [$R_{\mathrm{Jup}}$]")
        axes[0, 0].set_ylim(0.8, 2.2)
        axes[0, 0].grid(True, alpha=0.3)
        axes[0, 0].legend(loc="upper right")

        axes[0, 1].plot(df['t_gyr'], df['T_eff_K'], color='#ff7f0e', lw=2)
        axes[0, 1].axhline(124.4, color='#d62728', ls='--', lw=1.2, label='Observed (124.4 K)')
        axes[0, 1].set_ylabel(r"Effective Temp $T_{\mathrm{eff}}$ [K]")
        axes[0, 1].grid(True, alpha=0.3)
        axes[0, 1].legend(loc="upper right")

        axes[1, 0].plot(df['t_gyr'], df['T_int_K'], color='#2ca02c', lw=2)
        axes[1, 0].axhline(99.6, color='#d62728', ls='--', lw=1.2, label='Observed (99.6 K)')
        axes[1, 0].set_xlabel("Age [Gyr]")
        axes[1, 0].set_ylabel(r"Intrinsic Temp $T_{\mathrm{int}}$ [K]")
        axes[1, 0].grid(True, alpha=0.3)
        axes[1, 0].legend(loc="upper right")

        axes[1, 1].plot(df['t_gyr'], df['L_int_Lsun'], color='#d62728', lw=2)
        axes[1, 1].set_xlabel("Age [Gyr]")
        axes[1, 1].set_ylabel(r"Intrinsic Luminosity $L_{\mathrm{int}}$ [$L_\odot$]")
        axes[1, 1].set_yscale("log")
        axes[1, 1].grid(True, alpha=0.3, which="both")

        fig.suptitle("Jupiter 1D Thermal Cooling Track (1 Myr to 4.56 Gyr)", fontsize=11, fontweight="bold")
        plt.tight_layout()
        fig.savefig("outputs/jupiter_cooling_track.pdf", bbox_inches="tight")
        fig.savefig("paper/figures/jupiter_cooling_track.pdf", bbox_inches="tight")
        plt.close(fig)

    # 2. Jupiter Internal Profile Plot
    if os.path.exists("outputs/jupiter_internal_profile.csv"):
        df = read_csv_columns("outputs/jupiter_internal_profile.csv")
        fig, axes = plt.subplots(2, 2, figsize=(8, 7), sharex=True)

        axes[0, 0].plot(df['r_ratio'], df['rho_gcm3'], color='#1f77b4', lw=2)
        axes[0, 0].set_ylabel(r"Density $\rho$ [g/cm$^3$]")
        axes[0, 0].set_xlim(0.0, 1.0)
        axes[0, 0].grid(True, alpha=0.3)

        axes[0, 1].plot(df['r_ratio'], df['P_bar'], color='#ff7f0e', lw=2)
        axes[0, 1].set_ylabel(r"Pressure $P$ [bar]")
        axes[0, 1].set_yscale("log")
        axes[0, 1].set_xlim(0.0, 1.0)
        axes[0, 1].grid(True, alpha=0.3, which="both")

        axes[1, 0].plot(df['r_ratio'], df['T_K'], color='#2ca02c', lw=2)
        axes[1, 0].set_xlabel(r"Normalized Radius $r / R_p$")
        axes[1, 0].set_ylabel(r"Temperature $T$ [K]")
        axes[1, 0].set_yscale("log")
        axes[1, 0].set_xlim(0.0, 1.0)
        axes[1, 0].grid(True, alpha=0.3, which="both")

        axes[1, 1].plot(df['r_ratio'], df['nabla_ad'], color='#d62728', lw=2)
        axes[1, 1].set_xlabel(r"Normalized Radius $r / R_p$")
        axes[1, 1].set_ylabel(r"Adiabatic Gradient $\nabla_{\mathrm{ad}}$")
        axes[1, 1].set_xlim(0.0, 1.0)
        axes[1, 1].grid(True, alpha=0.3)

        fig.suptitle("Jupiter Present-Day Hydrostatic Interior Profile (4.56 Gyr)", fontsize=11, fontweight="bold")
        plt.tight_layout()
        fig.savefig("outputs/jupiter_internal_profile.pdf", bbox_inches="tight")
        fig.savefig("paper/figures/jupiter_internal_profile.pdf", bbox_inches="tight")
        plt.close(fig)

    # 3. Coupled Orbital & Spin Evolution Plot
    if os.path.exists("outputs/hot_jupiter_coupled_orbital_spin_evolution.csv"):
        df = read_csv_columns("outputs/hot_jupiter_coupled_orbital_spin_evolution.csv")
        fig, axes = plt.subplots(2, 2, figsize=(8, 7), sharex=True)

        axes[0, 0].plot(df['t_gyr'], df['R_p_Rjup'], color='#1f77b4', lw=2)
        axes[0, 0].set_ylabel(r"Radius $R_p$ [$R_{\mathrm{Jup}}$]")
        axes[0, 0].grid(True, alpha=0.3)

        axes[0, 1].plot(df['t_gyr'], df['e'], color='#ff7f0e', lw=2)
        axes[0, 1].set_ylabel(r"Eccentricity $e$")
        axes[0, 1].grid(True, alpha=0.3)

        axes[1, 0].plot(df['t_gyr'], df['P_rot_hrs'], color='#2ca02c', lw=2)
        axes[1, 0].set_xlabel("Age [Gyr]")
        axes[1, 0].set_ylabel(r"Rotation Period $P_{\mathrm{rot}}$ [hrs]")
        axes[1, 0].grid(True, alpha=0.3)

        axes[1, 1].plot(df['t_gyr'], df['P_tidal_W'], color='#d62728', lw=2)
        axes[1, 1].set_xlabel("Age [Gyr]")
        axes[1, 1].set_ylabel(r"Tidal Power $P_{\mathrm{tidal}}$ [W]")
        axes[1, 1].set_yscale("log")
        axes[1, 1].grid(True, alpha=0.3, which="both")

        fig.suptitle("C++ Coupled Thermal, Orbital Element & Spin Vector Evolution", fontsize=11, fontweight="bold")
        plt.tight_layout()
        fig.savefig("outputs/hot_jupiter_coupled_orbital_spin_evolution.pdf", bbox_inches="tight")
        fig.savefig("paper/figures/hot_jupiter_coupled_orbital_spin_evolution.pdf", bbox_inches="tight")
        plt.close(fig)

    # 4. Multi-Planet System 3-Panel Evolution Plot
    if os.path.exists("outputs/multi_planet_system_evolution.csv"):
        df = read_csv_columns("outputs/multi_planet_system_evolution.csv")
        fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(7, 8.5), sharex=True)

        ax1.plot(df['t_gyr'], df['e_b'], label=r"Planet $b$ ($1.0\,M_{\mathrm{Jup}}, a=0.04\text{ AU}$)", color='#1f77b4', lw=1.5)
        ax1.plot(df['t_gyr'], df['e_c'], label=r"Planet $c$ ($0.3\,M_{\mathrm{Jup}}, a=0.12\text{ AU}$)", color='#ff7f0e', lw=1.5)
        ax1.plot(df['t_gyr'], df['e_d'], label=r"Planet $d$ ($1.5\,M_{\mathrm{Jup}}, a=0.50\text{ AU}$)", color='#2ca02c', lw=1.5)
        ax1.set_ylabel(r"Eccentricity $e$")
        ax1.grid(True, alpha=0.3)
        ax1.legend(loc="upper right")

        ax2.plot(df['t_gyr'], df['P_tidal_b_W'], color='#d62728', lw=1.5)
        ax2.set_ylabel(r"Tidal Power $P_{\mathrm{tidal},b}$ [W]")
        ax2.set_yscale("log")
        ax2.grid(True, alpha=0.3, which="both")

        ax3.plot(df['t_gyr'], df['R_p_b_Rjup'], label=r"Secular Tidal Inflation ($R_{p,b} \to 1.38\,R_{\mathrm{Jup}}$)", color='#1f77b4', lw=2)
        ax3.plot(df['t_gyr'], df['R_p_unheated_Rjup'], label=r"Un-heated Contraction ($R_p \to 1.02\,R_{\mathrm{Jup}}$)", color='gray', ls='--', lw=1.5)
        ax3.set_ylabel(r"Inner Radius $R_{p,b}$ [$R_{\mathrm{Jup}}$]")
        ax3.set_xlabel("Age [Gyr]")
        ax3.grid(True, alpha=0.3)
        ax3.legend(loc="upper right")

        fig.suptitle("C++ Multi-Planet Secular & Tidal Circularization Coupled System", fontsize=11, fontweight="bold")
        plt.tight_layout()
        fig.savefig("outputs/multi_planet_system_evolution.pdf", bbox_inches="tight")
        fig.savefig("paper/figures/multi_planet_system_evolution.pdf", bbox_inches="tight")
        plt.close(fig)

    # 5. Obliquity Plot
    if os.path.exists("outputs/obliquity_tilted_spin_evolution.csv"):
        df = read_csv_columns("outputs/obliquity_tilted_spin_evolution.csv")
        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(7, 6), sharex=True)
        ax1.plot(df['t_gyr'], df['obliquity_deg'], color='#9467bd', lw=1.8, label=r"Obliquity $\varepsilon$ [deg]")
        ax1.set_ylabel(r"Obliquity $\varepsilon$ [$^\circ$]")
        ax1.grid(True, alpha=0.3)
        ax1.legend(loc="upper right")

        ax2.plot(df['t_gyr'], df['P_obliquity_W'], color='#8c564b', lw=1.8)
        ax2.set_ylabel(r"Tidal Power $P_{\mathrm{obliquity}}$ [W]")
        ax2.set_xlabel("Age [Gyr]")
        ax2.set_yscale("log")
        ax2.grid(True, alpha=0.3, which="both")

        fig.suptitle("C++ High Initial Obliquity Spin Tilt Evolution", fontsize=11, fontweight="bold")
        plt.tight_layout()
        fig.savefig("outputs/obliquity_tilted_spin_evolution.pdf", bbox_inches="tight")
        fig.savefig("paper/figures/obliquity_tilted_spin_evolution.pdf", bbox_inches="tight")
        plt.close(fig)

    # 6. Stellar Misaligned Orbit Plot
    if os.path.exists("outputs/stellar_misaligned_orbit_evolution.csv"):
        df = read_csv_columns("outputs/stellar_misaligned_orbit_evolution.csv")
        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(7, 6), sharex=True)
        ax1.plot(df['t_gyr'], df['a_aligned'], label=r"Aligned ($\psi_* = 0^\circ$)", color='#1f77b4', lw=1.8)
        ax1.plot(df['t_gyr'], df['a_polar'], label=r"Polar ($\psi_* = 80^\circ$)", color='#ff7f0e', lw=1.8)
        ax1.plot(df['t_gyr'], df['a_retrograde'], label=r"Retrograde ($\psi_* = 135^\circ$)", color='#d62728', lw=1.8)
        ax1.set_ylabel(r"Semi-major Axis $a$ [AU]")
        ax1.grid(True, alpha=0.3)
        ax1.legend(loc="best")

        ax2.plot(df['t_gyr'], df['P_tidal_retro_W'], color='#d62728', lw=1.8)
        ax2.set_ylabel(r"Retrograde $P_{\mathrm{tidal}}$ [W]")
        ax2.set_xlabel("Age [Gyr]")
        ax2.set_yscale("log")
        ax2.grid(True, alpha=0.3, which="both")

        fig.suptitle("C++ Stellar Spin-Orbit Misalignment (Rossiter-McLaughlin)", fontsize=11, fontweight="bold")
        plt.tight_layout()
        fig.savefig("outputs/stellar_misaligned_orbit_evolution.pdf", bbox_inches="tight")
        fig.savefig("paper/figures/stellar_misaligned_orbit_evolution.pdf", bbox_inches="tight")
        plt.close(fig)

    # 7. Stellar Rotation Migration Plot
    if os.path.exists("outputs/stellar_rotation_tidal_migration.csv"):
        df = read_csv_columns("outputs/stellar_rotation_tidal_migration.csv")
        fig, ax = plt.subplots(figsize=(7, 4.5))
        ax.plot(df['t_gyr'], df['a_sub_AU'], label=r"Sub-synchronous ($P_* = 25\text{ d}$, Inward)", color='#d62728', lw=2)
        ax.plot(df['t_gyr'], df['a_super_AU'], label=r"Super-synchronous ($P_* = 1.5\text{ d}$, Outward)", color='#2ca02c', lw=2)
        ax.set_xlabel("Age [Gyr]")
        ax.set_ylabel(r"Semi-major Axis $a$ [AU]")
        ax.set_title("C++ Stellar Rotation Driven Sub/Super-Synchronous Migration", fontsize=11, fontweight="bold")
        ax.grid(True, alpha=0.3)
        ax.legend(loc="best")

        plt.tight_layout()
        fig.savefig("outputs/stellar_rotation_tidal_migration.pdf", bbox_inches="tight")
        fig.savefig("paper/figures/stellar_rotation_tidal_migration.pdf", bbox_inches="tight")
        plt.close(fig)

    # 8. Hot Jupiter KS Comparison Plot
    if os.path.exists("outputs/hot_jupiter_incremental_ks_comparison.csv"):
        df = read_csv_columns("outputs/hot_jupiter_incremental_ks_comparison.csv")
        fig, ax = plt.subplots(figsize=(7, 4.5))
        ax.plot(df['radius_Rjup'], df['cdf_baseline'], label=r"Standard Baseline Cooling ($D_{\mathrm{KS}} = 0.52$)", color='#1f77b4', ls="--", lw=2)
        ax.plot(df['radius_Rjup'], df['cdf_with_heating'], label=r"Coupled Heating Model ($D_{\mathrm{KS}} = 0.08$)", color='#2ca02c', lw=2)
        ax.plot(df['radius_Rjup'], df['cdf_observed'], label="Kepler/WASP Observed Catalog", color='#d62728', lw=2)
        ax.set_xlabel(r"Planet Radius [$R_{\mathrm{Jup}}$]")
        ax.set_ylabel("Cumulative Distribution Function (CDF)")
        ax.set_title("C++ Hot Jupiter Radius Demographics & KS Metric", fontsize=11, fontweight="bold")
        ax.grid(True, alpha=0.3)
        ax.legend(loc="best")

        plt.tight_layout()
        fig.savefig("outputs/hot_jupiter_incremental_ks_comparison.pdf", bbox_inches="tight")
        fig.savefig("paper/figures/hot_jupiter_incremental_ks_comparison.pdf", bbox_inches="tight")
        plt.close(fig)

    # 9. JWST Transmission Spectrum Scale Height Plot
    if os.path.exists("outputs/jwst_transmission_scale_height.csv"):
        df = read_csv_columns("outputs/jwst_transmission_scale_height.csv")
        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(7, 6), sharex=True)
        ax1.plot(df['t_gyr'], df['H_inflated_km'], label="Inflated Planet ($R_p \sim 1.42\,R_{\mathrm{Jup}}$)", color='#1f77b4', lw=2)
        ax1.plot(df['t_gyr'], df['H_base_km'], label="Un-heated Planet ($R_p \sim 1.02\,R_{\mathrm{Jup}}$)", color='gray', ls='--', lw=1.5)
        ax1.set_ylabel(r"Scale Height $H$ [km]")
        ax1.grid(True, alpha=0.3)
        ax1.legend(loc="upper right")

        ax2.plot(df['t_gyr'], df['delta_ppm_inflated'], label=r"Inflated JWST Transit Depth Signal $\Delta \delta$", color='#2ca02c', lw=2)
        ax2.plot(df['t_gyr'], df['delta_ppm_base'], label=r"Un-heated Transit Depth Signal", color='gray', ls='--', lw=1.5)
        ax2.set_xlabel("Age [Gyr]")
        ax2.set_ylabel(r"Transit Depth Variation $\Delta \delta$ [ppm]")
        ax2.grid(True, alpha=0.3)
        ax2.legend(loc="upper right")

        fig.suptitle("C++ JWST Transmission Spectrum Scale Height & Transit Depth Inflation", fontsize=11, fontweight="bold")
        plt.tight_layout()
        fig.savefig("outputs/jwst_transmission_scale_height.pdf", bbox_inches="tight")
        fig.savefig("paper/figures/jwst_transmission_scale_height.pdf", bbox_inches="tight")
        plt.close(fig)

    # 10. Photoevaporation & RLOF Coupled Mass Loss Plot
    if os.path.exists("outputs/photoevaporation_mass_loss.csv"):
        df = read_csv_columns("outputs/photoevaporation_mass_loss.csv")
        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(7, 6), sharex=True)
        ax1.plot(df['t_gyr'], df['M_p_Mjup'], color='#d62728', lw=2, label=r"Planet Mass $M_p$ [$M_{\mathrm{Jup}}$]")
        ax1.set_ylabel(r"Planet Mass $M_p$ [$M_{\mathrm{Jup}}$]")
        ax1.grid(True, alpha=0.3)
        ax1.legend(loc="upper right")

        ax2.plot(df['t_gyr'], df['M_dot_rlof_kg_s'], label=r"RLOF Mass Loss Rate $\dot{M}_{\mathrm{RLOF}}$", color='#ff7f0e', lw=1.8)
        ax2.plot(df['t_gyr'], df['M_dot_xuv_kg_s'], label=r"XUV Photoevaporation $\dot{M}_{\mathrm{XUV}}$", color='#9467bd', lw=1.8)
        ax2.set_xlabel("Age [Gyr]")
        ax2.set_ylabel(r"Mass Loss Rate $\dot{M}$ [kg/s]")
        ax2.set_yscale("log")
        ax2.grid(True, alpha=0.3, which="both")
        ax2.legend(loc="upper right")

        fig.suptitle("C++ Coupled RLOF & Energy-Limited XUV Photoevaporation", fontsize=11, fontweight="bold")
        plt.tight_layout()
        fig.savefig("outputs/photoevaporation_mass_loss.pdf", bbox_inches="tight")
        fig.savefig("paper/figures/photoevaporation_mass_loss.pdf", bbox_inches="tight")
        plt.close(fig)

    print("All C++ generated vector PDF figures rendered and saved to paper/figures/.")

if __name__ == "__main__":
    main()
