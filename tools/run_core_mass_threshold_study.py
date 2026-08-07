"""
Simulation suite and figure generator for the Critical Core Mass Threshold study.
Answers the scientific question: What minimum core mass M_core,crit is required
for a RLOF Hot Jupiter to survive as a stable USP sub-Neptune / super-Earth core remnant?

Outputs:
  paper_rlof/figures/fig6_core_mass_remnants.png
  paper_rlof/figures/fig7_core_threshold_scaling.png
  paper_rlof/figures/fig8_heavy_element_enrichment.png
"""

import os

import matplotlib.pyplot as plt
import numpy as np

from hot_jupiter.constants import AU, M_EARTH, M_JUP, M_SUN, R_EARTH, R_JUP

# Non-interactive backend
plt.switch_backend('Agg')

OUT_DIR = 'paper_rlof/figures'
os.makedirs(OUT_DIR, exist_ok=True)


def simulate_core_survival(m_core_earth: float,
                           m_env_init_jup: float,
                           a_0_au: float,
                           num_pts: int = 400):
    """
    Simulates coupled RLOF envelope stripping and tidal orbital decay
    for a planet with explicit core mass M_core.
    Returns final remnant mass (M_Earth) and bulk heavy element fraction Z_bulk.
    """
    m_core_kg = m_core_earth * M_EARTH
    m_env_kg = m_env_init_jup * M_JUP
    m_total_kg = m_core_kg + m_env_kg

    a_curr = a_0_au * AU

    t_arr = np.geomspace(1.0e6, 5.0e9, num_pts)  # 1 Myr to 5 Gyr

    for idx in range(num_pts):
        if idx == 0:
            dt_yr = t_arr[0]
        else:
            dt_yr = t_arr[idx] - t_arr[idx - 1]

        t_gyr = t_arr[idx] / 1.0e9

        # Core radius equation of state: R_core ~ 1.0 * R_Earth * (M_core / M_Earth)^0.27
        r_core = 1.0 * R_EARTH * ((m_core_earth / 1.0)**0.27)

        # Envelope radius equation of state with cooling & inflation
        if m_env_kg > 0.1 * M_EARTH:
            r_env = 1.25 * R_JUP * ((
                (m_env_kg / M_JUP))**0.15) * np.exp(-0.08 * t_gyr)
            r_p_curr = max(r_core, r_env)
        else:
            r_p_curr = r_core

        # Roche Lobe Radius R_Roche = a * 0.49 * q^(2/3) / (0.6 * q^(2/3) + ln(1 + q^(1/3)))
        q = m_total_kg / M_SUN
        q_13 = q**(1.0 / 3.0)
        q_23 = q**(2.0 / 3.0)
        r_roche_ratio = 0.49 * q_23 / (0.6 * q_23 + np.log(1.0 + q_13))
        r_roche_curr = a_curr * r_roche_ratio

        ff = r_p_curr / r_roche_curr if r_roche_curr > 0 else 0.0

        # Check for core disruption condition:
        # If filling factor exceeds 1.0 at core boundary (r_p == r_core), core self-gravity is exceeded
        if r_p_curr == r_core and ff >= 1.0:
            return 0.0, 0.0  # Total Hydrodynamic Core Disruption

        # Envelope mass loss rate
        if ff >= 0.95 and m_env_kg > 0.0:
            m_dot_0 = 5.0e-8 * M_JUP  # kg/yr
            m_dot = m_dot_0 * np.exp(4.0 * (ff - 1.0))
            loss_kg = m_dot * dt_yr

            if loss_kg >= m_env_kg:
                m_env_kg = 0.0
            else:
                m_env_kg -= loss_kg

            m_total_kg = m_core_kg + m_env_kg

            # Mass loss orbital evolution
            da_rlof = -2.0 * a_curr * (-loss_kg / m_total_kg) * 0.5
            a_curr += da_rlof

        # Stellar tidal orbital decay da/dt |_tide
        k2_Q_star = 1.5e-5  # Q_*' = 1.5e5
        da_tide = -9.0 * k2_Q_star * np.sqrt(1.327e20) * (6.957e8**5) * (
            m_total_kg / M_SUN) * (a_curr**(-5.5)) * dt_yr * 3.154e7
        a_curr += da_tide

        if a_curr <= 0.008 * AU:
            return 0.0, 0.0  # Engulfed by star

    final_m_earth = m_total_kg / M_EARTH
    z_bulk = m_core_kg / m_total_kg if m_total_kg > 0 else 0.0
    return final_m_earth, z_bulk


