#ifndef WEB_UI_H
#define WEB_UI_H

#include "ui_css.h"
#include "ui_js.h"
#include <Arduino.h>
#include <ESP8266WebServer.h>

// ==========================================
// FUNGSI CHUNKED STREAMING HTML
// ==========================================
inline void sendHtmlChunked(ESP8266WebServer &server) {
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html; charset=utf-8", "");

  server.sendContent_P(R"=====(<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name='viewport' content='width=device-width, initial-scale=1, maximum-scale=1, user-scalable=0'/>
  <title>AMOS RC Terminal</title>
  <style>
)=====");

  server.sendContent_P(WEB_CSS);

  server.sendContent_P(R"=====(  </style>
</head>
<body>

  <div class="terminal-card">
    <div class="top-bar">
      <button class="settings-btn" onclick="openSettings()">
        <svg viewBox="0 0 24 24"><path d="M19.14,12.94c0.04-0.3,0.06-0.61,0.06-0.94c0-0.32-0.02-0.64-0.06-0.94l2.03-1.58c0.18-0.14,0.23-0.41,0.12-0.61 l-1.92-3.32c-0.12-0.22-0.37-0.29-0.59-0.22l-2.39,0.96c-0.5-0.38-1.03-0.7-1.62-0.94L14.4,2.81c-0.04-0.24-0.24-0.41-0.48-0.41 h-3.84c-0.24,0-0.43,0.17-0.47,0.41L9.25,5.35C8.66,5.59,8.12,5.92,7.63,6.29L5.24,5.33c-0.22-0.08-0.47,0-0.59,0.22L2.73,8.87 C2.62,9.08,2.66,9.34,2.86,9.48l2.03,1.58C4.84,11.36,4.8,11.69,4.8,12s0.02,0.64,0.06,0.94l-2.03,1.58 c-0.18,0.14-0.23,0.41-0.12,0.61l1.92,3.32c0.12,0.22,0.37,0.29,0.59,0.22l2.39-0.96c0.5,0.38,1.03,0.7,1.62,0.94l0.36,2.54 c0.05,0.24,0.24,0.41,0.48,0.41h3.84c0.24,0,0.43-0.17,0.47-0.41l0.36-2.54c0.59-0.24,1.13-0.56,1.62-0.94l2.39,0.96 c0.22,0.08,0.47,0,0.59-0.22l1.92-3.32c0.12-0.22,0.07-0.49-0.12-0.61L19.14,12.94z M12,15.6c-1.98,0-3.6-1.62-3.6-3.6 s1.62-3.6,3.6-3.6s3.6,1.62,3.6,3.6S13.98,15.6,12,15.6z"/></svg>
      </button>
      <div class="battery-container">
        <svg class="battery-icon" viewBox="0 0 24 24"><path d="M17 5H3a2 2 0 0 0-2 2v10a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2V7a2 2 0 0 0-2-2zm4 4v6h1v-6h-1z"/></svg>
        <div class="battery-bar-bg"><div id="battery-level" class="battery-bar-level"></div></div>
        <span id="battery-text" class="battery-text">--%</span>
      </div>
    </div>

    <h1>AMOS</h1>
    <div class="subtitle">Control Hub</div>
    
    <div class="power-container">
      <button id="power-btn" class="btn-power" onclick="togglePower()">POWER: OFF</button>
    </div>

    <div id="controls" class="dpad-container disabled">
      <div class="dpad-btn btn-up" onmousedown="startCmd('F')" onmouseup="stopCmd()" ontouchstart="startCmd('F')" ontouchend="stopCmd()" ontouchcancel="stopCmd()"><svg viewBox="0 0 24 24"><path d="M7 14l5-5 5 5z"/></svg></div>
      <div class="dpad-btn btn-left" onmousedown="startCmd('L')" onmouseup="stopCmd()" ontouchstart="startCmd('L')" ontouchend="stopCmd()" ontouchcancel="stopCmd()"><svg viewBox="0 0 24 24"><path d="M14 7l-5 5 5 5z"/></svg></div>
      <div class="dpad-btn btn-stop" onmousedown="startCmd('S')" ontouchstart="startCmd('S')" ontouchcancel="stopCmd()"><svg viewBox="0 0 24 24"><path d="M6 6h12v12H6z"/></svg></div>
      <div class="dpad-btn btn-right" onmousedown="startCmd('R')" onmouseup="stopCmd()" ontouchstart="startCmd('R')" ontouchend="stopCmd()" ontouchcancel="stopCmd()"><svg viewBox="0 0 24 24"><path d="M10 17l5-5-5-5z"/></svg></div>
      <div class="dpad-btn btn-down" onmousedown="startCmd('B')" onmouseup="stopCmd()" ontouchstart="startCmd('B')" ontouchend="stopCmd()" ontouchcancel="stopCmd()"><svg viewBox="0 0 24 24"><path d="M7 10l5 5 5-5z"/></svg></div>
    </div>

    <div class="monitor-box">
      <div class="monitor-label">Sistem Monitor</div>
      <div class="monitor-row"><span>Deteksi Sampah:</span><span id="monitor-dist" class="val-highlight">-- cm</span></div>
      <div class="monitor-row"><span>Jenis Sampah:</span><span id="monitor-prox" class="val-highlight">Nonaktif</span></div>
      <div class="monitor-row"><span>Status Robot:</span><span id="status" class="val-highlight">Robot Mati (Standby)</span></div>
    </div>
  </div>

  <!-- Modal Settings -->
  <div class="modal-overlay" id="settingsModal">
    <div class="modal-content">
      <div class="modal-title">Konfigurasi Robot</div>
      
      <div class="form-group"><label>Jarak Pemantauan (cm)</label><input type="number" id="cfg_dist" min="2" max="250"></div>
      <div class="form-group"><label>Timer Evaluasi (ms)</label><input type="number" id="cfg_time" min="1000" max="30000"></div>
      <div class="form-group"><label>Servo Standby (&deg;)</label><input type="number" id="cfg_s_stb" min="0" max="180"></div>
      <div class="form-group"><label>Servo Logam (&deg;)</label><input type="number" id="cfg_s_log" min="0" max="180"></div>
      <div class="form-group"><label>Servo Plastik (&deg;)</label><input type="number" id="cfg_s_non" min="0" max="180"></div>
      <div class="form-group"><label>Delay Servo (ms)</label><input type="number" id="cfg_s_del" min="500" max="10000"></div>
      <div class="form-group"><label>Speed Kiri (0-255)</label><input type="number" id="cfg_spd_l" min="0" max="255"></div>
      <div class="form-group"><label>Speed Kanan (0-255)</label><input type="number" id="cfg_spd_r" min="0" max="255"></div>

      <button class="btn-save" onclick="saveSettings()">SIMPAN & RESTART</button>
      <button class="btn-close" onclick="closeSettings()">Batal</button>
    </div>
  </div>

  <!-- Custom Dialog Overlay -->
  <div id="customDialog" class="dialog-overlay">
    <div class="dialog-card">
      <div id="dialogIcon" class="dialog-icon">⚠️</div>
      <div id="dialogTitle" class="dialog-title">Peringatan</div>
      <div id="dialogMessage" class="dialog-message">Pesan peringatan.</div>
      <div id="dialogButtons" class="dialog-buttons">
        <button id="dialogBtnOk" class="btn-dialog-ok" onclick="closeDialog()">OK</button>
        <button id="dialogBtnYes" class="btn-dialog-yes" onclick="handleDialogYes()">Ya, Simpan</button>
        <button id="dialogBtnNo" class="btn-dialog-no" onclick="closeDialog()">Batal</button>
      </div>
    </div>
  </div>

  <script>
)=====");

  server.sendContent_P(WEB_JS);

  server.sendContent_P(R"=====(
  </script>
</body>
</html>)=====");

  server.sendContent("");
}

#endif
