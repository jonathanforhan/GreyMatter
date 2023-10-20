const { app, BrowserWindow } = require('electron');
const path = require('path');

require('electron-reload')(__dirname, { 
    electron: path.join(__dirname, '..', 'node_modules', '.bin', 'electron'), 
    hardResetMethod: 'exit'
}); 

function createWindow() {
  const mainWindow = new BrowserWindow({
    height: 800,
    width: 1200,
    webPreferences: {
      nodeIntegration: true,
      contextIsolation: false,
    },
  });

  mainWindow.loadFile(path.join(__dirname, "../index.html"));
}

app.whenReady().then(() => {
  createWindow();

  app.on("activate", function () {
    if (BrowserWindow.getAllWindows().length === 0) createWindow();
  });
});

app.on("window-all-closed", () => {
  if (process.platform !== "darwin") {
    app.quit();
  }
});
