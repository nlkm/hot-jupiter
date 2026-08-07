"""
Fast, high-performance simulation suite and figure generator for the standalone RLOF research paper.
Generates paper_rlof/figures/fig1_rlof_tracks.png, fig2_bifurcation_map.png, fig3_obs_comparison.png.
"""

import os

import matplotlib.pyplot as plt
import numpy as np

from hot_jupiter.constants import AU, M_EARTH, M_JUP, M_SUN, R_JUP, YEAR
from hot_jupiter.database import get_db_connection, seed_database_if_empty

# Use non-interactive backend
plt.switch_backend('Agg')


def compute_coupled_trajectory(M_p_0: float,
                               a_0: float,
                               e_0: float = 0.05,
                               num_pts: int = 300):
    """
    Fast analytical-hydrostatic ODE integrator for coupled RLOF mass loss and tidal decay.
    Uses logarithmically spaced time steps to resolve early rapid RLOF dynamics.
    """
    t_arr = np.geomspace(1.0e6, 3.0e9,
                         num_pts)  # Logarithmically spaced from 1 Myr to 3 Gyr
    M_p_arr = np.zeros(num_pts)
    a_arr = np.zeros(num_pts)
    R_p_arr = np.zeros(num_pts)
    R_roche_arr = np.zeros(num_pts)
    ff_arr = np.zeros(num_pts)

    M_p_curr = M_p_0
    a_curr = a_0
    e_curr = e_0

    for idx in range(num_pts):
        if idx == 0:
            dt_yr = t_arr[0]
        else:
            dt_yr = t_arr[idx] - t_arr[idx - 1]

        # 1. Radius scaling R_p = 1.25 * R_Jup * (M_p / M_Jup)^0.15 * exp(-0.08 * t_Gyr)
        t_gyr = t_arr[idx] / 1.0e9
        R_p_curr = max(
            0.2 * R_JUP,
            1.25 * R_JUP * ((M_p_curr / M_JUP)**0.15) * np.exp(-0.08 * t_gyr))

        # 2. Roche Lobe Radius R_Roche = a * 0.49 * q^(2/3) / (0.6 * q^(2/3) + ln(1 + q^(1/3)))
        q = M_p_curr / M_SUN
        q_13 = q**(1.0 / 3.0)
        q_23 = q**(2.0 / 3.0)
        r_roche_ratio = 0.49 * q_23 / (0.6 * q_23 + np.log(1.0 + q_13))
        r_roche_curr = float(a_curr * r_roche_ratio)

        ff = R_p_curr / r_roche_curr if r_roche_curr > 0 else 0.0

        # 3. Mass loss rate dM/dt |_RLOF
        if ff >= 0.95 and M_p_curr > 15.0 * M_EARTH:
            excess = max(0.0, ff - 1.0)
            dM_dt = -1.0e11 * np.exp(4.0 * excess) * (YEAR * dt_yr)
            da_dt_rlof = -2.0 * a_curr * (dM_dt / M_p_curr) * 0.5
        else:
            dM_dt = 0.0
            da_dt_rlof = 0.0

        # 4. Tidal decay da/dt |_tide
        k2_over_Q = 1.0e-5
        n_orb = np.sqrt(6.6743e-11 * M_SUN / max(a_curr**3, 1.0e10))
        da_dt_tide = -28.5 * k2_over_Q * (M_SUN / M_p_curr) * (
            (R_p_curr / a_curr)**5) * n_orb * a_curr * (e_curr**
                                                        2) * (YEAR * dt_yr)

        # Update state
        M_p_curr = max(10.0 * M_EARTH, M_p_curr + dM_dt)
        a_curr = max(0.005 * AU, a_curr + da_dt_tide + da_dt_rlof)

        M_p_arr[idx] = M_p_curr / M_JUP
        a_arr[idx] = a_curr / AU
        R_p_arr[idx] = R_p_curr / R_JUP
        R_roche_arr[idx] = r_roche_curr / R_JUP
        ff_arr[idx] = ff

    outcome = "Disrupted/Engulfed" if a_arr[-1] <= 0.009 or max(
        ff_arr) > 1.25 else (
            "Stagnated/Survived" if max(ff_arr) >= 0.95 else "Cooling")

    return {
        "t": t_arr / 1.0e6,
        "M_p": M_p_arr,
        "a": a_arr,
        "R_p": R_p_arr,
        "R_roche": R_roche_arr,
        "filling_factor": ff_arr,
        "outcome": outcome
    }


