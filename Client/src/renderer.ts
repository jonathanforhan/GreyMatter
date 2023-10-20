/**
 * render.ts is the render process code. renderer.ts has access to Node API
 */

const { Chart } = require("chart.js/auto");
const { SerialPort, ReadlineParser } = require('serialport');

document.addEventListener('DOMContentLoaded', () => {
    
    const humidityChart = new Chart(document.getElementById('humidity-chart'), {
      type: "line",
      data: {
        labels: Array.from(Array(100).keys()),
        datasets: [
          {
            label: "humidity (%)",
            data: [],
            borderWidth: 1,
          },
        ],
      },
      options: {
        scales: {
            y: {
                beginAtZero: true,
                max: 100,
            }
        }
      }
    });
    humidityChart.update();

    const temperatureChart = new Chart(document.getElementById('temperature-chart'), {
      type: "line",
      data: {
        labels: Array.from(Array(100).keys()),
        datasets: [
          {
            label: "temperature (C)",
            data: [],
            borderWidth: 1,
          },
        ],
      },
      options: {
        scales: {
            y: {
                beginAtZero: true,
                max: 100,
            }
        }
      }
    });
    temperatureChart.update();

    const photoChart = new Chart(document.getElementById('photo-chart'), {
      type: "line",
      data: {
        labels: Array.from(Array(200).keys()),
        datasets: [
          {
            label: "photo (lux)",
            data: [],
            borderWidth: 1,
          },
        ],
      },
      options: {
        scales: {
            y: {
                beginAtZero: true,
            }
        }
      }
    });
    photoChart.update();

    const uvChart = new Chart(document.getElementById('uv-chart'), {
      type: "line",
      data: {
        labels: Array.from(Array(100).keys()),
        datasets: [
          {
            label: "uv (0-1023)",
            data: [],
          },
        ],
      },
      options: {
        scales: {
            y: {
                beginAtZero: true,
            }
        }
      }
    });
    uvChart.update();

    const com = new SerialPort({
        path: 'COM5',
        baudRate: 9600,
    });
    const parser = com.pipe(new ReadlineParser({ delimiter: '\n' }));

    parser.on('data', (data: string) => {
        try {
            const json = JSON.parse(data);
            const reading = parseFloat(json.reading);
            document.getElementById(`${json.sensor}-read`)!.innerHTML = `${json.sensor} reading: ${json.reading}`;
            if (json.sensor === "humidity") {
                humidityChart.data.datasets[0].data.push(reading);
                if (humidityChart.data.datasets[0].data.length > 100) {
                  humidityChart.data.datasets[0].data = humidityChart.data.datasets[0].data.slice(1);
                }
                humidityChart.update();
            }
            else if (json.sensor === "temperature") {
                temperatureChart.data.datasets[0].data.push(reading);
                // console.log(+json.reading);
                if (temperatureChart.data.datasets[0].data.length > 100) {
                  temperatureChart.data.datasets[0].data = temperatureChart.data.datasets[0].data.slice(1);
                }
                temperatureChart.update();
            }
            else if (json.sensor === "photo") {
                photoChart.data.datasets[0].data.push(reading);
                if (photoChart.data.datasets[0].data.length > 200) {
                  photoChart.data.datasets[0].data = photoChart.data.datasets[0].data.slice(1);
                }
                photoChart.update();
            }
            else if (json.sensor === "uv") {
                uvChart.data.datasets[0].data.push(reading);
                if (uvChart.data.datasets[0].data.length > 200) {
                  uvChart.data.datasets[0].data = uvChart.data.datasets[0].data.slice(1);
                }
                uvChart.update();
            }
        } catch (e) {
            console.log(e);
        }
    });
})

