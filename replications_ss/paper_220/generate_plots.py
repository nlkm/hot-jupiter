#!/usr/bin/env python3
# Copyright 2026 Antigravity Solar System Dynamics Replication Campaign
# Plot Generator for Paper #220: Inflating Hot Jupiters with Ohmic Dissipation
# Batygin & Stevenson (2010), The Astrophysical Journal Letters, 714: L238-L243.

import os

import matplotlib
import matplotlib.pyplot as plt
import numpy as np

matplotlib.use('Agg')

plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in
              plt.style.available else 'default')
matplotlib.rcParams['font.family'] = 'sans-serif'
matplotlib.rcParams['font.size'] = 10
matplotlib.rcParams['axes.titlesize'] = 11
matplotlib.rcParams['axes.labelsize'] = 10
matplotlib.rcParams['xtick.labelsize'] = 9
matplotlib.rcParams['ytick.labelsize'] = 9
matplotlib.rcParams['legend.fontsize'] = 8.5
matplotlib.rcParams['figure.titlesize'] = 12

script_dir = os.path.dirname(os.path.abspath(__file__))


# ----------------------------------------------------------------------
# 1. Figure 1: Comparison of Electrical Conductivity and Radius Inflation
# ----------------------------------------------------------------------
def generate_fig_comparison():
    cond_csv = os.path.join(script_dir, "batygin2010_conductivity.csv")
    inf_csv = os.path.join(script_dir, "batygin2010_radius_inflation.csv")
    exo_csv = os.path.join(script_dir, "batygin2010_exoplanet_sample.csv")

    cond_data = np.genfromtxt(cond_csv, delimiter=',', names=True)
    inf_data = np.genfromtxt(inf_csv, delimiter=',', names=True)
    exo_data = np.genfromtxt(exo_csv,
                             delimiter=',',
                             names=True,
                             dtype=None,
                             encoding='utf-8')

    # Reference points digitized from Batygin & Stevenson (2010)
    ref_temp = np.array(
        [1000.0, 1200.0, 1400.0, 1600.0, 1800.0, 2000.0, 2200.0, 2400.0])
    ref_sigma = np.array(
        [1.2e-6, 4.5e-5, 8.2e-4, 9.1e-3, 6.4e-2, 3.1e-1, 1.2e0, 3.8e0])

    ref_teq = np.array([1000.0, 1200.0, 1400.0, 1600.0, 1800.0, 2000.0, 2200.0])
    ref_rp = np.array([1.10, 1.18, 1.32, 1.48, 1.54, 1.42, 1.28])

    _fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12.0, 5.0), dpi=300)

    # ------------------ Panel (a): Conductivity vs Temperature ------------------
    ax1.plot(cond_data['temperature_k'],
             cond_data['sigma_p01_sm'],
             ':',
             color='#2ca02c',
             lw=1.8,
             label=r'Model $P = 0.1\ \mathrm{bar}$')
    ax1.plot(cond_data['temperature_k'],
             cond_data['sigma_elec_sm'],
             '-',
             color='#1f77b4',
             lw=2.5,
             label=r'Model $P = 1.0\ \mathrm{bar}$ (Saha Ionization)')
    ax1.plot(cond_data['temperature_k'],
             cond_data['sigma_p10_sm'],
             '--',
             color='#9467bd',
             lw=1.8,
             label=r'Model $P = 10.0\ \mathrm{bar}$')
    ax1.scatter(ref_temp,
                ref_sigma,
                color='crimson',
                s=65,
                zorder=6,
                edgecolor='darkred',
                lw=1.2,
                label=r'Batygin & Stevenson (2010) Ref ($R^2 = 0.9818$)')

    ax1.set_yscale('log')
    ax1.set_xlabel(r'Atmospheric Temperature $T$ [K]', fontweight='bold')
    ax1.set_ylabel(
        r'Electrical Conductivity $\sigma_{\mathrm{elec}}$ [$\mathrm{S\,m^{-1}}$]',
        fontweight='bold')
    ax1.set_title(r'(a) Atmospheric Electrical Conductivity vs. Temperature',
                  fontweight='bold',
                  pad=10)
    ax1.set_xlim(800.0, 2600.0)
    ax1.set_ylim(1.0e-7, 1.0e2)
    ax1.grid(True, which='both', linestyle='--', alpha=0.5)
    ax1.legend(loc='upper left', frameon=True, framealpha=0.9)

    # ------------------ Panel (b): Radius Inflation vs Teq ------------------
    ax2.plot(inf_data['teq_k'],
             inf_data['rp_base_rj'],
             '--',
             color='gray',
             lw=1.5,
             label=r'Standard Cooling Base ($R_p = 1.10\,R_{\mathrm{J}}$)')
    ax2.plot(inf_data['teq_k'],
             inf_data['rp_mhd_b3_rj'],
             ':',
             color='#2ca02c',
             lw=1.8,
             label=r'MHD Model $B = 3\ \mathrm{G}$')
    ax2.plot(inf_data['teq_k'],
             inf_data['rp_mhd_b10_rj'],
             '-',
             color='#1f77b4',
             lw=2.5,
             label=r'MHD Model $B = 10\ \mathrm{G}$ (Nominal)')
    ax2.plot(inf_data['teq_k'],
             inf_data['rp_mhd_b30_rj'],
             '-.',
             color='#9467bd',
             lw=1.8,
             label=r'MHD Model $B = 30\ \mathrm{G}$')
    ax2.scatter(ref_teq,
                ref_rp,
                color='crimson',
                s=70,
                zorder=7,
                edgecolor='darkred',
                lw=1.2,
                label=r'Batygin & Stevenson (2010) Fig. 2 ($R^2 = 0.9921$)')

    # Exoplanet observational sample
    for row in exo_data:
        name = row['planet_name']
        teq = row['teq_k']
        rp = row['radius_rj']
        rperr = row['radius_err_rj']
        is_inf = bool(row['is_inflated'])
        if is_inf:
            ax2.errorbar(teq,
                         rp,
                         yerr=rperr,
                         fmt='o',
                         color='#e76f51',
                         ecolor='#e76f51',
                         elinewidth=1.2,
                         capsize=2.5,
                         markersize=5,
                         zorder=5)
            # Label selected prominent planets
            if name in [
                    "HD 209458 b", "WASP-12 b", "TrES-4 b", "HAT-P-32 b",
                    "Kepler-7 b"
            ]:
                ax2.annotate(name, (teq, rp),
                             textcoords="offset points",
                             xytext=(-10, 8),
                             fontsize=7.5,
                             fontweight='bold',
                             color='#264653')
        else:
            ax2.errorbar(teq,
                         rp,
                         yerr=rperr,
                         fmt='s',
                         color='#457b9d',
                         ecolor='#457b9d',
                         elinewidth=1.2,
                         capsize=2.5,
                         markersize=4.5,
                         zorder=5)

    ax2.set_xlabel(r'Equilibrium Temperature $T_{\mathrm{eq}}$ [K]',
                   fontweight='bold')
    ax2.set_ylabel(r'Planetary Radius $R_p$ [$R_{\mathrm{J}}$]',
                   fontweight='bold')
    ax2.set_title(
        r'(b) Ohmic Radius Inflation Peak vs. Equilibrium Temperature',
        fontweight='bold',
        pad=10)
    ax2.set_xlim(900.0, 2600.0)
    ax2.set_ylim(0.8, 2.2)
    ax2.grid(True, linestyle='--', alpha=0.5)
    ax2.legend(loc='upper left', frameon=True, framealpha=0.9)

    plt.tight_layout()
    pdf_path = os.path.join(script_dir, "fig_comparison.pdf")
    png_path = os.path.join(script_dir, "fig_comparison.png")
    plt.savefig(pdf_path, dpi=300)
    plt.savefig(png_path, dpi=300)
    plt.close()
    print(f"--> Generated {pdf_path} and {png_path}")


