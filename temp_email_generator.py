#!/usr/bin/env python3
"""
╔══════════════════════════════════════════════════╗
║        TEMPMAIL — Temporary Email Generator      ║
║     Modern UI · Copy to Clipboard · Auto-refresh ║
╚══════════════════════════════════════════════════╝

Usage:
    python3 temp_email_generator.py

Opens a browser-based UI. Uses Guerrilla Mail public API.
No external Python dependencies required.
"""

import http.server
import json
import threading
import urllib.request
import urllib.parse
import urllib.error
import webbrowser
import time
import random
import string
import sys
from datetime import datetime

# ──────────────────────────────────────────────
#  Guerrilla Mail API wrapper
# ──────────────────────────────────────────────
GUERRILLA_API = "https://api.guerrillamail.com/ajax.php"

def api_get(params: dict) -> dict:
    url = GUERRILLA_API + "?" + urllib.parse.urlencode(params)
    req = urllib.request.Request(url, headers={"User-Agent": "TempMailApp/1.0"})
    with urllib.request.urlopen(req, timeout=10) as r:
        return json.loads(r.read().decode())

def get_email_address(sid_token: str = "") -> dict:
    params = {"f": "get_email_address"}
    if sid_token:
        params["sid_token"] = sid_token
    return api_get(params)

def check_inbox(seq: int, sid_token: str) -> dict:
    return api_get({"f": "get_email_list", "offset": 0, "seq": seq, "sid_token": sid_token})

def fetch_email(mail_id: str, sid_token: str) -> dict:
    return api_get({"f": "fetch_email", "email_id": mail_id, "sid_token": sid_token})

def delete_email(mail_ids: list, sid_token: str) -> dict:
    ids = ",".join(mail_ids)
    return api_get({"f": "del_email", "email_ids": ids, "sid_token": sid_token})

def forget_me(sid_token: str, email_addr: str) -> dict:
    return api_get({"f": "forget_me", "sid_token": sid_token, "email_addr": email_addr})

# ──────────────────────────────────────────────
#  In-memory session state
# ──────────────────────────────────────────────
session = {
    "sid_token": "",
    "email": "",
    "alias": "",
    "seq": 0,
    "emails": [],
    "last_refresh": None,
    "error": None,
}

def init_session():
    try:
        data = get_email_address()
        session["sid_token"] = data.get("sid_token", "")
        session["email"] = data.get("email_addr", "")
        session["alias"] = data.get("alias", "")
        session["seq"] = 0
        session["emails"] = []
        session["last_refresh"] = datetime.now().isoformat()
        session["error"] = None
    except Exception as e:
        session["error"] = str(e)

def refresh_inbox():
    try:
        data = check_inbox(session["seq"], session["sid_token"])
        new_emails = data.get("list", [])
        if new_emails:
            session["seq"] = data.get("count", session["seq"])
            # Merge without duplicates
            existing_ids = {e["mail_id"] for e in session["emails"]}
            for mail in new_emails:
                if mail["mail_id"] not in existing_ids:
                    session["emails"].insert(0, mail)
        session["last_refresh"] = datetime.now().isoformat()
        session["error"] = None
        return {"ok": True, "count": len(session["emails"])}
    except Exception as e:
        session["error"] = str(e)
        return {"ok": False, "error": str(e)}

