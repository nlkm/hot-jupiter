"""
Populate SQLite database (hot_jupiter/data/hot_jupiter.db) from NASA Exoplanet Archive 342-planet dataset and literature references.
"""

import csv
import os
import sys

# Add project root to sys.path
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__),
                                                "..")))

from hot_jupiter.database import DEFAULT_DB_PATH, get_db_connection, seed_database_if_empty


def populate_from_csv(
        csv_path: str = "outputs/nasa_exoplanet_archive_hot_jupiters_342.csv",
        db_path: str = DEFAULT_DB_PATH):
    seed_database_if_empty(db_path)
    conn = get_db_connection(db_path)
    cursor = conn.cursor()

    if not os.path.exists(csv_path):
        print(f"CSV file {csv_path} not found.")
        return

    with open(csv_path, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            name = row["planet_name"].strip()
            porb = float(row["period_days"])
            a_au = float(row["a_au"])
            m_star = float(row["M_star_Msun"])
            fe_h = float(row["Fe_H"])
            m_p = float(row["M_p_Mjup"])
            r_p = float(row["R_p_Rjup"])
            teq = float(row["T_eq_K"])
            ref = row.get("reference", "").strip()

            cursor.execute(
                """
            INSERT INTO exoplanets (
                name, period_days, semi_major_axis_au, mass_jup, radius_jup, radius_err_jup,
                eccentricity, star_mass_sun, star_radius_sun, metallicity_fe_h, teq_k, age_gyr, reference
            ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
            ON CONFLICT(name) DO UPDATE SET
                period_days=excluded.period_days,
                semi_major_axis_au=excluded.semi_major_axis_au,
                mass_jup=excluded.mass_jup,
                radius_jup=excluded.radius_jup,
                star_mass_sun=excluded.star_mass_sun,
                metallicity_fe_h=excluded.metallicity_fe_h,
                teq_k=excluded.teq_k,
                reference=excluded.reference;
            """, (name, porb, a_au, m_p, r_p, 0.05, 0.0, m_star, 1.0, fe_h, teq,
                  4.56, ref))

    conn.commit()
    cursor.execute("SELECT COUNT(*) FROM exoplanets;")
    total = cursor.fetchone()[0]
    conn.close()
    print(
        f"Successfully populated SQLite database '{db_path}' with total {total} exoplanets."
    )


if __name__ == "__main__":
    populate_from_csv()
