const { Chart } = require("chart.js/auto");

export function newTemperatureChart() {
  const chart = new Chart(
    document.getElementById("temperature-chart") as HTMLCanvasElement,
    {
      type: "line",
      data: {
        labels: Array.from(Array(100).keys()),
        datasets: [
          {
            label: "temperature (F)",
            data: [],
            borderWidth: 1,
          },
        ],
      },
      options: {
        animation: false,
        scales: {
          y: {
            // beginAtZero: true,
            min: 32,
            max: 100,
          },
        },
      },
    }
  );
  chart.update();
  return chart;
}
