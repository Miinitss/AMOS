#ifndef UI_JS_H
#define UI_JS_H

#include <Arduino.h>

const char WEB_JS[] PROGMEM = R"=====(
    // ==========================================
    // VARIABEL GLOBAL & NAVIGASI D-PAD
    // ==========================================
    let systemOn = false; 
    let cmdInterval = null;
    let currentAction = 'S';

    function sendCmd() { fetch('/cmd?a=' + currentAction); }

    function togglePower() {
      systemOn = !systemOn;
      fetch('/cmd?a=' + (systemOn ? 'ON' : 'OFF'));
      
      let btn = document.getElementById('power-btn');
      let controls = document.getElementById('controls');
      let statusText = document.getElementById('status');
      
      if (systemOn) {
        btn.innerText = 'POWER: ON'; btn.classList.add('on'); controls.classList.remove('disabled');
        statusText.innerText = 'SIAGA (Mencari Orang)'; statusText.style.color = 'var(--cyan-primary)';
      } else {
        if (cmdInterval) { clearInterval(cmdInterval); cmdInterval = null; }
        btn.innerText = 'POWER: OFF'; btn.classList.remove('on'); controls.classList.add('disabled');
        statusText.innerText = 'Robot Mati (Standby)'; statusText.style.color = 'var(--text-muted)';
      }
    }

    function startCmd(action) {
      if (!systemOn) return;
      currentAction = action;
      sendCmd();
      if (cmdInterval) clearInterval(cmdInterval);
      cmdInterval = setInterval(sendCmd, 300);
      let st = { 'F':'Maju', 'B':'Mundur', 'L':'Belok Kiri', 'R':'Belok Kanan', 'S':'Berhenti' };
      document.getElementById('status').innerText = st[action];
    }

    function stopCmd() {
      if (!systemOn) return;
      if (cmdInterval) { clearInterval(cmdInterval); cmdInterval = null; }
      currentAction = 'S'; sendCmd();
      document.getElementById('status').innerText = 'Berhenti (Inersia 1.5s)';
    }

    // ==========================================
    // POLLING STATUS & DATA SENSOR
    // ==========================================
    function updateStatus() {
      fetch('/status')
        .then(response => response.json())
        .then(data => {
          systemOn = (data.power === "ON");
          let btn = document.getElementById('power-btn');
          let controls = document.getElementById('controls');
          let statusText = document.getElementById('status');
          
          if (systemOn) {
            btn.innerText = 'POWER: ON'; btn.classList.add('on'); controls.classList.remove('disabled');
            statusText.style.color = 'var(--cyan-primary)';
          } else {
            btn.innerText = 'POWER: OFF'; btn.classList.remove('on'); controls.classList.add('disabled');
            statusText.style.color = 'var(--text-muted)';
          }

          let batContainer = document.querySelector('.battery-container');
          if (data.battery < 0) {
            if (batContainer) batContainer.style.display = 'none';
          } else {
            if (batContainer) batContainer.style.display = 'flex';
            document.getElementById('battery-level').style.width = data.battery + '%';
            document.getElementById('battery-text').innerText = data.battery + '%';
            document.getElementById('battery-level').style.backgroundColor = (data.battery < 20) ? 'var(--red-stop)' : 'var(--power-on)';
          }

          let distEl = document.getElementById('monitor-dist');
          let proxEl = document.getElementById('monitor-prox');

          if (systemOn) {
            if (data.proximity.includes("Bergerak")) {
              distEl.innerText = "Nonaktif (Bergerak)";
              distEl.style.color = 'var(--text-muted)';
            } else if (data.distance > 0 && data.distance <= data.max_dist) {
              distEl.innerText = "Ada Sampah (" + data.distance + "cm)";
              distEl.style.color = 'var(--cyan-primary)';
            } else {
              distEl.innerText = "Kosong";
              distEl.style.color = 'var(--text-muted)';
            }
            
            proxEl.innerText = data.proximity;
            if (data.proximity === "LOGAM") proxEl.style.color = 'var(--red-stop)';
            else if (data.proximity === "MENGEVALUASI SAMPAH..." || data.proximity.includes("TERTUTUP")) proxEl.style.color = 'var(--cyan-primary)';
            else proxEl.style.color = 'var(--text-muted)';
          } else {
            distEl.innerText = "Nonaktif";
            proxEl.innerText = "Nonaktif";
          }
          const moveTexts = { 'F': 'Maju', 'B': 'Mundur', 'L': 'Belok Kiri', 'R': 'Belok Kanan' };
          if (systemOn) {
            statusText.innerText = moveTexts[currentAction] || data.state;
          } else {
            statusText.innerText = "Robot Mati (Standby)";
          }
        })
        .catch(err => console.log("Status err"));
    }
    setInterval(updateStatus, 500);

    // ==========================================
    // LOGIKA PENGATURAN (SETTINGS)
    // ==========================================
    function openSettings() {
      document.getElementById('settingsModal').classList.add('show');
      fetch('/get_config').then(r=>r.json()).then(data => {
        document.getElementById('cfg_dist').value = data.dist;
        document.getElementById('cfg_time').value = data.time;
        document.getElementById('cfg_s_stb').value = data.s_stb;
        document.getElementById('cfg_s_log').value = data.s_log;
        document.getElementById('cfg_s_non').value = data.s_non;
        document.getElementById('cfg_s_del').value = data.s_del;
        document.getElementById('cfg_spd_l').value = data.spd_l;
        document.getElementById('cfg_spd_r').value = data.spd_r;
      });
    }
    
    function closeSettings() {
      document.getElementById('settingsModal').classList.remove('show');
    }

    // ==========================================
    // DIALOG PERINGATAN & KONFIRMASI KUSTOM
    // ==========================================
    let dialogCallback = null;

    function showCustomAlert(message, title = "Peringatan", icon = "⚠️") {
      document.getElementById('dialogIcon').innerText = icon;
      document.getElementById('dialogTitle').innerText = title;
      document.getElementById('dialogMessage').innerText = message;
      
      document.getElementById('dialogBtnOk').style.display = 'block';
      document.getElementById('dialogBtnYes').style.display = 'none';
      document.getElementById('dialogBtnNo').style.display = 'none';
      
      const dlg = document.getElementById('customDialog');
      dlg.style.display = 'flex';
      setTimeout(() => dlg.classList.add('show'), 10);
    }

    function showCustomConfirm(message, callback, title = "Konfirmasi", icon = "❓") {
      document.getElementById('dialogIcon').innerText = icon;
      document.getElementById('dialogTitle').innerText = title;
      document.getElementById('dialogMessage').innerText = message;
      
      document.getElementById('dialogBtnOk').style.display = 'none';
      document.getElementById('dialogBtnYes').style.display = 'block';
      document.getElementById('dialogBtnNo').style.display = 'block';
      
      dialogCallback = callback;
      
      const dlg = document.getElementById('customDialog');
      dlg.style.display = 'flex';
      setTimeout(() => dlg.classList.add('show'), 10);
    }

    function closeDialog() {
      const dlg = document.getElementById('customDialog');
      dlg.classList.remove('show');
      setTimeout(() => { dlg.style.display = 'none'; }, 300);
    }

    function handleDialogYes() {
      closeDialog();
      if (dialogCallback) {
        dialogCallback();
        dialogCallback = null;
      }
    }

    function saveSettings() {
      const dist = parseInt(document.getElementById('cfg_dist').value);
      const time = parseInt(document.getElementById('cfg_time').value);
      const stb = parseInt(document.getElementById('cfg_s_stb').value);
      const log = parseInt(document.getElementById('cfg_s_log').value);
      const non = parseInt(document.getElementById('cfg_s_non').value);
      const del = parseInt(document.getElementById('cfg_s_del').value);
      const spdL = parseInt(document.getElementById('cfg_spd_l').value);
      const spdR = parseInt(document.getElementById('cfg_spd_r').value);

      if (isNaN(dist) || dist < 2 || dist > 250) { showCustomAlert("Jarak Pemantauan harus antara 2 - 250 cm!"); return; }
      if (isNaN(time) || time < 1000 || time > 30000) { showCustomAlert("Timer Evaluasi harus antara 1000 - 30000 ms!"); return; }
      if (isNaN(stb) || stb < 0 || stb > 180) { showCustomAlert("Servo Standby harus antara 0 - 180°!"); return; }
      if (isNaN(log) || log < 0 || log > 180) { showCustomAlert("Servo Logam harus antara 0 - 180°!"); return; }
      if (isNaN(non) || non < 0 || non > 180) { showCustomAlert("Servo Plastik harus antara 0 - 180°!"); return; }
      if (isNaN(del) || del < 500 || del > 10000) { showCustomAlert("Delay Servo harus antara 500 - 10000 ms!"); return; }
      if (isNaN(spdL) || spdL < 0 || spdL > 255) { showCustomAlert("Speed Kiri harus antara 0 - 255!"); return; }
      if (isNaN(spdR) || spdR < 0 || spdR > 255) { showCustomAlert("Speed Kanan harus antara 0 - 255!"); return; }

      showCustomConfirm("Apakah Anda yakin ingin menyimpan perubahan ini? Robot akan direstart otomatis.", () => {
        let q = "?dist=" + dist +
                "&time=" + time +
                "&s_stb=" + stb +
                "&s_log=" + log +
                "&s_non=" + non +
                "&s_del=" + del +
                "&spd_l=" + spdL +
                "&spd_r=" + spdR;
        fetch('/save_config' + q).then(() => {
          showCustomAlert("Konfigurasi disimpan! ESP8266 sedang direstart otomatis...", "Sukses", "💾");
          closeSettings();
          setTimeout(() => location.reload(), 3000);
        });
      }, "Konfirmasi Simpan", "❓");
    }
)=====";

#endif
