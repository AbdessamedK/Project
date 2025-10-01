#pragma once

#include <getfem/getfem_mesh.h>
#include <getfem/getfem_mesh_fem.h>
#include <getfem/getfem_models.h>
typedef getfem::modeling_standard_plain_vector plain_vector;

#include <vector>

#include "convex.h"

template<typename point_type, CONVEX_TYPE ct>
class mesh_interface {
public:
    explicit mesh_interface(const getfem::mesh &gf_mesh) {
        for (dal::bv_visitor ic(gf_mesh.convex_index()); !ic.finished(); ++ic) {
            elements.emplace_back(gf_mesh.points_of_convex(ic));
        }
    }

    explicit mesh_interface(const LCC &lcc) {
        const auto begin = lcc.one_dart_per_cell<3>().begin();
        const auto end = lcc.one_dart_per_cell<3>().end();

        for (auto d = begin; d != end; ++d) {
            auto darts = lcc.one_dart_per_incident_cell<0, 3>(lcc.dart_descriptor(*d));

            std::vector<point_type> points;
            for (auto it = darts.begin(); it != darts.end(); ++it) {
                Point p = lcc.point(it);
                if constexpr (std::is_same_v<point_type, point3>) {
                    points.emplace_back(p.x(), p.y(), p.z());
                } else if constexpr (std::is_same_v<point_type, point2>) {
                    points.emplace_back(p.x(), p.y());
                }
            }

            for (size_t i = 0; i < points.size(); i += ct) {
                convex<point_type, ct> convex;
                for (size_t j = 0; j < ct && (i + j) < points.size(); ++j) {
                    convex.points[j] = points[i + j];
                }
                elements.emplace_back(convex);
            }
        }
    }


    explicit operator LCC() const {
        LCC result;

        for (const auto &element: elements) {
            element.add_to(&result);
        }

        return result;
    }

    explicit operator getfem::mesh() const {
        getfem::mesh result;

        for (const auto &element: elements) {
            element.add_to(&result);
        }

        return result;
    }

    size_t size() const {
        return elements.size();
    }

    void apply_displacements(const getfem::mesh_fem &mf_u, const plain_vector &U) {
        auto displacements = format_displacements(mf_u, U);
        translate(displacements);
    }

    static std::unordered_map<point_type, point_type> format_displacements(const getfem::mesh_fem &mf_u, const plain_vector &U) {
        std::unordered_map<point_type, point_type> displacements;
        for (int j = 0; j < mf_u.nb_dof() / 3; j++) {
            bgeot::base_node pos = mf_u.point_of_basic_dof(j * 3);
            displacements.insert({point_type(pos[0], pos[1], pos[2]), point_type(U[3 * j], U[3 * j + 1], U[3 * j + 2])});
        }
        return displacements;
    }

    void translate(const std::unordered_map<point_type, point_type> &displacements) {
        for (auto &element: elements) {
            for (auto &point: element.points) {
                if (auto it = displacements.find(point); it != displacements.end()) {
                    point += it->second;
                }
            }
        }
    }

    void display() const {
        for (const auto &element: elements) {
            for (const auto &point: element.points) {
                std::cout << point << " ";
            }
            std::cout << std::endl;
        }
    }

    static void analyse(const getfem::mesh &gf_mesh) {
        const auto &points = gf_mesh.points();
        for (dal::bv_visitor ip(gf_mesh.points_index()); !ip.finished(); ++ip) {
            const auto &coords = points[ip];
            std::cout << "coords: " << coords << std::endl;
        }

        for (dal::bv_visitor ic(gf_mesh.convex_index()); !ic.finished(); ++ic) {
            std::cout << "convex_index: " << ic << std::endl;

            auto convex_points = gf_mesh.points_of_convex(ic);
            for (const auto &convex_point: convex_points)
                std::cout << convex_point << " ";
            std::cout << std::endl;
        }
    }

private:
    std::vector<convex<point_type, ct> > elements;
};


