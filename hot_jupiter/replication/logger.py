"""
Repository logger for appending replication events and progress updates to REPLICATION_LOG.md.
"""

import datetime
import logging
from pathlib import Path

logger = logging.getLogger(__name__)

DEFAULT_LOG_PATH = Path("REPLICATION_LOG.md")

HEADER = """# Exoplanet Literature Autonomous Replication Log

This running log records all paper replications, mathematical derivations, numerical methods, agreement scores, and discrepancy diagnostics executed by the autonomous replication engine.

---

"""


class ReplicationLogger:
    """
    Appends replication entries and summary updates to REPLICATION_LOG.md.
    """

    def __init__(self, log_path: Path | str = DEFAULT_LOG_PATH):
        self.log_path = Path(log_path)
        self._ensure_header()

    def _ensure_header(self):
        if not self.log_path.exists() or self.log_path.stat().st_size == 0:
            self.log_path.write_text(HEADER, encoding="utf-8")

    def log_replication_event(
        self,
        paper: dict,
        agreement_score: float,
        discrepancy_type: str,
        details: str,
    ) -> bool:
        """
        Appends a paper replication event to REPLICATION_LOG.md.
        """
        now_str = datetime.datetime.now(datetime.timezone.utc).isoformat()
        entry = f"""### [{now_str}] {paper.get('authors', 'Unknown')} ({paper.get('year', '')})
- **Title**: {paper.get('title', 'N/A')}
- **arXiv ID**: {paper.get('arxiv_id', 'N/A')}
- **Topic**: {paper.get('topic', 'N/A')}
- **Status**: **{paper.get('replication_status', 'VERIFIED')}** (Agreement Score: {agreement_score:.1%})
- **Discrepancy Category**: `{discrepancy_type}`
- **Details**: {details}

---

"""
        try:
            with open(self.log_path, "a", encoding="utf-8") as f:
                f.write(entry)
            logger.info("Appended replication entry for %s to %s",
                        paper.get('arxiv_id'), self.log_path)
            return True
        except OSError as e:
            logger.error("Failed to append entry to %s: %s", self.log_path, e)
            return False

    def log_digest_summary(self, stats: dict,
                           recent_papers: list[dict]) -> bool:
        """
        Appends a daily digest summary to REPLICATION_LOG.md.
        """
        now_str = datetime.datetime.now(datetime.timezone.utc).isoformat()
        rows = "\n".join([
            f"  - [{p.get('arxiv_id')}] {p.get('title')} ({p.get('authors')}, {p.get('year')}) | Score: {p.get('agreement_score', 0):.1%}"
            for p in recent_papers
        ])
        entry = f"""## Daily Replication Digest Summary [{now_str}]
- **Total Cataloged Papers**: {stats.get('total_papers', 0)}
- **Total Verified Papers**: {stats.get('verified_papers', 0)}
- **Average Agreement Score**: {stats.get('avg_agreement_score', 0.0):.1%}

**Recent Verified Replications**:
{rows}

---

"""
        try:
            with open(self.log_path, "a", encoding="utf-8") as f:
                f.write(entry)
            return True
        except OSError as e:
            logger.error("Failed to write digest to %s: %s", self.log_path, e)
            return False
