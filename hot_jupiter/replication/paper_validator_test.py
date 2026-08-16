"""
Unit tests for TripartitePaperValidator.
Verifies that all tripartite paper benchmark validations run cleanly and produce valid statistical metrics.
"""

from pathlib import Path

from hot_jupiter.replication.paper_validator import TripartitePaperValidator


def test_tripartite_paper_validator(tmp_path: Path):
    validator = TripartitePaperValidator(output_dir=str(tmp_path / "figures"))
    results = validator.run_all_validations()

    assert len(results) >= 6, "Should run at least 6 validation benchmarks"

    for r in results:
        assert r.r2_score >= 0.98, f"Benchmark {r.paper_id} R^2 = {r.r2_score:.4f} < 0.98"
        assert r.rmse >= 0.0, "RMSE must be non-negative"
        assert Path(
            r.figure_path).exists(), f"Figure {r.figure_path} must exist"
        assert len(r.physical_summary) > 10, "Summary must be informative"
        assert len(r.discrepancy_analysis
                  ) > 10, "Discrepancy analysis must be informative"
