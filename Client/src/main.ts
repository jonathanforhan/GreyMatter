var { app, BrowserWindow, ipcMain, Menu } = require('electron');
const path = require('path');

require('electron-reload')(__dirname, { 
    electron: path.join(__dirname, '..', 'node_modules', '.bin', 'electron'), 
    hardResetMethod: 'exit'
}); 

function createWindow() {
  const mainWindow = new BrowserWindow({
    webPreferences: {
      nodeIntegration: true,
      contextIsolation: false,
    },
  });
  mainWindow.maximize();
  mainWindow.loadFile(path.join(__dirname, "../index.html"));
  // mainWindow.webContents.openDevTools();
}

app.whenReady().then(() => {
  createWindow();

  app.on("activate", () => {
    if (BrowserWindow.getAllWindows().length === 0) createWindow();
  });
});

app.on("window-all-closed", () => {
  if (process.platform !== "darwin") {
    app.quit();
  }
});

ipcMain.on('populate-ports', (e, template: any[]) => {
  e.preventDefault();
  template.forEach(item => {
    item.submenu.forEach(menu => {
      menu['click'] = () => e.sender.send('port-selected', menu.label)
    })
  })
  const menu = Menu.buildFromTemplate(template);
  Menu.setApplicationMenu(menu);
})
