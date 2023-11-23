// Funktion zum Umschalten der Relais über einen HTTP-Request
function toggleRelay(relay) {
  var xhr = new XMLHttpRequest();
  xhr.open("PUT", "/relais/toggle", true);
  xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
  xhr.send("relay=" + relay);
  updateRelayStatus();
}


  // Funktion zum periodischen Abrufen des Feuchtigkeitswertes
  function refreshData() {
    var xhr = new XMLHttpRequest();
    // Festlegen einer Funktion, die aufgerufen wird, wenn sich der Zustand des XMLHttpRequests ändert.
    xhr.onreadystatechange = function () {
      // Überprüfen, ob die Anfrage abgeschlossen ist (readyState == 4) und ob sie erfolgreich war (status == 200).
      if (this.readyState == 4 && this.status == 200) {
        // Aktualisieren des Textinhalts des HTML-Elements mit der ID "feuchtigkeit" mit der Antwort des Servers.
        document.getElementById("feuchtigkeit").innerText =
          this.responseText;
      }
    };
    // Konfigurieren der GET-Anfrage
    xhr.open("GET", "/feuchtigkeit", true);
    //Senden der Anfrage an den Server
    xhr.send();
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



