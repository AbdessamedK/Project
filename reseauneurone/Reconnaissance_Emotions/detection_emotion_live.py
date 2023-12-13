import cv2
import torch
from transformers import ViTForImageClassification, ViTImageProcessor

model = ViTForImageClassification.from_pretrained("lebin")
processor = ViTImageProcessor.from_pretrained("lebin")

# Charger le classificateur de visage Haar
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# Ouvrir la caméra (l'argument peut être 0 pour la caméra par défaut)
cap = cv2.VideoCapture(0)

mot = ""

if not cap.isOpened():
    print("Erreur : Impossible d'ouvrir la caméra.")
else:
    while True:
        # Lire une image depuis la caméra
        ret, frame = cap.read()
        
        # Attendre 1 milliseconde et vérifier si une touche est pressée
        key = cv2.waitKey(1) & 0xFF

        if not ret:
            print("Erreur : Impossible de capturer une image depuis la caméra.")
            break

        # Convertir l'image en niveaux de gris
        gray_image = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        # Détecter les visages dans l'image
        faces = face_cascade.detectMultiScale(gray_image, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

        # Afficher uniquement le premier visage détecté (si présent)
        if len(faces) > 0:
            x, y, w, h = faces[0]
            # Dessiner le rectangle du visage directement sur l'image complète
            cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)

            # Si la touche 'e' est pressé, lancer la prédiction
            if key == ord('e'):
                image = frame[y:y+h+30,(x-5):x+w+10]
                image = processor(images=image, return_tensors="pt")
                outputs = model(**image)
                predicted_num = torch.argmax(outputs.logits, dim=1).item()

                emotions = ["Sad", "Disgust", "Angry", "Neutral", "Fear", "Surprise", "Happy", "Aucune"]
                color = [(255,0,0),(0,255,0),(0,0,255),(255,255,255),(255,0,255),(255,165,255),(0,255,255),(147,20,255)]
                mot = emotions[predicted_num]
                
        if mot :
            cv2.putText(frame, mot, (10, 280), cv2.FONT_HERSHEY_SIMPLEX, 1, color[predicted_num], 2, cv2.LINE_AA)
        
        text = "Appuyez sur 'q' pour quitter"
        cv2.putText(frame, text, (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 100, 0), 2, cv2.LINE_AA)
        text = "Appuyez sur 'e' pour reconnaitre l'emotion actuelle"
        cv2.putText(frame, text, (10, 80), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 200, 0), 2, cv2.LINE_AA)
            
        # Afficher la vidéo avec le visage détecté
        cv2.imshow("Caméra avec Visage", frame)

        # Si la touche 'q' est pressée, quitter la boucle
        if key == ord('q'):
            break

    # Libérer la caméra à la fin
    cap.release()
    cv2.destroyAllWindows()