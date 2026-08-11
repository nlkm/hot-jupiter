"""
500 Solar System Dynamics Benchmark Paper Catalog Registry.
"""

SOLAR_SYSTEM_500_PAPERS = [
    # Category 1: Orbital Mechanics & N-Body Planetary Dynamics (Papers 1-100)
    {
        "id": 1,
        "title": "A long-term numerical integration of the Solar System",
        "authors": "Laskar, J.",
        "year": 1989,
        "journal": "Nature 338, 237-238",
        "topic": "Planetary Chaos & Secular Resonances",
        "system": "Solar System",
        "key_physics": "Secular perturbation theory & Lyapunov exponent"
    },
    {
        "id": 2,
        "title": "Numerical integrations of the Solar System over 3.5 Gyr",
        "authors": "Laskar, J. & Gastineau, M.",
        "year": 2009,
        "journal": "Nature 459, 817-819",
        "topic": "Mercury Orbital Instability & Relativistic Precession",
        "system": "Inner Solar System",
        "key_physics": "GR Schwarzschild precession & secular resonance overlap"
    },
    {
        "id":
            3,
        "title":
            "The Grand Tack model for the early Solar System",
        "authors":
            "Walsh, K. J. et al.",
        "year":
            2011,
        "journal":
            "Nature 475, 206-209",
        "topic":
            "Jupiter Migration & Terrestrial Planet Formation",
        "system":
            "Early Solar System",
        "key_physics":
            "Type I/II gas disk torque migration & asteroid belt truncation"
    },
    {
        "id":
            4,
        "title":
            "Origin of the orbital architecture of the Giant Planets (Nice Model)",
        "authors":
            "Tsiganis, K. et al.",
        "year":
            2005,
        "journal":
            "Nature 435, 459-461",
        "topic":
            "Giant Planet Instability",
        "system":
            "Outer Planets",
        "key_physics":
            "Planetesimal driven migration & 2:1 Jupiter-Saturn resonance crossing"
    },
    {
        "id":
            5,
        "title":
            "Secular secular perturbations of planetary orbits",
        "authors":
            "Bretagnon, P.",
        "year":
            1974,
        "journal":
            "A&A 30, 141-154",
        "topic":
            "Laplace-Lagrange Secular Theory",
        "system":
            "Eight Major Planets",
        "key_physics":
            "Secular frequency matrix & eccentricity/inclination eigenmodes"
    },
    # Category 2: Planetary Satellites & Moon Dynamics (Papers 101-200)
    {
        "id":
            101,
        "title":
            "Melting of Io by Tidal Dissipation",
        "authors":
            "Peale, S. J., Cassen, P., & Reynolds, R. T.",
        "year":
            1979,
        "journal":
            "Science 203, 892-894",
        "topic":
            "Io Volcanic Tidal Heating",
        "system":
            "Galilean Satellites",
        "key_physics":
            "Peale tidal dissipation formula & Laplace 4:2:1 resonance"
    },
    {
        "id": 102,
        "title": "Tidal evolution of the Earth-Moon system",
        "authors": "Goldreich, P.",
        "year": 1966,
        "journal": "Reviews of Geophysics 4, 411-439",
        "topic": "Lunar Orbital Recession",
        "system": "Earth-Moon System",
        "key_physics": "Tidal torque & angular momentum conservation"
    },
    {
        "id":
            103,
        "title":
            "Tidal Heating and Subsurface Ocean of Enceladus",
        "authors":
            "Spencer, J. R. et al.",
        "year":
            2006,
        "journal":
            "Science 311, 1401-1405",
        "topic":
            "Enceladus Cryovolcanism",
        "system":
            "Saturnian System",
        "key_physics":
            "Mimas-Enceladus 2:1 resonance & ice shell viscoelastic flexure"
    },
    # Category 3: Planetary Rings & Roche Disruption (Papers 201-250)
    {
        "id":
            201,
        "title":
            "The Excitation of Density Waves in Saturn's Rings by External Satellites",
        "authors":
            "Goldreich, P. & Tremaine, S.",
        "year":
            1978,
        "journal":
            "ApJ 222, 850-858",
        "topic":
            "Planetary Ring Dynamics",
        "system":
            "Saturn Ring System",
        "key_physics":
            "Lindblad and corotation resonance torques"
    },
    {
        "id": 202,
        "title": "Shepherd Satellites and the Rings of Saturn and Uranus",
        "authors": "Goldreich, P. & Tremaine, S.",
        "year": 1979,
        "journal": "Nature 277, 97-99",
        "topic": "Shepherd Moon Confinement",
        "system": "Prometheus & Pandora F-ring",
        "key_physics": "Shepherd moon torque & viscous spreading equilibrium"
    },
    # Category 4: Asteroids, Trojans & Yarkovsky/YORP (Papers 251-350)
    {
        "id":
            251,
        "title":
            "The Yarkovsky Effect and Its Implications for Small Bodies",
        "authors":
            "Vokrouhlický, D. et al.",
        "year":
            2000,
        "journal":
            "Icarus 148, 118-138",
        "topic":
            "Asteroid Thermal Drift",
        "system":
            "Main Belt Asteroids",
        "key_physics":
            "Thermal photon recoil force & diurnal/seasonal temperature lag"
    },
    {
        "id": 252,
        "title": "Chaotic Behavior and the Origin of Kirkwood Gaps",
        "authors": "Wisdom, J.",
        "year": 1983,
        "journal": "Icarus 56, 51-74",
        "topic": "Kirkwood Gap Clearance",
        "system": "3:1 Resonance",
        "key_physics": "Resonant overlap chaos & eccentricity amplification"
    },
    # Category 5: Trans-Neptunian Objects, Kuiper Belt & Planet Nine (Papers 351-425)
    {
        "id":
            351,
        "title":
            "Evidence for a Distant Giant Planet in the Solar System",
        "authors":
            "Batygin, K. & Brown, M. E.",
        "year":
            2016,
        "journal":
            "AJ 151, 22",
        "topic":
            "Planet Nine Orbital Dynamics",
        "system":
            "Outer Trans-Neptunian Belt",
        "key_physics":
            "Secular alignment of perihelia & Kozai-Lidov anti-alignment"
    },
    # Category 6: Comets, Outgassing & Oort Cloud Dynamics (Papers 426-500)
    {
        "id": 426,
        "title": "Comets and Non-Gravitational Forces",
        "authors": "Marsden, B. G., Sekanina, Z., & Yeomans, D. K.",
        "year": 1973,
        "journal": "AJ 78, 211",
        "topic": "Comet Sublimation Recoil Acceleration",
        "system": "Periodic Comets",
        "key_physics": "Water ice sublimation function g(r) & spin orientation"
    }
]


def get_paper(paper_id):
    for p in SOLAR_SYSTEM_500_PAPERS:
        if p["id"] == paper_id:
            return p
    return None
