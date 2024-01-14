// Funktion zum Umschalten der Pumpen ueber einen HTTP-Request
function togglepump(pump) {
  fetch('/pumpe/toggle', { // PUT-Request an den Server
    method: 'PUT',
    headers: {
      'Content-Type': 'application/x-www-form-urlencoded', // Formulardaten
    },
    body: 'pump=' + pump
  })
  .then(response => {
    if (!response.ok) {
      throw new Error('Netzwerkantwort war nicht ok');
    }
    updatepumpStatus();
  })
  .catch(error => {
    console.error('Fehler beim Umschalten des pump:', error);
  });
}

  
// Funktion zum periodischen Abrufen des Feuchtigkeitswertes
function refreshData() {
  fetch('/feuchtigkeit') // GET-Request an den Server
    .then(response => {
      // Sicherstellen, dass die Anfrage erfolgreich war
      if (!response.ok) {
        throw new Error('Netzwerkfehler');
      }
      return response.text();
    })
    .then(data => {
      // Aktualisieren des Textinhalts des HTML-Elements mit der ID "feuchtigkeit"
      document.getElementById("feuchtigkeit").innerText = data;
    })
    .catch(error => {
      console.error('Fehler beim Abrufen des Feuchtigkeitswerts:', error);
    });
}

// Aktualisieren der Feuchtigkeitsdaten jede Sekunde
setInterval(refreshData, 1000);

// Funktion zum Abrufen des pump-Status
function updatepumpStatus() {
  fetch('/pumpe/status') // GET-Request an den Server
  .then(response => response.json()) // Antwort als JSON interpretieren
  .then(data => { // JSON-Daten verarbeiten
      const currentTime = new Date().getTime(); // Aktuelle Zeit in Millisekunden
      const table = document.getElementById('pumpStatus');
      while (table.rows.length > 1) {
          table.deleteRow(1);
      }
      data.pumps.forEach((pump, index) => {
          const row = table.insertRow(-1);
          const cell1 = row.insertCell(0);
          const cell2 = row.insertCell(1);
          cell1.innerHTML = index + 1; // pump Nummer
          cell2.innerHTML = pump.lastActivated; // Zeitpunkt der letzten Aktivierung
      });
  })
  .catch(error => console.error('Fehler beim Abrufen des pump-Status:', error));
}


// Aktualisieren des pump-Status beim Laden der Seite
document.addEventListener('DOMContentLoaded', function() {
  updatepumpStatus();
});



