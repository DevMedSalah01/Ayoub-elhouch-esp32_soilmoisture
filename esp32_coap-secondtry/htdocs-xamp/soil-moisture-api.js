const express = require('express');
const bodyParser = require('body-parser');
const fs = require('fs');

const app = express();
app.use(bodyParser.json());

// Stocker les données d'humidité du sol dans un fichier
const dataFilePath = 'soil-moisture-data.json';

app.post('/api/soil-moisture', (req, res) => {
  const { soilMoisture } = req.body;

  // Écrire les données dans le fichier
  fs.appendFileSync(dataFilePath, JSON.stringify({ soilMoisture, timestamp: new Date().toISOString() }) + '\n');

  res.status(200).json({ message: 'Données d\'humidité du sol enregistrées' });
});

app.get('/api/soil-moisture', (req, res) => {
  try {
    // Lire les données d'humidité du sol à partir du fichier
    const data = fs.readFileSync(dataFilePath, 'utf8')
      .split('\n')
      .slice(0, -1)
      .map(line => JSON.parse(line));
    res.status(200).json(data);
  } catch (err) {
    res.status(500).json({ error: 'Erreur lors de la lecture des données' });
  }
});

const port = 3001;
app.listen(port, () => {
  console.log(`Serveur API REST démarré sur le port ${port}`);
});