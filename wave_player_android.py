"""
╔══════════════════════════════════════════════════════╗
║     WAVE — Android Music Player (Pydroid 3)          ║
║                                                      ║
║  Optimized for touch screens & Android storage       ║
║                                                      ║
║  SETUP in Pydroid 3:                                 ║
║    Menu → Pip → install: pygame                      ║
║                                                      ║
║  HOW TO USE:                                         ║
║    1. Tap "SCAN MUSIC" to find all audio on device   ║
║    2. Or tap "BROWSE" to pick a folder manually      ║
║    3. Tap any track to play                          ║
║    4. Use big buttons at bottom for controls         ║
╚══════════════════════════════════════════════════════╝
"""

import os
import time
import random
import tkinter as tk
from tkinter import messagebox
from pathlib import Path

# ── Try pygame ────────────────────────────────────────────────────────────────
try:
    import pygame
    pygame.mixer.init(frequency=44100, size=-16, channels=2, buffer=4096)
    PYGAME_OK = True
except Exception as e:
    PYGAME_OK = False
    print(f"pygame error: {e}")

# ── Android common music directories ─────────────────────────────────────────
ANDROID_MUSIC_DIRS = [
    "/sdcard/Music",
    "/sdcard/Download",
    "/sdcard/Downloads",
    "/storage/emulated/0/Music",
    "/storage/emulated/0/Download",
    "/storage/emulated/0/Downloads",
    "/sdcard",
    os.path.expanduser("~/Music"),
    os.path.expanduser("~/Downloads"),
]
SUPPORTED_EXT = {".mp3", ".wav", ".ogg", ".flac", ".aac", ".m4a", ".opus"}

# ── Color palette ─────────────────────────────────────────────────────────────
BG       = "#0d0d14"
PANEL    = "#14141f"
CARD     = "#1a1a28"
ACCENT   = "#00e5c8"
PINK     = "#ff2d6f"
TEXT     = "#eeeef5"
DIM      = "#4a4a6a"
GREEN    = "#00ff88"
AMBER    = "#ffaa00"
BORDER   = "#252535"

# ── Fonts — larger for touch ─────────────────────────────────────────────────
F_TITLE  = ("sans-serif", 13, "bold")
F_BODY   = ("sans-serif", 11)
F_SMALL  = ("sans-serif", 9)
F_ICON   = ("sans-serif", 20, "bold")
F_CTRL   = ("sans-serif", 18, "bold")
F_LOGO   = ("sans-serif", 17, "bold")


# ══════════════════════════════════════════════════════════════════════════════
#  Audio Engine
# ══════════════════════════════════════════════════════════════════════════════
class AudioEngine:
    def __init__(self):
        self._paused   = False
        self._start_t  = 0.0
        self._pause_t  = 0.0
        self._duration = 0.0

    def load(self, path: str, duration: float = 0.0) -> bool:
        if not PYGAME_OK:
            return False
        try:
            pygame.mixer.music.load(path)
            self._duration = duration
            self._paused   = False
            self._start_t  = 0.0
            self._pause_t  = 0.0
            return True
        except Exception as e:
            print(f"[load] {e}")
            return False

    def play(self, start: float = 0.0):
        if not PYGAME_OK:
            return
        try:
            pygame.mixer.music.play(start=start)
        except Exception:
            pygame.mixer.music.play()
        self._start_t = time.time() - start
        self._paused  = False

    def pause(self):
        if not PYGAME_OK or self._paused:
            return
        pygame.mixer.music.pause()
        self._pause_t = self.position
        self._paused  = True

    def resume(self):
        if not PYGAME_OK or not self._paused:
            return
        pygame.mixer.music.unpause()
        self._start_t = time.time() - self._pause_t
        self._paused  = False

    def stop(self):
        if not PYGAME_OK:
            return
        pygame.mixer.music.stop()
        self._paused  = False
        self._start_t = 0.0
        self._pause_t = 0.0

    def seek(self, pos: float):
        self.play(start=max(0.0, pos))

    def set_volume(self, v: float):
        if PYGAME_OK:
            pygame.mixer.music.set_volume(max(0.0, min(1.0, v)))

    @property
    def position(self) -> float:
        if self._paused:
            return self._pause_t
        if self._start_t == 0.0:
            return 0.0
        return time.time() - self._start_t

    @property
    def is_playing(self) -> bool:
        return PYGAME_OK and pygame.mixer.music.get_busy() and not self._paused

    @property
    def is_paused(self) -> bool:
        return self._paused

    @property
    def finished(self) -> bool:
        if not PYGAME_OK or self._paused or self._start_t == 0.0:
            return False
        if self._duration > 0:
            return self.position >= self._duration and not pygame.mixer.music.get_busy()
        return not pygame.mixer.music.get_busy() and self.position > 1.0


