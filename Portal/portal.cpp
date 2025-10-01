// exemple : lecture et affichage d'un mesh, avec un shader de base + gestion des evenements par window.h
// bon point de depart...

#include "vec.h"
#include "mat.h"

#include "window.h"
#include "glcore.h"

#include "mesh_io.h"
#include "buffers.h"
#include "draw.h"

#include "verifyData.cpp"
#include "control.cpp"

GLuint vao = 0;
GLuint cube = 0;
unsigned count = 0;

MeshIOData data;

Control control;
VerifyData verify;

bool init()
{
    
    if (!read_meshio_data("../data/cubes.obj", data))
        return false; // erreur de lecture

    MeshIOData cubemesh;
    if (!read_meshio_data("../data/cube.obj", cubemesh))
        return false; // erreur de lecture
    
    verify(data);

    cube = create_buffers(cubemesh.positions, cubemesh.indices, {}, cubemesh.normals);

    vao = create_buffers(data.positions, data.indices, {}, data.normals);
    /* ou
        vao= create_buffers(data.positions, data.indices, data.texcoords, data.normals);
        mais s'il y a des coordonn�es de textures dans l'objet, il faut aussi une texture pour le dessiner.
        on verra plus tard. donc pour l'instant on ne cree pas de coordonn�es de texture...
     */

    count = data.indices.size();

    return true;
}

void quit()
{
    release_buffers(vao);
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Transform model;                          // placer le modele
    Transform view = Translation(0, -2, -10); // camera
    Transform projection = Perspective(45, 1024.0 / 576.0, 0.1, 100);

    control.check_click(view, projection, data);

    Transform click_transform = Translation(control.click_scene.x,control.click_scene.y,control.click_scene.z);

    // super pratique, mais il faut faire traiter les evenements par la fonction events() de window.h
    if (key_state(' '))
        model = Translation(1, 0, 0);

    draw(vao, GL_TRIANGLES, count, model, view, projection);

    draw(cube, GL_TRIANGLES, count, click_transform, view, projection);
}

int main(int argc, char **argv)
{
    Window window = create_window(1024, 576);
    Context context = create_context(window);

    // etat openGL de base / par defaut
    glViewport(0, 0, 1024, 576);
    glClearColor(0.2, 0.2, 0.2, 1);
    glClearDepth(1);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    if (!init())
        return 1;

    while (events(window))
    {
        // dessiner
        draw();

        // presenter / montrer le resultat
        SDL_GL_SwapWindow(window);


    }

    quit();

    release_context(context);
    release_window(window);
    return 0;
}
