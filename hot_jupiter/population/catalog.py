"""
Curated Hot Jupiter Exoplanet Catalog and NASA Exoplanet Archive loader using SQLite database.
"""

from dataclasses import dataclass

from hot_jupiter.constants import AU, M_JUP, M_SUN, R_JUP, R_SUN
from hot_jupiter.database import (
    DEFAULT_DB_PATH,
    get_db_connection,
    seed_database_if_empty,
)


@dataclass
class ExoplanetSystem:
    """Individual exoplanet system parameters."""
    name: str
    M_p: float  # Planet mass [kg]
    R_p_obs: float  # Observed planet radius [m]
    R_p_err: float  # Radius error [m]
    a: float  # Semi-major axis [m]
    P_orb_days: float  # Orbital period [days]
    eccentricity: float  # Orbital eccentricity
    M_star: float  # Host star mass [kg]
    R_star: float  # Host star radius [m]
    fe_h: float  # Host star metallicity [Fe/H]
    age_gyr: float  # Estimated system age [Gyr]


def get_curated_hot_jupiter_catalog(
        db_path: str = DEFAULT_DB_PATH) -> list[ExoplanetSystem]:
    """
    Return a curated dataset of well-characterized Hot Jupiters dynamically queried from SQLite database.
    """
    seed_database_if_empty(db_path)
    conn = get_db_connection(db_path)
    cursor = conn.cursor()
    cursor.execute("""
        SELECT name, mass_jup, radius_jup, radius_err_jup, semi_major_axis_au, period_days,
               eccentricity, star_mass_sun, star_radius_sun, metallicity_fe_h, age_gyr
        FROM exoplanets
        ORDER BY id ASC;
    """)
    rows = cursor.fetchall()
    conn.close()

    catalog = []
    for row in rows:
        sys = ExoplanetSystem(
            name=row["name"],
            M_p=row["mass_jup"] * M_JUP,
            R_p_obs=row["radius_jup"] * R_JUP,
            R_p_err=row["radius_err_jup"] * R_JUP,
            a=row["semi_major_axis_au"] * AU,
            P_orb_days=row["period_days"],
            eccentricity=row["eccentricity"],
            M_star=row["star_mass_sun"] * M_SUN,
            R_star=row["star_radius_sun"] * R_SUN,
            fe_h=row["metallicity_fe_h"],
            age_gyr=row["age_gyr"],
        )
        catalog.append(sys)

    return catalog
