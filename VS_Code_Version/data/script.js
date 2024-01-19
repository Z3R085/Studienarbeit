


// Funktion zum Umschalten der pumpn ueber einen HTTP-Request
function togglepump(pump) {
  fetch('/pump/toggle', { // PUT-Request an den Server
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
  fetch('/pump/status') // GET-Request an den Server
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

// Funktion zum Setzen der Pumpendauer über einen HTTP-Request
function setPumpDuration(pumpNumber, duration) {
  fetch(`/pump/duration`, { // PUT-Request an den Server 
    method: 'PUT',
    headers: {
      'Content-Type': 'application/x-www-form-urlencoded', // Formulardaten als Inhalt des Requests 
    },
    body: `pump=${pumpNumber}&duration=${duration}` // Sendet die Pumpe und die gewünschte Dauer
  })
  .then(response => {
    if (!response.ok) {
      throw new Error('Netzwerkantwort war nicht ok');
    }
    return response.text();
  })
  .then(text => {
    console.log(text); // Log die Antwort vom Server
    updatepumpStatus(); // Aktualisiert den Status der Pumpen, optional je nach Implementierung
  })
  .catch(error => {
    console.error('Fehler beim Setzen der Pumpendauer:', error);
  });
}
// Funktion zum Ändern des Gießmodus
function changeWateringMode(selectedMode) {
  fetch('/pump/mode', { 
    method: 'PUT',
    headers: {
      'Content-Type': 'application/x-www-form-urlencoded',
    },
    body: 'mode=' + selectedMode
  })
  .then(response => {
    if (!response.ok) {
      throw new Error('Netzwerkantwort war nicht ok');
    }
    return response.text();
  })
  .then(text => {
    console.log(text); // Log die Antwort vom Server
    // Optional: Aktualisieren Sie die Benutzeroberfläche, um den neuen Modus anzuzeigen
  })
  .catch(error => {
    console.error('Fehler beim Ändern des Gießmodus:', error);
  });
}




// Aktualisieren des pump-Status beim Laden der Seite
document.addEventListener('DOMContentLoaded', function() {
  updatepumpStatus();
});