# ══════════════════════════════════════════════════════════════════════════════
#  Helpers
# ══════════════════════════════════════════════════════════════════════════════
def fmt(sec: float) -> str:
    sec = max(0, int(sec))
    m, s = divmod(sec, 60)
    h, m = divmod(m, 60)
    return f"{h}:{m:02d}:{s:02d}" if h else f"{m}:{s:02d}"

def scan_dir(folder: str) -> list:
    results = []
    try:
        for root, _, files in os.walk(folder):
            for f in sorted(files):
                if Path(f).suffix.lower() in SUPPORTED_EXT:
                    results.append(os.path.join(root, f))
    except PermissionError:
        pass
    return results

def track_name(path: str) -> str:
    return Path(path).stem[:42]


# ══════════════════════════════════════════════════════════════════════════════
#  Browse Dialog (folder picker — works without filedialog on Android)
# ══════════════════════════════════════════════════════════════════════════════
class FolderBrowser(tk.Toplevel):
    """A simple file-system browser that works on Android (no native dialog)."""

    def __init__(self, parent, start_dir: str = "/sdcard", callback=None):
        super().__init__(parent)
        self.title("Browse Folder")
        self.configure(bg=BG)
        self.geometry("400x560")
        self.callback = callback
        self.current  = start_dir if os.path.isdir(start_dir) else "/sdcard"

        self._build()
        self._refresh()
        self.grab_set()

    def _build(self):
        # Path bar
        top = tk.Frame(self, bg=PANEL, pady=8)
        top.pack(fill="x")

        self._path_var = tk.StringVar()
        tk.Label(top, textvariable=self._path_var, font=F_SMALL,
                 fg=ACCENT, bg=PANEL, wraplength=360, justify="left").pack(padx=10, fill="x")

        # Listbox
        lf = tk.Frame(self, bg=BG)
        lf.pack(fill="both", expand=True, padx=6, pady=6)

        sb = tk.Scrollbar(lf, bg=PANEL, troughcolor=BORDER)
        sb.pack(side="right", fill="y")

        self.lb = tk.Listbox(lf, bg=CARD, fg=TEXT, selectbackground=ACCENT,
                             selectforeground=BG, font=F_BODY, bd=0,
                             highlightthickness=0, yscrollcommand=sb.set,
                             activestyle="none")
        self.lb.pack(fill="both", expand=True)
        sb.config(command=self.lb.yview)
        self.lb.bind("<Double-Button-1>", self._on_dbl)
        self.lb.bind("<Button-1>",        self._on_tap)

        # Buttons
        btn_row = tk.Frame(self, bg=PANEL, pady=10)
        btn_row.pack(fill="x")

        self._mk_btn(btn_row, "⬆ UP",           self._go_up,   ACCENT).pack(side="left",  padx=8)
        self._mk_btn(btn_row, "✔ USE THIS FOLDER", self._use,  GREEN).pack(side="left",  padx=4)
        self._mk_btn(btn_row, "✕ CANCEL",        self.destroy, PINK).pack(side="right", padx=8)

    def _mk_btn(self, p, t, cmd, fg):
        b = tk.Button(p, text=t, command=cmd, font=F_SMALL,
                      fg=fg, bg=BORDER, activeforeground=fg,
                      activebackground=CARD, bd=0, relief="flat",
                      padx=10, pady=6, cursor="hand2")
        return b

    def _refresh(self):
        self._path_var.set(self.current)
        self.lb.delete(0, "end")
        self._entries = []
        try:
            items = sorted(os.scandir(self.current), key=lambda e: (not e.is_dir(), e.name.lower()))
            for e in items:
                icon = "📁 " if e.is_dir() else "🎵 "
                label = icon + e.name[:44]
                self.lb.insert("end", label)
                self._entries.append(e)
        except PermissionError:
            self.lb.insert("end", "  ⛔ Permission denied")

    def _on_tap(self, _e):
        pass  # single tap — do nothing extra

    def _on_dbl(self, _e):
        sel = self.lb.curselection()
        if not sel:
            return
        entry = self._entries[sel[0]]
        if entry.is_dir():
            self.current = entry.path
            self._refresh()

    def _go_up(self):
        parent = str(Path(self.current).parent)
        if parent != self.current:
            self.current = parent
            self._refresh()

    def _use(self):
        if self.callback:
            self.callback(self.current)
        self.destroy()


