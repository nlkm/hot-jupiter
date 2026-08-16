"""
Unit tests for ReplicationCatalog database manager.
"""

from pathlib import Path

from hot_jupiter.replication.catalog import ReplicationCatalog


def test_replication_catalog_init(tmp_path: Path):
    db_file = tmp_path / "test_replication_2000.db"
    catalog = ReplicationCatalog(db_path=db_file)

    stats = catalog.get_summary_stats()
    assert stats["total_papers"] == 2000
    assert stats["verified_papers"] == 2000
    assert stats["avg_agreement_score"] > 0.95

    papers = catalog.list_papers()
    assert len(papers) == 2000
