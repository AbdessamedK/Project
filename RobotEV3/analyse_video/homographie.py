import numpy as np
import cv2
import matplotlib.pyplot as plt

# ----------- Polygon setup -----------
n = 20  # nombre de côtés
L = 70  # longueur en cm
R = L / (2 * np.sin(np.pi / n))

angles = np.linspace(0, 2 * np.pi, n, endpoint=False)
points_polygon = np.stack([
    R * np.cos(angles),
    R * np.sin(angles)
], axis=-1).astype(np.float32)

points_closed = np.vstack([points_polygon, points_polygon[0]])

center_point = np.array([[0.0, 0.0]], dtype=np.float32)

# ----------- Homographie bas -----------
pts_img1 = np.array([
    [527, 44],
    [458, 24],
    [396, 17],
    [333, 18],
    [272, 28],
    [208, 47],
    [151, 77],
    [104, 121],
    [63, 168],
    [43, 232],
    [47, 303],
    [592, 76],
    [326, 156]
], dtype=np.float32)

indices1 = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 19]
pts_poly1 = np.vstack([points_polygon[indices1], center_point])

H1, _ = cv2.findHomography(pts_img1, pts_poly1)

print(H1)

# ----------- Homographie haut -----------
pts_img2 = np.array([
    [427, 34],
    [370, 32],
    [311, 40],
    [258, 54],
    [208, 78],
    [166, 111],
    [135, 152],
    [119, 200],
    [128, 258],
    [366, 142]
], dtype=np.float32)

indices2 = [11, 12, 13, 14, 15, 16, 17, 18, 19]
pts_poly2 = np.vstack([points_polygon[indices2], center_point])

H2, _ = cv2.findHomography(pts_img2, pts_poly2)

print(H2)

# ----------- Charger l'image -----------
image = cv2.imread("imageC.png")
original_image = image.copy()
points_img = []
points_plan = []

# ----------- Matplotlib pour dessiner les points projetés -----------

plt.ion()  # mode interactif
fig, ax = plt.subplots(figsize=(6, 6))
ax.plot(points_closed[:, 0], points_closed[:, 1], 'k-')  # <- polygone fermé ici
sc = ax.scatter([], [], c='blue')
ax.axis('equal')
ax.grid(True)
ax.legend()
ax.set_title("Points projetés sur le plan 2D")


# ----------- Fonctions -----------

def image_to_plan(point_img, H):
    pt = np.array([[point_img]], dtype=np.float32)
    pt_transformed = cv2.perspectiveTransform(pt, H)
    return pt_transformed[0][0]

def update_plot():
    arr = np.array(points_plan)
    if arr.shape[0] > 0:
        sc.set_offsets(arr)
    plt.draw()
    plt.pause(0.001)

def click_event(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONDOWN:
        points_img.append([x, y])
        print(f"Point cliqué dans l'image : ({x}, {y})")

        y_adjusted = 0
        H = H1
        if (y >= (image.shape[0]/2)):
            y_adjusted = y - (image.shape[0] / 2)
            H = H1
        else:
            y_adjusted = y
            H = H2

        pt_polygon = image_to_plan((x, y_adjusted), H)
        print(f"Coordonnée projetée sur le plan : {pt_polygon}")
        
        points_plan.append(pt_polygon)

        # Dessiner cercle rouge sur image
        cv2.circle(image, (x, y), 5, (0, 0, 255), -1)
        cv2.imshow("Image", image)

        # Mettre à jour le graphe matplotlib
        update_plot()

# ----------- Exécution principale -----------

cv2.imshow("Image", image)
cv2.setMouseCallback("Image", click_event)
cv2.waitKey(0)
cv2.destroyAllWindows()
