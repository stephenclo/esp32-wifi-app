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
setSwicthThemeText();