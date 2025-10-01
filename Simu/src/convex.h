#pragma once

#include <getfem/getfem_mesh.h>
#include <getfem/bgeot_mesh.h>
typedef bgeot::basic_mesh::ref_mesh_pt_ct convex_point_array;

#include <CGAL/Linear_cell_complex_for_combinatorial_map.h>
typedef CGAL::Linear_cell_complex_for_combinatorial_map<3> LCC;

#include "point.h"

enum CONVEX_TYPE {
    segment = 2,
    triangle = 3,
    tetrahedron = 4,
    // quadrilateron
    // prism
    // hexahedron
    // pyramid
};

template<typename point_t, CONVEX_TYPE ct>
struct convex {
    convex() = default;

    explicit convex(convex_point_array arr) {
        for (size_t i = 0; i < ct; ++i) {
            points[i] = arr[i];
        }
    }

    void add_to(LCC *lcc) const {
        if constexpr (ct == segment) {
            lcc->make_edge(points[0], points[1]);
        } else if constexpr (ct == triangle) {
            lcc->make_triangle(points[0], points[1], points[2]);
        } else if constexpr (ct == tetrahedron) {
            lcc->make_tetrahedron(points[0], points[1], points[2], points[3]);
        }
    }

    void add_to(getfem::mesh *mesh) const {
        if constexpr (ct == segment) {
            mesh->add_segment(points[0], points[1]);
        } else if constexpr (ct == triangle) {
            mesh->add_triangle(points[0], points[1], points[2]);
        } else if constexpr (ct == tetrahedron) {
            mesh->add_tetrahedron(points[0], points[1], points[2], points[3]);
        }
    }

    std::array<point_t, ct> points;
};

