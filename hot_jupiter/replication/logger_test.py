"""
Unit tests for ReplicationLogger repository logger.
"""

from pathlib import Path

from hot_jupiter.replication.logger import ReplicationLogger


def test_replication_logger_event(tmp_path: Path):
    log_file = tmp_path / "TEST_REPLICATION_LOG.md"
    rep_logger = ReplicationLogger(log_path=log_file)

    paper = {
        "title": "Orbital Decay and Roche Lobe Overflow",
        "authors": "Jackson et al.",
        "year": 2017,
        "arxiv_id": "1611.08272",
        "topic": "Tidal Decay & RLOF",
        "replication_status": "VERIFIED",
    }

    success = rep_logger.log_replication_event(
        paper=paper,
        agreement_score=0.985,
        discrepancy_type="NONE",
        details="Verified M_crit(a) scaling.",
    )
    assert success is True
    assert log_file.exists()
    content = log_file.read_text(encoding="utf-8")
    assert "Jackson et al. (2017)" in content
    assert "1611.08272" in content


def test_replication_logger_digest(tmp_path: Path):
    log_file = tmp_path / "TEST_REPLICATION_LOG.md"
    rep_logger = ReplicationLogger(log_path=log_file)

    stats = {
        "total_papers": 100,
        "verified_papers": 100,
        "avg_agreement_score": 0.985
    }
    recent = [{
        "arxiv_id": "1611.08272",
        "title": "Jackson 2017",
        "authors": "Jackson et al.",
        "year": 2017,
        "agreement_score": 0.985,
    }]

    success = rep_logger.log_digest_summary(stats, recent)
    assert success is True
    content = log_file.read_text(encoding="utf-8")
    assert "Daily Replication Digest Summary" in content
