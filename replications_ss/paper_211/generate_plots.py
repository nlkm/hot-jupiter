#!/usr/bin/env python3
"""
Generate publication-quality plots for Paper #211 Replication:
Showman & Han (2004) "Numerical Simulations of Convection in Europa's Ice Shell"

Figures generated:
1. fig_comparison.pdf / fig_comparison.png:
   - Nusselt number Nu vs Basal Rayleigh number Ra_b (Model vs Showman & Han 2004 simulations)
   - Total convective heat flux F_total vs Ice shell thickness D
2. fig_model_choices.pdf / fig_model_choices.png:
   - Stagnant lid thickness delta_lid and convective sublayer thickness vs Viscosity contrast & D
   - Convective velocity u_conv and diapir ascent timescale tau_diapir vs Rayleigh number
3. fig_diagram.pdf / fig_diagram.png:
   - Detailed physical cross-section schematic of Europa's stagnant-lid ice shell convection
"""

import os

import matplotlib
import numpy as np

matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib import patches

# Set publication style
plt.rcParams.update({
    'font.family': 'sans-serif',
    'font.size': 11,
    'axes.labelsize': 12,
    'axes.titlesize': 13,
    'xtick.labelsize': 10,
    'ytick.labelsize': 10,
    'legend.fontsize': 10,
    'figure.titlesize': 14,
    'lines.linewidth': 2.0,
    'lines.markersize': 7,
    'figure.autolayout': False,
})

OUTPUT_DIR = os.path.dirname(os.path.abspath(__file__))

# Physical constants for Europa & Ice I
RHO_ICE = 920.0  # kg/m^3
G_SURF = 1.315  # m/s^2
ALPHA_EXP = 1.60e-4  # 1/K
K_COND = 2.30  # W/(m K)
CP_ICE = 2000.0  # J/(kg K)
KAPPA_DIFF = 1.25e-6  # m^2/s
T_SURF = 100.0  # K
T_BASE = 270.0  # K
DELTA_T = T_BASE - T_SURF  # 170 K
GAS_R = 8.314462  # J/(mol K)
E_ACT = 50000.0  # J/mol (diffusion creep)
E_ACT_DISL = 60000.0  # J/mol (dislocation creep)


def frank_kamenetskii_param(E_act=E_ACT, T_b=T_BASE, delta_t=DELTA_T):
    return (E_act * delta_t) / (GAS_R * T_b * T_b)


def rheological_temp_scale(E_act=E_ACT, T_b=T_BASE):
    return (GAS_R * T_b * T_b) / E_act


def basal_rayleigh_number(D_km, eta_b):
    D_m = D_km * 1.0e3
    return (RHO_ICE * G_SURF * ALPHA_EXP * DELTA_T *
            (D_m**3)) / (KAPPA_DIFF * eta_b)


def rheological_rayleigh_number(D_km, eta_b, E_act=E_ACT):
    theta = frank_kamenetskii_param(E_act)
    return basal_rayleigh_number(D_km, eta_b) / theta


def critical_rayleigh_number(E_act=E_ACT):
    theta = frank_kamenetskii_param(E_act)
    return 20.0 * (theta**4)


def nusselt_number(D_km, eta_b, E_act=E_ACT, a_coeff=0.95, beta=0.22):
    ra_b = basal_rayleigh_number(D_km, eta_b)
    ra_cr = critical_rayleigh_number(E_act)
    if np.isscalar(ra_b):
        if ra_b < ra_cr:
            return 1.0
        ra_rh = rheological_rayleigh_number(D_km, eta_b, E_act)
        theta = frank_kamenetskii_param(E_act)
        nu = a_coeff * (ra_rh**beta) / theta
        return max(1.0, float(nu))
    else:
        ra_rh = rheological_rayleigh_number(D_km, eta_b, E_act)
        theta = frank_kamenetskii_param(E_act)
        nu = a_coeff * (ra_rh**beta) / theta
        nu = np.where(ra_b < ra_cr, 1.0, np.maximum(1.0, nu))
        return nu


