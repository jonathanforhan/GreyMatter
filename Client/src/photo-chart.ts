const { Chart } = require("chart.js/auto");

export function newPhotoChart() {
  const chart = new Chart(
    document.getElementById("photo-chart") as HTMLCanvasElement,
    {
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