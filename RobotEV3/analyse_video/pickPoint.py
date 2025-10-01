import cv2
import numpy as np
import matplotlib.pyplot as plt


image = cv2.imread("imageC.png")

points_img = []

if image is None:
    print("Erreur : l'image n'a pas pu être chargée.")
    exit(1)

n = 20  # Nombre de côtés
L = 70  # Longueur de côté en cm

R = L / (2 * np.sin(np.pi / n))

angles = np.linspace(0, 2*np.pi, n, endpoint=False)
points_polygon = np.stack([
    R * np.cos(angles),
    R * np.sin(angles)
], axis=-1).astype(np.float32)

points_closed = np.vstack([points_polygon, points_polygon[0]])

plt.figure(figsize=(8, 8))
plt.plot(points_closed[:, 0], points_closed[:, 1], 'b-', linewidth=2)
plt.scatter(points_closed[:, 0], points_closed[:, 1], color='red')

for i, (x, y) in enumerate(points_polygon):
    plt.text(x + 5, y + 5, str(i), fontsize=10, color='black')

plt.axis('equal')
plt.xlabel("x (cm)")
plt.ylabel("y (cm)")
plt.show(block=False)

def click_event(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONDOWN:
        points_img.append([x, y])
        print(f"Point cliqué : ({x}, {y})")
        cv2.circle(image, (x, y), 5, (0, 0, 255), -1)
        cv2.imshow("Image", image)

cv2.imshow("Image", image)
cv2.setMouseCallback("Image", click_event)
cv2.waitKey(0)
cv2.destroyAllWindows()
