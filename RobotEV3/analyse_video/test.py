import cv2
import numpy as np

# Charger l'image (remplacer "image.jpg" par le chemin de votre screenshot)
img = cv2.imread("screen4.png")
if img is None:
    print("Erreur lors du chargement de l'image.")
    exit()

# Convertir l'image en espace HSV
hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

# --- Détection de la balle rouge ---
# Définir deux plages pour le rouge (car le rouge se trouve aux deux extrémités de la roue HSV)
lower_red1 = np.array([0, 100, 100])
upper_red1 = np.array([10, 255, 255])
lower_red2 = np.array([160, 100, 100])
upper_red2 = np.array([180, 255, 255])

mask_red1 = cv2.inRange(hsv, lower_red1, upper_red1)
mask_red2 = cv2.inRange(hsv, lower_red2, upper_red2)
mask_red = cv2.add(mask_red1, mask_red2)

# --- Détection de la balle bleue ---
lower_blue = np.array([100, 150, 0])
upper_blue = np.array([140, 255, 255])
mask_blue = cv2.inRange(hsv, lower_blue, upper_blue)

# Paramètres pour filtrer les contours
min_area = 5          
max_area = 50
min_circularity = 0.75

# Fonction pour traiter un masque et dessiner le point sur le centre de la balle
def detect_and_draw(mask, point_color):
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    for cnt in contours:
        area = cv2.contourArea(cnt)
        if area > min_area and area < max_area:
            perimeter = cv2.arcLength(cnt, True)
            if perimeter > 0:
                circularity = 4 * np.pi * area / (perimeter * perimeter)
                if circularity >= min_circularity:
                    M = cv2.moments(cnt)
                    if M["m00"] != 0:
                        cx = int(M["m10"] / M["m00"])
                        cy = int(M["m01"] / M["m00"])
                        cv2.circle(img, (cx, cy), 5, point_color, -1)

# Détecter et marquer la balle rouge avec un point violet (BGR: (255, 0, 255))
detect_and_draw(mask_red, (255, 0, 255))

# Détecter et marquer la balle bleue avec un point jaune (BGR: (0, 255, 255))
detect_and_draw(mask_blue, (0, 255, 255))

# Afficher le résultat
cv2.imshow("Detection des balles", img)
cv2.waitKey(0)
cv2.destroyAllWindows()
