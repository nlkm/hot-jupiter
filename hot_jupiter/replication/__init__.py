"""
Replication module for autonomous verification of exoplanet astrophysics literature.
"""

from hot_jupiter.replication.catalog import ReplicationCatalog
from hot_jupiter.replication.email_notifier import EmailNotifier

__all__ = ["EmailNotifier", "ReplicationCatalog"]
