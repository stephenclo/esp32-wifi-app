// Save light-theme param
function toggleLightTheme() {
  const isLight = localStorage.getItem('light-theme');
  if (isLight && JSON.parse(isLight)) {  
    localStorage.setItem('light-theme', JSON.stringify(false));  
  } else {
    localStorage.setItem('light-theme', JSON.stringify(true));
  }

  location.reload();
}

function resetWifi() {
  if (confirm("Es-tu sûr de vouloir réinitialiser les paramètres WiFi ?")) {
    fetch("/reset-wifi");
  }
}

function setSwicthThemeText() {
  const value = localStorage.getItem('light-theme');
  if (value && JSON.parse(value)) {
    document.getElementById('theme-mode').textContent = 'dark';    
  } else {
    document.getElementById('theme-mode').textContent = 'ligth';
  }
}

async function getRefreshInterval() {
  const res = await fetch('/refresh-interval');
  
  if (!res.ok) {
    throw new Error(`HTTP error! status: ${res.status}`);
  }

  const value = await res.json();
  if (value && JSON.parse(value)) {
    document.getElementById('refreh-interval-input').value = value;    
  }
}

async function setRefreshInterval(interval = null) {
  if (!interval) {
    interval = document.getElementById("refreh-interval-input").value;
  }

  const res = await fetch('/refresh-interval', {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: interval
  });
  
  if (!res.ok) {
    throw new Error(`HTTP error! status: ${res.status}`);
  } else {
    document.getElementById("refreh-interval-input").value = interval; 
  }
}

setSwicthThemeText();
getRefreshInterval();
