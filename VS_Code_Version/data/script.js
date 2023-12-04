// Funktion zum Umschalten der Relais über einen HTTP-Request
function toggleRelay(relay) {
  fetch('/relais/toggle', {
    method: 'PUT',
    headers: {
      'Content-Type': 'application/x-www-form-urlencoded',
    },
    body: 'relay=' + relay
  })
  .then(response => {
    if (!response.ok) {
      throw new Error('Netzwerkantwort war nicht ok');
    }
    updateRelayStatus();
  })
  .catch(error => {
    console.error('Fehler beim Umschalten des Relais:', error);
  });
}



  
// Funktion zum periodischen Abrufen des Feuchtigkeitswertes
function refreshData() {
  fetch('/feuchtigkeit')
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

// Funktion zum Abrufen des Relais-Status
function updateRelayStatus() {
  fetch('/relay-status') // GET-Request an den Server
  .then(response => response.json()) // Antwort als JSON interpretieren
  .then(data => { // JSON-Daten verarbeiten
      const currentTime = new Date().getTime(); // Aktuelle Zeit in Millisekunden
      const table = document.getElementById('relaisStatus');
      while (table.rows.length > 1) {
          table.deleteRow(1);
      }
      data.relays.forEach((relay, index) => {
          const row = table.insertRow(-1);
          const cell1 = row.insertCell(0);
          const cell2 = row.insertCell(1);
          cell1.innerHTML = index + 1; // Relais Nummer
          cell2.innerHTML = relay.lastActivated; // Zeitpunkt der letzten Aktivierung
      });
  })
  .catch(error => console.error('Fehler beim Abrufen des Relais-Status:', error));
}


// Aktualisieren des Relais-Status beim Laden der Seite
document.addEventListener('DOMContentLoaded', function() {
  updateRelayStatus();
});