# ──────────────────────────────────────────────
#  Embedded HTML/CSS/JS UI
# ──────────────────────────────────────────────
HTML = r"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>TempMail — Disposable Email</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link href="https://fonts.googleapis.com/css2?family=Space+Mono:ital,wght@0,400;0,700;1,400&family=Syne:wght@400;700;800&display=swap" rel="stylesheet">
<style>
  :root {
    --bg:       #0a0a0f;
    --surface:  #111118;
    --card:     #16161f;
    --border:   #252535;
    --accent:   #7c6af7;
    --accent2:  #3de6c8;
    --accent3:  #f7c06a;
    --danger:   #f76a6a;
    --text:     #e8e6ff;
    --muted:    #6b6a88;
    --mono:     'Space Mono', monospace;
    --sans:     'Syne', sans-serif;
  }

  *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }

  body {
    background: var(--bg);
    color: var(--text);
    font-family: var(--sans);
    min-height: 100vh;
    display: flex;
    flex-direction: column;
    align-items: center;
    overflow-x: hidden;
  }

  /* ── Background grid ── */
  body::before {
    content: '';
    position: fixed;
    inset: 0;
    background-image:
      linear-gradient(var(--border) 1px, transparent 1px),
      linear-gradient(90deg, var(--border) 1px, transparent 1px);
    background-size: 40px 40px;
    opacity: 0.35;
    pointer-events: none;
    z-index: 0;
  }

  body::after {
    content: '';
    position: fixed;
    top: -200px; left: 50%;
    transform: translateX(-50%);
    width: 700px; height: 700px;
    background: radial-gradient(ellipse at center, rgba(124,106,247,0.12) 0%, transparent 70%);
    pointer-events: none;
    z-index: 0;
  }

  /* ── Header ── */
  header {
    position: relative;
    z-index: 1;
    text-align: center;
    padding: 52px 20px 32px;
  }

  .logo-tag {
    display: inline-block;
    font-family: var(--mono);
    font-size: 11px;
    letter-spacing: 3px;
    color: var(--accent2);
    border: 1px solid var(--accent2);
    padding: 3px 10px;
    margin-bottom: 16px;
    text-transform: uppercase;
  }

  h1 {
    font-size: clamp(36px, 7vw, 64px);
    font-weight: 800;
    letter-spacing: -2px;
    line-height: 1;
    background: linear-gradient(135deg, #fff 0%, var(--accent) 60%, var(--accent2) 100%);
    -webkit-background-clip: text;
    -webkit-text-fill-color: transparent;
    background-clip: text;
  }

  .subtitle {
    margin-top: 10px;
    font-family: var(--mono);
    font-size: 13px;
    color: var(--muted);
    letter-spacing: 0.5px;
  }

  /* ── Main layout ── */
  main {
    position: relative;
    z-index: 1;
    width: 100%;
    max-width: 780px;
    padding: 0 20px 60px;
    display: flex;
    flex-direction: column;
    gap: 20px;
  }

  /* ── Email card ── */
  .email-card {
    background: var(--card);
    border: 1px solid var(--border);
    border-radius: 16px;
    padding: 28px;
    position: relative;
    overflow: hidden;
  }

  .email-card::before {
    content: '';
    position: absolute;
    top: 0; left: 0; right: 0;
    height: 2px;
    background: linear-gradient(90deg, var(--accent), var(--accent2));
  }

  .email-label {
    font-family: var(--mono);
    font-size: 10px;
    letter-spacing: 2.5px;
    color: var(--muted);
    text-transform: uppercase;
    margin-bottom: 12px;
  }

  .email-display {
    display: flex;
    align-items: center;
    gap: 12px;
    flex-wrap: wrap;
  }

  .email-address {
    flex: 1;
    font-family: var(--mono);
    font-size: clamp(16px, 3.5vw, 22px);
    font-weight: 700;
    color: var(--accent2);
    background: rgba(61,230,200,0.06);
    border: 1px solid rgba(61,230,200,0.15);
    border-radius: 10px;
    padding: 14px 18px;
    min-width: 200px;
    word-break: break-all;
    letter-spacing: 0.5px;
    cursor: text;
    user-select: all;
    transition: background 0.2s;
  }

  .email-address:hover {
    background: rgba(61,230,200,0.1);
  }

  .email-address.loading {
    color: var(--muted);
    font-size: 14px;
    animation: pulse 1.5s infinite;
  }

  @keyframes pulse {
    0%, 100% { opacity: 0.5; }
    50% { opacity: 1; }
  }

  /* ── Buttons ── */
  .btn {
    display: inline-flex;
    align-items: center;
    gap: 7px;
    font-family: var(--mono);
    font-size: 12px;
    font-weight: 700;
    letter-spacing: 1px;
    padding: 12px 20px;
    border-radius: 10px;
    border: none;
    cursor: pointer;
    transition: all 0.2s;
    text-transform: uppercase;
    white-space: nowrap;
    position: relative;
    overflow: hidden;
  }

  .btn::after {
    content: '';
    position: absolute;
    inset: 0;
    background: rgba(255,255,255,0);
    transition: background 0.15s;
  }

  .btn:hover::after { background: rgba(255,255,255,0.07); }
  .btn:active { transform: scale(0.97); }

  .btn-copy {
    background: linear-gradient(135deg, var(--accent), #5a4fd4);
    color: white;
  }

  .btn-copy.copied {
    background: linear-gradient(135deg, #27a868, #1d8a55);
  }

  .btn-refresh {
    background: var(--surface);
    color: var(--text);
    border: 1px solid var(--border);
  }

  .btn-refresh:hover { border-color: var(--accent); }

  .btn-new {
    background: var(--surface);
    color: var(--accent3);
    border: 1px solid rgba(247,192,106,0.25);
  }

  .btn-new:hover { border-color: var(--accent3); }

  .btn-danger {
    background: var(--surface);
    color: var(--danger);
    border: 1px solid rgba(247,106,106,0.2);
  }

  .btn-danger:hover { border-color: var(--danger); }

  .actions {
    display: flex;
    gap: 10px;
    margin-top: 20px;
    flex-wrap: wrap;
  }

  /* ── Status bar ── */
  .status-bar {
    display: flex;
    align-items: center;
    gap: 10px;
    margin-top: 16px;
    padding-top: 16px;
    border-top: 1px solid var(--border);
    font-family: var(--mono);
    font-size: 11px;
    color: var(--muted);
    flex-wrap: wrap;
  }

  .dot {
    width: 7px; height: 7px;
    border-radius: 50%;
    background: var(--accent2);
    animation: blink 2s infinite;
  }

  .dot.offline { background: var(--muted); animation: none; }
  .dot.error { background: var(--danger); animation: none; }

  @keyframes blink {
    0%, 100% { opacity: 1; }
    50% { opacity: 0.3; }
  }

  /* ── Inbox ── */
  .inbox-header {
    display: flex;
    align-items: center;
    justify-content: space-between;
    margin-bottom: 14px;
  }

  .inbox-title {
    font-size: 18px;
    font-weight: 700;
    display: flex;
    align-items: center;
    gap: 10px;
  }

  .badge {
    display: inline-flex;
    align-items: center;
    justify-content: center;
    min-width: 22px;
    height: 22px;
    padding: 0 6px;
    border-radius: 20px;
    background: var(--accent);
    font-family: var(--mono);
    font-size: 11px;
    font-weight: 700;
    color: white;
  }

  .inbox-empty {
    text-align: center;
    padding: 48px 20px;
    color: var(--muted);
    font-family: var(--mono);
    font-size: 13px;
    line-height: 2;
  }

  .inbox-empty .icon {
    font-size: 40px;
    display: block;
    margin-bottom: 14px;
    opacity: 0.3;
  }

  .mail-list {
    display: flex;
    flex-direction: column;
    gap: 10px;
  }

  .mail-item {
    background: var(--surface);
    border: 1px solid var(--border);
    border-radius: 12px;
    padding: 16px 18px;
    cursor: pointer;
    transition: all 0.2s;
    display: grid;
    grid-template-columns: auto 1fr auto;
    gap: 12px;
    align-items: center;
  }

  .mail-item:hover {
    border-color: var(--accent);
    background: rgba(124,106,247,0.05);
    transform: translateX(3px);
  }

  .mail-item.unread .mail-subject { color: var(--text); }
  .mail-item .mail-subject { color: var(--muted); font-size: 14px; }

  .mail-avatar {
    width: 38px; height: 38px;
    border-radius: 50%;
    display: flex;
    align-items: center;
    justify-content: center;
    font-family: var(--mono);
    font-size: 14px;
    font-weight: 700;
    color: white;
    background: linear-gradient(135deg, var(--accent), var(--accent2));
    flex-shrink: 0;
    text-transform: uppercase;
  }

  .mail-meta { overflow: hidden; }

  .mail-from {
    font-size: 13px;
    font-weight: 700;
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
    margin-bottom: 3px;
    color: var(--text);
  }

  .mail-subject {
    font-family: var(--mono);
    font-size: 12px;
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
  }

  .mail-time {
    font-family: var(--mono);
    font-size: 10px;
    color: var(--muted);
    white-space: nowrap;
    text-align: right;
  }

  /* ── Email viewer modal ── */
  .modal-overlay {
    display: none;
    position: fixed;
    inset: 0;
    background: rgba(0,0,0,0.75);
    backdrop-filter: blur(6px);
    z-index: 100;
    align-items: center;
    justify-content: center;
    padding: 20px;
  }

  .modal-overlay.open { display: flex; }

  .modal {
    background: var(--card);
    border: 1px solid var(--border);
    border-radius: 18px;
    max-width: 680px;
    width: 100%;
    max-height: 85vh;
    display: flex;
    flex-direction: column;
    position: relative;
    overflow: hidden;
    animation: slideUp 0.25s ease;
  }

  @keyframes slideUp {
    from { transform: translateY(30px); opacity: 0; }
    to { transform: translateY(0); opacity: 1; }
  }

  .modal::before {
    content: '';
    position: absolute;
    top: 0; left: 0; right: 0;
    height: 2px;
    background: linear-gradient(90deg, var(--accent2), var(--accent));
  }

  .modal-header {
    padding: 22px 24px;
    border-bottom: 1px solid var(--border);
  }

  .modal-close {
    position: absolute;
    top: 16px; right: 16px;
    background: var(--surface);
    border: 1px solid var(--border);
    color: var(--text);
    border-radius: 8px;
    width: 32px; height: 32px;
    display: flex; align-items: center; justify-content: center;
    cursor: pointer;
    font-size: 16px;
    transition: all 0.2s;
  }

  .modal-close:hover { border-color: var(--danger); color: var(--danger); }

  .modal-from {
    font-family: var(--mono);
    font-size: 11px;
    color: var(--muted);
    letter-spacing: 1px;
    margin-bottom: 6px;
  }

  .modal-subject {
    font-size: 18px;
    font-weight: 700;
    line-height: 1.3;
    margin-bottom: 8px;
  }

  .modal-time {
    font-family: var(--mono);
    font-size: 11px;
    color: var(--muted);
  }

  .modal-body {
    padding: 22px 24px;
    overflow-y: auto;
    flex: 1;
    font-size: 14px;
    line-height: 1.8;
    color: var(--text);
    white-space: pre-wrap;
    font-family: var(--mono);
  }

  /* ── Toast ── */
  .toast {
    position: fixed;
    bottom: 30px; left: 50%;
    transform: translateX(-50%) translateY(80px);
    background: var(--accent2);
    color: #0a0a0f;
    font-family: var(--mono);
    font-size: 13px;
    font-weight: 700;
    padding: 12px 24px;
    border-radius: 30px;
    z-index: 200;
    transition: transform 0.3s cubic-bezier(0.34, 1.56, 0.64, 1);
    pointer-events: none;
    letter-spacing: 0.5px;
  }

  .toast.show { transform: translateX(-50%) translateY(0); }

  /* ── Error banner ── */
  .error-banner {
    display: none;
    background: rgba(247,106,106,0.1);
    border: 1px solid rgba(247,106,106,0.3);
    border-radius: 10px;
    padding: 12px 16px;
    font-family: var(--mono);
    font-size: 12px;
    color: var(--danger);
    align-items: center;
    gap: 8px;
  }

  .error-banner.show { display: flex; }

  /* ── Spinner ── */
  .spinner {
    width: 14px; height: 14px;
    border: 2px solid rgba(255,255,255,0.2);
    border-top-color: currentColor;
    border-radius: 50%;
    animation: spin 0.6s linear infinite;
    display: none;
  }

  .spinning .spinner { display: inline-block; }
  .spinning .btn-icon { display: none; }

  @keyframes spin { to { transform: rotate(360deg); } }
</style>
</head>
<body>

<header>
  <div class="logo-tag">⬡ disposable · private · instant</div>
  <h1>TempMail</h1>
  <p class="subtitle">// burner inbox. no sign-up. auto-expires.</p>
</header>

<main>
  <!-- Error banner -->
  <div class="error-banner" id="errorBanner">
    <span>⚠</span>
    <span id="errorText">Connection error</span>
  </div>

  <!-- Email address card -->
  <div class="email-card">
    <div class="email-label">▸ your temporary address</div>
    <div class="email-display">
      <div class="email-address loading" id="emailAddr">Generating address…</div>
      <button class="btn btn-copy" id="copyBtn" onclick="copyEmail()">
        <span class="btn-icon">⎘</span>
        <span class="spinner"></span>
        Copy
      </button>
    </div>
    <div class="actions">
      <button class="btn btn-refresh" id="refreshBtn" onclick="refreshInbox()">
        <span class="btn-icon">↺</span>
        <span class="spinner"></span>
        Refresh
      </button>
      <button class="btn btn-new" onclick="newEmail()">
        <span class="btn-icon">✦</span>
        New Address
      </button>
      <button class="btn btn-danger" onclick="deleteAll()">
        <span class="btn-icon">✕</span>
        Clear Inbox
      </button>
    </div>
    <div class="status-bar">
      <div class="dot" id="statusDot"></div>
      <span id="statusText">Initializing…</span>
      <span style="margin-left:auto" id="lastRefresh"></span>
    </div>
  </div>

  <!-- Inbox card -->
  <div class="email-card">
    <div class="inbox-header">
      <div class="inbox-title">
        Inbox
        <span class="badge" id="inboxBadge">0</span>
      </div>
      <span style="font-family:var(--mono);font-size:11px;color:var(--muted)">
        auto-refreshes every 15s
      </span>
    </div>
    <div id="mailList">
      <div class="inbox-empty">
        <span class="icon">✉</span>
        No messages yet<br>
        Send an email to your temp address
      </div>
    </div>
  </div>
</main>

<!-- Email viewer modal -->
<div class="modal-overlay" id="modalOverlay" onclick="closeModal(event)">
  <div class="modal">
    <button class="modal-close" onclick="closeModal()">✕</button>
    <div class="modal-header">
      <div class="modal-from" id="modalFrom"></div>
      <div class="modal-subject" id="modalSubject"></div>
      <div class="modal-time" id="modalTime"></div>
    </div>
    <div class="modal-body" id="modalBody">Loading…</div>
  </div>
</div>

<!-- Toast -->
<div class="toast" id="toast"></div>

<script>
let emails = [];
let autoTimer = null;

// ── Init ─────────────────────────────────────
async function init() {
  setStatus('loading', 'Connecting to mail server…');
  try {
    const data = await api('/api/init');
    if (data.error) throw new Error(data.error);
    document.getElementById('emailAddr').textContent = data.email;
    document.getElementById('emailAddr').classList.remove('loading');
    setStatus('ok', 'Connected · inbox active');
    hideError();
    scheduleAutoRefresh();
  } catch(e) {
    document.getElementById('emailAddr').textContent = 'Connection failed';
    setStatus('error', 'Failed to connect');
    showError('Could not reach mail server. Check your internet connection.');
  }
}

// ── Copy email ───────────────────────────────
async function copyEmail() {
  const addr = document.getElementById('emailAddr').textContent;
  if (!addr || addr.includes('…') || addr.includes('failed')) return;
  try {
    await navigator.clipboard.writeText(addr);
    const btn = document.getElementById('copyBtn');
    btn.classList.add('copied');
    btn.querySelector('.btn-icon').textContent = '✓';
    showToast('📋 Copied to clipboard!');
    setTimeout(() => {
      btn.classList.remove('copied');
      btn.querySelector('.btn-icon').textContent = '⎘';
    }, 2000);
  } catch {
    // Fallback
    const el = document.createElement('textarea');
    el.value = addr;
    document.body.appendChild(el);
    el.select();
    document.execCommand('copy');
    document.body.removeChild(el);
    showToast('📋 Copied!');
  }
}

// ── Refresh inbox ────────────────────────────
async function refreshInbox() {
  const btn = document.getElementById('refreshBtn');
  btn.classList.add('spinning');
  setStatus('loading', 'Checking inbox…');
  try {
    const data = await api('/api/refresh');
    if (data.error) throw new Error(data.error);
    emails = data.emails || [];
    renderInbox();
    setStatus('ok', `Connected · ${emails.length} message${emails.length !== 1 ? 's' : ''}`);
    updateLastRefresh();
    hideError();
  } catch(e) {
    setStatus('error', 'Refresh failed');
    showError(e.message);
  } finally {
    btn.classList.remove('spinning');
  }
}

// ── Render inbox ─────────────────────────────
function renderInbox() {
  const list = document.getElementById('mailList');
  const badge = document.getElementById('inboxBadge');
  badge.textContent = emails.length;

  if (!emails.length) {
    list.innerHTML = `
      <div class="inbox-empty">
        <span class="icon">✉</span>
        No messages yet<br>Send an email to your temp address
      </div>`;
    return;
  }

  list.innerHTML = emails.map((m, i) => `
    <div class="mail-item unread" onclick="openEmail(${i})">
      <div class="mail-avatar">${(m.mail_from || 'X')[0]}</div>
      <div class="mail-meta">
        <div class="mail-from">${escHtml(m.mail_from || 'Unknown Sender')}</div>
        <div class="mail-subject">${escHtml(m.mail_subject || '(no subject)')}</div>
      </div>
      <div class="mail-time">${formatTime(m.mail_date)}</div>
    </div>
  `).join('');
}

// ── Open email ───────────────────────────────
async function openEmail(idx) {
  const mail = emails[idx];
  document.getElementById('modalFrom').textContent = '▸ FROM: ' + (mail.mail_from || 'Unknown');
  document.getElementById('modalSubject').textContent = mail.mail_subject || '(no subject)';
  document.getElementById('modalTime').textContent = formatTime(mail.mail_date, true);
  document.getElementById('modalBody').textContent = 'Loading email body…';
  document.getElementById('modalOverlay').classList.add('open');

  try {
    const data = await api(`/api/email?id=${mail.mail_id}`);
    const body = data.mail_body || data.mail_excerpt || '(empty message)';
    document.getElementById('modalBody').textContent = body;
  } catch {
    document.getElementById('modalBody').textContent = mail.mail_excerpt || '(could not load body)';
  }
}

// ── Close modal ──────────────────────────────
function closeModal(e) {
  if (!e || e.target === document.getElementById('modalOverlay') || e.currentTarget.tagName === 'BUTTON') {
    document.getElementById('modalOverlay').classList.remove('open');
  }
}

// ── New address ──────────────────────────────
async function newEmail() {
  if (!confirm('Generate a new temporary address? Your current inbox will be cleared.')) return;
  document.getElementById('emailAddr').textContent = 'Generating…';
  document.getElementById('emailAddr').classList.add('loading');
  emails = [];
  renderInbox();
  clearAutoRefresh();
  try {
    const data = await api('/api/new');
    if (data.error) throw new Error(data.error);
    document.getElementById('emailAddr').textContent = data.email;
    document.getElementById('emailAddr').classList.remove('loading');
    setStatus('ok', 'New address ready');
    scheduleAutoRefresh();
    showToast('✦ New address generated!');
  } catch(e) {
    setStatus('error', 'Failed to generate');
    showError(e.message);
  }
}

// ── Delete all ───────────────────────────────
async function deleteAll() {
  if (!emails.length) { showToast('Inbox already empty'); return; }
  if (!confirm('Delete all messages from your inbox?')) return;
  try {
    const ids = emails.map(m => m.mail_id);
    await api('/api/delete?ids=' + ids.join(','));
    emails = [];
    renderInbox();
    setStatus('ok', 'Inbox cleared');
    showToast('✕ All messages deleted');
  } catch(e) {
    showError(e.message);
  }
}

// ── Auto-refresh ─────────────────────────────
function scheduleAutoRefresh() {
  clearAutoRefresh();
  autoTimer = setInterval(refreshInbox, 15000);
}

function clearAutoRefresh() {
  if (autoTimer) { clearInterval(autoTimer); autoTimer = null; }
}

// ── Helpers ──────────────────────────────────
async function api(path) {
  const r = await fetch(path);
  return r.json();
}

function setStatus(state, msg) {
  const dot = document.getElementById('statusDot');
  const text = document.getElementById('statusText');
  dot.className = 'dot' + (state === 'error' ? ' error' : state === 'loading' ? ' offline' : '');
  text.textContent = msg;
}

function updateLastRefresh() {
  document.getElementById('lastRefresh').textContent =
    'updated ' + new Date().toLocaleTimeString();
}

function showToast(msg) {
  const t = document.getElementById('toast');
  t.textContent = msg;
  t.classList.add('show');
  setTimeout(() => t.classList.remove('show'), 2500);
}

function showError(msg) {
  const b = document.getElementById('errorBanner');
  document.getElementById('errorText').textContent = msg;
  b.classList.add('show');
}

function hideError() {
  document.getElementById('errorBanner').classList.remove('show');
}

function escHtml(s) {
  return String(s).replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;');
}

function formatTime(ts, long = false) {
  if (!ts) return '';
  const d = new Date(typeof ts === 'number' ? ts * 1000 : ts);
  if (long) return d.toLocaleString();
  const now = Date.now();
  const diff = Math.floor((now - d.getTime()) / 1000);
  if (diff < 60) return 'just now';
  if (diff < 3600) return Math.floor(diff/60) + 'm ago';
  if (diff < 86400) return Math.floor(diff/3600) + 'h ago';
  return d.toLocaleDateString();
}

// keyboard: Escape closes modal
document.addEventListener('keydown', e => {
  if (e.key === 'Escape') closeModal({target: document.getElementById('modalOverlay')});
});

init();
</script>
</body>
</html>"""

# ──────────────────────────────────────────────
#  HTTP Request Handler
# ──────────────────────────────────────────────
class TempMailHandler(http.server.BaseHTTPRequestHandler):

    def log_message(self, fmt, *args):
        pass  # silence default logging

    def send_json(self, data, code=200):
        body = json.dumps(data).encode()
        self.send_response(code)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def send_html(self, html):
        body = html.encode()
        self.send_response(200)
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self):
        path = urllib.parse.urlparse(self.path)

        # ── Serve UI
        if path.path in ("/", "/index.html"):
            self.send_html(HTML)

        # ── Init session
        elif path.path == "/api/init":
            if not session["sid_token"]:
                init_session()
            if session["error"]:
                self.send_json({"error": session["error"]})
            else:
                self.send_json({"email": session["email"], "alias": session["alias"]})

        # ── Refresh inbox
        elif path.path == "/api/refresh":
            result = refresh_inbox()
            self.send_json({
                **result,
                "emails": session["emails"],
                "count": len(session["emails"])
            })

        # ── Fetch single email
        elif path.path == "/api/email":
            params = urllib.parse.parse_qs(path.query)
            mail_id = params.get("id", [""])[0]
            try:
                data = fetch_email(mail_id, session["sid_token"])
                self.send_json(data)
            except Exception as e:
                self.send_json({"error": str(e)}, 500)

        # ── New address
        elif path.path == "/api/new":
            try:
                data = forget_me(session["sid_token"], session["email"])
            except Exception:
                pass
            init_session()
            if session["error"]:
                self.send_json({"error": session["error"]})
            else:
                self.send_json({"email": session["email"]})

        # ── Delete emails
        elif path.path == "/api/delete":
            params = urllib.parse.parse_qs(path.query)
            ids_str = params.get("ids", [""])[0]
            mail_ids = [i for i in ids_str.split(",") if i]
            try:
                delete_email(mail_ids, session["sid_token"])
                session["emails"] = [e for e in session["emails"] if e["mail_id"] not in mail_ids]
                self.send_json({"ok": True})
            except Exception as e:
                self.send_json({"error": str(e)}, 500)

        else:
            self.send_response(404)
            self.end_headers()

# ──────────────────────────────────────────────
#  Entry point
# ──────────────────────────────────────────────
def find_free_port(start=7788):
    import socket
    for port in range(start, start + 20):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            if s.connect_ex(("localhost", port)) != 0:
                return port
    return start

def main():
    port = find_free_port()
    url  = f"http://localhost:{port}"

    server = http.server.HTTPServer(("localhost", port), TempMailHandler)

    print()
    print("  ╔═══════════════════════════════════════╗")
    print("  ║   TempMail — Temporary Email Generator ║")
    print("  ╚═══════════════════════════════════════╝")
    print(f"  ▸ Server running at: {url}")
    print("  ▸ Opening browser…")
    print("  ▸ Press Ctrl+C to stop")
    print()

    # Open browser after short delay
    def open_browser():
        time.sleep(0.8)
        webbrowser.open(url)

    threading.Thread(target=open_browser, daemon=True).start()

    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\n  ▸ Shutting down. Goodbye!")
        server.shutdown()

if __name__ == "__main__":
    main()
