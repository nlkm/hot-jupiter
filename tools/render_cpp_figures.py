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
                try:
                    data[k].append(float(v))
                except ValueError:
                    data[k].append(v)
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

    # 2. Single Planet Orbital Spin Evolution Plot
    if os.path.exists("outputs/hot_jupiter_coupled_orbital_spin_evolution.csv"):
        df = read_csv_columns("outputs/hot_jupiter_coupled_orbital_spin_evolution.csv")
        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(7, 6), sharex=True)
        ax1.plot(df['t_gyr'], df['a_AU'], label=r"Semi-Major Axis $a$ [AU]", color='#1f77b4', lw=2)
        ax1.set_ylabel(r"Semi-Major Axis $a$ [AU]")
        ax1.grid(True, alpha=0.3)
        ax1.legend(loc="upper right")

        ax2.plot(df['t_gyr'], df['e'], label=r"Eccentricity $e$", color='#ff7f0e', lw=2)
        ax2.set_xlabel("Age [Gyr]")
        ax2.set_ylabel(r"Eccentricity $e$")
        ax2.grid(True, alpha=0.3)
        ax2.legend(loc="upper right")

        fig.suptitle("C++ Coupled Single-Planet Orbital & Spin Dynamics", fontsize=11, fontweight="bold")
        plt.tight_layout()
        fig.savefig("outputs/hot_jupiter_coupled_orbital_spin_evolution.pdf", bbox_inches="tight")
        fig.savefig("paper/figures/hot_jupiter_coupled_orbital_spin_evolution.pdf", bbox_inches="tight")
        plt.close(fig)

    # 3. Multi-Planet System Benchmark Plot
    if os.path.exists("outputs/multi_planet_system_evolution.csv"):
        df = read_csv_columns("outputs/multi_planet_system_evolution.csv")
        fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(7, 7), sharex=True)
        ax1.plot(df['t_gyr'], df['e_b'], label=r"Planet b $e_b(t)$", color='#1f77b4', lw=2)
        ax1.plot(df['t_gyr'], df['e_c'], label=r"Planet c $e_c(t)$", color='#ff7f0e', lw=1.5, ls='--')
        ax1.plot(df['t_gyr'], df['e_d'], label=r"Planet d $e_d(t)$", color='#2ca02c', lw=1.5, ls=':')
        ax1.set_ylabel(r"Eccentricity $e$")
        ax1.grid(True, alpha=0.3)
        ax1.legend(loc="upper right")

        ax2.plot(df['t_gyr'], df['P_tidal_b_W'], label=r"Tidal Power $P_{\mathrm{tidal},b}$ [W]", color='#d62728', lw=2)
        ax2.set_ylabel(r"Tidal Power [W]")
        ax2.set_yscale("log")
        ax2.grid(True, alpha=0.3, which="both")
        ax2.legend(loc="upper right")

        ax3.plot(df['t_gyr'], df['R_p_b_Rjup'], label=r"Inner Planet Radius $R_{p,b}(t)$", color='#9467bd', lw=2)
        ax3.axhline(1.02, color='gray', ls='--', lw=1.2, label='Un-heated Baseline (1.02 R_Jup)')
        ax3.set_xlabel("Age [Gyr]")
        ax3.set_ylabel(r"Radius $R_p$ [$R_{\mathrm{Jup}}$]")
        ax3.grid(True, alpha=0.3)
        ax3.legend(loc="lower right")

        fig.suptitle("C++ Multi-Planet Laplace-Lagrange Secular Perturbations & Inflation", fontsize=11, fontweight="bold")
        plt.tight_layout()
        fig.savefig("outputs/multi_planet_system_evolution.pdf", bbox_inches="tight")
        fig.savefig("paper/figures/multi_planet_system_evolution.pdf", bbox_inches="tight")
        plt.close(fig)

    # 4. Obliquity Tilted Spin Scenario Plot
    if os.path.exists("outputs/obliquity_tilted_spin_evolution.csv"):
        df = read_csv_columns("outputs/obliquity_tilted_spin_evolution.csv")
        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(7, 6), sharex=True)
        ax1.plot(df['t_gyr'], df['obliquity_deg'], label=r"Obliquity $\varepsilon$ [deg]", color='#9467bd', lw=2)
        ax1.set_ylabel(r"Obliquity [deg]")
        ax1.grid(True, alpha=0.3)
        ax1.legend(loc="upper right")

        ax2.plot(df['t_gyr'], df['P_obliquity_W'], label=r"Obliquity Tidal Power $P_{\mathrm{obl}}$ [W]", color='#d62728', lw=2)
        ax2.set_xlabel("Age [Gyr]")
        ax2.set_ylabel(r"Power [W]")
        ax2.set_yscale("log")
        ax2.grid(True, alpha=0.3, which="both")
        ax2.legend(loc="upper right")

        fig.suptitle("C++ Obliquity Tilted Spin Tidal Dissipation", fontsize=11, fontweight="bold")
        plt.tight_layout()
        fig.savefig("outputs/obliquity_tilted_spin_evolution.pdf", bbox_inches="tight")
        fig.savefig("paper/figures/obliquity_tilted_spin_evolution.pdf", bbox_inches="tight")
        plt.close(fig)

    # 5. Stellar Misaligned Orbit Scenario Plot
    if os.path.exists("outputs/stellar_misaligned_orbit_evolution.csv"):
        df = read_csv_columns("outputs/stellar_misaligned_orbit_evolution.csv")
        fig, ax = plt.subplots(figsize=(7, 5))
        ax.plot(df['t_gyr'], df['a_aligned'], label=r"Aligned Orbit ($\psi_* = 0^\circ$)", color='#1f77b4', lw=2)
        ax.plot(df['t_gyr'], df['a_polar'], label=r"Polar Orbit ($\psi_* = 80^\circ$)", color='#ff7f0e', lw=2, ls='--')
        ax.plot(df['t_gyr'], df['a_retrograde'], label=r"Retrograde Orbit ($\psi_* = 135^\circ$)", color='#d62728', lw=2, ls=':')
        ax.set_xlabel("Age [Gyr]")
        ax.set_ylabel(r"Semi-Major Axis $a$ [AU]")
        ax.grid(True, alpha=0.3)
        ax.legend(loc="upper right")

        fig.suptitle("C++ Stellar Spin-Orbit Misalignment Dynamics", fontsize=11, fontweight="bold")
        plt.tight_layout()
        fig.savefig("outputs/stellar_misaligned_orbit_evolution.pdf", bbox_inches="tight")
        fig.savefig("paper/figures/stellar_misaligned_orbit_evolution.pdf", bbox_inches="tight")
        plt.close(fig)

    # 6. Stellar Rotation Tides Scenario Plot
    if os.path.exists("outputs/stellar_rotation_tidal_migration.csv"):
        df = read_csv_columns("outputs/stellar_rotation_tidal_migration.csv")
        fig, ax = plt.subplots(figsize=(7, 5))
        ax.plot(df['t_gyr'], df['a_sub_AU'], label=r"Sub-synchronous Star ($P_* = 25\mathrm{d}$, Inward Decay)", color='#d62728', lw=2)
        ax.plot(df['t_gyr'], df['a_super_AU'], label=r"Super-synchronous Star ($P_* = 1.5\mathrm{d}$, Outward Expansion)", color='#2ca02c', lw=2, ls='--')
        ax.set_xlabel("Age [Gyr]")
        ax.set_ylabel(r"Semi-Major Axis $a$ [AU]")
        ax.grid(True, alpha=0.3)
        ax.legend(loc="center right")

        fig.suptitle("C++ Stellar Rotation Driven Tidal Orbital Migration", fontsize=11, fontweight="bold")
        plt.tight_layout()
        fig.savefig("outputs/stellar_rotation_tidal_migration.pdf", bbox_inches="tight")
        fig.savefig("paper/figures/stellar_rotation_tidal_migration.pdf", bbox_inches="tight")
        plt.close(fig)

    # 7. Roche Lobe Overflow Mass Loss Plot
    if os.path.exists("outputs/eccentric_rlof_evolution.csv"):
        df = read_csv_columns("outputs/eccentric_rlof_evolution.csv")
        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(7, 6), sharex=True)
        ax1.plot(df['t_gyr'], df['M_p_Mjup'], label=r"Planet Mass $M_p$ [$M_{\mathrm{Jup}}$]", color='#1f77b4', lw=2)
        ax1.set_ylabel(r"Planet Mass [$M_{\mathrm{Jup}}$]")
        ax1.grid(True, alpha=0.3)
        ax1.legend(loc="upper right")

        ax2.plot(df['t_gyr'], df['fill_peri'], label=r"Periastron Filling Factor $R_p / R_{\mathrm{Roche,peri}}$", color='#ff7f0e', lw=2)
        ax2.axhline(1.0, color='gray', ls='--', lw=1.2)
        ax2.set_xlabel("Age [Gyr]")
        ax2.set_ylabel(r"Filling Factor")
        ax2.grid(True, alpha=0.3)
        ax2.legend(loc="upper right")

        fig.suptitle("C++ Eccentric Periastron Roche Lobe Overflow Mass Loss", fontsize=11, fontweight="bold")
        plt.tight_layout()
        fig.savefig("outputs/roche_lobe_overflow_mass_loss.pdf", bbox_inches="tight")
        fig.savefig("paper/figures/roche_lobe_overflow_mass_loss.pdf", bbox_inches="tight")
        plt.close(fig)

    # 8. Hot Jupiter Demographic Population Comparison Plot
    if os.path.exists("outputs/hot_jupiter_incremental_ks_comparison.csv"):
        df = read_csv_columns("outputs/hot_jupiter_incremental_ks_comparison.csv")
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(9, 4.5))

        ax1.hist(df['radius_Rjup'], weights=df['cdf_baseline'], bins=20, alpha=0.5, color='gray', label='Stage 0: Baseline')
        ax1.hist(df['radius_Rjup'], weights=df['cdf_with_heating'], bins=20, alpha=0.6, color='#1f77b4', label='Stage 5: Full Model')
        ax1.hist(df['radius_Rjup'], weights=df['cdf_observed'], bins=20, histtype='step', lw=2, color='#d62728', label='Observed Catalog')
        ax1.set_xlabel(r"Radius $R_p$ [$R_{\mathrm{Jup}}$]")
        ax1.set_ylabel("Normalized Count")
        ax1.grid(True, alpha=0.3)
        ax1.legend(loc="upper right", fontsize=8)

        ax2.plot(df['radius_Rjup'], df['cdf_baseline'], color='gray', ls='--', lw=1.5, label='Stage 0: Baseline')
        ax2.plot(df['radius_Rjup'], df['cdf_with_heating'], color='#1f77b4', lw=2, label=r'Stage 5: Full Model ($D_{\mathrm{KS}}=0.080$)')
        ax2.plot(df['radius_Rjup'], df['cdf_observed'], color='#d62728', lw=2, label=r'Observed Catalog ($N=342$)')
        ax2.set_xlabel(r"Radius $R_p$ [$R_{\mathrm{Jup}}$]")
        ax2.set_ylabel("Cumulative Distribution (CDF)")
        ax2.grid(True, alpha=0.3)
        ax2.legend(loc="lower right", fontsize=8)

        fig.suptitle("Hot Jupiter Population Synthesis Demographic Comparison ($N = 10,000$)", fontsize=11, fontweight="bold")
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

    # 11. Dual-Panel Core Mass & Mass Ratio vs Metallicity Correlation Plot
    if os.path.exists("outputs/estimated_core_masses_342_planets.csv"):
        df = read_csv_columns("outputs/estimated_core_masses_342_planets.csv")
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(11, 4.8))
        
        mc_earth = df['M_c_est_Mearth']
        mp_jup = df['M_p_Mjup']
        ratio_Zp = [mc / (mp * 317.8) for mc, mp in zip(mc_earth, mp_jup)]
        
        # Grid for smooth theoretical curves
        min_feh, max_feh = min(df['Fe_H']), max(df['Fe_H'])
        feh_grid = [min_feh + i * (max_feh - min_feh) / 200.0 for i in range(201)]
        
        # Left Panel: Heavy-Element Core Mass M_c [M_earth]
        ax1.scatter(df['Fe_H'], mc_earth, alpha=0.6, color='#1f77b4', edgecolors='none', s=30, label=r'Inverted $M_c$ ($N=342$)')
        mc_thorngren_grid = [15.0 * (10.0 ** (0.50 * x)) for x in feh_grid]
        ax1.plot(feh_grid, mc_thorngren_grid, color='#d62728', ls='--', lw=2, label=r'Thorngren (2016): $M_c \propto 10^{0.50 [\mathrm{Fe/H}]}$')
        ax1.set_xlabel(r"Host Star Metallicity $[\mathrm{Fe/H}]$ [dex]")
        ax1.set_ylabel(r"Heavy-Element Core Mass $M_c$ [$M_\oplus$]")
        ax1.set_yscale("log")
        ax1.grid(True, alpha=0.3, which="both")
        ax1.legend(loc="upper left", fontsize=8.5)
        ax1.set_title("(a) Heavy-Element Mass $M_c$ [$M_\oplus$]", fontsize=10)

        # Right Panel: Heavy-Element Mass Ratio Z_p = M_c / M_p
        ax2.scatter(df['Fe_H'], ratio_Zp, alpha=0.6, color='#2ca02c', edgecolors='none', s=30, label=r'Inverted Ratio $Z_p = M_c / M_p$ ($N=342$)')
        ratio_thorngren_grid = [(15.0 / 317.8) * (10.0 ** (0.50 * x)) for x in feh_grid]
        ax2.plot(feh_grid, ratio_thorngren_grid, color='#d62728', ls='--', lw=2, label=r'Thorngren (2016): $Z_p \propto 10^{0.50 [\mathrm{Fe/H}]}$')
        ax2.set_xlabel(r"Host Star Metallicity $[\mathrm{Fe/H}]$ [dex]")
        ax2.set_ylabel(r"Heavy-Element Core Mass Ratio $Z_p = M_c / M_p$")
        ax2.set_yscale("log")
        ax2.grid(True, alpha=0.3, which="both")
        ax2.legend(loc="upper left", fontsize=8.5)
        ax2.set_title(r"(b) Heavy-Element Ratio $Z_p = M_c / M_p$", fontsize=10)
        
        fig.suptitle("Inverted Heavy-Element Core Mass and Mass Ratio vs Metallicity ($N = 342$)", fontsize=11, fontweight="bold")
        plt.tight_layout()
        fig.savefig("outputs/core_mass_metallicity_correlation.pdf", bbox_inches="tight")
        fig.savefig("paper/figures/core_mass_metallicity_correlation.pdf", bbox_inches="tight")
        plt.close(fig)

    print("All C++ generated vector PDF figures rendered and saved to paper/figures/.")

if __name__ == "__main__":
    main()