# ══════════════════════════════════════════════════════════════════════════════
#  Main App
# ══════════════════════════════════════════════════════════════════════════════
class WavePlayer:
    REPEAT_CYCLE = ["none", "all", "one"]

    def __init__(self, root: tk.Tk):
        self.root     = root
        self.root.title("WAVE Player")
        self.root.configure(bg=BG)
        # Portrait orientation for phones
        self.root.geometry("400x800")
        self.root.resizable(True, True)

        self.engine   = AudioEngine()
        self.playlist : list[str] = []
        self.current  : int = -1
        self.shuffle  : bool = False
        self.repeat   : str = "none"
        self._dragging = False
        self._vol      = 0.8
        self.engine.set_volume(self._vol)

        self._build_ui()
        self._poll()

        if not PYGAME_OK:
            self._toast("pygame not found!\nPydroid → Pip → install pygame")

    # ── UI ────────────────────────────────────────────────────────────────────
    def _build_ui(self):
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(2, weight=1)   # playlist grows

        self._build_header(row=0)
        self._build_now_playing(row=1)
        self._build_playlist(row=2)
        self._build_controls(row=3)
        self._build_statusbar(row=4)

    # ── Header ────────────────────────────────────────────────────────────────
    def _build_header(self, row):
        hdr = tk.Frame(self.root, bg=BG, pady=10)
        hdr.grid(row=row, column=0, sticky="ew", padx=14)
        hdr.columnconfigure(1, weight=1)

        tk.Label(hdr, text="▶ WAVE", font=F_LOGO,
                 fg=ACCENT, bg=BG).grid(row=0, column=0, sticky="w")
        tk.Label(hdr, text="android edition",
                 font=F_SMALL, fg=DIM, bg=BG).grid(row=0, column=1, sticky="w", padx=10)

        # Scan + Browse buttons top-right
        btns = tk.Frame(hdr, bg=BG)
        btns.grid(row=0, column=2, sticky="e")
        self._touch_btn(btns, "SCAN", self._scan_music, ACCENT, pad=6).pack(side="left", padx=2)
        self._touch_btn(btns, "BROWSE", self._browse,   ACCENT, pad=6).pack(side="left", padx=2)

    # ── Now Playing panel ─────────────────────────────────────────────────────
    def _build_now_playing(self, row):
        card = tk.Frame(self.root, bg=PANEL, pady=12)
        card.grid(row=row, column=0, sticky="ew", padx=10, pady=(0, 6))
        card.columnconfigure(0, weight=1)

        # VU meter canvas
        self._vu = tk.Canvas(card, bg=BG, height=70, bd=0, highlightthickness=0)
        self._vu.grid(row=0, column=0, sticky="ew", padx=10, pady=(0, 8))
        self._vu.bind("<Configure>", lambda e: self._draw_vu_idle())
        self._draw_vu_idle()

        # Track title
        self._title_var = tk.StringVar(value="No track loaded")
        tk.Label(card, textvariable=self._title_var, font=F_TITLE,
                 fg=TEXT, bg=PANEL, wraplength=360,
                 anchor="w", justify="left").grid(row=1, column=0, sticky="ew", padx=12)

        # Artist
        self._artist_var = tk.StringVar(value="Tap SCAN to find music")
        tk.Label(card, textvariable=self._artist_var, font=F_SMALL,
                 fg=ACCENT, bg=PANEL, anchor="w").grid(row=2, column=0, sticky="ew", padx=12)

        # Progress row
        prog = tk.Frame(card, bg=PANEL)
        prog.grid(row=3, column=0, sticky="ew", padx=10, pady=(10, 0))
        prog.columnconfigure(1, weight=1)

        self._pos_var = tk.StringVar(value="0:00")
        self._dur_var = tk.StringVar(value="0:00")
        tk.Label(prog, textvariable=self._pos_var, font=F_SMALL,
                 fg=DIM, bg=PANEL, width=5).grid(row=0, column=0)

        self._seek_var = tk.DoubleVar(value=0)
        self._seek = tk.Scale(prog, variable=self._seek_var, from_=0, to=100,
                              orient="horizontal", showvalue=False,
                              bg=PANEL, fg=ACCENT, troughcolor=BORDER,
                              activebackground=ACCENT, sliderlength=22,
                              sliderrelief="flat", highlightthickness=0, bd=0)
        self._seek.grid(row=0, column=1, sticky="ew")
        self._seek.bind("<ButtonPress-1>",   lambda e: setattr(self, "_dragging", True))
        self._seek.bind("<ButtonRelease-1>", self._on_seek_release)

        tk.Label(prog, textvariable=self._dur_var, font=F_SMALL,
                 fg=DIM, bg=PANEL, width=5).grid(row=0, column=2)

        # Volume row
        vol = tk.Frame(card, bg=PANEL)
        vol.grid(row=4, column=0, sticky="ew", padx=10, pady=(6, 4))
        vol.columnconfigure(1, weight=1)

        tk.Label(vol, text="VOL", font=F_SMALL, fg=DIM, bg=PANEL, width=4).grid(row=0, column=0)
        self._vol_var = tk.DoubleVar(value=self._vol)
        vol_sl = tk.Scale(vol, variable=self._vol_var, from_=0, to=1, resolution=0.01,
                          orient="horizontal", showvalue=False,
                          bg=PANEL, fg=PINK, troughcolor=BORDER,
                          activebackground=PINK, sliderlength=22,
                          sliderrelief="flat", highlightthickness=0, bd=0,
                          command=self._on_vol)
        vol_sl.grid(row=0, column=1, sticky="ew")

        self._vol_lbl = tk.Label(vol, text="80%", font=F_SMALL,
                                 fg=DIM, bg=PANEL, width=4)
        self._vol_lbl.grid(row=0, column=2)

    # ── Playlist ──────────────────────────────────────────────────────────────
    def _build_playlist(self, row):
        outer = tk.Frame(self.root, bg=BG)
        outer.grid(row=row, column=0, sticky="nsew", padx=10, pady=(0, 4))
        outer.columnconfigure(0, weight=1)
        outer.rowconfigure(1, weight=1)

        # Playlist header
        ph = tk.Frame(outer, bg=PANEL, pady=6)
        ph.grid(row=0, column=0, sticky="ew")
        ph.columnconfigure(0, weight=1)

        self._count_var = tk.StringVar(value="0 tracks")
        tk.Label(ph, textvariable=self._count_var, font=F_SMALL,
                 fg=ACCENT, bg=PANEL).grid(row=0, column=0, sticky="w", padx=10)

        rm_row = tk.Frame(ph, bg=PANEL)
        rm_row.grid(row=0, column=1, sticky="e", padx=6)
        self._touch_btn(rm_row, "- Remove", self._remove, PINK, pad=5).pack(side="left", padx=2)
        self._touch_btn(rm_row, "Clear All", self._clear, PINK, pad=5).pack(side="left", padx=2)

        # Listbox
        lb_frame = tk.Frame(outer, bg=CARD)
        lb_frame.grid(row=1, column=0, sticky="nsew")
        lb_frame.columnconfigure(0, weight=1)
        lb_frame.rowconfigure(0, weight=1)

        sb = tk.Scrollbar(lb_frame, bg=PANEL, troughcolor=BORDER)
        sb.pack(side="right", fill="y")

        self.lb = tk.Listbox(lb_frame, bg=CARD, fg=TEXT,
                             selectbackground=ACCENT, selectforeground=BG,
                             font=F_BODY, bd=0, highlightthickness=0,
                             activestyle="none", yscrollcommand=sb.set)
        self.lb.pack(side="left", fill="both", expand=True)
        sb.config(command=self.lb.yview)

        # Touch: single tap selects; double tap plays
        self.lb.bind("<Double-Button-1>", self._on_lb_dbl)
        self.lb.bind("<Button-1>",        self._on_lb_tap)

    # ── Controls ──────────────────────────────────────────────────────────────
    def _build_controls(self, row):
        ctrl = tk.Frame(self.root, bg=PANEL, pady=10)
        ctrl.grid(row=row, column=0, sticky="ew", padx=0, pady=(4, 0))
        ctrl.columnconfigure(0, weight=1)

        # Mode row
        mode_row = tk.Frame(ctrl, bg=PANEL)
        mode_row.grid(row=0, column=0, pady=(0, 8))

        self._shuf_btn = self._mode_btn(mode_row, "⇄ SHUFFLE", self._toggle_shuffle, False)
        self._shuf_btn.pack(side="left", padx=10)
        self._rep_btn  = self._mode_btn(mode_row, "↺ REPEAT",  self._toggle_repeat,  False)
        self._rep_btn.pack(side="left", padx=10)

        # Big transport buttons
        transport = tk.Frame(ctrl, bg=PANEL)
        transport.grid(row=1, column=0)

        buttons = [
            ("⏮", self._prev,       ACCENT, 54),
            ("⏪", self._rwd,        DIM,    46),
            ("⏹", self._stop,       PINK,   46),
            ("⏯", self._play_pause, GREEN,  70),
            ("⏩", self._fwd,        DIM,    46),
            ("⏭", self._next,       ACCENT, 54),
        ]
        for txt, cmd, fg, size in buttons:
            b = tk.Button(
                transport, text=txt, command=cmd,
                font=("sans-serif", size // 5, "bold"),
                fg=fg, bg=PANEL, activeforeground=fg,
                activebackground=CARD, bd=0, relief="flat",
                highlightthickness=0, cursor="hand2",
                padx=10, pady=10,
                width=2
            )
            b.pack(side="left", padx=4)

    # ── Status bar ────────────────────────────────────────────────────────────
    def _build_statusbar(self, row):
        bar = tk.Frame(self.root, bg=BG, pady=5)
        bar.grid(row=row, column=0, sticky="ew", padx=12, pady=(2, 8))
        bar.columnconfigure(0, weight=1)

        self._status_var = tk.StringVar(value="Ready — tap SCAN or BROWSE to load music")
        tk.Label(bar, textvariable=self._status_var, font=F_SMALL,
                 fg=DIM, bg=BG, anchor="w", wraplength=380,
                 justify="left").grid(row=0, column=0, sticky="ew")

        ind = "pygame ●" if PYGAME_OK else "pygame ✕"
        col = GREEN if PYGAME_OK else PINK
        tk.Label(bar, text=ind, font=F_SMALL,
                 fg=col, bg=BG).grid(row=0, column=1, sticky="e")

    # ── Widget helpers ────────────────────────────────────────────────────────
    def _touch_btn(self, parent, text, cmd, fg=ACCENT, pad=8) -> tk.Button:
        b = tk.Button(parent, text=text, command=cmd,
                      font=F_SMALL, fg=fg, bg=BORDER,
                      activeforeground=fg, activebackground=CARD,
                      bd=0, relief="flat", padx=pad, pady=5,
                      cursor="hand2")
        return b

    def _mode_btn(self, parent, text, cmd, active: bool) -> tk.Button:
        b = tk.Button(parent, text=text, command=cmd,
                      font=F_SMALL,
                      fg=ACCENT if active else DIM,
                      bg=CARD   if active else BORDER,
                      activeforeground=ACCENT, activebackground=CARD,
                      bd=0, relief="flat", padx=12, pady=6,
                      cursor="hand2")
        return b

    # ── VU Meter ──────────────────────────────────────────────────────────────
    def _draw_vu_idle(self):
        c = self._vu
        c.delete("all")
        w = c.winfo_width() or 380
        h = 70
        n = 28
        bw = w / n
        for i in range(n):
            x  = i * bw
            ht = 6 + (i % 4) * 3
            c.create_rectangle(x+1, h-ht, x+bw-2, h-2, fill=BORDER, outline="")

    def _draw_vu_active(self):
        c = self._vu
        c.delete("all")
        w = c.winfo_width() or 380
        h = 70
        n = 28
        bw = w / n
        for i in range(n):
            x  = i * bw
            ht = random.randint(8, h-4)
            r  = ht / h
            col = PINK if r > 0.75 else (ACCENT if r > 0.4 else DIM)
            c.create_rectangle(x+1, h-ht, x+bw-2, h-2, fill=col, outline="")

    # ── Scan & Browse ─────────────────────────────────────────────────────────
    def _scan_music(self):
        self._toast("Scanning device for music…")
        self.root.update()
        found = []
        for d in ANDROID_MUSIC_DIRS:
            if os.path.isdir(d):
                found.extend(scan_dir(d))
        # Deduplicate by name
        seen = set()
        unique = []
        for p in found:
            if p not in seen:
                seen.add(p)
                unique.append(p)
        if unique:
            for p in unique:
                self._add(p)
            self._toast(f"Found {len(unique)} tracks")
        else:
            self._toast("No audio files found — try BROWSE")

    def _browse(self):
        start = "/sdcard" if os.path.isdir("/sdcard") else os.path.expanduser("~")
        FolderBrowser(self.root, start_dir=start, callback=self._load_folder)

    def _load_folder(self, folder: str):
        files = scan_dir(folder)
        if files:
            for f in files:
                self._add(f)
            self._toast(f"Added {len(files)} tracks from {Path(folder).name}")
        else:
            self._toast("No audio files in that folder")

    # ── Playlist management ───────────────────────────────────────────────────
    def _add(self, path: str):
        if path in self.playlist:
            return
        self.playlist.append(path)
        name = track_name(path)
        idx  = len(self.playlist)
        self.lb.insert("end", f"  {idx:>3}  {name}")
        self._count_var.set(f"{len(self.playlist)} track{'s' if len(self.playlist)!=1 else ''}")

    def _remove(self):
        sel = self.lb.curselection()
        if not sel:
            return
        idx = sel[0]
        if idx == self.current:
            self._stop()
        self.lb.delete(idx)
        self.playlist.pop(idx)
        if self.current > idx:
            self.current -= 1
        elif self.current == idx:
            self.current = -1
        self._count_var.set(f"{len(self.playlist)} track{'s' if len(self.playlist)!=1 else ''}")

    def _clear(self):
        if not self.playlist:
            return
        if messagebox.askyesno("Clear", "Remove all tracks from playlist?"):
            self._stop()
            self.playlist.clear()
            self.lb.delete(0, "end")
            self.current = -1
            self._count_var.set("0 tracks")
            self._reset_info()

    # ── Playback ──────────────────────────────────────────────────────────────
    def _load_idx(self, idx: int) -> bool:
        if not 0 <= idx < len(self.playlist):
            return False
        path = self.playlist[idx]
        ok   = self.engine.load(path, duration=0.0)
        if not ok:
            self._toast(f"Cannot play: {Path(path).name}")
            return False
        self.current = idx
        self.lb.selection_clear(0, "end")
        self.lb.selection_set(idx)
        self.lb.see(idx)
        self._title_var.set(track_name(path))
        self._artist_var.set(str(Path(path).parent.name))
        self._seek_var.set(0)
        self._pos_var.set("0:00")
        self._dur_var.set("--:--")
        self._status(f"Playing: {track_name(path)}")
        return True

    def _play_pause(self):
        if not self.playlist:
            self._toast("Add tracks first — tap SCAN or BROWSE")
            return
        if self.engine.is_playing:
            self.engine.pause()
            self._status("Paused")
        elif self.engine.is_paused:
            self.engine.resume()
            self._status(f"Playing: {track_name(self.playlist[self.current])}")
        else:
            if self.current < 0:
                self.current = 0
            if self._load_idx(self.current):
                self.engine.play()

    def _stop(self):
        self.engine.stop()
        self._seek_var.set(0)
        self._pos_var.set("0:00")
        self._status("Stopped")
        self._draw_vu_idle()

    def _prev(self):
        if not self.playlist:
            return
        idx = (self.current - 1) % len(self.playlist)
        if self._load_idx(idx):
            self.engine.play()

    def _next(self):
        self._advance(auto=False)

    def _advance(self, auto=True):
        if not self.playlist:
            return
        if auto and self.repeat == "one":
            if self._load_idx(self.current):
                self.engine.play()
            return
        if self.shuffle:
            idx = random.randint(0, len(self.playlist) - 1)
        else:
            idx = self.current + 1
            if idx >= len(self.playlist):
                if self.repeat == "all":
                    idx = 0
                else:
                    self._stop()
                    return
        if self._load_idx(idx):
            self.engine.play()

    def _rwd(self):
        self.engine.seek(max(0, self.engine.position - 10))

    def _fwd(self):
        self.engine.seek(self.engine.position + 10)

    def _on_lb_tap(self, _e):
        pass  # select only

    def _on_lb_dbl(self, _e):
        sel = self.lb.curselection()
        if sel:
            if self._load_idx(sel[0]):
                self.engine.play()

    def _on_seek_release(self, _e):
        self._dragging = False
        self.engine.seek(self._seek_var.get())

    def _on_vol(self, v):
        fv = float(v)
        self.engine.set_volume(fv)
        self._vol_lbl.config(text=f"{int(fv*100)}%")

    # ── Modes ─────────────────────────────────────────────────────────────────
    def _toggle_shuffle(self):
        self.shuffle = not self.shuffle
        if self.shuffle:
            self._shuf_btn.config(fg=ACCENT, bg=CARD)
        else:
            self._shuf_btn.config(fg=DIM, bg=BORDER)

    def _toggle_repeat(self):
        i = self.REPEAT_CYCLE.index(self.repeat)
        self.repeat = self.REPEAT_CYCLE[(i + 1) % len(self.REPEAT_CYCLE)]
        labels = {
            "none": ("↺ REPEAT", DIM,   BORDER),
            "all":  ("↺ ALL",    ACCENT, CARD),
            "one":  ("↺ ONE",    PINK,   CARD),
        }
        t, fg, bg = labels[self.repeat]
        self._rep_btn.config(text=t, fg=fg, bg=bg)

    # ── Polling ───────────────────────────────────────────────────────────────
    def _poll(self):
        if self.engine.is_playing:
            pos = self.engine.position
            if not self._dragging:
                self._seek_var.set(pos)
            self._pos_var.set(fmt(pos))
            self._draw_vu_active()
        elif self.engine.finished:
            self._advance(auto=True)

        self.root.after(120, self._poll)

    # ── Helpers ───────────────────────────────────────────────────────────────
    def _status(self, msg: str):
        self._status_var.set(msg)

    def _toast(self, msg: str):
        self._status_var.set(msg)
        # Simple on-screen toast via a temporary label
        t = tk.Toplevel(self.root)
        t.overrideredirect(True)
        t.configure(bg=CARD)
        t.attributes("-topmost", True)
        tk.Label(t, text=msg, font=F_SMALL, fg=TEXT, bg=CARD,
                 padx=16, pady=10, wraplength=340).pack()
        # Position near bottom
        self.root.update_idletasks()
        rw = self.root.winfo_rootx()
        ry = self.root.winfo_rooty()
        rh = self.root.winfo_height()
        t.geometry(f"+{rw+20}+{ry+rh-100}")
        t.after(2400, t.destroy)

    def _reset_info(self):
        self._title_var.set("No track loaded")
        self._artist_var.set("Tap SCAN to find music")
        self._pos_var.set("0:00")
        self._dur_var.set("0:00")
        self._seek_var.set(0)
        self._draw_vu_idle()


# ══════════════════════════════════════════════════════════════════════════════
#  Entry point
# ══════════════════════════════════════════════════════════════════════════════
def main():
    root = tk.Tk()
    root.tk.call("tk", "scaling", 1.4)   # Larger widgets on high-DPI phone screen
    WavePlayer(root)
    root.mainloop()

if __name__ == "__main__":
    main()
