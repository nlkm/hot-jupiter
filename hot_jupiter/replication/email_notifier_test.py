"""
Unit tests for EmailNotifier.
"""

from hot_jupiter.replication.email_notifier import EmailNotifier


def test_email_notifier_dry_run():
    notifier = EmailNotifier()
    assert not notifier.is_configured()

    paper = {
        "title": "Orbital Decay and Roche Lobe Overflow",
        "authors": "Jackson et al.",
        "year": 2017,
        "arxiv_id": "1611.08272",
        "topic": "Tidal Decay & RLOF",
        "replication_status": "VERIFIED",
    }

    # Dry-run mode returns True safely without attempting network SMTP connection
    success = notifier.send_replication_report(
        paper=paper,
        agreement_score=0.985,
        discrepancy_type="NONE",
        details="Verified M_crit(a) scaling.",
    )
    assert success is True


def test_email_notifier_daily_digest():
    notifier = EmailNotifier()
    stats = {
        "total_papers": 100,
        "verified_papers": 100,
        "avg_agreement_score": 0.985
    }
    recent_papers = [{
        "arxiv_id": "1611.08272",
        "title": "Jackson 2017",
        "authors": "Jackson et al.",
        "year": 2017,
        "agreement_score": 0.985,
    }]

    success = notifier.send_daily_digest(stats, recent_papers)
    assert success is True