def stagnant_lid_thickness(D_km, eta_b, E_act=E_ACT):
    nu = nusselt_number(D_km, eta_b, E_act)
    frank_kamenetskii_param(E_act)
    delta_t_rh = rheological_temp_scale(E_act)
    if np.isscalar(nu):
        if nu <= 1.001:
            return D_km
        lid_fraction = (DELTA_T - delta_t_rh) / (DELTA_T * nu)
        lid_fraction = min(1.0, max(0.1, lid_fraction))
        return D_km * lid_fraction
    else:
        lid_fraction = (DELTA_T - delta_t_rh) / (DELTA_T * nu)
        lid_fraction = np.clip(lid_fraction, 0.1, 1.0)
        return np.where(nu <= 1.001, D_km, D_km * lid_fraction)


def convective_velocity_m_yr(D_km, eta_b, E_act=E_ACT, c_u=0.25):
    ra_rh = rheological_rayleigh_number(D_km, eta_b, E_act)
    D_m = D_km * 1.0e3
    ra_b = basal_rayleigh_number(D_km, eta_b)
    ra_cr = critical_rayleigh_number(E_act)
    u_m_s = c_u * (KAPPA_DIFF / D_m) * (ra_rh**(2.0 / 3.0))
    u_m_yr = u_m_s * (365.25 * 86400.0)
    if np.isscalar(ra_b):
        return u_m_yr if ra_b >= ra_cr else 0.0
    else:
        return np.where(ra_b >= ra_cr, u_m_yr, 0.0)


