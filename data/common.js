// chek if ligth theme 
function getLightTheme() {
  const value = localStorage.getItem('light-theme');
  return value ? JSON.parse(value) : null;
}

function setTheme() {
  document.body.classList.toggle('light-mode', getLightTheme());
}

setTheme();