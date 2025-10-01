#include <SDL2/SDL_mouse.h>
#include "window.h"
#include <vec.h>
#include <mat.h>
#include <mesh_io.h>
#include "hit.cpp"
#include <array>

struct Control
{

    Point click_scene;

    void check_click(const Transform &view, const Transform &projection, const MeshIOData &data)
    {
        int x, y;
        unsigned buttons = SDL_GetMouseState(&x, &y);

        if (buttons & SDL_BUTTON(1))
        {
            std::cout << "clique recu" << std::endl;
            float xr, yr;

            xr = x;
            yr = y;

            xr = 2 * xr / window_width() - 1;
            yr = -2 * yr / window_height() + 1;

            Point pnt_cam = view.inverse()(Point(0, 0, 0));
            Vector dir = click_scene - pnt_cam;

            Point mina = data.positions[data.indices[3 * 0]];
            Point minb = data.positions[data.indices[3 * 0 + 1]];
            Point minc = data.positions[data.indices[3 * 0 + 2]];
            std::array<Point, 3> minpts;
            minpts[0] = mina; minpts[1] = minb ; minpts[2] = minc;
            Hit hitmin =  triangle_hit(pnt_cam, dir, minpts);

            for(unsigned int i = 0; i < data.object_indices.size(); i++)
            {
                Point a = data.positions[data.indices[3 * i]];
                Point b = data.positions[data.indices[3 * i + 1]];
                Point c = data.positions[data.indices[3 * i + 2]];
                std::array<Point, 3> pts;
                pts[0] = a; pts[1] = b ;pts[2] = c;
                
                Hit hit = triangle_hit(pnt_cam, dir, pts);

                if(hit.t == std::numeric_limits<float>::infinity())
                {
                    continue;
                }

                if(hit.t < hitmin.t || hitmin.t == std::numeric_limits<float>::infinity())
                {
                    hitmin = hit;
                    minpts = pts;
                }
            }

            std::cout << "min t : " << hitmin.t << std::endl;

            for(Point p : minpts)
            {
                std::cout << p << std::endl;
            }

            Point click_img(xr, yr, 1.0);
            Transform scene = view.inverse()(projection.inverse());
            click_scene = scene(click_img);

            std::cout << "click_scene : " << click_scene << std::endl;
        }
    }
};