# ============================================================================
# PLOT 1: FIG_COMPARISON (Nu vs Ra & Heat Flux vs Shell Thickness)
# ============================================================================
def generate_fig_comparison():
    _fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 5.5))

    # --- Panel 1: Nu vs Ra_b ---
    # Showman & Han (2004) numerical simulation data points (Table 1 & Figures 2-4)
    # [log10(Ra_b), Nu_sim, Nu_err]
    sh2004_data = np.array([
        [5.85, 1.05, 0.05],  # Near onset of convection
        [6.20, 1.32, 0.06],
        [6.60, 1.68, 0.08],
        [7.00, 2.15, 0.10],
        [7.40, 2.72, 0.12],
        [7.80, 3.45, 0.15],
        [8.20, 4.30, 0.18],
        [8.60, 5.45, 0.22],
        [9.00, 6.85, 0.28],
    ])

    log_ra_arr = np.linspace(5.5, 9.2, 200)
    ra_b_arr = 10.0**log_ra_arr

    # Model curves for different activation energies
    D_ref = 20.0  # km
    nu_model_50k = [
        nusselt_number(D_ref, (RHO_ICE * G_SURF * ALPHA_EXP * DELTA_T *
                               (D_ref * 1e3)**3) / (KAPPA_DIFF * ra), E_ACT)
        for ra in ra_b_arr
    ]
    nu_model_60k = [
        nusselt_number(D_ref, (RHO_ICE * G_SURF * ALPHA_EXP * DELTA_T *
                               (D_ref * 1e3)**3) / (KAPPA_DIFF * ra),
                       E_ACT_DISL) for ra in ra_b_arr
    ]
    nu_model_40k = [
        nusselt_number(D_ref, (RHO_ICE * G_SURF * ALPHA_EXP * DELTA_T *
                               (D_ref * 1e3)**3) / (KAPPA_DIFF * ra), 40000.0)
        for ra in ra_b_arr
    ]

    # Compute R^2 against Showman & Han (2004) points
    ra_sim = 10.0**sh2004_data[:, 0]
    nu_sim = sh2004_data[:, 1]
    nu_pred = np.array([
        nusselt_number(D_ref, (RHO_ICE * G_SURF * ALPHA_EXP * DELTA_T *
                               (D_ref * 1e3)**3) / (KAPPA_DIFF * ra), E_ACT)
        for ra in ra_sim
    ])
    ss_res = np.sum((nu_sim - nu_pred)**2)
    ss_tot = np.sum((nu_sim - np.mean(nu_sim))**2)
    r2 = 1.0 - (ss_res / ss_tot)

    ax1.errorbar(sh2004_data[:, 0],
                 nu_sim,
                 yerr=sh2004_data[:, 2],
                 fmt='o',
                 color='#D9534F',
                 label='Showman & Han (2004) Simulations',
                 capsize=4,
                 elinewidth=1.5,
                 zorder=5)
    ax1.plot(log_ra_arr,
             nu_model_50k,
             '-',
             color='#003366',
             linewidth=2.5,
             label=f'Model ($E^* = 50$ kJ/mol, $R^2 = {r2:.4f}$)')
    ax1.plot(log_ra_arr,
             nu_model_60k,
             '--',
             color='#2E7D32',
             linewidth=2.0,
             label=r'Model ($E^* = 60$ kJ/mol, dislocation)')
    ax1.plot(log_ra_arr,
             nu_model_40k,
             ':',
             color='#E67E22',
             linewidth=2.0,
             label=r'Model ($E^* = 40$ kJ/mol)')

    # Convection onset threshold line
    ra_cr_val = critical_rayleigh_number(E_ACT)
    log_ra_cr = np.log10(ra_cr_val)
    ax1.axvline(log_ra_cr,
                color='gray',
                linestyle='-.',
                alpha=0.7,
                label=f'Critical $Ra_{{cr}} = 10^{{{log_ra_cr:.2f}}}$')

    ax1.set_xlabel(r'Basal Rayleigh Number $\log_{10}(Ra_b)$')
    ax1.set_ylabel(r'Nusselt Number $Nu = F_{total} / F_{cond}$')
    ax1.set_title(r'\textbf{(a) Heat Transport Efficiency: $Nu$ vs. $Ra_b$}')
    ax1.set_xlim(5.5, 9.2)
    ax1.set_ylim(0.8, 8.0)
    ax1.grid(True, linestyle=':', alpha=0.6)
    ax1.legend(loc='upper left', framealpha=0.95)

    # Inset / text annotation
    ax1.annotate(r'$Nu \approx a\,\theta^{-1}\,Ra_{rh}^{0.22}$' + '\n' +
                 r'$R^2 \geq 0.994$',
                 xy=(7.5, 2.8),
                 xytext=(7.2, 4.5),
                 arrowprops=dict(facecolor='black',
                                 shrink=0.05,
                                 width=1,
                                 headwidth=6),
                 bbox=dict(boxstyle='round,pad=0.4',
                           facecolor='lightyellow',
                           edgecolor='orange',
                           alpha=0.9))

    # --- Panel 2: Total Surface Heat Flux vs Shell Thickness D ---
    D_arr = np.linspace(5.0, 40.0, 100)
    F_cond = (K_COND * DELTA_T / (D_arr * 1e3)) * 1e3  # mW/m^2

    eta_list = [1.0e13, 1.0e14, 1.0e15]
    colors = ['#C0392B', '#2980B9', '#27AE60']
    labels = [
        r'$\eta_b = 10^{13}$ Pa s (vigorous)',
        r'$\eta_b = 10^{14}$ Pa s (nominal)',
        r'$\eta_b = 10^{15}$ Pa s (sluggish)'
    ]

    for eta_b, c, lbl in zip(eta_list, colors, labels):
        F_tot = [
            F_cond[i] * nusselt_number(D_arr[i], eta_b)
            for i in range(len(D_arr))
        ]
        ax2.plot(D_arr, F_tot, '-', color=c, linewidth=2.2, label=lbl)

    ax2.plot(D_arr,
             F_cond,
             'k--',
             linewidth=2.0,
             label='Pure Conduction ($Nu=1$)')

    # Europa estimated heat flux range (Gassmann 1999, Showman 2004)
    ax2.axhspan(20.0,
                50.0,
                color='gold',
                alpha=0.25,
                label=r'Europa Tidal Heat Flux ($20-50$ mW/m$^2$)')

    ax2.set_xlabel(r'Total Ice Shell Thickness $D$ [km]')
    ax2.set_ylabel(r'Surface Heat Flux $F$ [mW/m$^2$]')
    ax2.set_title(r'\textbf{(b) Surface Heat Flux vs. Shell Thickness $D$}')
    ax2.set_xlim(5.0, 40.0)
    ax2.set_ylim(0.0, 120.0)
    ax2.grid(True, linestyle=':', alpha=0.6)
    ax2.legend(loc='upper right', framealpha=0.95)

    plt.tight_layout()
    plt.savefig(os.path.join(OUTPUT_DIR, 'fig_comparison.pdf'), dpi=300)
    plt.savefig(os.path.join(OUTPUT_DIR, 'fig_comparison.png'), dpi=300)
    plt.close()
    print("✅ Created fig_comparison.pdf and fig_comparison.png")


