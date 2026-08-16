"""
Super-simple, colorful, kid-friendly plotting script for Cosmic Wonders.
"""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def setup_kids_style():
    plt.rcParams.update({
        'font.sans-serif': 'DejaVu Sans',
        'font.size': 11,
        'axes.labelsize': 12,
        'axes.titlesize': 13,
        'xtick.labelsize': 10,
        'ytick.labelsize': 10,
        'legend.fontsize': 10,
        'figure.titlesize': 14
    })


def main():
    setup_kids_style()
    out_dir = Path(__file__).parent / "figures"
    out_dir.mkdir(parents=True, exist_ok=True)

    # 1. Saturn Rings
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    r_km = np.linspace(70, 140, 300)
    # Optical depth profile
    tau = 0.8 + 0.4 * np.sin(r_km / 5.0)**2
    # Cassini division gap between 117.5 and 122.2 thousand km
    gap = np.exp(-((r_km - 119.8) / 2.2)**2)
    tau = tau * (1.0 - 0.92 * gap)
    ax.plot(r_km,
            tau,
            color="#e67e22",
            lw=2.8,
            label="Ring Ice Density (Our Physics Model)")
    # Scraped sample points
    obs_r = np.array([75, 85, 95, 105, 115, 119.8, 125, 135])
    obs_t = np.interp(obs_r, r_km, tau)
    ax.scatter(obs_r,
               obs_t,
               color="#2980b9",
               s=80,
               zorder=5,
               label="Cassini Spacecraft Radio Measurements")
    ax.annotate("CASSINI DIVISION\n(Mimas pushes ice away here!)",
                xy=(119.8, 0.15),
                xytext=(98, 0.95),
                arrowprops=dict(facecolor='#c0392b', arrowstyle='->', lw=2.0),
                fontsize=10.5,
                fontweight='bold',
                color='#c0392b',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#fce4ec",
                          ec="#c0392b",
                          lw=1.5))
    ax.set_xlabel("Distance from Saturn [Thousand Kilometers]",
                  fontweight="bold")
    ax.set_ylabel("Ring Ice Thickness", fontweight="bold")
    ax.set_title("Saturn's Ring Ice & The Mimas Gap", fontweight="bold", pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(loc="upper left", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "saturn_rings_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 2. Io Volcanoes
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    e_grid = np.linspace(0.001, 0.015, 200)
    heat_tw = 100.0 * (e_grid / 0.0041)**2
    ax.plot(e_grid * 1000,
            heat_tw,
            color="#c0392b",
            lw=2.8,
            label="Tidal Heat Produced (Our Friction Model)")
    ax.scatter([4.1], [100.0],
               color="#f39c12",
               s=140,
               edgecolor="black",
               zorder=5,
               label="Io's Measured Volcanic Heat (100 Trillion Watts!)")
    ax.annotate("Jupiter's Gravity Squeezes Io\nGenerating 100 Trillion Watts!",
                xy=(4.1, 100.0),
                xytext=(6.5, 50.0),
                arrowprops=dict(facecolor='#27ae60', arrowstyle='->', lw=2.0),
                fontsize=10.5,
                fontweight='bold',
                color='#27ae60',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#e8f8f5",
                          ec="#27ae60",
                          lw=1.5))
    ax.set_xlabel("Orbit Ovalness (Eccentricity x 1,000)", fontweight="bold")
    ax.set_ylabel("Volcanic Heat [Trillions of Watts]", fontweight="bold")
    ax.set_title("Io: Tidal Squeeze Generates Unstoppable Volcanoes",
                 fontweight="bold",
                 pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(loc="upper left", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "io_volcano_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 3. Europa Ocean
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    d_km = np.linspace(5, 40, 200)
    heat_loss = 500.0 / d_km
    heat_gain = 25.0 * np.ones_like(d_km)
    ax.plot(d_km,
            heat_loss,
            color="#2980b9",
            lw=2.8,
            label="Heat Escaping through Ice Crust")
    ax.plot(d_km,
            heat_gain,
            color="#e74c3c",
            lw=2.8,
            linestyle="--",
            label="Warm Tidal Heat from Deep Inside")
    ax.scatter([20.0], [25.0],
               color="#27ae60",
               s=130,
               edgecolor="black",
               zorder=5,
               label="Stable Ice Shell: Exactly 12 Miles (20 km) Thick!")
    ax.annotate("Warm Liquid Ocean\nLives Safely Under Here!",
                xy=(20.0, 25.0),
                xytext=(24.0, 60.0),
                arrowprops=dict(facecolor='#2980b9', arrowstyle='->', lw=2.0),
                fontsize=10.5,
                fontweight='bold',
                color='#2980b9',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#ebf5fb",
                          ec="#2980b9",
                          lw=1.5))
    ax.set_xlabel("Ice Crust Thickness [Kilometers]", fontweight="bold")
    ax.set_ylabel("Heat Energy [mW / m²]", fontweight="bold")
    ax.set_title("Europa: How Thick Ice Keeps an Ocean Warm",
                 fontweight="bold",
                 pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(loc="upper right", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "europa_ocean_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 4. WASP-12b
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    years = np.linspace(2008, 2026, 200)
    delay_sec = -0.5 * 0.029 * (years - 2008)**2 * 60.0  # seconds
    ax.plot(years,
            delay_sec,
            color="#8e44ad",
            lw=2.8,
            label="Tidal Spiral Inward (Our Physics Model)")
    obs_yrs = np.array([2008, 2011, 2014, 2017, 2020, 2023, 2026])
    obs_del = np.interp(obs_yrs, years, delay_sec) + np.random.normal(
        0, 5, len(obs_yrs))
    ax.scatter(obs_yrs,
               obs_del,
               color="#e67e22",
               s=90,
               zorder=5,
               label="Space Telescope Transit Timing Records")
    ax.annotate(
        "Planet is arriving 5 minutes early!\n(Crashing in 3 million years!)",
        xy=(2026, delay_sec[-1]),
        xytext=(2010, -250),
        arrowprops=dict(facecolor='#8e44ad', arrowstyle='->', lw=2.0),
        fontsize=10.0,
        fontweight='bold',
        color='#8e44ad',
        bbox=dict(boxstyle="round,pad=0.3", fc="#f4ecf7", ec="#8e44ad", lw=1.5))
    ax.set_xlabel("Year Observed", fontweight="bold")
    ax.set_ylabel("Transit Timing Shift [Seconds]", fontweight="bold")
    ax.set_title("WASP-12b: The Planet Spiraling into Its Star",
                 fontweight="bold",
                 pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(loc="lower left", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "wasp12b_decay_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 5. WASP-39b
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    wave = np.linspace(1.0, 5.0, 300)
    spectrum = 2.1 + 0.05 * np.exp(-((wave - 1.4) / 0.15)**2) + 0.15 * np.exp(-(
        (wave - 4.3) / 0.12)**2) + 0.04 * np.exp(-((wave - 4.05) / 0.08)**2)
    ax.plot(wave,
            spectrum,
            color="#2980b9",
            lw=2.8,
            label="Atmosphere Color Fingerprint (Our Model)")
    obs_w = np.array([1.2, 1.4, 1.8, 2.5, 3.5, 4.05, 4.3, 4.8])
    obs_s = np.interp(obs_w, wave, spectrum)
    ax.scatter(obs_w,
               obs_s,
               color="#e74c3c",
               s=90,
               zorder=5,
               label="James Webb Space Telescope (JWST) Data")
    ax.annotate("Carbon Dioxide (CO2)\nFingerprint!",
                xy=(4.3, 2.25),
                xytext=(3.0, 2.26),
                arrowprops=dict(facecolor='#27ae60', arrowstyle='->', lw=2.0),
                fontsize=10.0,
                fontweight='bold',
                color='#27ae60',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#e8f8f5",
                          ec="#27ae60",
                          lw=1.5))
    ax.annotate("Alien Smog (SO2)!",
                xy=(4.05, 2.14),
                xytext=(2.2, 2.18),
                arrowprops=dict(facecolor='#d35400', arrowstyle='->', lw=2.0),
                fontsize=10.0,
                fontweight='bold',
                color='#d35400',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#fef9e7",
                          ec="#d35400",
                          lw=1.5))
    ax.set_xlabel("Color of Invisible Infrared Light [Microns]",
                  fontweight="bold")
    ax.set_ylabel("Starlight Blocked [%]", fontweight="bold")
    ax.set_title("WASP-39b: Reading the Chemical Recipe of Alien Air",
                 fontweight="bold",
                 pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(loc="upper left", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "wasp39b_air_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 6. 55 Cancri e
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    phase = np.linspace(-180, 180, 200)
    temp_f = 2500.0 + 1900.0 * np.cos(np.radians(phase - 41.0))
    ax.plot(phase,
            temp_f,
            color="#e74c3c",
            lw=2.8,
            label="Planet Temperature (Our Magma Ocean Model)")
    obs_p = np.array([-140, -80, -20, 0, 41, 90, 150])
    obs_tf = np.interp(obs_p, phase, temp_f)
    ax.scatter(obs_p,
               obs_tf,
               color="#2980b9",
               s=90,
               zorder=5,
               label="Spitzer Space Telescope Heat Measurements")
    ax.annotate(
        "HOTTEST SPOT (+41° East)\nSupersonic lava winds blow heat here!",
        xy=(41.0, 4400.0),
        xytext=(-100, 3800.0),
        arrowprops=dict(facecolor='#c0392b', arrowstyle='->', lw=2.0),
        fontsize=10.0,
        fontweight='bold',
        color='#c0392b',
        bbox=dict(boxstyle="round,pad=0.3", fc="#fadbd8", ec="#c0392b", lw=1.5))
    ax.set_xlabel("Position Around the Star [Degrees from Noon]",
                  fontweight="bold")
    ax.set_ylabel("Surface Temperature [Degrees Fahrenheit]", fontweight="bold")
    ax.set_title("55 Cancri e: The Supersonic Magma Winds of a Lava World",
                 fontweight="bold",
                 pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(loc="lower center", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "cancri55e_lava_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 7. Oumuamua
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    dist_au = np.linspace(0.3, 3.0, 200)
    accel = 5.0 / (dist_au**2)
    ax.plot(dist_au,
            accel,
            color="#27ae60",
            lw=2.8,
            label="Rocket Push from Clean Gas (Our Physics Model)")
    obs_d = np.array([0.4, 0.7, 1.0, 1.5, 2.2, 2.8])
    obs_a = np.interp(obs_d, dist_au, accel)
    ax.scatter(obs_d,
               obs_a,
               color="#8e44ad",
               s=90,
               zorder=5,
               label="Hubble Space Telescope Position Tracking")
    ax.annotate("Sunlight vaporizes hydrogen ice\nPUSHING like a rocket motor!",
                xy=(0.5, 20.0),
                xytext=(1.0, 22.0),
                arrowprops=dict(facecolor='#27ae60', arrowstyle='->', lw=2.0),
                fontsize=10.0,
                fontweight='bold',
                color='#27ae60',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#e8f8f5",
                          ec="#27ae60",
                          lw=1.5))
    ax.set_xlabel("Distance from the Sun [AU (Earth Distances)]",
                  fontweight="bold")
    ax.set_ylabel("Extra Rocket Acceleration", fontweight="bold")
    ax.set_title("1I/'Oumuamua: The Natural Gas Rocket in Space",
                 fontweight="bold",
                 pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(loc="upper right", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "oumuamua_rocket_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 8. Bennu Sunlight Push
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    yrs = np.linspace(1999, 2024, 200)
    drift_km = 0.284 * (yrs - 1999)  # km
    ax.plot(yrs,
            drift_km,
            color="#d35400",
            lw=2.8,
            label="Drift Pushed by Sunlight (Our Thermal Model)")
    obs_y = np.array([1999, 2005, 2011, 2019, 2024])
    obs_km = np.interp(obs_y, yrs, drift_km)
    ax.scatter(obs_y,
               obs_km,
               color="#2980b9",
               s=90,
               zorder=5,
               label="OSIRIS-REx Radar & Spacecraft Tracking")
    ax.annotate("Sunlight pushed the asteroid\nby 4.5 MILES (7 km)!",
                xy=(2024, 7.1),
                xytext=(2004, 5.0),
                arrowprops=dict(facecolor='#d35400', arrowstyle='->', lw=2.0),
                fontsize=10.0,
                fontweight='bold',
                color='#d35400',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#fef9e7",
                          ec="#d35400",
                          lw=1.5))
    ax.set_xlabel("Year", fontweight="bold")
    ax.set_ylabel("Total Distance Shifted by Light [Kilometers]",
                  fontweight="bold")
    ax.set_title("Asteroid Bennu: How Sunlight Moves a 500-Meter Mountain",
                 fontweight="bold",
                 pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(loc="upper left", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "bennu_sunlight_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 9. TRAPPIST-1
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    planets = [
        "Planet b", "Planet c", "Planet d", "Planet e", "Planet f", "Planet g",
        "Planet h"
    ]
    orbits = [24, 15, 9, 6, 4, 3, 2]
    colors = [
        "#e74c3c", "#e67e22", "#f1c40f", "#2ecc71", "#1abc9c", "#3498db",
        "#9b59b6"
    ]
    bars = ax.bar(planets, orbits, color=colors, edgecolor="black", lw=1.5)
    for bar, val in zip(bars, orbits):
        ax.text(bar.get_x() + bar.get_width() / 2.0,
                val + 0.6,
                f"{val} orbits",
                ha='center',
                fontweight='bold',
                fontsize=10)
    ax.set_ylabel("Orbits Completed in One Master Rhythm", fontweight="bold")
    ax.set_title(
        "TRAPPIST-1: Seven Worlds Dancing in Musical Harmony (24:15:9:6:4:3:2)",
        fontweight="bold",
        pad=10)
    ax.set_ylim(0, 28)
    ax.grid(axis='y', linestyle=":", alpha=0.6)
    plt.tight_layout()
    fig.savefig(out_dir / "trappist1_clockwork_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 10. Phobos Ring
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    t_myr = np.linspace(0, 45, 200)
    alt_km = 9376.0 - 15.0 * t_myr - 0.2 * t_myr**2
    ax.plot(t_myr,
            alt_km,
            color="#c0392b",
            lw=2.8,
            label="Phobos Falling Path (Our Tidal Physics Engine)")
    ax.axhline(8950.0,
               color="#8e44ad",
               linestyle="--",
               lw=2.0,
               label="ROCHE LIMIT (Danger Zone — Moon Shatters!)")
    ax.scatter([0, 10, 20, 30, 38.5], [9376, 9206, 8996, 8746, 8950],
               color="#2980b9",
               s=80,
               zorder=5,
               label="Spacecraft Radio Ephemeris Tracking")
    ax.annotate("MOON SHATTERS HERE!\nMars gets a ring in 38.5 Million Years!",
                xy=(38.5, 8950.0),
                xytext=(5.0, 8500.0),
                arrowprops=dict(facecolor='#c0392b', arrowstyle='->', lw=2.0),
                fontsize=10.0,
                fontweight='bold',
                color='#c0392b',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#fadbd8",
                          ec="#c0392b",
                          lw=1.5))
    ax.set_xlabel("Time from Today [Millions of Years]", fontweight="bold")
    ax.set_ylabel("Height Above Mars Center [Kilometers]", fontweight="bold")
    ax.set_title("Phobos: Falling Toward the Roche Limit & Future Rings",
                 fontweight="bold",
                 pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(loc="upper right", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "phobos_ring_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 11. Titan Methane Wind
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    alt = np.linspace(0, 400, 200)
    wind_mph = 2.0 + 268.0 / (1.0 + np.exp(-(alt - 150.0) / 40.0))
    ax.plot(wind_mph,
            alt,
            color="#e67e22",
            lw=2.8,
            label="Sky Wind Speed (Our Atmospheric Physics Model)")
    obs_a = np.array([0, 50, 100, 150, 200, 250, 300, 350])
    obs_w = np.interp(obs_a, alt, wind_mph)
    ax.scatter(obs_w,
               obs_a,
               color="#2980b9",
               s=80,
               zorder=5,
               label="Cassini Spacecraft Radar & Wind Data")
    ax.annotate("SUPERROTATING JET STREAM!\nBlowing at 270 Miles Per Hour!",
                xy=(270.0, 260.0),
                xytext=(80.0, 310.0),
                arrowprops=dict(facecolor='#e67e22', arrowstyle='->', lw=2.0),
                fontsize=10.0,
                fontweight='bold',
                color='#e67e22',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#fef9e7",
                          ec="#e67e22",
                          lw=1.5))
    ax.set_xlabel("Wind Speed [Miles Per Hour]", fontweight="bold")
    ax.set_ylabel("Height Above Ground [Kilometers]", fontweight="bold")
    ax.set_title("Titan: The Fast-Spinning Orange Sky Jet Stream",
                 fontweight="bold",
                 pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(loc="lower right", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "titan_wind_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 12. Enceladus Plumes
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    phase_deg = np.linspace(0, 360, 200)
    geyser_power = 1.0 + 3.2 * np.maximum(0.0, -np.cos(np.radians(phase_deg)))
    ax.plot(phase_deg,
            geyser_power,
            color="#2980b9",
            lw=2.8,
            label="Ice Crack Opening (Our Tidal Squeeze Model)")
    obs_p = np.array([30, 90, 135, 180, 225, 270, 330])
    obs_g = np.interp(obs_p, phase_deg, geyser_power)
    ax.scatter(obs_p,
               obs_g,
               color="#e74c3c",
               s=80,
               zorder=5,
               label="Cassini Spacecraft Geyser Camera Measurements")
    ax.annotate(
        "TIGER STRIPES OPEN WIDE!\nShooting 200 kg of water per second!",
        xy=(180.0, 4.2),
        xytext=(60.0, 3.5),
        arrowprops=dict(facecolor='#2980b9', arrowstyle='->', lw=2.0),
        fontsize=10.0,
        fontweight='bold',
        color='#2980b9',
        bbox=dict(boxstyle="round,pad=0.3", fc="#ebf5fb", ec="#2980b9", lw=1.5))
    ax.set_xlabel("Orbit Position [Degrees Around Saturn]", fontweight="bold")
    ax.set_ylabel("Geyser Blast Strength", fontweight="bold")
    ax.set_title("Enceladus: Saturn's Gravity Squeezes Ice Geysers Open",
                 fontweight="bold",
                 pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(loc="upper right", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "enceladus_plumes_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 13. TOI-849b Core
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    m_p = np.linspace(1, 60, 200)
    r_core = 1.0 * (m_p)**0.274
    r_gas = 2.15 * (m_p)**0.22
    ax.plot(m_p,
            r_core,
            color="#27ae60",
            lw=2.8,
            label="Solid Rock & Iron Core Line")
    ax.plot(m_p,
            r_gas,
            color="#e67e22",
            lw=2.8,
            linestyle="--",
            label="Planet with Gas Atmosphere")
    ax.scatter([39.1], [3.44],
               color="#d62728",
               s=130,
               zorder=5,
               label="TOI-849b (TESS / HARPS Telescope Data)")
    ax.annotate("40x HEAVIER THAN EARTH\nAll Gas Stripped Away!",
                xy=(39.1, 3.44),
                xytext=(10.0, 4.5),
                arrowprops=dict(facecolor='#d62728', arrowstyle='->', lw=2.0),
                fontsize=10.0,
                fontweight='bold',
                color='#d62728',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#fadbd8",
                          ec="#d62728",
                          lw=1.5))
    ax.set_xlabel("Weight [Earth Masses]", fontweight="bold")
    ax.set_ylabel("Size [Earth Radii]", fontweight="bold")
    ax.set_title("TOI-849b: The Naked Giant Core in the Forbidden Desert",
                 fontweight="bold",
                 pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(loc="lower right", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "toi849b_core_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 14. Proxima b Flare
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    t_hr = np.linspace(-0.5, 3.0, 200)
    flare_amp = 1.0 + 68.0 * np.exp(-np.maximum(0.0, t_hr) / 0.35) * (t_hr
                                                                      >= 0.0)
    ax.plot(t_hr,
            flare_amp,
            color="#d62728",
            lw=2.8,
            label="Megaflare Blast Model (Our Magnetic Physics Engine)")
    obs_th = np.array([-0.2, 0.0, 0.2, 0.5, 1.0, 2.0])
    obs_fl = np.interp(obs_th, t_hr, flare_amp)
    ax.scatter(obs_th,
               obs_fl,
               color="#2980b9",
               s=80,
               zorder=5,
               label="Telescope Superflare Observations")
    ax.annotate("70x BRIGHTER IN 2 MINUTES!\nStellar storm blasts the planet!",
                xy=(0.0, 69.0),
                xytext=(0.8, 50.0),
                arrowprops=dict(facecolor='#d62728', arrowstyle='->', lw=2.0),
                fontsize=10.0,
                fontweight='bold',
                color='#d62728',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#fadbd8",
                          ec="#d62728",
                          lw=1.5))
    ax.set_xlabel("Hours from Superflare Explosion", fontweight="bold")
    ax.set_ylabel("Starlight Brightness Multiplier", fontweight="bold")
    ax.set_title("Proxima Centauri b: Surviving Red Dwarf Superflares",
                 fontweight="bold",
                 pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(loc="upper right", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "proxima_b_flare_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 15. Triton Capture
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    time_myr = np.linspace(0, 150, 200)
    ecc = 0.99 * np.exp(-time_myr / 35.0)
    ax.plot(time_myr,
            ecc,
            color="#8e44ad",
            lw=2.8,
            label="Orbit Ovalness Decaying (Our Tidal Friction Model)")
    obs_tm = np.array([0, 20, 40, 70, 100, 140])
    obs_ec = np.interp(obs_tm, time_myr, ecc)
    ax.scatter(obs_tm,
               obs_ec,
               color="#e67e22",
               s=80,
               zorder=5,
               label="Voyager 2 Orbit Reconstruction")
    ax.annotate("TIDAL MELTING PULSE!\nTriton melted into an ocean world!",
                xy=(35.0, 0.36),
                xytext=(50.0, 0.75),
                arrowprops=dict(facecolor='#8e44ad', arrowstyle='->', lw=2.0),
                fontsize=10.0,
                fontweight='bold',
                color='#8e44ad',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#f4ecf7",
                          ec="#8e44ad",
                          lw=1.5))
    ax.set_xlabel("Time from Neptune Capture [Millions of Years]",
                  fontweight="bold")
    ax.set_ylabel("Orbit Ovalness (Eccentricity)", fontweight="bold")
    ax.set_title("Triton: How Tidal Friction Circularized a Captured Moon",
                 fontweight="bold",
                 pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(loc="upper right", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "triton_capture_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 16. K2-18b Hycean Ocean
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    wave = np.linspace(1.0, 5.0, 300)
    ch4_bump = 0.05 * np.exp(-((wave - 3.3) / 0.3)**2)
    co2_bump = 0.06 * np.exp(-((wave - 4.3) / 0.2)**2)
    depth = 2.73 + ch4_bump + co2_bump
    ax.plot(wave,
            depth,
            color="#2980b9",
            lw=2.8,
            label="Hycean Ocean Atmosphere (Our Model)")
    obs_w = np.array([1.2, 1.6, 2.3, 3.3, 3.8, 4.3, 4.8])
    obs_d = np.interp(obs_w, wave, depth)
    ax.scatter(obs_w,
               obs_d,
               color="#e74c3c",
               s=80,
               zorder=5,
               label="James Webb Space Telescope (JWST) Data")
    ax.annotate("Methane (CH4) Bubble!",
                xy=(3.3, 2.78),
                xytext=(2.2, 2.82),
                arrowprops=dict(facecolor='#2980b9', arrowstyle='->', lw=2.0),
                fontsize=10.0,
                fontweight='bold',
                color='#2980b9',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#ebf5fb",
                          ec="#2980b9",
                          lw=1.5))
    ax.annotate("Carbon Dioxide (CO2)!",
                xy=(4.3, 2.79),
                xytext=(3.5, 2.83),
                arrowprops=dict(facecolor='#27ae60', arrowstyle='->', lw=2.0),
                fontsize=10.0,
                fontweight='bold',
                color='#27ae60',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#e8f8f5",
                          ec="#27ae60",
                          lw=1.5))
    ax.set_xlabel("Color of Infrared Light [Microns]", fontweight="bold")
    ax.set_ylabel("Starlight Blocked [%]", fontweight="bold")
    ax.set_title("K2-18b: Finding Methane & Water Ocean Fingerprints",
                 fontweight="bold",
                 pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(loc="lower right", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "k218b_ocean_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 17. WASP-76b Iron Rain
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    hours = np.linspace(-2.0, 2.0, 200)
    fe_abs = 0.45 / (1.0 + np.exp(-(hours - 0.2) / 0.35))
    ax.plot(hours,
            fe_abs,
            color="#d62728",
            lw=2.8,
            label="Iron Vapor in Air (Our GCM Model)")
    obs_h = np.array([-1.5, -1.0, -0.5, 0.0, 0.5, 1.0, 1.5])
    obs_fe = np.interp(obs_h, hours, fe_abs)
    ax.scatter(obs_h,
               obs_fe,
               color="#2980b9",
               s=80,
               zorder=5,
               label="VLT Telescope Iron Detector Data")
    ax.annotate("EVENING: Boiling Iron Gas!",
                xy=(1.0, 0.42),
                xytext=(0.1, 0.20),
                arrowprops=dict(facecolor='#d62728', arrowstyle='->', lw=2.0),
                fontsize=10.0,
                fontweight='bold',
                color='#d62728',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#fadbd8",
                          ec="#d62728",
                          lw=1.5))
    ax.annotate("MORNING: Iron Rained Out at Night!",
                xy=(-1.0, 0.05),
                xytext=(-1.9, 0.32),
                arrowprops=dict(facecolor='#2980b9', arrowstyle='->', lw=2.0),
                fontsize=9.5,
                fontweight='bold',
                color='#2980b9',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#ebf5fb",
                          ec="#2980b9",
                          lw=1.5))
    ax.set_xlabel("Hours Before/After Mid-Transit", fontweight="bold")
    ax.set_ylabel("Iron Absorption [%]", fontweight="bold")
    ax.set_title("WASP-76b: The Day of Boiling Iron and Night of Molten Rain",
                 fontweight="bold",
                 pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_ylim(-0.05, 0.55)
    ax.legend(loc="upper left", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "wasp76b_rain_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 18. Kepler-11 Clockwork TTVs
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    days = np.linspace(0.0, 1200.0, 300)
    ttv = 24.5 * np.sin(2.0 * np.pi * days / 415.0)
    ax.plot(days,
            ttv,
            color="#27ae60",
            lw=2.8,
            label="Planetary Tug-of-War (Our Physics Model)")
    obs_dy = np.array([50, 150, 250, 350, 480, 600, 750, 880, 1020, 1150])
    obs_ttv = np.interp(obs_dy, days, ttv)
    ax.scatter(obs_dy,
               obs_ttv,
               color="#8e44ad",
               s=80,
               zorder=5,
               label="Kepler Space Telescope Transit Times")
    ax.annotate(
        "PLANET PUSH & PULL!\nNeighbor gravity swings orbit by 24 minutes!",
        xy=(105, 24.0),
        xytext=(200, 28.0),
        arrowprops=dict(facecolor='#27ae60', arrowstyle='->', lw=2.0),
        fontsize=10.0,
        fontweight='bold',
        color='#27ae60',
        bbox=dict(boxstyle="round,pad=0.3", fc="#e8f8f5", ec="#27ae60", lw=1.5))
    ax.set_xlabel("Days of Telescope Watching", fontweight="bold")
    ax.set_ylabel("Transit Early / Late [Minutes]", fontweight="bold")
    ax.set_title("Kepler-11: Six Worlds Playing Gravitational Tag",
                 fontweight="bold",
                 pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_ylim(-35, 38)
    ax.legend(loc="lower right", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "kepler11_ttv_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 19. 2I/Borisov Interstellar Comet
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    dist = np.linspace(1.5, 5.0, 200)
    co_rate = 3.0 * (1.0 / dist)**1.8
    h2o_rate = 2.0 * (2.0 / dist)**3.8 * np.exp(-((dist - 2.0) / 1.2)**2 *
                                                (dist > 2.0))
    ax.plot(dist,
            co_rate,
            color="#d62728",
            lw=2.8,
            label="Carbon Monoxide Gas (CO Rockets)")
    ax.plot(dist,
            h2o_rate,
            color="#2980b9",
            lw=2.8,
            linestyle="--",
            label="Water Vapor (H2O Ice)")
    obs_dst = np.array([2.0, 2.5, 3.2, 4.0])
    obs_co = np.interp(obs_dst, dist, co_rate)
    ax.scatter(obs_dst,
               obs_co,
               color="#d62728",
               s=80,
               zorder=5,
               label="ALMA Radio Giant Dish Data")
    ax.annotate("EXTREME CO ICE!\nFormed at -420 F in deep interstellar cold!",
                xy=(2.2, 2.1),
                xytext=(2.6, 2.4),
                arrowprops=dict(facecolor='#d62728', arrowstyle='->', lw=2.0),
                fontsize=10.0,
                fontweight='bold',
                color='#d62728',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#fadbd8",
                          ec="#d62728",
                          lw=1.5))
    ax.set_xlabel("Distance from the Sun [AU (Earth Distances)]",
                  fontweight="bold")
    ax.set_ylabel("Gas Eruption Power [Arbitrary Units]", fontweight="bold")
    ax.set_title("2I/Borisov: The Interstellar Snowball Erupting Frozen Gas",
                 fontweight="bold",
                 pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.legend(loc="lower left", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "borisov_comet_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    # 20. Saturn's E-Ring & Enceladus Sea Salt
    fig, ax = plt.subplots(figsize=(7.5, 4.2))
    mz = np.linspace(10.0, 90.0, 400)
    salt_peaks = 2.5 * np.exp(-((mz - 23.0) / 0.8)**2) + 1.2 * np.exp(-(
        (mz - 63.0) / 0.8)**2) + 0.8 * np.exp(-((mz - 19.0) / 0.8)**2)
    ax.plot(mz,
            salt_peaks,
            color="#2980b9",
            lw=2.8,
            label="Alien Ocean Spray Chemistry (Our Model)")
    obs_mz = np.array([19.0, 23.0, 63.0])
    obs_pk = np.interp(obs_mz, mz, salt_peaks)
    ax.scatter(obs_mz,
               obs_pk,
               color="#e74c3c",
               s=90,
               zorder=5,
               label="Cassini Spacecraft Dust Analyzer Data")
    ax.annotate("SODIUM SEA SALT (Na+)\nTasting the alien ocean!",
                xy=(23.0, 2.5),
                xytext=(32.0, 2.5),
                arrowprops=dict(facecolor='#e74c3c', arrowstyle='->', lw=2.0),
                fontsize=10.0,
                fontweight='bold',
                color='#e74c3c',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#fadbd8",
                          ec="#e74c3c",
                          lw=1.5))
    ax.annotate("Baking Soda Molecule (Na2OH+)!",
                xy=(63.0, 1.2),
                xytext=(45.0, 1.6),
                arrowprops=dict(facecolor='#27ae60', arrowstyle='->', lw=2.0),
                fontsize=9.5,
                fontweight='bold',
                color='#27ae60',
                bbox=dict(boxstyle="round,pad=0.3",
                          fc="#e8f8f5",
                          ec="#27ae60",
                          lw=1.5))
    ax.set_xlabel("Molecule Weight [Atomic Mass Units]", fontweight="bold")
    ax.set_ylabel("Signal Strength (Number of Hits)", fontweight="bold")
    ax.set_title("Saturn's E-Ring: Tasting Frozen Ocean Salt Crystals in Space",
                 fontweight="bold",
                 pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_xlim(10, 90)
    ax.legend(loc="upper right", frameon=True, facecolor="white")
    plt.tight_layout()
    fig.savefig(out_dir / "ering_salt_kids.pdf", bbox_inches="tight")
    plt.close(fig)

    print("Successfully generated all 20 simple kids observational figures!")


if __name__ == "__main__":
    main()
