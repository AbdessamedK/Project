#pragma once

#include <unordered_map>

#include <CGAL/Linear_cell_complex_for_combinatorial_map.h>
typedef CGAL::Linear_cell_complex_for_combinatorial_map<3> LCC;

#include <getfem/getfem_models.h>
typedef getfem::modeling_standard_plain_vector plain_vector;

namespace algorithm {

    inline std::unordered_map<LCC::Point, LCC::Vector> format_displacements(const getfem::mesh_fem &mf_u, const plain_vector &U) {
        std::unordered_map<LCC::Point, LCC::Vector> displacements;
        for (int j = 0; j < mf_u.nb_dof() / 3; j++) {
            bgeot::base_node pos = mf_u.point_of_basic_dof(j * 3);
            displacements.insert({
                LCC::Point(pos[0], pos[1], pos[2]),
                LCC::Vector(U[3 * j], U[3 * j + 1], U[3 * j + 2])
            });
        }
        return displacements;
    }

    inline void apply_displacements_to_lcc(LCC &lcc, const std::unordered_map<LCC::Point, LCC::Vector> &displacements) {
        for (auto it = lcc.vertex_attributes().begin(), ite = lcc.vertex_attributes().end(); it != ite; ++it) {
            LCC::Point &initial_point = lcc.point_of_vertex_attribute(it);
            if (auto fi = displacements.find(initial_point); fi != displacements.end()) {
                initial_point = LCC::Traits::Construct_translated_point_3()(initial_point, fi->second);
            }
        }
    }


}