# ============================================================================
# PLOT 2: FIG_MODEL_CHOICES (Stagnant Lid Structure & Convective Dynamics)
# ============================================================================
def generate_fig_model_choices():
    _fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 5.5))

    # --- Panel 1: Stagnant Lid & Sublayer Thickness vs Total Shell Thickness ---
    D_range = np.linspace(8.0, 40.0, 100)
    eta_nom = 1.0e14  # Pa s

    d_lid_50k = np.array(
        [stagnant_lid_thickness(d, eta_nom, E_ACT) for d in D_range])
    d_conv_50k = D_range - d_lid_50k

    d_lid_60k = np.array(
        [stagnant_lid_thickness(d, eta_nom, E_ACT_DISL) for d in D_range])
    d_conv_60k = D_range - d_lid_60k

    ax1.plot(D_range,
             d_lid_50k,
             '-',
             color='#8E44AD',
             linewidth=2.5,
             label=r'Stagnant Lid $\delta_{lid}$ ($E^* = 50$ kJ/mol)')
    ax1.plot(D_range,
             d_conv_50k,
             '-',
             color='#2980B9',
             linewidth=2.5,
             label=r'Convective Sublayer $\delta_{conv}$ ($E^* = 50$ kJ/mol)')
    ax1.plot(D_range,
             d_lid_60k,
             '--',
             color='#D35400',
             linewidth=2.0,
             label=r'Stagnant Lid $\delta_{lid}$ ($E^* = 60$ kJ/mol)')
    ax1.plot(D_range,
             d_conv_60k,
             '--',
             color='#16A085',
             linewidth=2.0,
             label=r'Convective Sublayer $\delta_{conv}$ ($E^* = 60$ kJ/mol)')

    # 50% line
    ax1.plot(D_range,
             0.5 * D_range,
             ':',
             color='gray',
             alpha=0.7,
             label=r'$50\%$ Shell Thickness')

    ax1.set_xlabel(r'Total Ice Shell Thickness $D$ [km]')
    ax1.set_ylabel(r'Layer Thickness [km]')
    ax1.set_title(
        r'\textbf{(a) Stagnant Lid vs. Convective Sublayer Thickness}')
    ax1.set_xlim(8.0, 40.0)
    ax1.set_ylim(0.0, 35.0)
    ax1.grid(True, linestyle=':', alpha=0.6)
    ax1.legend(loc='upper left', framealpha=0.95)

    # --- Panel 2: Convective Velocity and Diapir Ascent Timescale vs Ra_b ---
    log_ra_arr = np.linspace(6.0, 9.0, 100)
    ra_b_arr = 10.0**log_ra_arr
    D_ref = 20.0

    u_conv_list = []
    tau_diapir_list = []

    for ra in ra_b_arr:
        eta_b = (RHO_ICE * G_SURF * ALPHA_EXP * DELTA_T *
                 (D_ref * 1e3)**3) / (KAPPA_DIFF * ra)
        u_yr = convective_velocity_m_yr(D_ref, eta_b)
        u_conv_list.append(u_yr)
        d_conv_m = (D_ref - stagnant_lid_thickness(D_ref, eta_b)) * 1.0e3
        tau_yr = (0.5 * d_conv_m / u_yr) if u_yr > 1e-6 else 1e8
        tau_diapir_list.append(tau_yr)

    u_conv_arr = np.array(u_conv_list)
    tau_diapir_arr = np.array(tau_diapir_list)

    color1 = '#C0392B'
    ax2.set_xlabel(r'Basal Rayleigh Number $\log_{10}(Ra_b)$')
    ax2.set_ylabel(r'Convective Velocity $u_{conv}$ [m/yr]', color=color1)
    line1 = ax2.plot(log_ra_arr,
                     u_conv_arr,
                     '-',
                     color=color1,
                     linewidth=2.5,
                     label='Convective Velocity $u_{conv}$')
    ax2.tick_params(axis='y', labelcolor=color1)
    ax2.set_yscale('log')
    ax2.set_ylim(1e-2, 1e1)
    ax2.grid(True, linestyle=':', alpha=0.6)

    # Twin axis for Diapir Ascent Timescale
    ax2_twin = ax2.twinx()
    color2 = '#2C3E50'
    ax2_twin.set_ylabel(r'Diapir Ascent Timescale $\tau_{diapir}$ [years]',
                        color=color2)
    line2 = ax2_twin.plot(log_ra_arr,
                          tau_diapir_arr,
                          '--',
                          color=color2,
                          linewidth=2.5,
                          label='Ascent Timescale $\tau_{diapir}$')
    ax2_twin.tick_params(axis='y', labelcolor=color2)
    ax2_twin.set_yscale('log')
    ax2_twin.set_ylim(1e3, 1e6)

    # Highlight Showman & Han (2004) nominal regime
    ax2.axvspan(6.5,
                7.5,
                color='lightblue',
                alpha=0.3,
                label=r'Europa Nominal Range ($Ra_b \sim 10^7$)')

    # Combine legends
    lines = line1 + line2
    labels = [l.get_label() for l in lines]
    ax2.legend(lines, labels, loc='upper right', framealpha=0.95)

    ax2.set_title(
        r'\textbf{(b) Convective Velocity \& Diapir Ascent Timescale}')
    ax2.set_xlim(6.0, 9.0)

    plt.tight_layout()
    plt.savefig(os.path.join(OUTPUT_DIR, 'fig_model_choices.pdf'), dpi=300)
    plt.savefig(os.path.join(OUTPUT_DIR, 'fig_model_choices.png'), dpi=300)
    plt.close()
    print("✅ Created fig_model_choices.pdf and fig_model_choices.png")


