const { Chart } = require("chart.js/auto");

export function newHumidityChart() {
  const chart = new Chart(
    document.getElementById("humidity-chart") as HTMLCanvasElement,
    {
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
        animation: false,
        scales: {
          y: {
            beginAtZero: true,
            max: 100,
          },
        },
      },
    }
  );
  chart.update;
  return chart;
}