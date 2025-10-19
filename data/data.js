
// Check if connected and update the vue every 15s
function subscribeIsConnected() {
  const intervalIsConnected = setInterval(async() => {
    let value = "loding...";
    try {
      const res = await fetch("/is-connected");
      
      if (!res.ok) {
          throw new Error(`HTTP error! status: ${res.status}`);
      }

      value = await res.text();
    } catch (err) {
      value = "False";
      clearInterval(intervalIsConnected);
    }

    document.getElementById("is-connected-value").textContent = value;
    }, 15000);
}

// Get temperature values and update the vue
async function drawTemperatureDataViz() {
  let averageValue = '-°';

  try {
    const res = await fetch("/sensor-values");
    
    if (!res.ok) {
      throw new Error(`HTTP error! status: ${res.status}`);
    }

    const values = await res.json();
    
    averageValue = ((values[0] + values[1] + values[2] + values[3] + values[4]) / 5);      
    document.getElementById('sensor-average-value').textContent = averageValue.toFixed(1) + '°C';
    
    document.getElementById('sensor-value-1').textContent = values[0] + "°C";
    document.getElementById('sensor-value-2').textContent = values[1] + "°C";
    document.getElementById('sensor-value-3').textContent = values[2] + "°C";
    document.getElementById('sensor-value-4').textContent = values[3] + "°C";
    document.getElementById('sensor-value-5').textContent = values[4] + "°C";

    const svg = document.getElementById("chart");
    
    const smoothness = 0.7;
    const width = 600;
    const height = 300;
    const pad = { top: 20, right: 20, bottom: 30, left: 40 };

    const minVal = Math.min(...values);
    const maxVal = Math.max(...values);
    const vMin = minVal - (Math.abs(maxVal - minVal) * 0.1 || 1);
    const vMax = maxVal + (Math.abs(maxVal - minVal) * 0.1 || 1);

    const plotW = width - pad.left - pad.right;
    const plotH = height - pad.top - pad.bottom;

    const points = values.map((v, i) => {
      const x = pad.left + (i / (values.length - 1)) * plotW;
      const y = pad.top + (1 - (v - vMin) / (vMax - vMin)) * plotH;
      return { x, y, v };
    });

    let d = `M ${points[0].x},${points[0].y}`;

    for (let i = 0; i < points.length - 1; i++) {
        const p0 = points[i - 1] || points[i];
        const p1 = points[i];
        const p2 = points[i + 1];
        const p3 = points[i + 2] || p2;

        const c1x = p1.x + (p2.x - p0.x) * (smoothness / 6);
        const c1y = p1.y + (p2.y - p0.y) * (smoothness / 6);
        const c2x = p2.x - (p3.x - p1.x) * (smoothness / 6);
        const c2y = p2.y - (p3.y - p1.y) * (smoothness / 6);

        d += ` C ${c1x},${c1y} ${c2x},${c2y} ${p2.x},${p2.y}`;
    }

    const sorted = [...values].sort((a, b) => a - b);
   
    const medianY = pad.top + (1 - (averageValue - vMin) / (vMax - vMin)) * plotH;

    // Axis, curve and points
    const line = `<path d="${d}" class="line"/>`;
    const dots = points.map(p => `<circle class="point" cx="${p.x}" cy="${p.y}" />`).join('');
    const averageLine = `<line x1="${pad.left}" x2="${width - pad.right}" y1="${medianY}" y2="${medianY}" class="axis"></line>
      <text x="${width - pad.right - 60}" y="${medianY - 12}" class="label">
        ${averageValue.toFixed(1)}°C
      </text>`;

    svg.innerHTML = line + dots + averageLine;
  } catch (err) {
    document.getElementById('sensor-average-value').textContent = 'error';
  }
}

drawTemperatureDataViz();
