(function() {

  var width = 320; // We will scale the photo width to this
  var height = 0; // This will be computed based on the input stream

  var streaming = false;

  var video = null;
  var canvas = null;
  var photo = null;
  var startbutton = null;


  function startup() {
    video = document.getElementById('video');
    canvas = document.getElementById('canvas');
    photo = document.getElementById('photo');
    startbutton = document.getElementById('startbutton');

    // access video stream from webcam
    navigator.mediaDevices.getUserMedia({
            video: true,
            audio: false
        })
        // on success, stream it in video tag
        .then(function(stream) {
            video.srcObject = stream;
            video.play();
        })
        .catch(function(err) {
            console.log("An error occurred: " + err);
        });

    video.addEventListener('canplay', function(ev) {
        if (!streaming) {
            height = video.videoHeight / (video.videoWidth / width);

            if (isNaN(height)) {
                height = width / (4 / 3);
            }

            video.setAttribute('width', width);
            video.setAttribute('height', height);
            canvas.setAttribute('width', width);
            canvas.setAttribute('height', height);
            streaming = true;
        }
    }, false);

    startbutton.addEventListener('click', function(ev) {
        takepicture();
        ev.preventDefault();
    }, false);

    clearphoto();
}

function clearphoto() {
  var context = canvas.getContext('2d');
  context.fillStyle = "#AAA";
  context.fillRect(0, 0, canvas.width, canvas.height);

  var data = canvas.toDataURL('image/png');
  //photo.setAttribute('src', data);
}

function takepicture() {
  var context = canvas.getContext('2d');
  if (width && height) {
    canvas.width = width;
    canvas.height = height;
    context.drawImage(video, 0, 0, width, height);

    var data = canvas.toDataURL('image/png');

    // Envoi de l'image capturée vers le serveur
    fetch('/saveimage', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({ image: data }) // Envoyer les données encodées en base64
    })
    .then(response => response.json())
    .then(data => {

      var reponse = data.stdout;

      reponse = reponse.split("|");
      const reponse1 = reponse[0];
      const reponse2 = reponse[1];
      
      console.log('Réponse du serveur :', data.stdout);

      // Gérer la réponse du serveur ici si nécessaire
      // Supposons que votre réponse soit stockée dans une variable appelée "reponse"
      // Sélection de l'élément <p> par son ID
      const paragrapheEmotion = document.getElementById('emotion');
      const paragrapheHistoire = document.getElementById('histoire');

      // // Mise à jour du contenu de la balise <p> avec la réponse
      paragrapheEmotion.textContent = reponse1;
      paragrapheHistoire.textContent = reponse2;
    })
    .catch(error => {
      console.error('Erreur lors de l\'envoi de l\'image au serveur :', error);
      // Gérer les erreurs ici si nécessaire
    });

    //photo.setAttribute('src', data);
  } else {
    clearphoto();
  }
}
  
  window.addEventListener('load', startup, false);
})();