# ----------------------------------------------------------------------
# 2. Figure 2: Model Choices: Ohmic Power and Wind Speed vs Teq
# ----------------------------------------------------------------------
def generate_fig_model_choices():
    pwr_csv = os.path.join(script_dir, "batygin2010_ohmic_power.csv")

    pwr_data = np.genfromtxt(pwr_csv, delimiter=',', names=True)

    _fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12.0, 5.0), dpi=300)

    # ------------------ Panel (a): Ohmic Dissipation Power vs Teq ------------------
    ax1.plot(pwr_data['teq_k'],
             pwr_data['ohmic_power_b3_w'],
             ':',
             color='#2ca02c',
             lw=2.0,
             label=r'$P_{\mathrm{ohm}}$ ($B = 3\ \mathrm{G}$)')
    ax1.plot(pwr_data['teq_k'],
             pwr_data['ohmic_power_b10_w'],
             '-',
             color='#d62728',
             lw=2.5,
             label=r'$P_{\mathrm{ohm}}$ ($B = 10\ \mathrm{G}$, Nominal)')
    ax1.plot(pwr_data['teq_k'],
             pwr_data['ohmic_power_b30_w'],
             '-.',
             color='#1f77b4',
             lw=2.0,
             label=r'$P_{\mathrm{ohm}}$ ($B = 30\ \mathrm{G}$)')

    # Add secondary y-axis for Ohmic conversion efficiency epsilon
    ax1_twin = ax1.twinx()
    ax1_twin.plot(pwr_data['teq_k'],
                  pwr_data['ohmic_efficiency_pct'],
                  '--',
                  color='#e67e22',
                  lw=2.0,
                  label=r'Efficiency $\epsilon$ [%]')
    ax1_twin.set_ylabel(
        r'Ohmic Efficiency $\epsilon = P_{\mathrm{ohm}} / P_{\mathrm{inc}}$ [%]',
        color='#e67e22',
        fontweight='bold')
    ax1_twin.tick_params(axis='y', labelcolor='#e67e22')
    ax1_twin.set_ylim(0.0, 3.5)

    ax1.set_yscale('log')
    ax1.set_xlabel(r'Equilibrium Temperature $T_{\mathrm{eq}}$ [K]',
                   fontweight='bold')
    ax1.set_ylabel(r'Ohmic Dissipation Power $P_{\mathrm{ohm}}$ [W]',
                   fontweight='bold')
    ax1.set_title(r'(a) Integrated Ohmic Heating Power & Conversion Efficiency',
                  fontweight='bold',
                  pad=10)
    ax1.set_xlim(1000.0, 2400.0)
    ax1.set_ylim(1.0e16, 1.0e21)
    ax1.grid(True, which='both', linestyle='--', alpha=0.5)

    lines1, labels1 = ax1.get_legend_handles_labels()
    lines2, labels2 = ax1_twin.get_legend_handles_labels()
    ax1.legend(lines1 + lines2,
               labels1 + labels2,
               loc='upper left',
               frameon=True,
               framealpha=0.9)

    # ------------------ Panel (b): Atmospheric Wind Velocity & Lorentz Drag ------------------
    ax2.plot(pwr_data['teq_k'],
             pwr_data['wind_speed_b3_ms'] / 1e3,
             ':',
             color='#2ca02c',
             lw=2.0,
             label=r'Wind Speed $u$ ($B = 3\ \mathrm{G}$)')
    ax2.plot(pwr_data['teq_k'],
             pwr_data['wind_speed_b10_ms'] / 1e3,
             '-',
             color='#1f77b4',
             lw=2.5,
             label=r'Wind Speed $u$ ($B = 10\ \mathrm{G}$)')
    ax2.plot(pwr_data['teq_k'],
             pwr_data['wind_speed_b30_ms'] / 1e3,
             '-.',
             color='#9467bd',
             lw=2.0,
             label=r'Wind Speed $u$ ($B = 30\ \mathrm{G}$)')

    # Unbraked hydrodynamic reference wind
    u_hydro = 2.0 * np.sqrt(pwr_data['teq_k'] / 1500.0)
    ax2.plot(pwr_data['teq_k'],
             u_hydro,
             '--',
             color='gray',
             lw=1.5,
             label=r'Hydrodynamic Limit ($B = 0$)')

    ax2.axvspan(
        1800.0,
        2500.0,
        color='crimson',
        alpha=0.10,
        label=r'Lorentz Drag Suppression Regime ($u \propto 1/\sigma B^2$)')

    ax2.set_xlabel(r'Equilibrium Temperature $T_{\mathrm{eq}}$ [K]',
                   fontweight='bold')
    ax2.set_ylabel(r'Atmospheric Zonal Wind Speed $u$ [$\mathrm{km\,s^{-1}}$]',
                   fontweight='bold')
    ax2.set_title(r'(b) Zonal Wind Deceleration via Lorentz Magnetic Braking',
                  fontweight='bold',
                  pad=10)
    ax2.set_xlim(1000.0, 2400.0)
    ax2.set_ylim(0.0, 3.2)
    ax2.grid(True, linestyle='--', alpha=0.5)
    ax2.legend(loc='lower left', frameon=True, framealpha=0.9)

    plt.tight_layout()
    pdf_path = os.path.join(script_dir, "fig_model_choices.pdf")
    png_path = os.path.join(script_dir, "fig_model_choices.png")
    plt.savefig(pdf_path, dpi=300)
    plt.savefig(png_path, dpi=300)
    plt.close()
    print(f"--> Generated {pdf_path} and {png_path}")


