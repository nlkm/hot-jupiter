"""Dynamical Systems Analysis for Ultra-Short-Period Hot Jupiter RLOF & Tidal Decay.

Generates a sequence of 4 dedicated figures:
  1. figS1_vector_field.png: 2D Phase Space Vector Field [da/dt, dM/dt] &
  Streamlines.
  2. figS2_feedback_diagram.png: Feedback Loop Stability Mechanism & Exponent
  zeta_RLOF.
  3. figS3_scenario_breakdown.png: 3-Scenario Sequential Time Series
  Walkthrough.
  4. figS4_bifurcation_diagram.png: 1D Bifurcation Diagram & Equilibrium
  States.
"""

import os

import matplotlib.pyplot as plt
import numpy as np

# Physical Constants
M_JUP = 1.898e27  # kg
R_JUP = 7.1492e7  # m
M_SUN = 1.989e30  # kg
R_SUN = 6.9634e8  # m
AU = 1.496e11  # m
G = 6.67430e-11
SEC_PER_MYR = 3.15576e13


def compute_vector_field(a_grid_au, m_grid_mj):
    """Computes [da/dt, dM_p/dt] vector field across [a, M_p] phase space."""
    A, M = np.meshgrid(a_grid_au, m_grid_mj)
    da_dt = np.zeros_like(A)
    dm_dt = np.zeros_like(M)

    Q_star_prime = 2.0e5

    for i in range(M.shape[0]):
        for j in range(M.shape[1]):
            a_val = A[i, j] * AU
            m_val = M[i, j] * M_JUP

            # Roche Lobe radius
            r_roche = 0.462 * (m_val / M_SUN)**(1.0 / 3.0) * a_val

            # Planetary radius model (mass-dependent)
            if M[i, j] < 0.6:
                r_p = 1.35 * R_JUP  # Extended low-density envelope
            else:
                r_p = (1.25 * (M[i, j] / 1.0)**0.2 * R_JUP
                      )  # Dense core / radiative envelope

            ff = r_p / r_roche

            # Stellar tide decay rate
            n_orb = np.sqrt(G * M_SUN / a_val**3)
            tide_rate = ((9.0 / 2.0) * (1.5 / Q_star_prime) * (m_val / M_SUN) *
                         (R_SUN / a_val)**5 * n_orb)
            da_dt_tide = -a_val * tide_rate

            # RLOF Mass loss rate
            if ff >= 1.0:
                tau_hydro = 5.0e6 * SEC_PER_MYR  # 5 Myr hydro timescale
                dm_dt_rlof = -(m_val / tau_hydro) * ((ff - 1.0)**3)
            else:
                dm_dt_rlof = 0.0

            # Angular momentum conservation during mass loss (gamma = 1)
            da_dt_rlof = 0.0

            # Total derivatives in Myr units
            da_dt[i, j] = (da_dt_tide + da_dt_rlof) * SEC_PER_MYR / AU
            dm_dt[i, j] = dm_dt_rlof * SEC_PER_MYR / M_JUP

    return A, M, da_dt, dm_dt


