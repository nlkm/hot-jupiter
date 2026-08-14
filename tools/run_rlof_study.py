"""
Fast, high-performance simulation suite and figure generator for the standalone RLOF research paper.
Generates paper_rlof/figures/fig1_scenario1_disruption.png through fig5_usp_cases.png.
"""

import os
import sys

# Add project root to sys.path
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__),
                                                "..")))

import matplotlib.pyplot as plt
import numpy as np

from hot_jupiter.constants import AU, M_EARTH, M_JUP, R_JUP
from hot_jupiter.database import get_db_connection, seed_database_if_empty
from hot_jupiter.evolution.rlof_engine import CoupledRLOFIntegrator

# Use non-interactive backend
plt.switch_backend('Agg')


def compute_coupled_trajectory(M_p_0: float,
                               a_0: float,
                               e_0: float = 0.15,
                               t_max_yr: float = 1.0e10,
                               num_pts: int = 500):
    """
    Coupled trajectory calculation delegating to central CoupledRLOFIntegrator engine.
    Integrates up to t_max_yr (10 Gyr).
    """
    integrator = CoupledRLOFIntegrator(m_p_init_jup=M_p_0 / M_JUP,
                                       a_init_au=a_0 / AU,
                                       e_init=e_0)
    res = integrator.integrate(t_max_yr=t_max_yr, num_pts=num_pts)
    return {
        "t": res.t_arr / 1.0e6,  # Myr
        "M_p": res.m_p_arr,  # Already in M_Jup
        "a": res.a_arr,  # Already in AU
        "e": res.e_arr,
        "R_p": res.r_p_arr,  # Already in R_Jup
        "R_roche": res.r_roche_arr * AU / R_JUP,
        "filling_factor": res.filling_factor_arr,
        "outcome": res.outcome.value
    }


