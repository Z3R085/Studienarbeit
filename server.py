from flask import Flask, request, jsonify
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
import pytz  # Importieren von pytz

app = Flask(__name__)

app.config['SQLALCHEMY_DATABASE_URI'] = 'postgresql://meinBenutzer:meinPasswort@localhost:5432/Studienarbeit'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)

class Feuchtigkeitsmessung(db.Model):
    __tablename__ = 'feuchtigkeitsmessungen'
    id = db.Column(db.Integer, primary_key=True)
    feuchtigkeitswert = db.Column(db.Numeric(5, 2))
    # Verwendung von pytz, um den Messzeitpunkt in einer spezifischen Zeitzone festzulegen
    messzeitpunkt = db.Column(db.DateTime, default=lambda: datetime.now(pytz.timezone('Europe/Berlin')))
    temperatur = db.Column(db.Numeric(5, 2))

    def __init__(self, feuchtigkeitswert, temperatur):
        self.feuchtigkeitswert = feuchtigkeitswert
        self.temperatur = temperatur

@app.route('/api/insertData', methods=['POST'])
def insert_data():
    try:
        data = request.get_json()
        feuchtigkeitsmessung = Feuchtigkeitsmessung(
            feuchtigkeitswert=data['feuchtigkeitswert'],
            temperatur=data['temperatur']
        )
        db.session.add(feuchtigkeitsmessung)
        db.session.commit()
        return jsonify({'message': 'Data inserted successfully'}), 200
    except Exception as e:
        return jsonify({'error': str(e)}), 500

if __name__ == '__main__':
    with app.app_context():
        db.create_all()
    app.run(debug=True, host='0.0.0.0')