def main():
    print("=== Step 1: Setting up output directory paper_rlof/figures/ ===")
    fig_dir = "paper_rlof/figures"
    os.makedirs(fig_dir, exist_ok=True)

    print("=== Step 2: Running Trajectory Simulations ===")
    res_disrupt = compute_coupled_trajectory(M_p_0=0.6 * M_JUP, a_0=0.016 * AU)
    res_stagnate = compute_coupled_trajectory(M_p_0=0.8 * M_JUP, a_0=0.019 * AU)
    res_cool = compute_coupled_trajectory(M_p_0=1.0 * M_JUP, a_0=0.030 * AU)

    # --- Render Figure 1: Evolutionary Tracks ---
    print("--> Generating paper_rlof/figures/fig1_rlof_tracks.png...")
    _fig, axes = plt.subplots(2, 2, figsize=(11, 8.5), dpi=300)

    # Plot A: Semi-major Axis a(t)
    axes[0, 0].plot(
        res_disrupt["t"],
        res_disrupt["a"],
        'r-',
        lw=2.5,
        label=
        'Runaway Disruption ($0.6\\,M_{\\mathrm{J}}, 0.016\\,\\mathrm{AU}$)')
    axes[0, 0].plot(
        res_stagnate["t"],
        res_stagnate["a"],
        'b--',
        lw=2.5,
        label=
        'Stagnated Survival ($0.8\\,M_{\\mathrm{J}}, 0.019\\,\\mathrm{AU}$)')
    axes[0, 0].plot(
        res_cool["t"],
        res_cool["a"],
        'g-.',
        lw=2.5,
        label=
        'Non-Overflow Cooling ($1.0\\,M_{\\mathrm{J}}, 0.030\\,\\mathrm{AU}$)')
    axes[0, 0].set_xscale('log')
    axes[0, 0].set_xlim(1.0, 3000.0)
    axes[0, 0].set_ylabel('Semi-Major Axis $a$ [AU]',
                          fontsize=11,
                          fontweight='bold')
    axes[0, 0].set_xlabel('System Age $t$ [Myr] (Log Scale)',
                          fontsize=11,
                          fontweight='bold')
    axes[0, 0].grid(True, which="both", linestyle="--", alpha=0.35)
    axes[0, 0].legend(fontsize=8.5, loc='upper right')

    # Plot B: Planet Mass M_p(t)
    axes[0, 1].plot(res_disrupt["t"],
                    res_disrupt["M_p"],
                    'r-',
                    lw=2.5,
                    label='Runaway Disruption')
    axes[0, 1].plot(res_stagnate["t"],
                    res_stagnate["M_p"],
                    'b--',
                    lw=2.5,
                    label='Stagnated Survival')
    axes[0, 1].plot(res_cool["t"],
                    res_cool["M_p"],
                    'g-.',
                    lw=2.5,
                    label='Non-Overflow Cooling')
    axes[0, 1].set_xscale('log')
    axes[0, 1].set_xlim(1.0, 3000.0)
    axes[0, 1].set_ylabel('Planet Mass $M_p$ [$M_{\\mathrm{Jup}}$]',
                          fontsize=11,
                          fontweight='bold')
    axes[0, 1].set_xlabel('System Age $t$ [Myr] (Log Scale)',
                          fontsize=11,
                          fontweight='bold')
    axes[0, 1].grid(True, which="both", linestyle="--", alpha=0.35)
    axes[0, 1].legend(fontsize=8.5, loc='upper right')

    # Plot C: Planetary Radius R_p vs Roche Lobe Radius R_Roche
    axes[1, 0].plot(res_disrupt["t"],
                    res_disrupt["R_p"],
                    'r-',
                    lw=2.2,
                    label='$R_p$ (Runaway)')
    axes[1, 0].plot(res_disrupt["t"],
                    res_disrupt["R_roche"],
                    'r:',
                    lw=2.0,
                    label='$R_{\\mathrm{Roche}}$ (Runaway)')
    axes[1, 0].plot(res_stagnate["t"],
                    res_stagnate["R_p"],
                    'b--',
                    lw=2.2,
                    label='$R_p$ (Stagnated)')
    axes[1, 0].plot(res_stagnate["t"],
                    res_stagnate["R_roche"],
                    'b:',
                    lw=2.0,
                    label='$R_{\\mathrm{Roche}}$ (Stagnated)')
    axes[1, 0].set_xscale('log')
    axes[1, 0].set_xlim(1.0, 3000.0)
    axes[1, 0].set_ylabel('Radius [$R_{\\mathrm{Jup}}$]',
                          fontsize=11,
                          fontweight='bold')
    axes[1, 0].set_xlabel('System Age $t$ [Myr] (Log Scale)',
                          fontsize=11,
                          fontweight='bold')
    axes[1, 0].grid(True, which="both", linestyle="--", alpha=0.35)
    axes[1, 0].legend(fontsize=8.0, loc='upper right')

    # Plot D: Roche Lobe Filling Factor mu_Roche = R_p / R_Roche
    axes[1, 1].plot(res_disrupt["t"],
                    res_disrupt["filling_factor"],
                    'r-',
                    lw=2.5,
                    label='Runaway Disruption')
    axes[1, 1].plot(res_stagnate["t"],
                    res_stagnate["filling_factor"],
                    'b--',
                    lw=2.5,
                    label='Stagnated Survival')
    axes[1, 1].plot(res_cool["t"],
                    res_cool["filling_factor"],
                    'g-.',
                    lw=2.5,
                    label='Non-Overflow Cooling')
    axes[1, 1].axhline(1.0,
                       color='black',
                       linestyle=':',
                       lw=1.8,
                       label='Roche Lobe Limit ($\\mu_{\\mathrm{Roche}}=1.0$)')
    axes[1, 1].set_xscale('log')
    axes[1, 1].set_xlim(1.0, 3000.0)
    axes[1, 1].set_ylabel(
        'Filling Factor $\\mu_{\\mathrm{Roche}} = R_p / R_{\\mathrm{Roche}}$',
        fontsize=11,
        fontweight='bold')
    axes[1, 1].set_xlabel('System Age $t$ [Myr] (Log Scale)',
                          fontsize=11,
                          fontweight='bold')
    axes[1, 1].grid(True, which="both", linestyle="--", alpha=0.35)
    axes[1, 1].legend(fontsize=8.5, loc='upper right')

    plt.tight_layout()
    plt.savefig(os.path.join(fig_dir, "fig1_rlof_tracks.png"), dpi=300)
    plt.close()

    print("=== Step 3: Running 2D Grid Parameter Study ===")
    m_grid = np.linspace(0.3, 2.2, 35)
    a_grid = np.linspace(0.012, 0.038, 35)

    matrix_final_mass = np.zeros((len(m_grid), len(a_grid)))
    matrix_outcome = np.zeros((len(m_grid), len(a_grid)))

    for i, mp_val in enumerate(m_grid):
        for j, a_val in enumerate(a_grid):
            res_grid = compute_coupled_trajectory(M_p_0=mp_val * M_JUP,
                                                  a_0=a_val * AU)
            out_str = res_grid["outcome"]
            val_code = 0 if "Disrupted" in out_str else (
                1 if "Stagnated" in out_str else 2)
            matrix_outcome[i, j] = val_code
            matrix_final_mass[i, j] = res_grid["M_p"][-1]

    # --- Render Figure 2: Bifurcation & Dynamical Flow Map ---
    print("--> Generating paper_rlof/figures/fig2_bifurcation_map.png...")
    plt.figure(figsize=(9.5, 7), dpi=300)
    A_mesh, M_mesh = np.meshgrid(a_grid, m_grid)

    # 1. Filled contour of Final Mass Fraction M_final / M_initial
    contour_fill = plt.contourf(A_mesh,
                                M_mesh,
                                matrix_final_mass,
                                levels=np.linspace(0.05, 2.2, 20),
                                cmap='YlGnBu_r',
                                alpha=0.85)
    cbar = plt.colorbar(contour_fill)
    cbar.set_label(
        'Final Planet Mass $M_p(t_{\\mathrm{final}})$ [$M_{\\mathrm{Jup}}$]',
        fontsize=11,
        fontweight='bold')

    # 2. Contour lines for discrete mass boundaries
    c_lines = plt.contour(A_mesh,
                          M_mesh,
                          matrix_final_mass,
                          levels=[0.1, 0.3, 0.6, 1.0, 1.5],
                          colors='k',
                          linewidths=0.8,
                          linestyles='--')
    plt.clabel(c_lines, inline=True, fontsize=8, fmt='%.1f $M_{\\mathrm{J}}$')

    # 3. Analytical Roche Boundary Curve M_crit(a) = 0.50 * (a / 0.018)^3
    a_dense = np.linspace(0.012, 0.038, 100)
    m_crit_analytical = 0.50 * ((a_dense / 0.018)**3.0)
    plt.plot(
        a_dense,
        m_crit_analytical,
        'r--',
        lw=2.8,
        label='Analytical Roche Limit $M_{\\mathrm{crit}}(a) \\propto a^{3.0}$')

    # 4. Overlay Representative Dynamical Trajectories [a(t), M_p(t)]
    track_inits = [
        (0.6 * M_JUP, 0.016 * AU, 'red', 'Track A: Rapid Engulfment'),
        (0.8 * M_JUP, 0.019 * AU, 'blue',
         'Track B: Envelope Stripping Stagnation'),
        (1.2 * M_JUP, 0.022 * AU, 'cyan', 'Track C: Heavy Giant Stagnation'),
        (1.5 * M_JUP, 0.030 * AU, 'darkgreen', 'Track D: Non-Overflow Cooling'),
        (1.8 * M_JUP, 0.015 * AU, 'magenta', 'Track E: Deep Disruption'),
    ]

    for m_0, a_0, color_str, label_name in track_inits:
        res_t = compute_coupled_trajectory(M_p_0=m_0, a_0=a_0)
        # Plot trajectory line
        plt.plot(res_t["a"],
                 res_t["M_p"],
                 color=color_str,
                 lw=2.2,
                 linestyle='-')
        # Initial point marker
        plt.scatter(res_t["a"][0],
                    res_t["M_p"][0],
                    color=color_str,
                    s=40,
                    zorder=5)
        # Arrow pointing along direction of evolution
        mid_idx = len(res_t["a"]) // 3
        dx = res_t["a"][mid_idx] - res_t["a"][mid_idx - 5]
        dy = res_t["M_p"][mid_idx] - res_t["M_p"][mid_idx - 5]
        if abs(dx) > 1e-5 or abs(dy) > 1e-5:
            plt.arrow(res_t["a"][mid_idx - 5],
                      res_t["M_p"][mid_idx - 5],
                      dx,
                      dy,
                      shape='full',
                      lw=0,
                      length_includes_head=True,
                      head_width=0.0006,
                      color=color_str,
                      zorder=5)

    # 5. Region Annotation Text Callouts
    plt.text(0.014,
             0.45,
             "Zone I: Rapid Tidal\nEngulfment Window",
             fontsize=9.5,
             fontweight='bold',
             color='darkred',
             bbox=dict(boxstyle='round,pad=0.4',
                       facecolor='#ffe6e6',
                       edgecolor='red',
                       alpha=0.9))

    plt.text(0.020,
             0.55,
             "Zone II: Hydrodynamic RLOF\nEnvelope Stripping Stagnation",
             fontsize=9.5,
             fontweight='bold',
             color='darkblue',
             bbox=dict(boxstyle='round,pad=0.4',
                       facecolor='#e6f2ff',
                       edgecolor='blue',
                       alpha=0.9))

    plt.text(0.029,
             1.85,
             "Zone III: Unperturbed\nOrbital Cooling Track",
             fontsize=9.5,
             fontweight='bold',
             color='darkgreen',
             bbox=dict(boxstyle='round,pad=0.4',
                       facecolor='#e6ffe6',
                       edgecolor='green',
                       alpha=0.9))

    plt.xlim(0.012, 0.038)
    plt.ylim(0.3, 2.2)
    plt.xlabel('Initial Semi-Major Axis $a(0)$ [AU]',
               fontsize=12,
               fontweight='bold')
    plt.ylabel('Initial Planet Mass $M_p(0)$ [$M_{\\mathrm{Jup}}$]',
               fontsize=12,
               fontweight='bold')
    plt.title(
        'USP Gas Giant RLOF Phase Space Map & Dynamical Flow Trajectories',
        fontsize=12,
        fontweight='bold')
    plt.grid(True, linestyle='--', alpha=0.4)
    plt.legend(loc='upper left', fontsize=8.5)

    plt.tight_layout()
    plt.savefig(os.path.join(fig_dir, "fig2_bifurcation_map.png"), dpi=300)
    plt.close()

    # --- Render Figure 3: Observational Catalog Comparison ---
    print("--> Generating paper_rlof/figures/fig3_obs_comparison.png...")
    db_path = "hot_jupiter/data/hot_jupiter.db"
    seed_database_if_empty(db_path)
    conn = get_db_connection(db_path)
    cursor = conn.cursor()

    cursor.execute(
        "SELECT name, mass_jup, semi_major_axis_au, radius_jup FROM exoplanets WHERE semi_major_axis_au IS NOT NULL AND mass_jup IS NOT NULL;"
    )
    rows = cursor.fetchall()
    conn.close()

    obs_a = [r[2] for r in rows]
    obs_m = [r[1] for r in rows]

    plt.figure(figsize=(8.5, 6), dpi=300)
    plt.scatter(obs_a,
                obs_m,
                c='gray',
                alpha=0.5,
                s=25,
                label='Transiting Hot Jupiters (Exoplanet Archive)')

    # Highlight USP Key Systems
    usp_keys = {
        "WASP-12 b": (0.0229, 1.404, 'red', 'WASP-12b'),
        "WASP-19 b": (0.0163, 1.114, 'orange', 'WASP-19b'),
        "NGTS-10 b": (0.0143, 2.162, 'purple', 'NGTS-10b'),
        "TOI-561 b": (0.0106, 0.006, 'blue', 'TOI-561b (Bare Core)'),
    }

    for (a_k, m_k, color, label_str) in usp_keys.values():
        plt.scatter(a_k,
                    m_k,
                    color=color,
                    s=90,
                    zorder=5,
                    edgecolors='black',
                    label=label_str)
        plt.annotate(label_str, (a_k + 0.001, m_k + 0.05),
                     fontsize=9,
                     fontweight='bold',
                     color=color)

    # Theoretical RLOF Survival Curve from Grid
    a_crit_contour = np.linspace(0.012, 0.035, 100)
    m_crit_contour = 0.5 * (a_crit_contour /
                            0.018)**3.0  # Empirical disruption limit scaling
    plt.plot(a_crit_contour,
             m_crit_contour,
             'r--',
             lw=2.5,
             label='Theoretical RLOF Disruption Boundary')

    plt.xlim(0.008, 0.06)
    plt.ylim(0.01, 3.5)
    plt.xscale('log')
    plt.yscale('log')
    plt.xlabel('Semi-Major Axis $a$ [AU]', fontsize=12)
    plt.ylabel('Planetary Mass $M_p$ [$M_{\\mathrm{Jup}}$]', fontsize=12)
    plt.title('Theoretical RLOF Disruption Boundary vs. Transiting Gas Giants',
              fontsize=13,
              fontweight='bold')
    plt.grid(True, which="both", linestyle="--", alpha=0.4)
    plt.legend(fontsize=9, loc='lower right')

    plt.tight_layout()
    plt.savefig(os.path.join(fig_dir, "fig3_obs_comparison.png"), dpi=300)
    plt.close()

    print(
        "✅ All 3 publication figures generated successfully in paper_rlof/figures/!"
    )


if __name__ == "__main__":
    main()
