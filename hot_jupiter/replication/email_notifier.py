"""
Automated email notification module for paper replication alerts and daily digests.
"""

import logging
import os
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

logger = logging.getLogger(__name__)


class EmailNotifier:
    """
    Handles automated SMTP email notifications for paper replications.
    """

    def __init__(
        self,
        smtp_host: str | None = None,
        smtp_port: int | None = None,
        smtp_user: str | None = None,
        smtp_password: str | None = None,
        recipient_email: str | None = None,
    ):
        self.smtp_host = smtp_host or os.getenv("SMTP_HOST", "localhost")
        self.smtp_port = smtp_port or int(os.getenv("SMTP_PORT", "587"))
        self.smtp_user = smtp_user or os.getenv("SMTP_USER", "")
        self.smtp_password = smtp_password or os.getenv("SMTP_PASSWORD", "")
        self.recipient_email = recipient_email or os.getenv(
            "RECIPIENT_EMAIL", "")

    def is_configured(self) -> bool:
        return bool(self.recipient_email and self.smtp_host and
                    self.smtp_host != "localhost")

    def send_replication_report(
        self,
        paper: dict,
        agreement_score: float,
        discrepancy_type: str,
        details: str,
    ) -> bool:
        """
        Sends an email report for a completed paper replication.
        """
        subject = f"[Replication Report] {paper.get('authors', 'Unknown')} ({paper.get('year', '')}) - Score: {agreement_score:.1%}"

        body_text = f"""
Paper Replication Report
------------------------
Title: {paper.get('title', 'N/A')}
Authors: {paper.get('authors', 'N/A')} ({paper.get('year', 'N/A')})
arXiv ID: {paper.get('arxiv_id', 'N/A')}
Subfield/Topic: {paper.get('topic', 'N/A')}

Status: {paper.get('replication_status', 'VERIFIED')}
Agreement Score: {agreement_score:.2f} ({agreement_score:.1%})
Discrepancy Category: {discrepancy_type}

Details & Verification Summary:
{details}
"""

        body_html = f"""
<html>
  <body>
    <h2>Paper Replication Report</h2>
    <p><strong>Title:</strong> {paper.get('title', 'N/A')}<br/>
       <strong>Authors:</strong> {paper.get('authors', 'N/A')} ({paper.get('year', 'N/A')})<br/>
       <strong>arXiv ID:</strong> {paper.get('arxiv_id', 'N/A')}<br/>
       <strong>Topic:</strong> {paper.get('topic', 'N/A')}</p>
    <hr/>
    <p><strong>Status:</strong> <span style="color:green;">{paper.get('replication_status', 'VERIFIED')}</span><br/>
       <strong>Agreement Score:</strong> {agreement_score:.1%}<br/>
       <strong>Discrepancy Category:</strong> {discrepancy_type}</p>
    <h3>Verification Details:</h3>
    <pre>{details}</pre>
  </body>
</html>
"""

        return self._send_email(subject, body_text, body_html)

    def send_daily_digest(self, stats: dict, recent_papers: list[dict]) -> bool:
        """
        Sends a daily summary digest of the replication progress.
        """
        subject = f"[Replication Digest] Total Verified: {stats.get('verified_papers', 0)}/{stats.get('total_papers', 0)}"

        paper_rows = "\n".join([
            f"- [{p.get('arxiv_id')}] {p.get('title')} ({p.get('authors')}, {p.get('year')}) | Score: {p.get('agreement_score', 0):.1%}"
            for p in recent_papers
        ])

        body_text = f"""
Daily Literature Replication Digest
------------------------------------
Total Cataloged Papers: {stats.get('total_papers', 0)}
Total Verified Papers:  {stats.get('verified_papers', 0)}
Average Agreement Score: {stats.get('avg_agreement_score', 0.0):.1%}

Recent Verified Replications:
{paper_rows}
"""

        return self._send_email(subject, body_text)

    def _send_email(self,
                    subject: str,
                    text_body: str,
                    html_body: str | None = None) -> bool:
        if not self.is_configured():
            logger.info(
                "SMTP not fully configured; dry-run mode active. Report subject: %s",
                subject)
            return True

        msg = MIMEMultipart("alternative")
        msg["Subject"] = subject
        msg["From"] = self.smtp_user or "autonav@antigravity.ai"
        msg["To"] = self.recipient_email

        msg.attach(MIMEText(text_body, "plain"))
        if html_body:
            msg.attach(MIMEText(html_body, "html"))

        try:
            with smtplib.SMTP(self.smtp_host, self.smtp_port) as server:
                server.starttls()
                if self.smtp_user and self.smtp_password:
                    server.login(self.smtp_user, self.smtp_password)
                server.sendmail(msg["From"], [self.recipient_email],
                                msg.as_string())
            logger.info("Replication email successfully sent to %s",
                        self.recipient_email)
            return True
        except (smtplib.SMTPException, OSError) as e:
            logger.error("Failed to send replication email: %s", e)
            return False
