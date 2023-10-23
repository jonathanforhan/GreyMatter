const { Chart } = require("chart.js/auto");

export function newUvChart() {
  const chart = new Chart(
    document.getElementById("uv-chart") as HTMLCanvasElement,
    {
      type: "line",
      data: {
        labels: Array.from(Array(200).keys()),
        datasets: [
          {
            label: "uv (0-1023)",
            data: [],
          },
        ],
      },
      options: {
        animation: false,
        scales: {
          y: {
            beginAtZero: true,
          },
        },
      },
    }
  );
  chart.update();
  return chart;
}
