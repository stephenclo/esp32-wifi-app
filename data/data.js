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

async function drawTemperatureDataViz() {
  let value = '-°';

  try {
    const res = await fetch("/sensor-values");
    
    if (!res.ok) {
      throw new Error(`HTTP error! status: ${res.status}`);
    }

    const values = await res.json();
    value = ((values[0] + values[1] + values[2] + values[3] + values[4]) / 5).toFixed(1) + '°';      
    document.getElementById('sensor-average-value').textContent = value;
    
    document.getElementById('sensor-value-1').textContent = values[0] + "°C";
    document.getElementById('sensor-value-2').textContent = values[1] + "°C";
    document.getElementById('sensor-value-3').textContent = values[2] + "°C";
    document.getElementById('sensor-value-4').textContent = values[3] + "°C";
    document.getElementById('sensor-value-5').textContent = values[4] + "°C";

    const svg = document.getElementById("chart");
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
      const p0 = points[i];
      const p1 = points[i + 1];
      const cx = (p0.x + p1.x) / 2;
      const cy = (p0.y + p1.y) / 2;
      d += ` Q ${p0.x},${p0.y} ${cx},${cy}`;
    }
    d += ` T ${points[points.length - 1].x},${points[points.length - 1].y}`;

    // Curve and points
    const line = `<path d="${d}" class="line"/>`;
    const dots = points.map(p => `<circle class="point" cx="${p.x}" cy="${p.y}" />`).join('');

    svg.innerHTML = line + dots;
  
  } catch (err) {
    value = 'error';
  }
}

drawTemperatureDataViz();
