"""
Python ctypes wrapper binding directly to compiled C++ engine (libhot_jupiter_cpp.so).
Ensures heavy 1D hydrostatic differential equation integrations and EOS calculations
delegate to the C++ core with zero duplication.
"""

import ctypes
import os

import numpy as np


class C_PlanetStructureResult(ctypes.Structure):
    _fields_ = [
        ("R_p", ctypes.c_double),
        ("M_p", ctypes.c_double),
        ("M_c", ctypes.c_double),
        ("S_env", ctypes.c_double),
        ("T_center", ctypes.c_double),
        ("P_center", ctypes.c_double),
        ("rho_center", ctypes.c_double),
        ("num_layers", ctypes.c_int),
    ]


class C_TrajectoryResult(ctypes.Structure):
    _fields_ = [
        ("final_m_remnant_earth", ctypes.c_double),
        ("z_bulk", ctypes.c_double),
        ("outcome", ctypes.c_int),
        ("num_pts_returned", ctypes.c_int),
    ]


def _find_lib() -> str | None:
    package_dir = os.path.dirname(os.path.abspath(__file__))
    workspace_root = os.path.abspath(os.path.join(package_dir, ".."))

    possible_paths = [
        os.path.join(workspace_root, "libhot_jupiter_cpp.so"),
        os.path.join(workspace_root, "bazel-bin", "libhot_jupiter_cpp.so"),
        os.path.join(workspace_root, "build", "libhot_jupiter_cpp.so"),
        "libhot_jupiter_cpp.so",
    ]
    for path in possible_paths:
        abs_p = os.path.realpath(path)
        if os.path.exists(abs_p):
            return abs_p
    return None


_lib_path = _find_lib()
_cpp_lib = None
if _lib_path:
    try:
        _cpp_lib = ctypes.CDLL(_lib_path)
        _cpp_lib.solve_planet_structure_c.argtypes = [
            ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,
            ctypes.POINTER(C_PlanetStructureResult)
        ]
        _cpp_lib.solve_planet_structure_c.restype = None

        _cpp_lib.evaluate_saumon_chabrier_density_c.argtypes = [
            ctypes.c_double, ctypes.c_double, ctypes.c_double
        ]
        _cpp_lib.evaluate_saumon_chabrier_density_c.restype = ctypes.c_double

        _cpp_lib.rlof_integrate_trajectory_c.argtypes = [
            ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,
            ctypes.c_double, ctypes.c_int,
            ctypes.POINTER(ctypes.c_double),
            ctypes.POINTER(ctypes.c_double),
            ctypes.POINTER(ctypes.c_double),
            ctypes.POINTER(ctypes.c_double),
            ctypes.POINTER(ctypes.c_double),
            ctypes.POINTER(ctypes.c_double),
            ctypes.POINTER(C_TrajectoryResult)
        ]
        _cpp_lib.rlof_integrate_trajectory_c.restype = None

        _cpp_lib.rlof_sweep_grid_c.argtypes = [
            ctypes.POINTER(ctypes.c_double), ctypes.c_int,
            ctypes.POINTER(ctypes.c_double), ctypes.c_int, ctypes.c_double,
            ctypes.c_double, ctypes.c_double, ctypes.c_int,
            ctypes.POINTER(ctypes.c_int)
        ]
        _cpp_lib.rlof_sweep_grid_c.restype = None

        _cpp_lib.solve_interior_profile_detailed_c.argtypes = [
            ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,
            ctypes.c_int, ctypes.c_double, ctypes.c_double,
            ctypes.POINTER(ctypes.c_double),
            ctypes.POINTER(ctypes.c_double),
            ctypes.POINTER(ctypes.c_double),
            ctypes.POINTER(ctypes.c_double),
            ctypes.POINTER(ctypes.c_double),
            ctypes.POINTER(ctypes.c_double),
            ctypes.POINTER(C_PlanetStructureResult)
        ]
        _cpp_lib.solve_interior_profile_detailed_c.restype = None

        _cpp_lib.simulate_population_c.argtypes = [
            ctypes.c_int, ctypes.c_double, ctypes.c_double, ctypes.c_double,
            ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_uint,
            ctypes.POINTER(ctypes.c_double),
            ctypes.POINTER(ctypes.c_double),
            ctypes.POINTER(ctypes.c_double),
            ctypes.POINTER(ctypes.c_double),
            ctypes.POINTER(ctypes.c_double),
            ctypes.POINTER(ctypes.c_int)
        ]
        _cpp_lib.simulate_population_c.restype = None
    except Exception:  # noqa: BLE001
        _cpp_lib = None