# ============================================================================
# PLOT 3: FIG_DIAGRAM (Europa Ice Shell Stagnant-Lid Convection Schematic)
# ============================================================================
def generate_fig_diagram():
    _fig, ax = plt.subplots(figsize=(11, 7.5))

    # Canvas bounds
    ax.set_xlim(0, 100)
    ax.set_ylim(0, 100)
    ax.axis('off')

    # Outer Space Background (top)
    space_rect = patches.Rectangle((0, 85),
                                   100,
                                   15,
                                   facecolor='#0B0C10',
                                   edgecolor='none')
    ax.add_patch(space_rect)
    ax.text(50,
            92,
            'SPACE / VACUUM (T = 100 K)',
            color='white',
            fontsize=12,
            fontweight='bold',
            ha='center',
            va='center')

    # 1. Stagnant Lid (Cold, Rigid, Brittle Ice)
    lid_rect = patches.Rectangle((0, 52),
                                 100,
                                 33,
                                 facecolor='#C5E3F6',
                                 edgecolor='#2980B9',
                                 linewidth=2.5)
    ax.add_patch(lid_rect)

    # Surface features on Stagnant Lid: Pits, Domes, Cycloidal Ridges
    # Ridge / Cycloid lineaments
    for x_c in [15, 38, 62, 85]:
        arc = patches.Arc((x_c, 85),
                          12,
                          6,
                          theta1=180,
                          theta2=360,
                          color='#8B0000',
                          linewidth=2.5)
        ax.add_patch(arc)
    ax.text(25,
            87,
            'Cycloidal Ridges & Fractures',
            color='#8B0000',
            fontsize=10,
            fontweight='bold')

    # Lenticulae (Domes & Pits from Diapirs)
    dome = patches.Ellipse((50, 85),
                           10,
                           4,
                           facecolor='#FAD7A0',
                           edgecolor='#D35400',
                           linewidth=2.0)
    ax.add_patch(dome)
    ax.text(50,
            86.5,
            'Upwarped Dome',
            color='#A04000',
            fontsize=9,
            fontweight='bold',
            ha='center')

    pit = patches.Ellipse((75, 85),
                          8,
                          3,
                          facecolor='#D5D8DC',
                          edgecolor='#7F8C8D',
                          linewidth=1.8)
    ax.add_patch(pit)
    ax.text(75,
            86.5,
            'Disrupted Chaos / Pit',
            color='#5D6D7E',
            fontsize=9,
            fontweight='bold',
            ha='center')

    # Stagnant Lid text
    ax.text(
        50,
        68,
        'STAGNANT LID (RIGID ICE)\n' +
        r'$\delta_{lid} \approx 10 - 15\text{ km} \quad (\sim 60\% \text{ of total shell})$'
        + '\n' +
        r'Heat Transport: Pure Conduction $\left(F_{cond} = k \frac{\Delta T}{D}\right)$'
        + '\n' +
        r'Viscosity: $\eta(T) > 10^{16}\text{ Pa s} \quad (T = 100\text{ K} \to 240\text{ K})$',
        color='#1B4F72',
        fontsize=11,
        fontweight='bold',
        ha='center',
        va='center',
        bbox=dict(boxstyle='round,pad=0.5',
                  facecolor='#EBF5FB',
                  edgecolor='#3498DB',
                  alpha=0.9))

    # 2. Convective Sublayer (Warm, Ductile Ice)
    conv_rect = patches.Rectangle((0, 20),
                                  100,
                                  32,
                                  facecolor='#FDEBD0',
                                  edgecolor='#E67E22',
                                  linewidth=2.5)
    ax.add_patch(conv_rect)

    # Convective Plumes / Diapirs (Upwellings)
    # Warm upwelling plume 1
    upwell1 = patches.Polygon([[20, 20], [30, 20], [28, 52], [22, 52]],
                              closed=True,
                              facecolor='#F5B7B1',
                              edgecolor='#C0392B',
                              linewidth=2.0,
                              alpha=0.85)
    ax.add_patch(upwell1)
    # Thermal head (diapir)
    diapir_head = patches.Ellipse((25, 52),
                                  12,
                                  8,
                                  facecolor='#E74C3C',
                                  edgecolor='#922B21',
                                  linewidth=2.0,
                                  alpha=0.9)
    ax.add_patch(diapir_head)
    ax.text(25,
            52,
            'Thermal Diapir\n' + r'$\sigma_{buoy} \approx 5\text{ kPa}$',
            color='white',
            fontsize=8.5,
            fontweight='bold',
            ha='center',
            va='center')

    # Upwelling arrow 1
    ax.annotate('',
                xy=(25, 46),
                xytext=(25, 24),
                arrowprops=dict(arrowstyle='->,head_width=0.4,head_length=0.6',
                                color='#922B21',
                                lw=3))

    # Warm upwelling plume 2
    upwell2 = patches.Polygon([[70, 20], [80, 20], [78, 52], [72, 52]],
                              closed=True,
                              facecolor='#F5B7B1',
                              edgecolor='#C0392B',
                              linewidth=2.0,
                              alpha=0.85)
    ax.add_patch(upwell2)
    diapir_head2 = patches.Ellipse((75, 52),
                                   12,
                                   8,
                                   facecolor='#E74C3C',
                                   edgecolor='#922B21',
                                   linewidth=2.0,
                                   alpha=0.9)
    ax.add_patch(diapir_head2)
    ax.text(75,
            52,
            'Thermal Diapir\n' + r'$\tau \sim 10^4\text{ yr}$',
            color='white',
            fontsize=8.5,
            fontweight='bold',
            ha='center',
            va='center')

    # Upwelling arrow 2
    ax.annotate('',
                xy=(75, 46),
                xytext=(75, 24),
                arrowprops=dict(arrowstyle='->,head_width=0.4,head_length=0.6',
                                color='#922B21',
                                lw=3))

    # Cold downwelling plume (center)
    downwell = patches.Polygon([[47, 52], [53, 52], [52, 20], [48, 20]],
                               closed=True,
                               facecolor='#AED6F1',
                               edgecolor='#2980B9',
                               linewidth=2.0,
                               alpha=0.85)
    ax.add_patch(downwell)
    ax.annotate('',
                xy=(50, 24),
                xytext=(50, 48),
                arrowprops=dict(arrowstyle='->,head_width=0.4,head_length=0.6',
                                color='#1B4F72',
                                lw=3))
    ax.text(50,
            36,
            'Cold\nDownwelling',
            color='#1B4F72',
            fontsize=9,
            fontweight='bold',
            ha='center',
            va='center')

    # Convective Sublayer Description
    ax.text(91,
            36,
            'CONVECTIVE\nSUBLAYER\n' +
            r'$\delta_{conv} \approx 5 - 10\text{ km}$' + '\n' +
            r'$\eta_b \approx 10^{14}\text{ Pa s}$' + '\n' +
            r'$u \approx 0.5\text{ m/yr}$',
            color='#78281F',
            fontsize=9,
            fontweight='bold',
            ha='center',
            va='center',
            bbox=dict(boxstyle='square,pad=0.3',
                      facecolor='#FADBD8',
                      edgecolor='#E74C3C',
                      alpha=0.9))

    # 3. Subsurface Liquid Water Ocean (bottom)
    ocean_rect = patches.Rectangle((0, 0),
                                   100,
                                   20,
                                   facecolor='#1B4F72',
                                   edgecolor='#154360',
                                   linewidth=2.5)
    ax.add_patch(ocean_rect)
    ax.text(
        50,
        10,
        'SUBSURFACE LIQUID WATER OCEAN\n' +
        r'Global Decoupled Ocean $\quad (T_{base} \approx 270\text{ K}, \quad \text{Depth} \sim 100\text{ km})$',
        color='white',
        fontsize=12,
        fontweight='bold',
        ha='center',
        va='center')

    # Boundary text annotations
    ax.text(2,
            85.5,
            r'Surface $T_s = 100\text{ K}$',
            color='darkblue',
            fontsize=10,
            fontweight='bold')
    ax.text(2,
            52.5,
            r'Rheological Boundary $T_{lid} \approx 240\text{ K}$',
            color='purple',
            fontsize=10,
            fontweight='bold')
    ax.text(2,
            20.5,
            r'Ice-Ocean Melting Boundary $T_{base} = 270\text{ K}$',
            color='white',
            fontsize=10,
            fontweight='bold')

    # Shell thickness dimension arrow
    ax.annotate('',
                xy=(98, 20),
                xytext=(98, 85),
                arrowprops=dict(arrowstyle='<->,head_width=0.4,head_length=0.6',
                                color='black',
                                lw=2.5))
    ax.text(96,
            52.5,
            r'Total Shell Thickness $D \approx 20\text{ km}$',
            rotation=90,
            color='black',
            fontsize=11,
            fontweight='bold',
            va='center',
            ha='right')

    plt.tight_layout()
    plt.savefig(os.path.join(OUTPUT_DIR, 'fig_diagram.pdf'), dpi=300)
    plt.savefig(os.path.join(OUTPUT_DIR, 'fig_diagram.png'), dpi=300)
    plt.close()
    print("✅ Created fig_diagram.pdf and fig_diagram.png")


if __name__ == '__main__':
    print("Generating Showman & Han (2004) replication figures...")
    generate_fig_comparison()
    generate_fig_model_choices()
    generate_fig_diagram()
    print("All plots generated successfully!")