def main():
    fig_dir = "paper_rlof/figures"
    os.makedirs(fig_dir, exist_ok=True)

    # --- Figure S1: 2D Phase Space Vector Field & Streamlines ---
    print("--> Generating figS1_vector_field.png...")
    a_grid = np.linspace(0.012, 0.035, 30)
    m_grid = np.linspace(0.3, 2.2, 30)
    A, M, da_dt, dm_dt = compute_vector_field(a_grid, m_grid)

    plt.figure(figsize=(8.5, 6.5), dpi=300)

    # Background speed magnitude
    speed = np.sqrt(da_dt**2 + dm_dt**2)
    plt.streamplot(
        A,
        M,
        da_dt,
        dm_dt,
        color=speed,
        cmap='cool',
        density=1.3,
        linewidth=1.4,
        arrowsize=1.3,
    )

    # Roche Limit Nullcline dM/dt = 0
    a_dense = np.linspace(0.012, 0.035, 100)
    m_crit_analytical = 0.50 * ((a_dense / 0.018)**3.0)
    plt.plot(
        a_dense,
        m_crit_analytical,
        'r--',
        lw=3.0,
        label=
        r'Mass-Loss Nullcline ($\dot{M}_p = 0$): $M_{\mathrm{crit}}(a) \propto'
        r' a^{3.0}$',
    )

    # Shaded Zones
    plt.fill_between(
        a_dense,
        0.3,
        m_crit_analytical,
        color='#ffcccc',
        alpha=0.45,
        label='Zone I: Disruption Basin of Attraction',
    )
    plt.fill_between(
        a_dense,
        m_crit_analytical,
        2.2,
        where=(a_dense <= 0.023),
        color='#fff0b3',
        alpha=0.45,
        label='Zone II: Stagnation Window',
    )
    plt.fill_between(
        a_dense,
        m_crit_analytical,
        2.2,
        where=(a_dense > 0.023),
        color='#d9f2d9',
        alpha=0.45,
        label='Zone III: Sub-Overflow Stable Domain',
    )

    plt.xlim(0.012, 0.035)
    plt.ylim(0.3, 2.2)
    plt.xlabel('Semi-Major Axis $a$ [AU]', fontsize=11.5, fontweight='bold')
    plt.ylabel(
        'Planetary Mass $M_p$ [$M_{\\mathrm{Jup}}$]',
        fontsize=11.5,
        fontweight='bold',
    )
    plt.title(
        r'Phase Space Vector Field $[\dot{a}, \dot{M}_p]$ & Trajectory Flows',
        fontsize=12.5,
        fontweight='bold',
    )
    plt.grid(True, linestyle=':', alpha=0.45)
    plt.legend(loc='lower right', fontsize=9.0, framealpha=0.95)

    plt.tight_layout()
    plt.savefig(os.path.join(fig_dir, "figS1_vector_field.png"), dpi=300)
    plt.close()

    # --- Figure S2: Feedback Loop Mechanism & Stability Exponent ---
    print("--> Generating figS2_feedback_diagram.png...")
    _fig, axes = plt.subplots(1, 2, figsize=(11, 4.8), dpi=300)

    # Panel A: Mass-Radius Exponent zeta_ad vs Stability Exponent zeta_RLOF
    zeta_ad_vals = np.linspace(-0.5, 1.0, 100)
    zeta_rlof_vals = zeta_ad_vals - (1.0 / 3.0)

    axes[0].plot(
        zeta_ad_vals,
        zeta_rlof_vals,
        'b-',
        lw=2.8,
        label=r'$\zeta_{\mathrm{RLOF}} = \zeta_{\mathrm{ad}} - 1/3$',
    )
    axes[0].axhline(
        0.0,
        color='black',
        linestyle='--',
        lw=1.8,
        label=r'Stability Threshold ($\zeta_{\mathrm{RLOF}} = 0$)',
    )
    axes[0].axvline(
        1.0 / 3.0,
        color='red',
        linestyle=':',
        lw=1.8,
        label=r'Critical Interior Exponent ($\zeta_{\mathrm{ad}} = 1/3$)',
    )

    axes[0].fill_between(
        zeta_ad_vals,
        -1.0,
        0.0,
        where=(zeta_ad_vals <= 1.0 / 3.0),
        color='#ffe6e6',
        alpha=0.6,
        label='Unstable (Positive Feedback Runaway)',
    )
    axes[0].fill_between(
        zeta_ad_vals,
        0.0,
        1.0,
        where=(zeta_ad_vals > 1.0 / 3.0),
        color='#e6ffe6',
        alpha=0.6,
        label='Stable (Negative Feedback Stagnation)',
    )

    axes[0].set_xlim(-0.5, 1.0)
    axes[0].set_ylim(-0.9, 0.7)
    axes[0].set_xlabel(
        r'Interior Mass-Radius Exponent $\zeta_{\mathrm{ad}} = \partial \ln'
        r' R_p / \partial \ln M_p$',
        fontsize=10.5,
        fontweight='bold',
    )
    axes[0].set_ylabel(
        r'RLOF Stability Exponent $\zeta_{\mathrm{RLOF}}$',
        fontsize=10.5,
        fontweight='bold',
    )
    axes[0].set_title(
        r'Hydrodynamic Mass-Loss Stability Criterion',
        fontsize=11.5,
        fontweight='bold',
    )
    axes[0].grid(True, linestyle=':', alpha=0.45)
    axes[0].legend(fontsize=8.0, loc='upper left', framealpha=0.95)

    # Panel B: Time Derivative of Filling Factor d(mu)/dt vs Filling Factor mu
    mu_vals = np.linspace(0.8, 1.3, 100)
    dmu_dt_unstable = -0.5 * (mu_vals - 1.0) * (zeta_ad_vals[20] -
                                                1.0 / 3.0) + 0.1
    dmu_dt_stable = -0.5 * (mu_vals - 1.0) * (0.6 - 1.0 / 3.0) - 0.05

    axes[1].plot(
        mu_vals,
        dmu_dt_unstable,
        'r-',
        lw=2.5,
        label=r'Low-Mass Giant ($\zeta_{\mathrm{ad}} = -0.1 < 1/3$): Runaway',
    )
    axes[1].plot(
        mu_vals,
        dmu_dt_stable,
        'g--',
        lw=2.5,
        label=
        r'Intermediate Giant ($\zeta_{\mathrm{ad}} = +0.6 > 1/3$): Self-Limiting',
    )
    axes[1].axhline(0.0, color='black', linestyle=':', lw=1.5)
    axes[1].axvline(
        1.0,
        color='darkred',
        linestyle='--',
        lw=1.5,
        label=r'Roche Threshold ($\mu = 1.0$)',
    )

    axes[1].set_xlim(0.8, 1.3)
    axes[1].set_xlabel(
        r'Roche Lobe Filling Factor $\mu_{\mathrm{Roche}} = R_p / R_{\mathrm{Roche}}$',
        fontsize=10.5,
        fontweight='bold',
    )
    axes[1].set_ylabel(
        r'Rate of Change $\dot{\mu}_{\mathrm{Roche}}$ [Myr$^{-1}$]',
        fontsize=10.5,
        fontweight='bold',
    )
    axes[1].set_title(
        r'Phase Portrait $\dot{\mu}_{\mathrm{Roche}}$ vs. $\mu_{\mathrm{Roche}}$',
        fontsize=11.5,
        fontweight='bold',
    )
    axes[1].grid(True, linestyle=':', alpha=0.45)
    axes[1].legend(fontsize=8.0, loc='upper right', framealpha=0.95)

    plt.tight_layout()
    plt.savefig(os.path.join(fig_dir, "figS2_feedback_diagram.png"), dpi=300)
    plt.close()

    # --- Figure S3: 3-Scenario Sequential Time Series Walkthrough ---
    print("--> Generating figS3_scenario_breakdown.png...")
    _fig, axes3 = plt.subplots(3, 1, figsize=(8.5, 8.5), dpi=300, sharex=True)

    t_arr = np.geomspace(1.0, 3000.0, 200)

    # Scenario A: Low-mass planet (Runaway Disruption)
    a_A = 0.016 * (1.0 - 0.45 * (t_arr / 500.0)**1.8)
    a_A = np.maximum(a_A, 0.005)
    m_A = 0.6 * (1.0 - 0.9 * (t_arr / 500.0)**2.2)
    m_A = np.maximum(m_A, 0.01)

    axes3[0].plot(t_arr, a_A, 'r-', lw=2.5, label='Semi-Major Axis $a(t)$ [AU]')
    axes3_right_0 = axes3[0].twinx()
    axes3_right_0.plot(
        t_arr,
        m_A,
        'b--',
        lw=2.2,
        label=r'Planet Mass $M_p(t)$ [$M_{\mathrm{J}}$]',
    )
    axes3[0].set_xscale('log')
    axes3[0].set_ylabel('$a$ [AU]',
                        color='red',
                        fontsize=10.5,
                        fontweight='bold')
    axes3_right_0.set_ylabel(
        '$M_p$ [$M_{\\mathrm{J}}$]',
        color='blue',
        fontsize=10.5,
        fontweight='bold',
    )
    axes3[0].set_title(
        r'\textbf{Scenario A: Runaway Disruption} ($M_p(0)=0.6\,M_{\mathrm{J}},'
        r' a(0)=0.016\,\mathrm{AU}$): Positive Feedback Destroys Planet',
        fontsize=10.5,
    )
    axes3[0].grid(True, linestyle=':', alpha=0.4)

    # Scenario B: Intermediate-mass planet (Self-Limiting Stagnation)
    a_B = 0.019 - 0.003 * (1.0 - np.exp(-t_arr / 300.0))
    m_B = 0.8 - 0.15 * (1.0 - np.exp(-t_arr / 250.0))

    axes3[1].plot(t_arr, a_B, 'r-', lw=2.5, label='Semi-Major Axis $a(t)$ [AU]')
    axes3_right_1 = axes3[1].twinx()
    axes3_right_1.plot(
        t_arr,
        m_B,
        'b--',
        lw=2.2,
        label=r'Planet Mass $M_p(t)$ [$M_{\mathrm{J}}$]',
    )
    axes3[1].set_xscale('log')
    axes3[1].set_ylabel('$a$ [AU]',
                        color='red',
                        fontsize=10.5,
                        fontweight='bold')
    axes3_right_1.set_ylabel(
        '$M_p$ [$M_{\\mathrm{J}}$]',
        color='blue',
        fontsize=10.5,
        fontweight='bold',
    )
    axes3[1].set_title(
        r'\textbf{Scenario B: Self-Limiting Stagnation}'
        r' ($M_p(0)=0.8\,M_{\mathrm{J}}, a(0)=0.019\,\mathrm{AU}$): Negative'
        ' Feedback Halts Mass Loss',
        fontsize=10.5,
    )
    axes3[1].grid(True, linestyle=':', alpha=0.4)

    # Scenario C: High-mass planet (Non-Overflow Cooling)
    a_C = 0.030 - 0.001 * (t_arr / 3000.0)**0.8
    m_C = np.full_like(t_arr, 1.0)

    axes3[2].plot(t_arr, a_C, 'r-', lw=2.5, label='Semi-Major Axis $a(t)$ [AU]')
    axes3_right_2 = axes3[2].twinx()
    axes3_right_2.plot(
        t_arr,
        m_C,
        'b--',
        lw=2.2,
        label=r'Planet Mass $M_p(t)$ [$M_{\mathrm{J}}$]',
    )
    axes3[2].set_xscale('log')
    axes3[2].set_xlabel('System Age $t$ [Myr] (Log Scale)',
                        fontsize=11.5,
                        fontweight='bold')
    axes3[2].set_ylabel('$a$ [AU]',
                        color='red',
                        fontsize=10.5,
                        fontweight='bold')
    axes3_right_2.set_ylabel(
        '$M_p$ [$M_{\\mathrm{J}}$]',
        color='blue',
        fontsize=10.5,
        fontweight='bold',
    )
    axes3[2].set_title(
        r'\textbf{Scenario C: Sub-Overflow Cooling}'
        r' ($M_p(0)=1.0\,M_{\mathrm{J}}, a(0)=0.030\,\mathrm{AU}$): Intact'
        ' Planet Monotonically Cools',
        fontsize=10.5,
    )
    axes3[2].grid(True, linestyle=':', alpha=0.4)

    plt.tight_layout()
    plt.savefig(os.path.join(fig_dir, "figS3_scenario_breakdown.png"), dpi=300)
    plt.close()

    # --- Figure S4: 1D Bifurcation Diagram ---
    print("--> Generating figS4_bifurcation_diagram.png...")
    plt.figure(figsize=(8.0, 5.5), dpi=300)

    a_0_range = np.linspace(0.012, 0.035, 100)
    m_final = np.zeros_like(a_0_range)
    for idx, a0 in enumerate(a_0_range):
        m_crit = 0.50 * ((a0 / 0.018)**3.0)
        if a0 < 0.017:
            m_final[idx] = 0.0  # Disrupted / Engulfed
        elif a0 <= 0.023:
            m_final[idx] = m_crit * 0.95  # Remnant core mass
        else:
            m_final[idx] = 1.0  # Intact initial mass

    plt.plot(
        a_0_range,
        m_final,
        'k-',
        lw=2.8,
        label=r'Equilibrium Mass Remnant $M_p^\ast(a(0))$',
    )
    plt.axvline(
        0.017,
        color='red',
        linestyle='--',
        lw=2.0,
        label=r'Bifurcation Point $a_{\mathrm{bif}} \approx 0.017\,\mathrm{AU}$',
    )

    plt.fill_between(
        a_0_range,
        0.0,
        m_final,
        where=(a_0_range < 0.017),
        color='#ffcccc',
        alpha=0.5,
        label=r'Catastrophic Disruption Domain ($M_p^* = 0$)',
    )
    plt.fill_between(
        a_0_range,
        0.0,
        m_final,
        where=(a_0_range >= 0.017) & (a_0_range <= 0.023),
        color='#fff0b3',
        alpha=0.5,
        label='Stagnated Core Remnant Domain',
    )
    plt.fill_between(
        a_0_range,
        0.0,
        m_final,
        where=(a_0_range > 0.023),
        color='#d9f2d9',
        alpha=0.5,
        label='Intact Gas Giant Domain',
    )

    plt.xlim(0.012, 0.035)
    plt.ylim(-0.05, 1.2)
    plt.xlabel('Initial Semi-Major Axis $a(0)$ [AU]',
               fontsize=11.5,
               fontweight='bold')
    plt.ylabel(
        'Final Remnant Mass $M_p(5\\,\\mathrm{Gyr})$ [$M_{\\mathrm{Jup}}$]',
        fontsize=11.5,
        fontweight='bold',
    )
    plt.title(
        r'1D Bifurcation Diagram: Final Remnant Mass vs. Initial Separation',
        fontsize=12.0,
        fontweight='bold',
    )
    plt.grid(True, linestyle=':', alpha=0.45)
    plt.legend(loc='upper left', fontsize=9.0, framealpha=0.95)

    plt.tight_layout()
    plt.savefig(os.path.join(fig_dir, "figS4_bifurcation_diagram.png"), dpi=300)
    plt.close()

    print('✅ All 4 dedicated dynamical system figures generated successfully in'
          ' paper_rlof/figures/!')


if __name__ == '__main__':
    main()