def solve_structure_cpp(M_p_kg: float,
                        M_c_kg: float,
                        S_env: float,
                        P_surf: float = 1e5) -> C_PlanetStructureResult:
    """Solve 1D hydrostatic planet structure delegating directly to compiled C++ engine."""
    if _cpp_lib is None:
        raise RuntimeError(
            "Compiled C++ library (libhot_jupiter_cpp.so) not found. Run 'bazel build //:libhot_jupiter_cpp.so'."
        )
    res = C_PlanetStructureResult()
    _cpp_lib.solve_planet_structure_c(M_p_kg, M_c_kg, S_env, P_surf,
                                      ctypes.byref(res))
    return res


def evaluate_density_cpp(P_pascal: float,
                         T_kelvin: float,
                         X: float = 0.75) -> float:
    """Evaluate EoS density delegating directly to compiled C++ engine."""
    if _cpp_lib is None:
        raise RuntimeError(
            "Compiled C++ library (libhot_jupiter_cpp.so) not found.")
    return _cpp_lib.evaluate_saumon_chabrier_density_c(P_pascal, T_kelvin, X)


def rlof_integrate_cpp(m_p_init_jup: float = 1.0,
                       a_init_au: float = 0.02,
                       m_core_earth: float = 10.0,
                       m_star_sun: float = 1.0,
                       t_max_yr: float = 5.0e9,
                       num_pts: int = 400) -> tuple[dict, C_TrajectoryResult]:
    """Integrate RLOF trajectory delegating directly to compiled C++ engine."""
    if _cpp_lib is None:
        raise RuntimeError(
            "Compiled C++ library (libhot_jupiter_cpp.so) not found.")

    t_arr = (ctypes.c_double * num_pts)()
    a_arr = (ctypes.c_double * num_pts)()
    e_arr = (ctypes.c_double * num_pts)()
    m_p_arr = (ctypes.c_double * num_pts)()
    r_p_arr = (ctypes.c_double * num_pts)()
    ff_arr = (ctypes.c_double * num_pts)()
    res = C_TrajectoryResult()

    _cpp_lib.rlof_integrate_trajectory_c(m_p_init_jup, a_init_au, m_core_earth,
                                         m_star_sun, t_max_yr, num_pts, t_arr,
                                         a_arr, e_arr, m_p_arr, r_p_arr, ff_arr,
                                         ctypes.byref(res))

    data = {
        "t": [t_arr[i] for i in range(res.num_pts_returned)],
        "a": [a_arr[i] for i in range(res.num_pts_returned)],
        "e": [e_arr[i] for i in range(res.num_pts_returned)],
        "M_p": [m_p_arr[i] for i in range(res.num_pts_returned)],
        "R_p": [r_p_arr[i] for i in range(res.num_pts_returned)],
        "filling_factor": [ff_arr[i] for i in range(res.num_pts_returned)],
    }
    return data, res


