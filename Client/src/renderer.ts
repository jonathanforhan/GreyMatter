/**
 * render.ts is the render process code. renderer.ts has access to Node API
 */
var { ipcRenderer } = require('electron');

const { newHumidityChart } = require("./dist/humidity-chart");
const { newTemperatureChart } = require("./dist/temperature-chart");
const { newPhotoChart } = require("./dist/photo-chart");
const { newUvChart } = require("./dist/uv-chart");
var { SerialPort, ReadlineParser } = require('serialport');

const humidityChart = newHumidityChart();
const temperatureChart = newTemperatureChart();
const photoChart = newPhotoChart();
const uvChart = newUvChart();

const getTemplate = async () => {
  const ports =[];
  (await SerialPort.list()).forEach(port => ports.push(port.path));

  const template = [];
  ports.forEach(x => template.push({ label: x }));
  return [
    {
      label: 'Ports',
      submenu: template,
    }
  ]
};

document.addEventListener("DOMContentLoaded", () => {
  getTemplate().then((template) => {
    ipcRenderer.send("populate-ports", template);
  });
})

ipcRenderer.on('port-selected', (e, port) => {
  const com = new SerialPort({
    path: port,
    baudRate: 9600,
  });
  const parser = com.pipe(new ReadlineParser({ delimiter: "\n" }));

  parser.on("data", (data: string) => {
    try {
      const json = JSON.parse(data);
      const reading = parseFloat(json.reading);
      document.getElementById(
        `${json.sensor}-read`
      )!.innerHTML = `${json.sensor} reading: ${json.reading}`;
      if (json.sensor === "humidity") {
        humidityChart.data.datasets[0].data.push(reading);
        if (humidityChart.data.datasets[0].data.length >= 100) {
          humidityChart.data.datasets[0].data.shift();
        }
        humidityChart.update();
      } else if (json.sensor === "temperature") {
        temperatureChart.data.datasets[0].data.push(reading);
        if (temperatureChart.data.datasets[0].data.length >= 100) {
          temperatureChart.data.datasets[0].data.shift();
        }
        temperatureChart.update();
      } else if (json.sensor === "photo") {
        photoChart.data.datasets[0].data.push(reading);
        if (photoChart.data.datasets[0].data.length >= 200) {
          photoChart.data.datasets[0].data.shift();
        }
        photoChart.update();
      } else if (json.sensor === "uv") {
        uvChart.data.datasets[0].data.push(reading);
        if (uvChart.data.datasets[0].data.length >= 200) {
          uvChart.data.datasets[0].data.shift();
        }
        uvChart.update();
      }
    } catch (e) {
      console.log(e);
    }
  });
})
