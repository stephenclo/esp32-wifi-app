function resetWifi() {
  if (confirm("Es-tu sûr de vouloir réinitialiser les paramètres WiFi ?")) {
    fetch("/reset-wifi");
  }
}

function arroser() {
}

// Exemple de mise à jour périodique
function subscribeIsConnected() {
  const intervalIsConnected = setInterval(async() => {
    let value = "loding...";
    try {
      const res = await fetch("/is-connected");
      
      if (!res.ok) { // ex: 404 ou 500
          throw new Error(`HTTP error! status: ${res.status}`);
      }

      value = await res.text();
    } catch (err) {
      value = "False";
      clearInterval(intervalIsConnected);
    }

    // TODO : gestion erreur, timeout court
    document.getElementById("is-connected-value").textContent = value;
  }, 15000);
}

subscribeIsConnected();