def rlof_sweep_grid_cpp(m_grid: np.ndarray,
                        a_grid: np.ndarray,
                        m_core_earth: float = 10.0,
                        m_star_sun: float = 1.0,
                        t_max_yr: float = 3.0e9,
                        num_pts: int = 200) -> np.ndarray:
    """Perform high-resolution 2D grid sweep directly using native C++ parallel execution."""
    if _cpp_lib is None:
        raise RuntimeError(
            "Compiled C++ library (libhot_jupiter_cpp.so) not found.")

    n_m = len(m_grid)
    n_a = len(a_grid)
    m_c_arr = (ctypes.c_double * n_m)(*m_grid)
    a_c_arr = (ctypes.c_double * n_a)(*a_grid)
    out_matrix = (ctypes.c_int * (n_m * n_a))()

    _cpp_lib.rlof_sweep_grid_c(m_c_arr, n_m, a_c_arr, n_a, m_core_earth,
                               m_star_sun, t_max_yr, num_pts, out_matrix)

    result = np.zeros((n_m, n_a), dtype=int)
    for i in range(n_m):
        for j in range(n_a):
            result[i, j] = out_matrix[i * n_a + j]
    return result


def solve_interior_profile_detailed_cpp(
        M_p_kg: float,
        M_c_kg: float,
        S_env: float,
        P_surf: float = 1e5,
        num_pts: int = 300,
        a_au: float = 0.0,
        m_star_sun: float = 1.0) -> tuple[dict, C_PlanetStructureResult]:
    """Solve 1D hydrostatic detailed interior profile delegating directly to compiled C++ engine."""
    if _cpp_lib is None:
        raise RuntimeError(
            "Compiled C++ library (libhot_jupiter_cpp.so) not found.")

    r_arr = (ctypes.c_double * num_pts)()
    m_arr = (ctypes.c_double * num_pts)()
    P_arr = (ctypes.c_double * num_pts)()
    rho_arr = (ctypes.c_double * num_pts)()
    T_arr = (ctypes.c_double * num_pts)()
    nad_arr = (ctypes.c_double * num_pts)()
    res = C_PlanetStructureResult()

    _cpp_lib.solve_interior_profile_detailed_c(M_p_kg, M_c_kg, S_env, P_surf,
                                               num_pts, a_au, m_star_sun, r_arr,
                                               m_arr, P_arr, rho_arr, T_arr,
                                               nad_arr, ctypes.byref(res))

    n = res.num_layers
    profile_data = {
        "r": [r_arr[i] for i in range(n)],
        "m": [m_arr[i] for i in range(n)],
        "P": [P_arr[i] for i in range(n)],
        "rho": [rho_arr[i] for i in range(n)],
        "T": [T_arr[i] for i in range(n)],
        "nabla_ad": [nad_arr[i] for i in range(n)],
    }
    return profile_data, res


def simulate_population_cpp(num_planets: int = 1000,
                            m_min_jup: float = 0.1,
                            m_max_jup: float = 5.0,
                            a_min_au: float = 0.012,
                            a_max_au: float = 0.035,
                            m_core_min_earth: float = 1.0,
                            m_core_max_earth: float = 25.0,
                            seed: int = 42) -> dict:
    """Run Monte Carlo synthetic population simulation delegating directly to compiled C++ engine."""
    if _cpp_lib is None:
        raise RuntimeError(
            "Compiled C++ library (libhot_jupiter_cpp.so) not found.")

    m_init_arr = (ctypes.c_double * num_planets)()
    a_init_arr = (ctypes.c_double * num_planets)()
    m_core_arr = (ctypes.c_double * num_planets)()
    m_remnant_arr = (ctypes.c_double * num_planets)()
    z_bulk_arr = (ctypes.c_double * num_planets)()
    outcome_arr = (ctypes.c_int * num_planets)()

    _cpp_lib.simulate_population_c(num_planets, m_min_jup, m_max_jup, a_min_au,
                                   a_max_au, m_core_min_earth, m_core_max_earth,
                                   seed, m_init_arr, a_init_arr, m_core_arr,
                                   m_remnant_arr, z_bulk_arr, outcome_arr)

    return {
        "m_p_init": [m_init_arr[i] for i in range(num_planets)],
        "a_init": [a_init_arr[i] for i in range(num_planets)],
        "m_core": [m_core_arr[i] for i in range(num_planets)],
        "m_remnant": [m_remnant_arr[i] for i in range(num_planets)],
        "z_bulk": [z_bulk_arr[i] for i in range(num_planets)],
        "outcome": [outcome_arr[i] for i in range(num_planets)],
    }