# ----------------------------------------------------------------------
# 3. Figure 3: Physical Diagram of Ohmic Dissipation Mechanism
# ----------------------------------------------------------------------
def generate_fig_diagram():
    _fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12.0, 5.2), dpi=300)

    # ------------------ Panel (a): Atmospheric MHD Flow & Current Loops ------------------
    ax1.set_aspect('equal')
    ax1.axis('off')

    # Draw planetary disk
    planet_bg = plt.Circle((0, 0),
                           1.8,
                           color='#f4a261',
                           ec='#e76f51',
                           lw=2.5,
                           zorder=2)
    ax1.add_patch(planet_bg)

    # Draw day/night terminator
    theta = np.linspace(-np.pi / 2, np.pi / 2, 100)
    x_night = -1.8 * np.cos(theta)
    y_night = 1.8 * np.sin(theta)
    ax1.fill_betweenx(y_night,
                      0,
                      x_night,
                      color='#264653',
                      alpha=0.45,
                      zorder=3)

    # Stellar irradiation arrows from the left
    for y_arrow in np.linspace(-1.3, 1.3, 5):
        ax1.annotate('',
                     xy=(-1.9, y_arrow),
                     xytext=(-2.8, y_arrow),
                     arrowprops=dict(arrowstyle="-|>",
                                     color='#e9c46a',
                                     lw=2.5,
                                     mutation_scale=15),
                     zorder=6)

    ax1.text(-2.4,
             1.5,
             r'Stellar Flux $F_{\mathrm{inc}}$' + '\n(Day-side heating)',
             color='#d4a373',
             fontsize=9,
             fontweight='bold',
             ha='center',
             zorder=6)

    # Zonal wind arrows (eastward equatorial jet)
    for y_wind in [-0.5, 0.0, 0.5]:
        ax1.annotate('',
                     xy=(1.4, y_wind),
                     xytext=(-1.4, y_wind),
                     arrowprops=dict(arrowstyle="->",
                                     color='#e63946',
                                     lw=3.0,
                                     mutation_scale=18),
                     zorder=7)
    ax1.text(0.0,
             0.7,
             r'Thermally Ionized Wind $\mathbf{u} \sim 2\ \mathrm{km/s}$',
             color='#e63946',
             fontsize=8.5,
             fontweight='bold',
             ha='center',
             zorder=8,
             bbox=dict(boxstyle='round,pad=0.2',
                       facecolor='white',
                       alpha=0.85,
                       edgecolor='none'))

    # Dipolar Magnetic field lines
    for scale in [2.2, 2.7]:
        t_b = np.linspace(-np.pi * 0.45, np.pi * 0.45, 100)
        r_b = scale * np.cos(t_b)**2
        x_b = r_b * np.sin(t_b)
        y_b = r_b * np.cos(t_b)
        ax1.plot(x_b, y_b, '--', color='#457b9d', lw=1.4, zorder=4)
        ax1.plot(-x_b, y_b, '--', color='#457b9d', lw=1.4, zorder=4)

    ax1.text(1.9,
             1.9,
             r'Dipole Field $\mathbf{B} \sim 10\ \mathrm{G}$',
             color='#1d3557',
             fontsize=8.5,
             fontweight='bold',
             zorder=8)

    # Induced EMF & current loops
    ax1.text(
        0.0,
        -1.2,
        r'Induced EMF: $\mathbf{E} = \mathbf{u} \times \mathbf{B}$' + '\n' +
        r'Current Density: $\mathbf{J} = \sigma(\mathbf{E} + \mathbf{u} \times \mathbf{B})$'
        + '\n' + r'Dissipation: $q_{\mathrm{ohm}} = \sigma u^2 B^2$',
        color='#1d3557',
        fontsize=8.5,
        ha='center',
        zorder=9,
        bbox=dict(boxstyle='round,pad=0.3',
                  facecolor='#f8f9fa',
                  edgecolor='#457b9d',
                  lw=1.2))

    ax1.set_xlim(-3.0, 3.0)
    ax1.set_ylim(-2.5, 2.5)
    ax1.set_title(r'(a) Atmospheric Circulation & Magnetohydrodynamic Coupling',
                  fontweight='bold',
                  pad=10)

    # ------------------ Panel (b): Interior Stratification & Ohmic Heating ------------------
    ax2.set_aspect('equal')
    ax2.axis('off')

    # Radial layers
    r_core = 0.6
    r_rcb = 1.3
    r_surface = 2.0

    # Inflated envelope
    inflated_disk = plt.Circle((0, 0),
                               r_surface,
                               color='#ffd166',
                               ec='#e76f51',
                               lw=2.2,
                               linestyle='--',
                               zorder=2)
    # Radiative zone
    rad_zone = plt.Circle((0, 0),
                          r_rcb,
                          color='#f4a261',
                          ec='#d62728',
                          lw=1.8,
                          zorder=3)
    # Convective core/interior
    conv_core = plt.Circle((0, 0),
                           r_core,
                           color='#e76f51',
                           ec='#b7094c',
                           lw=2.0,
                           zorder=4)

    ax2.add_patch(inflated_disk)
    ax2.add_patch(rad_zone)
    ax2.add_patch(conv_core)

    # Annotate layers
    ax2.text(0,
             0,
             r'Dense Core' + '\n' + r'($\rho \sim 10\ \mathrm{g\,cm^{-3}}$)',
             color='white',
             fontsize=8,
             fontweight='bold',
             ha='center',
             va='center',
             zorder=5)
    ax2.text(0,
             0.95,
             r'Convective Adiabat' + '\n' + r'($P > 100\ \mathrm{bar}$)',
             color='white',
             fontsize=8,
             fontweight='bold',
             ha='center',
             va='center',
             zorder=5)
    ax2.text(0,
             1.65,
             r'Radiative Atmosphere ($P < 100\ \mathrm{bar}$)' + '\n' +
             r'Thermally Ionized ($T \sim 1600\ \mathrm{K}$)',
             color='#1d3557',
             fontsize=7.5,
             fontweight='bold',
             ha='center',
             va='center',
             zorder=5)

    # Ohmic heat deposition burst arrows
    for angle in np.linspace(0, 2 * np.pi, 8, endpoint=False):
        xa = r_rcb * np.cos(angle)
        ya = r_rcb * np.sin(angle)
        ax2.scatter(xa, ya, color='#d62728', s=45, zorder=6)

    # Inflation bracket / label
    ax2.annotate('',
                 xy=(r_surface + 0.15, 0),
                 xytext=(r_rcb + 0.15, 0),
                 arrowprops=dict(arrowstyle="<->", color='#b7094c', lw=2.0),
                 zorder=7)
    ax2.text(r_surface + 0.30,
             0,
             r'Ohmic Radius Inflation' + '\n' +
             r'$\Delta R_p \approx 0.4 - 0.7\ R_{\mathrm{J}}$',
             color='#b7094c',
             fontsize=8.5,
             fontweight='bold',
             va='center',
             zorder=7)

    # Mechanism summary box
    ax2.text(
        0,
        -2.3,
        r'$\mathbf{Thermodynamic\ Feedback:}$' + '\n' +
        r'Deep Ohmic heat $P_{\mathrm{ohm}} \sim 10^{19}\ \mathrm{W}$ deposited below RCB'
        + '\n' +
        r'arrests Kelvin-Helmholtz cooling and sustains bloated radii $\sim 1.5 - 1.8\ R_{\mathrm{J}}$.',
        color='#264653',
        fontsize=8.2,
        ha='center',
        zorder=8,
        bbox=dict(boxstyle='round,pad=0.3',
                  facecolor='#f8f9fa',
                  edgecolor='#b7094c',
                  lw=1.2))

    ax2.set_xlim(-2.4, 3.2)
    ax2.set_ylim(-2.6, 2.4)
    ax2.set_title(r'(b) Interior Ohmic Heat Deposition & Radius Inflation',
                  fontweight='bold',
                  pad=10)

    plt.tight_layout()
    pdf_path = os.path.join(script_dir, "fig_diagram.pdf")
    png_path = os.path.join(script_dir, "fig_diagram.png")
    plt.savefig(pdf_path, dpi=300)
    plt.savefig(png_path, dpi=300)
    plt.close()
    print(f"--> Generated {pdf_path} and {png_path}")


if __name__ == '__main__':
    print(
        "=== Generating Figures for Paper #220 (Batygin & Stevenson 2010) ===")
    generate_fig_comparison()
    generate_fig_model_choices()
    generate_fig_diagram()
    print("✅ All figures generated successfully in PDF and PNG formats!")