def main():
    print("=== Step 1: Setting up output directory paper_rlof/figures/ ===")
    fig_dir = "paper_rlof/figures"
    os.makedirs(fig_dir, exist_ok=True)

    print(
        "=== Step 2: Running Trajectory Simulations (Full 10-Gyr Evolution) ==="
    )
    res_disrupt = compute_coupled_trajectory(M_p_0=0.6 * M_JUP,
                                             a_0=0.016 * AU,
                                             t_max_yr=3.0e9)
    res_stagnate = compute_coupled_trajectory(M_p_0=0.8 * M_JUP,
                                              a_0=0.022 * AU,
                                              t_max_yr=1.0e10)
    res_cool = compute_coupled_trajectory(M_p_0=1.0 * M_JUP,
                                          a_0=0.030 * AU,
                                          t_max_yr=1.0e10)

    # --- Render Figure 1: Scenario 1 - Runaway Disruption (a, e, M_p, R_p) ---
    print("--> Generating paper_rlof/figures/fig1_scenario1_disruption.png...")
    fig, axes = plt.subplots(1, 4, figsize=(16.5, 3.8), dpi=300)

    axes[0].plot(res_disrupt["t"], res_disrupt["a"], 'r-', lw=2.5)
    axes[0].set_xlim(0.0, 3000.0)
    axes[0].set_ylabel('Semi-Major Axis $a$ [AU]',
                       fontsize=10.5,
                       fontweight='bold')
    axes[0].set_xlabel('System Age $t$ [Myr]', fontsize=10.5, fontweight='bold')
    axes[0].set_title('Orbital Decay $a(t)$', fontsize=11, fontweight='bold')
    axes[0].grid(True, linestyle=":", alpha=0.45)

    axes[1].plot(res_disrupt["t"], res_disrupt["e"], 'r-', lw=2.5)
    axes[1].set_xlim(0.0, 3000.0)
    axes[1].set_ylim(0.0, 0.18)
    axes[1].set_ylabel('Eccentricity $e$', fontsize=10.5, fontweight='bold')
    axes[1].set_xlabel('System Age $t$ [Myr]', fontsize=10.5, fontweight='bold')
    axes[1].set_title('Tidal Circularization $e(t)$',
                      fontsize=11,
                      fontweight='bold')
    axes[1].grid(True, linestyle=":", alpha=0.45)

    axes[2].plot(res_disrupt["t"], res_disrupt["M_p"], 'r-', lw=2.5)
    axes[2].set_xlim(0.0, 3000.0)
    axes[2].set_ylabel('Planet Mass $M_p$ [$M_{\\mathrm{Jup}}$]',
                       fontsize=10.5,
                       fontweight='bold')
    axes[2].set_xlabel('System Age $t$ [Myr]', fontsize=10.5, fontweight='bold')
    axes[2].set_title('Envelope Mass Loss $M_p(t)$',
                      fontsize=11,
                      fontweight='bold')
    axes[2].grid(True, linestyle=":", alpha=0.45)

    axes[3].plot(res_disrupt["t"], res_disrupt["R_p"], 'r-', lw=2.5)
    axes[3].set_xlim(0.0, 3000.0)
    axes[3].set_ylabel('Planet Radius $R_p$ [$R_{\\mathrm{Jup}}$]',
                       fontsize=10.5,
                       fontweight='bold')
    axes[3].set_xlabel('System Age $t$ [Myr]', fontsize=10.5, fontweight='bold')
    axes[3].set_title('Radius Evolution $R_p(t)$',
                      fontsize=11,
                      fontweight='bold')
    axes[3].grid(True, linestyle=":", alpha=0.45)

    fig.suptitle(
        'Scenario 1: Runaway Tidal Disruption ($0.6\\,M_{\\mathrm{Jup}}, 0.016\\,\\mathrm{AU}, e_0=0.15$)',
        fontsize=12.5,
        fontweight='bold',
        y=1.03)
    plt.tight_layout()
    plt.savefig(os.path.join(fig_dir, "fig1_scenario1_disruption.png"),
                dpi=300,
                bbox_inches='tight')
    plt.close()

    # --- Render Figure 2: Scenario 2 - Stagnated Survival (10-Gyr Trajectory) ---
    print("--> Generating paper_rlof/figures/fig2_scenario2_stagnation.png...")
    fig, axes = plt.subplots(1, 4, figsize=(16.5, 3.8), dpi=300)

    axes[0].plot(res_stagnate["t"], res_stagnate["a"], 'b-', lw=2.5)
    axes[0].axhline(0.0185,
                    color='gray',
                    linestyle=':',
                    label='Remnant Orbit Floor')
    axes[0].set_xlim(0.0, 10000.0)
    axes[0].set_ylabel('Semi-Major Axis $a$ [AU]',
                       fontsize=10.5,
                       fontweight='bold')
    axes[0].set_xlabel('System Age $t$ [Myr]', fontsize=10.5, fontweight='bold')
    axes[0].set_title('Orbital Decay Stagnation $a(t)$',
                      fontsize=11,
                      fontweight='bold')
    axes[0].grid(True, linestyle=":", alpha=0.45)
    axes[0].legend(fontsize=8.5, loc='upper right')

    axes[1].plot(res_stagnate["t"], res_stagnate["e"], 'b-', lw=2.5)
    axes[1].set_xlim(0.0, 10000.0)
    axes[1].set_ylim(0.0, 0.18)
    axes[1].set_ylabel('Eccentricity $e$', fontsize=10.5, fontweight='bold')
    axes[1].set_xlabel('System Age $t$ [Myr]', fontsize=10.5, fontweight='bold')
    axes[1].set_title('Tidal Circularization $e(t)$',
                      fontsize=11,
                      fontweight='bold')
    axes[1].grid(True, linestyle=":", alpha=0.45)

    axes[2].plot(res_stagnate["t"], res_stagnate["M_p"], 'b-', lw=2.5)
    axes[2].axhline(10.0 * M_EARTH / M_JUP,
                    color='purple',
                    linestyle='--',
                    label=r'Core Mass ($10\,M_\oplus$)')
    axes[2].set_xlim(0.0, 10000.0)
    axes[2].set_ylabel('Planet Mass $M_p$ [$M_{\\mathrm{Jup}}$]',
                       fontsize=10.5,
                       fontweight='bold')
    axes[2].set_xlabel('System Age $t$ [Myr]', fontsize=10.5, fontweight='bold')
    axes[2].set_title('Self-Limiting Mass Loss $M_p(t)$',
                      fontsize=11,
                      fontweight='bold')
    axes[2].grid(True, linestyle=":", alpha=0.45)
    axes[2].legend(fontsize=8.5, loc='upper right')

    axes[3].plot(res_stagnate["t"], res_stagnate["R_p"], 'b-', lw=2.5)
    axes[3].set_xlim(0.0, 10000.0)
    axes[3].set_ylabel('Planet Radius $R_p$ [$R_{\\mathrm{Jup}}$]',
                       fontsize=10.5,
                       fontweight='bold')
    axes[3].set_xlabel('System Age $t$ [Myr]', fontsize=10.5, fontweight='bold')
    axes[3].set_title('Stripped Remnant Radius $R_p(t)$',
                      fontsize=11,
                      fontweight='bold')
    axes[3].grid(True, linestyle=":", alpha=0.45)

    fig.suptitle(
        'Scenario 2: Self-Limiting RLOF Mass Loss Stagnation ($0.8\\,M_{\\mathrm{Jup}}, 0.022\\,\\mathrm{AU}, e_0=0.15$)',
        fontsize=12.5,
        fontweight='bold',
        y=1.03)
    plt.tight_layout()
    plt.savefig(os.path.join(fig_dir, "fig2_scenario2_stagnation.png"),
                dpi=300,
                bbox_inches='tight')
    plt.close()

    # --- Render Figure 3: Scenario 3 - Non-Overflow Cooling ---
    print("--> Generating paper_rlof/figures/fig3_scenario3_cooling.png...")
    fig, axes = plt.subplots(1, 4, figsize=(16.5, 3.8), dpi=300)

    axes[0].plot(res_cool["t"], res_cool["a"], 'g-', lw=2.5)
    axes[0].set_xlim(0.0, 10000.0)
    axes[0].set_ylabel('Semi-Major Axis $a$ [AU]',
                       fontsize=10.5,
                       fontweight='bold')
    axes[0].set_xlabel('System Age $t$ [Myr]', fontsize=10.5, fontweight='bold')
    axes[0].set_title('Orbital Decay $a(t)$', fontsize=11, fontweight='bold')
    axes[0].grid(True, linestyle=":", alpha=0.45)

    axes[1].plot(res_cool["t"], res_cool["e"], 'g-', lw=2.5)
    axes[1].set_xlim(0.0, 10000.0)
    axes[1].set_ylim(0.0, 0.18)
    axes[1].set_ylabel('Eccentricity $e$', fontsize=10.5, fontweight='bold')
    axes[1].set_xlabel('System Age $t$ [Myr]', fontsize=10.5, fontweight='bold')
    axes[1].set_title('Tidal Circularization $e(t)$',
                      fontsize=11,
                      fontweight='bold')
    axes[1].grid(True, linestyle=":", alpha=0.45)

    axes[2].plot(res_cool["t"], res_cool["M_p"], 'g-', lw=2.5)
    axes[2].set_xlim(0.0, 10000.0)
    axes[2].set_ylabel('Planet Mass $M_p$ [$M_{\\mathrm{Jup}}$]',
                       fontsize=10.5,
                       fontweight='bold')
    axes[2].set_xlabel('System Age $t$ [Myr]', fontsize=10.5, fontweight='bold')
    axes[2].set_title('Constant Mass $M_p(t)$', fontsize=11, fontweight='bold')
    axes[2].grid(True, linestyle=":", alpha=0.45)

    axes[3].plot(res_cool["t"], res_cool["R_p"], 'g-', lw=2.5)
    axes[3].set_xlim(0.0, 10000.0)
    axes[3].set_ylabel('Planet Radius $R_p$ [$R_{\\mathrm{Jup}}$]',
                       fontsize=10.5,
                       fontweight='bold')
    axes[3].set_xlabel('System Age $t$ [Myr]', fontsize=10.5, fontweight='bold')
    axes[3].set_title('Thermal Contraction $R_p(t)$',
                      fontsize=11,
                      fontweight='bold')
    axes[3].grid(True, linestyle=":", alpha=0.45)

    fig.suptitle(
        'Scenario 3: Non-Overflow Intact Cooling ($1.0\\,M_{\\mathrm{Jup}}, 0.030\\,\\mathrm{AU}, e_0=0.15$)',
        fontsize=12.5,
        fontweight='bold',
        y=1.03)
    plt.tight_layout()
    plt.savefig(os.path.join(fig_dir, "fig3_scenario3_cooling.png"),
                dpi=300,
                bbox_inches='tight')
    plt.close()

    # --- Render Figure 4: Eccentric Evolution ---
    print("--> Generating paper_rlof/figures/fig4_scenario4_eccentric.png...")
    res_ecc = compute_coupled_trajectory(M_p_0=1.2 * M_JUP,
                                         a_0=0.025 * AU,
                                         e_0=0.35,
                                         t_max_yr=1.0e10)
    fig, axes = plt.subplots(1, 4, figsize=(16.5, 3.8), dpi=300)

    axes[0].plot(res_ecc["t"], res_ecc["a"], 'm-', lw=2.5)
    axes[0].set_xlim(0.0, 10000.0)
    axes[0].set_ylabel('Semi-Major Axis $a$ [AU]',
                       fontsize=10.5,
                       fontweight='bold')
    axes[0].set_xlabel('System Age $t$ [Myr]', fontsize=10.5, fontweight='bold')
    axes[0].set_title('Orbital Decay $a(t)$', fontsize=11, fontweight='bold')
    axes[0].grid(True, linestyle=":", alpha=0.45)

    axes[1].plot(res_ecc["t"], res_ecc["e"], 'm-', lw=2.5)
    axes[1].set_xlim(0.0, 10000.0)
    axes[1].set_ylim(0.0, 0.40)
    axes[1].set_ylabel('Eccentricity $e$', fontsize=10.5, fontweight='bold')
    axes[1].set_xlabel('System Age $t$ [Myr]', fontsize=10.5, fontweight='bold')
    axes[1].set_title('Rapid Circularization $e(t)$',
                      fontsize=11,
                      fontweight='bold')
    axes[1].grid(True, linestyle=":", alpha=0.45)

    axes[2].plot(res_ecc["t"], res_ecc["M_p"], 'm-', lw=2.5)
    axes[2].set_xlim(0.0, 10000.0)
    axes[2].set_ylabel('Planet Mass $M_p$ [$M_{\\mathrm{Jup}}$]',
                       fontsize=10.5,
                       fontweight='bold')
    axes[2].set_xlabel('System Age $t$ [Myr]', fontsize=10.5, fontweight='bold')
    axes[2].set_title('Mass Evolution $M_p(t)$', fontsize=11, fontweight='bold')
    axes[2].grid(True, linestyle=":", alpha=0.45)

    axes[3].plot(res_ecc["t"], res_ecc["R_p"], 'm-', lw=2.5)
    axes[3].set_xlim(0.0, 10000.0)
    axes[3].set_ylabel('Planet Radius $R_p$ [$R_{\\mathrm{Jup}}$]',
                       fontsize=10.5,
                       fontweight='bold')
    axes[3].set_xlabel('System Age $t$ [Myr]', fontsize=10.5, fontweight='bold')
    axes[3].set_title('Radius Evolution $R_p(t)$',
                      fontsize=11,
                      fontweight='bold')
    axes[3].grid(True, linestyle=":", alpha=0.45)

    fig.suptitle(
        'Scenario 4: High-Eccentricity Tidal Flexure ($1.2\\,M_{\\mathrm{Jup}}, 0.025\\,\\mathrm{AU}, e_0=0.35$)',
        fontsize=12.5,
        fontweight='bold',
        y=1.03)
    plt.tight_layout()
    plt.savefig(os.path.join(fig_dir, "fig4_scenario4_eccentric.png"),
                dpi=300,
                bbox_inches='tight')
    plt.close()

    # --- Render Figure 5: Roche Lobe Filling Factor Comparison ---
    print("--> Generating paper_rlof/figures/fig2_roche_filling.png...")
    plt.figure(figsize=(8.0, 5.0), dpi=300)
    plt.plot(
        res_disrupt["t"],
        res_disrupt["filling_factor"],
        'r-',
        lw=2.5,
        label=
        'Scenario 1: Disruption ($0.6\\,M_{\\mathrm{J}}, 0.016\\,\\mathrm{AU}$)'
    )
    plt.plot(
        res_stagnate["t"],
        res_stagnate["filling_factor"],
        'b--',
        lw=2.5,
        label=
        'Scenario 2: Stagnation ($0.8\\,M_{\\mathrm{J}}, 0.019\\,\\mathrm{AU}$)'
    )
    plt.plot(
        res_cool["t"],
        res_cool["filling_factor"],
        'g-.',
        lw=2.5,
        label=
        'Scenario 3: Non-Overflow ($1.0\\,M_{\\mathrm{J}}, 0.030\\,\\mathrm{AU}$)'
    )

    plt.axhline(1.0,
                color='black',
                linestyle=':',
                lw=1.8,
                label=r'Roche Overfilling Limit ($\mu_{\mathrm{Roche}} = 1.0$)')
    plt.xlim(0.0, 3000.0)
    plt.ylim(0.2, 1.35)
    plt.xlabel('System Age $t$ [Myr]', fontsize=11.5, fontweight='bold')
    plt.ylabel(
        r'Roche Lobe Filling Factor $\mu_{\mathrm{Roche}} = R_p / R_{\mathrm{Roche}}$',
        fontsize=11.5,
        fontweight='bold')
    plt.title(
        r'Time Evolution of Roche Lobe Overflow ($\mu_{\mathrm{Roche}} \geq 1.0$)',
        fontsize=12,
        fontweight='bold')
    plt.grid(True, linestyle=":", alpha=0.45)
    plt.legend(fontsize=8.5, loc='upper right', framealpha=0.95)
    plt.tight_layout()
    plt.savefig(os.path.join(fig_dir, "fig2_roche_filling.png"), dpi=400)
    plt.close()

    # --- Render Figure 6: Monotonic 2D Survival Zone Map ---
    print("--> Generating paper_rlof/figures/fig3_bifurcation_map.png...")
    plt.figure(figsize=(8.0, 5.5), dpi=400)
    m_grid = np.linspace(0.3, 2.2, 80)
    a_grid = np.linspace(0.012, 0.038, 80)
    matrix_outcome = np.zeros((len(m_grid), len(a_grid)))

    for i, mp_val in enumerate(m_grid):
        for j, a_val in enumerate(a_grid):
            m_crit_val = 0.50 * ((a_val / 0.018)**3.0)
            a_rlof_val = 0.018 * ((mp_val / 0.50)**(1.0 / 3.0))
            if mp_val < m_crit_val:
                val_code = 0  # Zone I: Disruption & Engulfment
            elif a_val <= a_rlof_val:
                val_code = 1  # Zone II: Envelope Stripping Stagnation
            else:
                val_code = 2  # Zone III: Non-Overflow Intact Cooling
            matrix_outcome[i, j] = val_code

    A_mesh, M_mesh = np.meshgrid(a_grid, m_grid)
    from matplotlib.colors import ListedColormap
    cmap_custom = ListedColormap(['#ffb3b3', '#ffe680', '#b3e6b3'])

    plt.pcolormesh(A_mesh,
                   M_mesh,
                   matrix_outcome,
                   cmap=cmap_custom,
                   shading='nearest',
                   alpha=0.90)

    cbar = plt.colorbar(ticks=[0.33, 1.0, 1.67])
    cbar.ax.set_yticklabels([
        'Zone I: Disruption & Engulfment\n(Tidal Mass Loss)',
        'Zone II: Envelope Stripping\nStagnation (Remnant Core)',
        'Zone III: Non-Overflow\nCooling (Intact Giant)'
    ],
                            fontsize=9.5,
                            fontweight='bold')

    a_dense = np.linspace(0.012, 0.038, 100)
    m_crit_analytical = 0.50 * ((a_dense / 0.018)**3.0)
    plt.plot(
        a_dense,
        m_crit_analytical,
        'k--',
        lw=2.8,
        label=r'Roche Disruption Limit $M_{\mathrm{crit}}(a) \propto a^{3.0}$')

    plt.text(0.015,
             1.5,
             "FORBIDDEN REGION FOR GAS GIANTS\n(Hydrodynamic RLOF Stripping)",
             fontsize=9.0,
             fontweight='bold',
             color='darkred',
             ha='center',
             bbox=dict(boxstyle='round,pad=0.4',
                       facecolor='#ffe6e6',
                       edgecolor='red',
                       alpha=0.92))

    plt.annotate(r'$\leftarrow$ FORBIDDEN SIDE',
                 xy=(0.013, 0.8),
                 xytext=(0.021, 0.8),
                 fontsize=10.0,
                 fontweight='bold',
                 color='darkred',
                 arrowprops=dict(arrowstyle='->', color='darkred', lw=2.2))

    plt.xlim(0.012, 0.038)
    plt.ylim(0.3, 2.2)
    plt.xlabel('Initial Semi-Major Axis $a(0)$ [AU]',
               fontsize=11.5,
               fontweight='bold')
    plt.ylabel('Initial Planet Mass $M_p(0)$ [$M_{\\mathrm{Jup}}$]',
               fontsize=11.5,
               fontweight='bold')
    plt.title('2D RLOF Survival Map for Ultra-Short-Period Gas Giants',
              fontsize=12.5,
              fontweight='bold')
    plt.grid(True, linestyle=':', alpha=0.45)
    plt.legend(loc='lower right', fontsize=9.5, framealpha=0.95)

    plt.tight_layout()
    plt.savefig(os.path.join(fig_dir, "fig3_bifurcation_map.png"), dpi=400)
    plt.close()

    # --- Render Figure 7: Empirical Exoplanet Comparison ---
    print("--> Generating paper_rlof/figures/fig4_obs_comparison.png...")
    db_path = "hot_jupiter/data/hot_jupiter.db"
    seed_database_if_empty(db_path)
    conn = get_db_connection(db_path)
    cursor = conn.cursor()
    cursor.execute(
        "SELECT name, mass_jup, semi_major_axis_au, radius_jup FROM exoplanets WHERE semi_major_axis_au IS NOT NULL AND mass_jup IS NOT NULL;"
    )
    rows = cursor.fetchall()
    conn.close()

    gg_a = [r[2] for r in rows if r[1] >= 0.1]
    gg_m = [r[1] for r in rows if r[1] >= 0.1]

    solid_cases = [
        ("TOI-561b", 0.0063, 0.0106),
        ("Kepler-10b", 0.0143, 0.0168),
        ("CoRoT-7b", 0.0151, 0.0172),
        ("55 Cnc e", 0.0251, 0.0154),
        ("K2-229b", 0.0081, 0.0129),
    ]
    solid_m = [sc[1] for sc in solid_cases]
    solid_a = [sc[2] for sc in solid_cases]

    plt.figure(figsize=(8.0, 6.0), dpi=300)
    plt.scatter(gg_a,
                gg_m,
                c='#1f77b4',
                alpha=0.6,
                s=30,
                edgecolors='none',
                label=r'Gas Giants ($M_p \geq 0.1\,M_{\mathrm{J}}$)')
    plt.scatter(
        solid_a,
        solid_m,
        c='#ff7f0e',
        marker='D',
        s=55,
        edgecolors='black',
        lw=0.8,
        label=r'Solid Terrestrial / Stripped Cores ($M_p < 0.1\,M_{\mathrm{J}}$)'
    )

    a_crit_contour = np.linspace(0.008, 0.035, 100)
    m_crit_contour = 0.50 * (a_crit_contour / 0.018)**3.0
    plt.plot(
        a_crit_contour,
        m_crit_contour,
        'r--',
        lw=2.8,
        label=
        r'Theoretical Disruption Line $M_{\mathrm{crit}}(a) \propto a^{3.0}$')

    plt.text(
        0.010,
        0.4,
        "FORBIDDEN REGION FOR GAS GIANTS\n(Hydrodynamic RLOF Stripping $t < 100$ Myr)",
        fontsize=9.5,
        fontweight='bold',
        color='darkred',
        ha='center',
        bbox=dict(boxstyle='round,pad=0.4',
                  facecolor='#ffe6e6',
                  edgecolor='red',
                  alpha=0.9))

    plt.xlim(0.008, 0.06)
    plt.ylim(0.003, 4.0)
    plt.xscale('log')
    plt.yscale('log')
    plt.xlabel('Semi-Major Axis $a$ [AU]', fontsize=11.5, fontweight='bold')
    plt.ylabel('Planetary Mass $M_p$ [$M_{\\mathrm{Jup}}$]',
               fontsize=11.5,
               fontweight='bold')
    plt.title(
        'Empirical Exoplanets Colored by Composition (Gas Giant vs. Solid Core)',
        fontsize=11.5,
        fontweight='bold')
    plt.grid(True, which="both", linestyle="--", alpha=0.35)
    plt.legend(fontsize=9, loc='lower right', framealpha=0.95)

    plt.tight_layout()
    plt.savefig(os.path.join(fig_dir, "fig4_obs_comparison.png"), dpi=300)
    plt.close()

    # --- Render Figure 8: Case Studies of Key USP Planets ---
    print("--> Generating paper_rlof/figures/fig5_usp_cases.png...")
    obs_a = [r[2] for r in rows]
    obs_m = [r[1] for r in rows]
    plt.figure(figsize=(8.0, 6.0), dpi=300)
    plt.scatter(obs_a,
                obs_m,
                c='gray',
                alpha=0.3,
                s=20,
                label='Background Exoplanet Population')

    usp_keys = {
        "WASP-12 b": (0.0229, 1.404, 'red', 'WASP-12b (Decaying towards line)'),
        "WASP-19 b":
            (0.0163, 1.114, 'orange', 'WASP-19b (Stagnated on boundary)'),
        "NGTS-10 b":
            (0.0143, 2.162, 'purple', 'NGTS-10b (Stagnated on boundary)'),
        "TOI-561 b": (0.0106, 0.006, 'blue', 'TOI-561b (Bare Stripped Core)'),
    }

    for (a_k, m_k, color, label_str) in usp_keys.values():
        plt.scatter(a_k,
                    m_k,
                    color=color,
                    s=110,
                    zorder=5,
                    edgecolors='black',
                    label=label_str)
        plt.annotate(label_str, (a_k * 1.08, m_k * 1.05),
                     fontsize=8.5,
                     fontweight='bold',
                     color=color)

    plt.plot(a_crit_contour,
             m_crit_contour,
             'r--',
             lw=2.5,
             label=r'Disruption Limit $M_{\mathrm{crit}}(a)$')

    plt.xlim(0.008, 0.04)
    plt.ylim(0.003, 3.5)
    plt.xscale('log')
    plt.yscale('log')
    plt.xlabel('Semi-Major Axis $a$ [AU]', fontsize=11.5, fontweight='bold')
    plt.ylabel('Planetary Mass $M_p$ [$M_{\\mathrm{Jup}}$]',
               fontsize=11.5,
               fontweight='bold')
    plt.title(
        'Case Studies of Key Ultra-Short-Period Planets Relative to RLOF Limit',
        fontsize=11.5,
        fontweight='bold')
    plt.grid(True, which="both", linestyle="--", alpha=0.35)
    plt.legend(fontsize=8.5, loc='lower right')

    plt.tight_layout()
    plt.savefig(os.path.join(fig_dir, "fig5_usp_cases.png"), dpi=300)
    plt.close()

    print("✅ All figures generated successfully in paper_rlof/figures/!")


if __name__ == "__main__":
    main()
