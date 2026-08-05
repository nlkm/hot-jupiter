"""
Unit tests for SQLite database manager (hot_jupiter/database.py).
"""

import os
import tempfile
import sqlite3
import pytest

from hot_jupiter.database import get_db_connection, seed_database_if_empty, _create_schema


def test_sqlite_database_connection():
    with tempfile.TemporaryDirectory() as tmpdir:
        db_path = os.path.join(tmpdir, "test.db")
        conn = get_db_connection(db_path)
        assert os.path.exists(db_path)
        
        cursor = conn.cursor()
        cursor.execute("SELECT name FROM sqlite_master WHERE type='table';")
        tables = [row[0] for row in cursor.fetchall()]
        assert "exoplanets" in tables
        assert "references_catalog" in tables
        conn.close()


def test_seed_database_if_empty():
    with tempfile.TemporaryDirectory() as tmpdir:
        db_path = os.path.join(tmpdir, "test.db")
        seed_database_if_empty(db_path)
        
        conn = get_db_connection(db_path)
        cursor = conn.cursor()
        
        cursor.execute("SELECT COUNT(*) FROM exoplanets;")
        ex_count = cursor.fetchone()[0]
        assert ex_count >= 20
        
        cursor.execute("SELECT COUNT(*) FROM references_catalog;")
        ref_count = cursor.fetchone()[0]
        assert ref_count >= 15
        
        conn.close()
