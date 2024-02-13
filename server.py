# Importieren der erforderlichen Module und Pakete
from flask import Flask, request, jsonify  # Flask für das Web-Framework, request für Anfragedaten, jsonify zur JSON-Antwortkonvertierung
from flask_sqlalchemy import SQLAlchemy  # SQLAlchemy für ORM (Object-Relational Mapping)
from datetime import datetime  # datetime für Datums- und Zeitoperationen
import pytz  # pytz zur Handhabung von Zeitzonen

app = Flask(__name__)  # Erstellen einer Flask-Anwendung

# Konfiguration der Anwendung, einschließlich der Datenbankverbindung
app.config['SQLALCHEMY_DATABASE_URI'] = 'postgresql://meinBenutzer:meinPasswort@localhost:5432/Studienarbeit'  # URI für die Datenbankverbindung
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False  # Deaktivieren der Verfolgung von Modifikationen zur Leistungsverbesserung
db = SQLAlchemy(app)  # Initialisieren von SQLAlchemy mit der Flask-App

# Definition des Datenbankmodells für Feuchtigkeitsmessungen
class Feuchtigkeitsmessung(db.Model):
    __tablename__ = 'feuchtigkeitsmessungen'  # Tabellenname in der Datenbank
    id = db.Column(db.Integer, primary_key=True)  # Primärschlüssel
    feuchtigkeitswert = db.Column(db.Numeric(5, 2))  # Spalte für den Feuchtigkeitswert
    messzeitpunkt = db.Column(db.DateTime, default=lambda: datetime.now(pytz.timezone('Europe/Berlin')))  # Spalte für den Messzeitpunkt, nutzt pytz für Zeitzonen
    temperatur = db.Column(db.Numeric(5, 2))  # Spalte für die Temperatur

    def __init__(self, feuchtigkeitswert, temperatur):
        # Initialisierungsfunktion für neue Instanzen des Modells
        self.feuchtigkeitswert = feuchtigkeitswert
        self.temperatur = temperatur

# Route und Funktion zur Verarbeitung von POST-Anfragen zum Einfügen neuer Daten
@app.route('/api/insertData', methods=['POST'])
def insert_data():
    try:
        data = request.get_json()  # Extrahieren von JSON-Daten aus der Anfrage
        feuchtigkeitsmessung = Feuchtigkeitsmessung(
            feuchtigkeitswert=data['feuchtigkeitswert'],  # Erstellen eines neuen Objekts mit den Daten
            temperatur=data['temperatur']
        )
        db.session.add(feuchtigkeitsmessung)  # Hinzufügen des neuen Objekts zur Datenbanksession
        db.session.commit()  # Speichern der Änderungen in der Datenbank
        return jsonify({'message': 'Data inserted successfully'}), 200  # Rückgabe einer Erfolgsmeldung
    except Exception as e:
        return jsonify({'error': str(e)}), 500  # Im Fehlerfall Rückgabe einer Fehlermeldung

# Hauptteil der Anwendung, der beim direkten Ausführen des Skripts aktiviert wird
if __name__ == '__main__':
    with app.app_context():
        db.create_all()  # Erstellen aller Tabellen basierend auf den Modelldefinitionen
    app.run(debug=True, host='0.0.0.0')  # Starten der Flask-Anwendung mit Debugging aktiviert und zugänglich auf jedem Interface
