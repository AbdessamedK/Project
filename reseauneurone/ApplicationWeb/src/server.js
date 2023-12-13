const express = require('express');
const fs = require('fs');
const path = require('path');
const { exec } = require('child_process');


const bodyParser = require('body-parser');

// Augmente la limite de taille des données pour les requêtes

const app = express();

app.use(bodyParser.json({ limit: '10mb' })); // Remplace '10mb' par la limite souhaitée en taille (ex: '50mb')
app.use(bodyParser.urlencoded({ extended: true, limit: '10mb' })); // Même limite pour les données encodées


app.use(express.static('.'));
app.use(express.json());

console.log("Bien coté serveur ! ");

app.post('/saveimage', (req, res) => {
    
  const base64Data = req.body.image.replace(/^data:image\/png;base64,/, '');
  //console.log(req.body.image);
  //console.log('Requête reçue sur /saveimage'); // Log pour indiquer que la route est atteinte
  // Log des données envoyées avec la requête
  //console.log('Données reçues :', req.body); // Adapté à ta structure de données envoyées

  const imageName = 'captured_image_' + Date.now() + '.png';
  const imagePath = path.join(__dirname, 'data', imageName); // Changement du chemin pour enregistrer dans le dossier 'data'

  // ... Ton code pour recevoir et traiter l'image côté serveur  
  // Exemple d'exécution d'un script Python
  exec('python3 python.py ' + base64Data, (error, stdout, stderr) => {
      if (error) {
            res.json({ message: 'Erreur lors de lexecution!'});
            console.log("Erreur lors de l'execution :");
            console.log(stderr);
          return;
      }
      res.json({ message: 'Tu as passé ',stdout});
      // Ici, tu peux envoyer stdout (la sortie du script Python) comme réponse à ta requête HTTP
  });
  

//   fs.writeFile(imagePath, base64Data, 'base64', (err) => {
//     if (err) {
        
//       res.status(500).json({ message: imagePath}); 
//     } else {
//       res.json({ message: 'Image enregistrée avec succès !', imagePath });
//     }
//   })
  
  ;
});

const PORT = 8080;
app.listen(PORT, () => {
  console.log(`Serveur en cours d'exécution sur le port ${PORT}`);
});
