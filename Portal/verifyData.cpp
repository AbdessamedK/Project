#include "vec.h"
#include <vector>
#include <mesh_io.h>

struct VerifyData
{
    Point centerOfTriangle(const Point &a, const Point &b, const Point &c)
    {
        return (a + b + c) / 3;
    }

    float surfaceOfTriangle(const Point &a, const Point &b, const Point &c)
    {
        Vector n = cross(Vector(a, b), Vector(a, c));
        return length(n)/2;
    }

    Vector normalOfTriangle(const Point &a, const Point &b, const Point &c)
    {
        return normalize(cross(Vector(a, b), Vector(a, c)));
    }

    Vector normalOfObject(std::vector<Vector> normals)
    {
        Vector res;
        for (unsigned int i = 0; i < normals.size(); i++)
        {
            res = res + normals[i];
        }

        res = res / normals.size();

        return normalize(res);
    }

    Point centerofObject(const std::vector<Point> &centers, const std::vector<float> &surfaces)
    {
        Point res;
        float sum = 0;
        for (unsigned int i = 0; i < centers.size(); i++)
        {
            res = res + (centers[i] * surfaces[i]);
            sum += surfaces[i];
        }

        return res / sum;
    }

    void operator()(MeshIOData &data)
    {
        //START 
        int start = data.find_object("start");
        if (start != -1)
        {
            std::vector<Point> centers;
            std::vector<float> surfaces;
            std::vector<Vector> normals;

            for (unsigned int i = 0; i < data.object_indices.size(); i++)
            {
                if (data.object_indices[i] == start)
                {
                    Point a = data.positions[data.indices[3 * i]];
                    Point b = data.positions[data.indices[3 * i + 1]];
                    Point c = data.positions[data.indices[3 * i + 2]];

                    centers.push_back(centerOfTriangle(a, b, c));
                    surfaces.push_back(surfaceOfTriangle(a, b, c));
                    normals.push_back(normalOfTriangle(a, b, c));
                }
            }

            std::cout << "start trouvé centre : " << centerofObject(centers, surfaces) << " normal : " << normalOfObject(normals) << std::endl;
        }
        else
            std::cout << "start non trouvé" << std::endl;


        //FINISH
        int finish = data.find_object("finish");
        if (finish != -1)
            std::cout << "finish trouvé" << std::endl;
        else
            std::cout << "finish non trouvé" << std::endl;
    }
};

