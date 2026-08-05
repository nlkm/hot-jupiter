"""
Python ctypes wrapper binding directly to compiled C++ engine (libhot_jupiter_cpp.so).
Ensures heavy 1D hydrostatic differential equation integrations and EOS calculations
delegate to the C++ core with zero duplication.
"""

import ctypes
import os


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


def _find_lib() -> str | None:
    package_dir = os.path.dirname(os.path.abspath(__file__))
    workspace_root = os.path.abspath(os.path.join(package_dir, ".."))

    possible_paths = [
        os.path.join(workspace_root, "bazel-bin", "libhot_jupiter_cpp.so"),
        os.path.join(workspace_root, "build", "libhot_jupiter_cpp.so"),
        os.path.join(workspace_root, "libhot_jupiter_cpp.so"),
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
