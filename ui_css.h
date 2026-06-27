#ifndef UI_CSS_H
#define UI_CSS_H

#include <Arduino.h>

const char WEB_CSS[] PROGMEM = R"=====(
    @import url('https://fonts.googleapis.com/css2?family=Outfit:wght@300;400;600;800&display=swap');
    
    * {
      box-sizing: border-box;
      -webkit-tap-highlight-color: transparent;
      -webkit-touch-callout: none;
      -webkit-user-select: none;
      user-select: none;
      outline: none;
    }

    input {
      -webkit-user-select: text;
      user-select: text;
    }

    /* ==========================================
       DESAIN ROOT & ELEMENT UTAMA
       ========================================== */
    :root {
      --bg-color: #0b0f19;       
      --card-bg: rgba(17, 24, 39, 0.7); 
      --cyan-primary: #00adb5;   
      --cyan-active: #00f0fc;
      --red-stop: #ff4a5a;       
      --red-stop-active: #ff7c88;
      --power-on: #10b981;       
      --power-off: #4b5563;      
      --text-main: #f9fafb;
      --text-muted: #9ca3af;
    }

    body { 
      text-align: center; 
      font-family: 'Outfit', sans-serif; 
      background-color: var(--bg-color); 
      color: var(--text-main); 
      margin: 0; 
      padding: 0;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      min-height: 100vh;
      box-sizing: border-box;
    }

    .terminal-card {
      background: var(--card-bg);
      backdrop-filter: blur(12px);
      -webkit-backdrop-filter: blur(12px);
      border: 1px solid rgba(255, 255, 255, 0.08);
      border-radius: 28px;
      padding: 25px 20px;
      width: 90%;
      max-width: 360px;
      box-shadow: 0 20px 40px rgba(0, 0, 0, 0.6);
      position: relative;
    }

    /* ==========================================
       INDIKATOR BATERAI & TOMBOL SETTINGS
       ========================================== */
    .top-bar {
      display: flex;
      justify-content: space-between;
      align-items: center;
      margin-bottom: 15px;
    }
    
    .settings-btn {
      background: rgba(255, 255, 255, 0.1);
      border: none;
      border-radius: 50%;
      width: 32px;
      height: 32px;
      display: flex;
      align-items: center;
      justify-content: center;
      cursor: pointer;
      transition: 0.2s;
    }
    
    .settings-btn svg {
      width: 18px;
      height: 18px;
      fill: var(--text-main);
    }
    
    .settings-btn:active { background: rgba(255, 255, 255, 0.2); }
    
    .battery-container {
      display: flex;
      align-items: center;
      gap: 6px;
    }
    
    .battery-icon {
      width: 14px;
      height: 14px;
      fill: var(--text-muted);
    }
    
    .battery-bar-bg {
      width: 30px;
      height: 10px;
      background: rgba(255, 255, 255, 0.1);
      border-radius: 3px;
      overflow: hidden;
      border: 1px solid rgba(255, 255, 255, 0.15);
    }
    
    .battery-bar-level {
      height: 100%;
      background: var(--power-on);
      width: 0%; 
      transition: width 0.3s ease;
    }
    
    .battery-text {
      font-size: 11px;
      font-weight: 600;
      color: var(--text-muted);
      min-width: 28px;
      text-align: right;
    }

    h1 { 
      font-size: 24px; 
      font-weight: 800; 
      letter-spacing: 1px;
      margin: 0 0 2px 0;
      text-transform: uppercase;
      background: linear-gradient(to right, #00adb5, #3b82f6);
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent;
    }

    .subtitle {
      font-size: 11px;
      color: var(--text-muted);
      text-transform: uppercase;
      letter-spacing: 2px;
      margin-bottom: 20px;
    }

    /* ==========================================
       TOMBOL POWER & PANEL KONTROL D-PAD
       ========================================== */
    .power-container { margin-bottom: 15px; }
    
    .btn-power {
      display: inline-flex;
      align-items: center;
      justify-content: center;
      width: 100%;
      height: 48px;
      background-color: var(--power-off); 
      color: white;
      font-size: 16px; 
      font-weight: 600; 
      border-radius: 14px;
      cursor: pointer;
      user-select: none; 
      box-shadow: 0 8px 16px rgba(0, 0, 0, 0.2);
      transition: all 0.2s ease;
      border: none;
    }
    
    .btn-power:active { transform: translateY(2px); }
    
    .btn-power.on { 
      background-color: var(--power-on); 
      box-shadow: 0 8px 16px rgba(16, 185, 129, 0.2);
    }

    .dpad-container { 
      margin: 25px auto; 
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      grid-template-rows: repeat(3, 1fr);
      gap: 16px;
      width: 240px;
      height: 240px;
      transition: opacity 0.3s ease, filter 0.3s ease;
    }
    
    .disabled { 
      opacity: 0.2; 
      pointer-events: none; 
      filter: grayscale(0.8);
    }
    
    .dpad-btn {
      background: rgba(255, 255, 255, 0.04);
      border: 1px solid rgba(255, 255, 255, 0.08);
      border-radius: 20px;
      display: flex;
      align-items: center;
      justify-content: center;
      cursor: pointer;
      transition: all 0.15s ease;
      user-select: none;
    }
    
    .dpad-btn svg { width: 32px; height: 32px; fill: var(--text-muted); }
    .btn-up { grid-column: 2; grid-row: 1; }
    .btn-left { grid-column: 1; grid-row: 2; }
    .btn-stop { grid-column: 2; grid-row: 2; background: rgba(255, 74, 90, 0.1); }
    .btn-right { grid-column: 3; grid-row: 2; }
    .btn-down { grid-column: 2; grid-row: 3; }
    .dpad-btn:active { transform: scale(0.92); background: var(--cyan-primary); }
    .btn-stop:active { background: var(--red-stop); }

    /* ==========================================
       SISTEM MONITOR STATUS & DATA SENSOR
       ========================================== */
    .monitor-box {
      background: rgba(0, 0, 0, 0.3);
      border-radius: 16px;
      padding: 15px;
      border: 1px solid rgba(255, 255, 255, 0.04);
      margin-top: 10px;
      text-align: left;
    }
    
    .monitor-label { 
      font-size: 10px; 
      font-weight: 700; 
      color: var(--text-muted); 
      text-transform: uppercase; 
      border-bottom: 1px solid rgba(255, 255, 255, 0.05); 
      padding-bottom: 4px; 
      margin-bottom: 8px;
    }
    
    .monitor-row { 
      display: flex; 
      justify-content: space-between; 
      font-size: 13px; 
      margin-bottom: 6px; 
      color: var(--text-muted); 
    }
    
    .val-highlight { font-weight: 600; color: var(--text-main); }
    
    /* ==========================================
       MODAL DIALOG & SETTINGS GAYA KUSTOM
       ========================================== */
    .modal-overlay {
      position: fixed; top: 0; left: 0; right: 0; bottom: 0;
      background: rgba(0,0,0,0.8);
      backdrop-filter: blur(5px);
      -webkit-backdrop-filter: blur(5px);
      display: none; justify-content: center; align-items: center;
      z-index: 100; opacity: 0; transition: 0.3s;
    }
    
    .modal-overlay.show { display: flex; opacity: 1; }
    
    .modal-content {
      background: var(--card-bg);
      border: 1px solid rgba(255,255,255,0.1);
      border-radius: 20px;
      width: 90%; max-width: 340px;
      padding: 20px; box-sizing: border-box;
      max-height: 90vh; overflow-y: auto;
    }
    
    .modal-title { font-size: 18px; font-weight: 700; margin-bottom: 15px; color: var(--cyan-primary); text-align: left;}
    .form-group { display: flex; justify-content: space-between; align-items: center; margin-bottom: 12px; font-size: 13px; text-align: left; }
    .form-group label { color: var(--text-muted); flex: 1; }
    .form-group input { 
      background: rgba(0,0,0,0.5); border: 1px solid rgba(255,255,255,0.2); 
      color: white; border-radius: 8px; padding: 6px; width: 60px; text-align: center; font-family: 'Outfit';
    }
    
    .btn-save {
      background: linear-gradient(135deg, var(--power-on), #059669);
      color: white; border: none; padding: 12px; width: 100%; border-radius: 12px;
      font-weight: 600; font-family: inherit; cursor: pointer; margin-top: 10px; transition: 0.2s;
    }
    
    .btn-save:active { transform: scale(0.95); }
    
    .btn-close {
      background: rgba(255,255,255,0.1); color: white; border: none; padding: 10px; width: 100%;
      border-radius: 12px; font-weight: 600; font-family: inherit; cursor: pointer; margin-top: 8px; transition: 0.2s;
    }
    
    .btn-close:active { transform: scale(0.95); }

    .dialog-overlay {
      position: fixed; top: 0; left: 0; right: 0; bottom: 0;
      background: rgba(0,0,0,0.85);
      backdrop-filter: blur(8px);
      -webkit-backdrop-filter: blur(8px);
      display: none; justify-content: center; align-items: center;
      z-index: 200; opacity: 0; transition: 0.3s;
    }
    
    .dialog-overlay.show { display: flex; opacity: 1; }
    
    .dialog-card {
      background: var(--card-bg);
      border: 1px solid rgba(255, 255, 255, 0.1);
      border-radius: 24px;
      width: 90%; max-width: 320px;
      padding: 24px; box-sizing: border-box;
      text-align: center;
      box-shadow: 0 20px 40px rgba(0, 0, 0, 0.7);
      transform: scale(0.9); transition: 0.3s;
    }
    
    .dialog-overlay.show .dialog-card { transform: scale(1); }
    .dialog-icon { font-size: 32px; margin-bottom: 12px; }
    .dialog-title { font-size: 18px; font-weight: 700; color: var(--cyan-primary); margin-bottom: 10px; }
    .dialog-message { font-size: 13px; color: var(--text-muted); line-height: 1.5; margin-bottom: 20px; }
    .dialog-buttons { display: flex; gap: 10px; justify-content: center; }
    
    .btn-dialog-ok, .btn-dialog-yes, .btn-dialog-no {
      font-family: inherit; font-weight: 600; font-size: 13px;
      padding: 10px 20px; border-radius: 10px; cursor: pointer; border: none; transition: 0.2s;
    }
    
    .btn-dialog-ok {
      background: var(--cyan-primary); color: white; width: 100%;
    }
    
    .btn-dialog-ok:active { transform: scale(0.95); }
    
    .btn-dialog-yes {
      background: var(--power-on); color: white; flex: 1;
    }
    
    .btn-dialog-yes:active { transform: scale(0.95); }
    
    .btn-dialog-no {
      background: rgba(255, 255, 255, 0.1); color: white; flex: 1;
    }
    
    .btn-dialog-no:active { transform: scale(0.95); }
)=====";

#endif
