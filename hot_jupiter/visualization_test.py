"""
Unit tests for PaperStyle design system and visualization module.
"""

import os

import matplotlib.pyplot as plt

from hot_jupiter.visualization import PaperStyle


def test_paper_style_colors_and_apply():
    """Test that PaperStyle applies rcParams and defines valid colors."""
    PaperStyle.apply()
    assert 'ZONE_I' in PaperStyle.COLORS
    assert 'ZONE_II' in PaperStyle.COLORS
    assert 'ZONE_III' in PaperStyle.COLORS
    assert plt.rcParams['figure.dpi'] == 300


def test_panel_label_and_save_figure(tmp_path):
    """Test panel label formatting and figure saving."""
    PaperStyle.apply()
    fig, ax = plt.subplots()
    ax.plot([0, 1], [0, 1])
    PaperStyle.add_panel_label(ax, 'a')

    save_path = os.path.join(tmp_path, "test_fig.png")
    PaperStyle.save_figure(fig, save_path)
    assert os.path.exists(save_path)
    assert os.path.exists(os.path.join(tmp_path, "test_fig.pdf"))
    plt.close(fig)
