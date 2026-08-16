"""
Replication module for autonomous verification of exoplanet astrophysics literature.
"""

from hot_jupiter.replication.catalog import ReplicationCatalog
from hot_jupiter.replication.email_notifier import EmailNotifier
from hot_jupiter.replication.logger import ReplicationLogger
from hot_jupiter.replication.paper_validator import (
    TripartitePaperValidator,
    ValidationResult,
)

__all__ = [
    "EmailNotifier",
    "ReplicationCatalog",
    "ReplicationLogger",
    "TripartitePaperValidator",
    "ValidationResult",
]
