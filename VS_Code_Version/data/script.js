// Funktion zum Umschalten der Relais über einen HTTP-Request
function toggleRelay(relay) {
    //toggleRelay bekommt Nummer der "Pumpe"
    var xhr = new XMLHttpRequest(); // Ein neues XMLHttpRequest-Objekt wird erstellt, das für die Durchführung von HTTP-Anfragen verwendet wird.
    // Die "open"-Methode konfiguriert die Art der Anfrage (GET), die Ziel-URL und setzt die Anfrage als asynchron (true).
    // Der URL wird der Query-Parameter "relay" angehängt, der den Wert des "relay"-Parameters enthält.
    xhr.open("GET", "/toggle?relay=" + relay, true);
    // Die "send"-Methode sendet die konfigurierte Anfrage an den Server.
    xhr.send();
    updateRelayStatus();  // Aktualisieren des Relais-Status
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
    fetch('/relay-status')
    .then(response => response.json())
    .then(data => {
        const table = document.getElementById('relaisStatus'); // Tabelle mit der ID "relaisStatus" wird ausgewählt
        // Lösche bestehende Zeilen, außer der Kopfzeile
        while(table.rows.length > 1) {
            table.deleteRow(1);
        }
        // Füge für jedes Relais eine neue Zeile hinzu
        data.relays.forEach((relay, index) => {
            const row = table.insertRow(-1); // Fügt eine neue Zeile am Ende der Tabelle ein
            const cell1 = row.insertCell(0);
            const cell2 = row.insertCell(1);
            const cell3 = row.insertCell(2);
            cell1.innerHTML = index + 1; // Relais Nummer
            cell2.innerHTML = relay.isOn ? 'Eingeschaltet' : 'Ausgeschaltet';
            cell3.innerHTML = new Date(relay.lastActivated).toLocaleString(); // Formatierung des Zeitstempels
        });
    })
    .catch(error => console.error('Fehler beim Abrufen des Relais-Status:', error)); // Fehlerbehandlung
}

// Aktualisieren des Relais-Status alle 2,5 Sekunden
setInterval(updateRelayStatus, 1000); 