def main():
    print("=== Step 1: Running Core Mass Threshold Sweep ===")

    m_core_grid = np.linspace(1.0, 25.0, 60)  # Core mass from 1 to 25 M_Earth
    a_0_list = [0.015, 0.018, 0.022, 0.026]  # AU

    results = {}

    for a_0 in a_0_list:
        remnants = []
        z_bulks = []
        for m_core in m_core_grid:
            m_rem, z_b = simulate_core_survival(m_core_earth=m_core,
                                                m_env_init_jup=0.8,
                                                a_0_au=a_0)
            remnants.append(m_rem)
            z_bulks.append(z_b)
        results[a_0] = (np.array(remnants), np.array(z_bulks))

    # --- Figure 6: Remnant Mass vs. Core Mass ---
    print("--> Generating paper_rlof/figures/fig6_core_mass_remnants.png...")
    plt.figure(figsize=(7.5, 5.0), dpi=300)

    colors = ['#d95f02', '#7570b3', '#1b9e77', '#e7298a']
    for idx, a_0 in enumerate(a_0_list):
        rems, _ = results[a_0]
        plt.plot(m_core_grid,
                 rems,
                 label=f'$a(0) = {a_0:.3f}$ AU',
                 color=colors[idx],
                 linewidth=2.5)

    plt.axhline(0, color='gray', linestyle=':', alpha=0.7)
    plt.xlabel(
        'Initial Planetary Core Mass $M_{\\mathrm{core}} \\, [M_{\\mathrm{Earth}}]$',
        fontsize=11)
    plt.ylabel(
        'Final Remnant Mass $M_{\\mathrm{remnant}} \\, [M_{\\mathrm{Earth}}]$ (at $t=5$ Gyr)',
        fontsize=11)
    plt.title(
        'Bifurcation in Core Survival: Critical Core Mass Threshold $M_{\\mathrm{core, crit}}$',
        fontsize=12,
        fontweight='bold')
    plt.grid(True, linestyle='--', alpha=0.4)
    plt.legend(frameon=True, facecolor='white', framealpha=0.9, fontsize=10)
    plt.tight_layout()
    plt.savefig(f'{OUT_DIR}/fig6_core_mass_remnants.png')
    plt.close()

    # --- Figure 7: Critical Core Mass Scaling Law M_core,crit(a) ---
    print(
        "--> Generating paper_rlof/figures/fig7_core_threshold_scaling.png...")
    a_dense_grid = np.linspace(0.013, 0.030, 50)
    m_core_crit_list = []

    for a_val in a_dense_grid:
        # Find minimum m_core for survival
        m_crit = 25.0
        for m_c in np.linspace(0.5, 25.0, 100):
            m_rem, _ = simulate_core_survival(m_core_earth=m_c,
                                              m_env_init_jup=0.8,
                                              a_0_au=a_val)
            if m_rem > 0:
                m_crit = m_c
                break
        m_core_crit_list.append(m_crit)

    m_core_crit_arr = np.array(m_core_crit_list)

    plt.figure(figsize=(7.5, 5.0), dpi=300)
    plt.plot(a_dense_grid,
             m_core_crit_arr,
             'r-',
             linewidth=3.0,
             label='Critical Core Boundary $M_{\\mathrm{core, crit}}(a)$')
    plt.fill_between(
        a_dense_grid,
        0,
        m_core_crit_arr,
        color='#ffb3b3',
        alpha=0.6,
        label='Zone of Core Disruption ($M_{\\mathrm{remnant}} = 0$)')
    plt.fill_between(
        a_dense_grid,
        m_core_crit_arr,
        25.0,
        color='#b3e6b3',
        alpha=0.4,
        label='Zone of Core Survival ($M_{\\mathrm{remnant}} > 0$)')

    plt.xlabel('Orbital Separation $a \\, [\\mathrm{AU}]$', fontsize=11)
    plt.ylabel(
        'Minimum Core Mass $M_{\\mathrm{core, crit}} \\, [M_{\\mathrm{Earth}}]$',
        fontsize=11)
    plt.title('Critical Survival Boundary for USP Bare Remnant Cores',
              fontsize=12,
              fontweight='bold')
    plt.grid(True, linestyle='--', alpha=0.4)
    plt.legend(frameon=True,
               facecolor='white',
               framealpha=0.9,
               fontsize=10,
               loc='upper right')
    plt.tight_layout()
    plt.savefig(f'{OUT_DIR}/fig7_core_threshold_scaling.png')
    plt.close()

    # --- Figure 8: Heavy Element Fraction Z_bulk Fingerprint ---
    print(
        "--> Generating paper_rlof/figures/fig8_heavy_element_enrichment.png..."
    )
    plt.figure(figsize=(7.5, 5.0), dpi=300)

    for idx, a_0 in enumerate([0.015, 0.018, 0.022]):
        rems, z_bulks = results[a_0]
        # Filter valid non-zero remnants
        valid = rems > 0
        plt.scatter(rems[valid],
                    z_bulks[valid],
                    label=f'$a(0) = {a_0:.3f}$ AU',
                    color=colors[idx],
                    s=40,
                    alpha=0.8)

    plt.axhline(0.30,
                color='black',
                linestyle='--',
                linewidth=1.5,
                label='Primordial Terrestrial Baseline ($Z \\approx 0.30$)')
    plt.xlabel('Final Planet Mass $M_p \\, [M_{\\mathrm{Earth}}]$', fontsize=11)
    plt.ylabel(
        'Bulk Heavy-Element Fraction $Z_{\\mathrm{bulk}} = M_{\\mathrm{core}} / M_{\\mathrm{total}}$',
        fontsize=11)
    plt.title('Heavy-Element Enrichment Fingerprint for RLOF Stripped Remnants',
              fontsize=12,
              fontweight='bold')
    plt.grid(True, linestyle='--', alpha=0.4)
    plt.legend(frameon=True, facecolor='white', framealpha=0.9, fontsize=9.5)
    plt.tight_layout()
    plt.savefig(f'{OUT_DIR}/fig8_heavy_element_enrichment.png')
    plt.close()

    print("✅ All 3 core mass threshold study figures generated successfully!")


if __name__ == '__main__':
    main()
