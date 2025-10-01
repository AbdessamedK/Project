/**
   Code from https://github.com/getfem/getfem/blob/master/tests/elastostatic.cc
*/

#include "getfem/getfem_export.h"
#include "getfem/getfem_regular_meshes.h"
#include "getfem/getfem_model_solvers.h"
#include "getfem/getfem_nonlinear_elasticity.h"
#include "gmm/gmm.h"

#include "CGAL/draw_linear_cell_complex.h"

#include "algorithm.h"
#include "mesh_interface.h"

/* some GetFEM types that we will be using */
using bgeot::base_matrix; /* small dense matrix. */
using bgeot::base_node; /* geometrical nodes(derived from base_small_vector)*/
using bgeot::base_small_vector; /* special class for small (dim<16) vectors  */
using bgeot::base_vector;
using bgeot::dim_type;
using bgeot::scalar_type; /* = double */
using bgeot::size_type; /* = unsigned long */

/* definition of some matrix/vector types. These ones are built
 * using the predefined types in Gmm++
 */
typedef getfem::modeling_standard_sparse_vector sparse_vector;
typedef getfem::modeling_standard_sparse_matrix sparse_matrix;
typedef getfem::modeling_standard_plain_vector plain_vector;


enum class simulation_type {
    TORSION = 1,
    EXTENSION = 2,
    SHEAR = 3,
    COMPRESSION = 4,
    BENDING = 5
};;

simulation_type get_simulation_type(int index) {
    switch (index) {
        case 1: return simulation_type::TORSION;
        case 2: return simulation_type::EXTENSION;
        case 3: return simulation_type::SHEAR;
        case 4: return simulation_type::COMPRESSION;
        case 5: return simulation_type::BENDING;
        default:
            std::cout << "Warning (PARAM: TRANSFO) Unknown simulation type, using TORSION by default" << std::endl;
            return simulation_type::TORSION;
    }
}

/*
  structure for the elastostatic problem
*/
struct elastostatic_problem {
    enum {
        DIRICHLET_BOUNDARY_NUM = 0,
        NEUMANN_BOUNDARY_NUM = 1
    };

    getfem::mesh mesh; /* the mesh */
    getfem::mesh_im mim; /* the integration methods */
    getfem::mesh_fem mf_u; /* main mesh_fem, for the elastostatic solution */
    getfem::mesh_fem mf_p; /* mesh_fem for the pressure.                   */
    getfem::mesh_fem mf_rhs; /* mesh_fem for the right hand side (f(x),..)   */
    getfem::mesh_fem mf_coef; /* mesh_fem used to represent pde coefficients  */

    scalar_type p1, p2, p3; /* elastic coefficients.                        */

    scalar_type residual; /* max residual for the iterative solvers         */

    std::string datafilename;
    bgeot::md_param PARAM;

    simulation_type simulation;

    bool solve(plain_vector &U);

    void init();

    elastostatic_problem() : mim(mesh), mf_u(mesh), mf_p(mesh), mf_rhs(mesh), mf_coef(mesh) {
    }

    void vtk_export(const plain_vector &U);
};


/* Read parameters from the .param file, build the mesh, set finite element
   and integration methods and selects the boundaries.

   (this is boilerplate code, not very interesting)
 */
void elastostatic_problem::init() {
    std::string MESH_TYPE = PARAM.string_value("MESH_TYPE", "Mesh type ");
    std::string FEM_TYPE = PARAM.string_value("FEM_TYPE", "FEM name");
    std::string FEM_TYPE_P = PARAM.string_value("FEM_TYPE_P", "FEM name for the pressure");
    std::string INTEGRATION = PARAM.string_value("INTEGRATION", "Name of integration method");

    /* Set the simulation type */
    simulation = get_simulation_type(PARAM.int_value("TRANSFO"));

    std::cout << "MESH_TYPE=" << MESH_TYPE << "\n";
    std::cout << "FEM_TYPE=" << FEM_TYPE << "\n";
    std::cout << "INTEGRATION=" << INTEGRATION << "\n";

    /* First step : build the mesh */
    bgeot::pgeometric_trans pgt = bgeot::geometric_trans_descriptor(MESH_TYPE);
    size_type N = pgt->dim();
    std::vector<size_type> nsubdiv(N);
    std::fill(nsubdiv.begin(), nsubdiv.end(), PARAM.int_value("NX", "Nomber of space steps "));
    nsubdiv[1] = PARAM.int_value("NY") ? PARAM.int_value("NY") : nsubdiv[0];
    if (N > 2)
        nsubdiv[2] = PARAM.int_value("NZ") ? PARAM.int_value("NZ") : nsubdiv[0];
    getfem::regular_unit_mesh(mesh, nsubdiv, pgt, PARAM.int_value("MESH_NOISED") != 0);

    bgeot::base_matrix M(N, N);

    for (size_type i = 0; i < N; ++i) {
        static const char *t[] = {"LX", "LY", "LZ"};
        M(i, i) = (i < 3) ? PARAM.real_value(t[i], t[i]) : 1.0;
    }

    if (simulation == simulation_type::BENDING) {
        M(0, 0) = 1.0;
        M(2, 2) = 8.0;
    }

    if (N > 1) {
        M(0, 1) = PARAM.real_value("INCLINE") * PARAM.real_value("LY");
    }

    /* scale the unit mesh to [LX,LY,..] and incline it */
    mesh.transformation(M);

    std::cout << "NUMBER OF POINTS : " << mesh.nb_points() << std::endl;
    std::cout << "POINTS " << mesh.convex_index().size() << std::endl;

    datafilename = PARAM.string_value("ROOTFILENAME", "Base name of data files.");
    residual = PARAM.real_value("RESIDUAL");
    if (residual == 0.)
        residual = 1e-10;

    p1 = PARAM.real_value("P1", "First Elastic coefficient");
    p2 = PARAM.real_value("P2", "Second Elastic coefficient");
    p3 = PARAM.real_value("P3", "Third Elastic coefficient");

    mf_u.set_qdim(dim_type(N));

    /* set the finite element on the mf_u */
    getfem::pfem pf_u = getfem::fem_descriptor(FEM_TYPE);
    getfem::pintegration_method ppi = getfem::int_method_descriptor(INTEGRATION);

    mim.set_integration_method(ppi);
    mf_u.set_finite_element(pf_u);

    mf_p.set_finite_element(getfem::fem_descriptor(FEM_TYPE_P));

    /* set the finite element on mf_rhs (same as mf_u is DATA_FEM_TYPE is
       not used in the .param file */
    std::string data_fem_name = PARAM.string_value("DATA_FEM_TYPE");
    if (data_fem_name.size() == 0) {
        GMM_ASSERT1(pf_u->is_lagrange(), "You are using a non-lagrange FEM"
                    ". In that case you need to set "
                    << "DATA_FEM_TYPE in the .param file");
        mf_rhs.set_finite_element(mesh.convex_index(), pf_u);
    } else {
        mf_rhs.set_finite_element(mesh.convex_index(), getfem::fem_descriptor(data_fem_name));
    }

    /* set the finite element on mf_coef. Here we use a very simple element
     *  since the only function that need to be interpolated on the mesh_fem
     * is f(x)=1 ... */
    mf_coef.set_finite_element(mesh.convex_index(), getfem::classical_fem(pgt, 0));

    /* set boundary conditions
     * (Neuman on the upper face, Dirichlet elsewhere) */
    std::cout << "Selecting Neumann and Dirichlet boundaries\n";
    getfem::mesh_region border_faces;
    getfem::outer_faces_of_mesh(mesh, border_faces);
    for (getfem::mr_visitor it(border_faces); !it.finished(); ++it) {
        assert(it.is_face());
        base_node un = mesh.normal_of_face_of_convex(it.cv(), it.f());
        un /= gmm::vect_norm2(un);
        if (gmm::abs(un[N - 1] - 1.0) < 1.0E-7) {
            if (simulation == simulation_type::BENDING) {
                // Let the beam free at one end during flexion
                mesh.region(NEUMANN_BOUNDARY_NUM).add(it.cv(), it.f());
            } else {
                mesh.region(DIRICHLET_BOUNDARY_NUM).add(it.cv(), it.f());
            }
        } else if (gmm::abs(un[N - 1] + 1.0) < 1.0E-7) {
            mesh.region(DIRICHLET_BOUNDARY_NUM).add(it.cv(), it.f());
        }
    }
}

/**************************************************************************/
/*  Model.                                                                */
/**************************************************************************/

bool elastostatic_problem::solve(plain_vector &U) {
    size_type nb_dof_rhs = mf_rhs.nb_dof();
    size_type N = mesh.dim();

    // Linearized elasticity brick.
    base_vector p(3);
    p[0] = p1;
    p[1] = p2;
    p[2] = p3;

    // Make the beam more rigid during flexion
    if (simulation == simulation_type::BENDING) {
        p[0] = 400.0;
        p[1] = 400.0;
        p[2] = 200.0;
    }

    /* choose the material law */
    size_type law_num = PARAM.int_value("LAW");
    std::string law_name;
    switch (law_num) {
        case 0:
        case 1:
            law_name = "Saint_Venant_Kirchhoff";
            p.resize(2);
            break;
        case 2:
            law_name = "Ciarlet_Geymonat";
            p.resize(3);
            break;
        case 3:
            law_name = "Incompressible_Mooney_Rivlin";
            p.resize(2);
            break;
        default:
            GMM_ASSERT1(false, "no such law");
    }

    if (N == 2) {
        std::cout << "2D plane strain hyper-elasticity\n";
        law_name = "Plane_Strain_" + law_name;
    }

    getfem::model model;

    // Main unknown of the problem (displacement).
    model.add_fem_variable("u", mf_u);

    // Nonlinear elasticity brick
    model.add_initialized_fixed_size_data("params", p);
    getfem::add_finite_strain_elasticity_brick(model, mim, law_name, "u", "params");

    // Incompressibility
    if (law_num == 1 || law_num == 3) {
        model.add_fem_variable("p", mf_p);
        getfem::add_finite_strain_incompressibility_brick(model, mim, "u", "p");
    }

    // Defining the volumic source term.
    base_vector f(N);
    f[0] = PARAM.real_value("FORCEX", "Amplitude of the gravity");
    f[1] = PARAM.real_value("FORCEY", "Amplitude of the gravity");
    if (N > 2)
        f[2] = PARAM.real_value("FORCEZ", "Amplitude of the gravity");

    if (simulation != simulation_type::BENDING)
        f[1] = 0;

    plain_vector F(nb_dof_rhs * N);
    for (size_type i = 0; i < nb_dof_rhs; ++i) {
        gmm::copy(f, gmm::sub_vector(F, gmm::sub_interval(i * N, N)));
    }

    // Volumic source term brick.
    model.add_initialized_fem_data("VolumicData", mf_rhs, F);
    getfem::add_source_term_brick(model, mim, "u", "VolumicData");

    // Dirichlet condition
    plain_vector F2(nb_dof_rhs * N);
    model.add_initialized_fem_data("DirichletData", mf_rhs, F2);
    if (PARAM.int_value("DIRICHLET_VERSION") == 0)
        add_Dirichlet_condition_with_multipliers(model, mim, "u", mf_u, DIRICHLET_BOUNDARY_NUM, "DirichletData");
    else
        add_Dirichlet_condition_with_penalization(model, mim, "u", 1E15, DIRICHLET_BOUNDARY_NUM, "DirichletData");

    scalar_type base_torsion = PARAM.real_value("TORSION", "Amplitude of the torsion");
    scalar_type base_extension = PARAM.real_value("EXTENSION", "Amplitude of the extension");

    gmm::iteration iter;
    gmm::set_traces_level(1); // To avoid some trace messages

    /* prepare the export routine for OpenDX (all time steps will be exported)
       (can be viewed with "dx -edit nonlinear_elastostatic.net")
    */
    getfem::dx_export exp(datafilename + ".dx", PARAM.int_value("VTK_EXPORT") == 1);
    getfem::stored_mesh_slice sl;
    sl.build(mesh, getfem::slicer_boundary(mesh), 8);
    exp.exporting(sl, true);
    exp.exporting_mesh_edges();
    // exp.begin_series("deformationsteps");
    exp.write_point_data(mf_u, U, "stepinit");
    exp.serie_add_object("deformationsteps");

    GMM_ASSERT1(!mf_rhs.is_reduced(), "To be adapted for reduced mesh_fems");


    int nb_step = int(PARAM.int_value("NBSTEP"));
    size_type maxit = PARAM.int_value("MAXITER");

    scalar_type shear_factor = PARAM.real_value("SHEAR");
    scalar_type base_compression = PARAM.real_value("COMPRESSION");

    // You should have the CGAL branch "gsoc2024-basic_viewer_glfw-tgrillon" to use this feature
#ifdef CGAL_BASIC_VIEWER_GLFW
    // Lambda function to make a screenshot
    auto make_screenshot = [](const LCC &lcc, const std::string &path) {
        CGAL::Graphics_scene buffer;
        add_to_graphics_scene(lcc, buffer);
        CGAL::Basic_viewer bv(buffer, "Basic viewer");
        bv.camera_orientation({1.0, 0.0, 0.0}, 360.0);
        bv.make_screenshot(std::string("./test/flexion/screenshot") + std::to_string(0) + ".png");
    };

    // First screenshot (initial state)
    mesh_interface<point3, tetrahedron> mit(mesh);
    LCC lcc = static_cast<LCC>(mit);
    make_screenshot(lcc, "./test/flexion/screenshot0.png");
#endif

    for (int step = 0; step < nb_step; ++step) {
        auto ds = (step + 1.) / static_cast<scalar_type>(nb_step);

        plain_vector DF(F);
        gmm::copy(gmm::scaled(F, ds), DF);
        gmm::copy(DF, model.set_real_variable("VolumicData"));

        if (N > 2) {
            /* Apply the gradual torsion/extension/cisaillement/compression */
            double torsion = base_torsion * ds;
            double extension = base_extension * ds;
            double shear = shear_factor * ds;
            double compression = base_compression * ds;

            base_node G(N);
            G[0] = G[1] = 0.5;
            for (size_type i = 0; i < nb_dof_rhs; ++i) {
                const base_node P = mf_rhs.point_of_basic_dof(i) - G;

                switch (simulation) {
                    case simulation_type::TORSION: {
                        scalar_type r = sqrt(P[0] * P[0] + P[1] * P[1]);
                        scalar_type theta = atan2(P[1], P[0]);
                        F2[i * N + 0] = r * cos(theta + (torsion * P[2])) - P[0];
                        F2[i * N + 1] = r * sin(theta + (torsion * P[2])) - P[1];
                        break;
                    }
                    case simulation_type::EXTENSION: {
                        F2[i * N + 2] = extension * P[2];
                        break;
                    }

                    case simulation_type::SHEAR: {
                        F2[i * N + 2] += shear * P[0];
                        break;
                    }

                    case simulation_type::COMPRESSION: {
                        F2[i * N + 2] = -compression * P[2];
                        break;
                    }
                }
            }
        }

        /* update the imposed displacement  */
        gmm::copy(F2, model.set_real_variable("DirichletData"));

        // std::cout << "step " << step << ", number of variables : " << model.nb_dof() << std::endl;
        iter = gmm::iteration(residual, int(PARAM.int_value("NOISY")), maxit ? maxit : 40000);

        /* let the non-linear solve (Newton) do its job */
        getfem::standard_solve(model, iter);

        gmm::copy(model.real_variable("u"), U);
        // char s[100]; snprintf(s, 100, "step%d", step+1);

        /* append the new displacement to the exported opendx file */
        exp.write_point_data(mf_u, U); //, s);
        exp.serie_add_object("deformationsteps");


#ifdef CGAL_BASIC_VIEWER_GLFW
        // Make a screenshot of the current state  
        LCC lcc_copy(lcc);
        auto displacements = algorithm::format_displacements(mf_u, U);
        algorithm::apply_displacements_to_lcc(lcc_copy, displacements);

        make_screenshot(lcc_copy, "./test/flexion/screenshot" + std::to_string(step + 1) + ".png");
#endif
    }

    // Solution extraction
    gmm::copy(model.real_variable("u"), U);

    return (iter.converged());
}

void elastostatic_problem::vtk_export(const plain_vector &U) {
    gmm::vecsave(datafilename + ".U", U);
    std::cout << "export to " << datafilename + ".vtk" << "..\n";
    getfem::vtk_export exp(datafilename + ".vtk", PARAM.int_value("VTK_EXPORT") == 1);
    exp.exporting(mf_u);
    exp.write_point_data(mf_u, U, "elastostatic_displacement");

    std::cout << "export done, you can view the data file with (for example)\n"
            "mayavi2 -d " << datafilename << ".vtk -f ExtractVectorNorm -f "
            "WarpVector -m Surface -m Outline\n";
}


/**************************************************************************/
/*  main program.                                                         */
/**************************************************************************/
int main(int argc, char *argv[]) {
    GETFEM_MPI_INIT(argc, argv);
    GMM_SET_EXCEPTION_DEBUG; // Exceptions make a memory fault, to debug.
    FE_ENABLE_EXCEPT; // Enable floating point exception for Nan.

    try {
        elastostatic_problem p;
        p.PARAM.read_command_line(argc, argv);
        p.init();
        p.mesh.write_to_file(p.datafilename + ".mesh");
        p.mf_u.write_to_file(p.datafilename + ".mf", true);
        p.mf_rhs.write_to_file(p.datafilename + ".mfd", true);

        // Do the simulation to calculate U (displacements vector)
        plain_vector U(p.mf_u.nb_dof());
        GMM_ASSERT1(p.solve(U), "Solve has failed");

        // Interface from GetFEM Mesh
        mesh_interface<point3, tetrahedron> mi_from_fem(p.mesh);

        // Convert to LCC
        auto lcc_copy = static_cast<LCC>(mi_from_fem);
        draw(lcc_copy);

        // Apply directly to LCC
        auto displacements = algorithm::format_displacements(p.mf_u, U);
        algorithm::apply_displacements_to_lcc(lcc_copy, displacements);
        //draw(lcc_copy);

        // Apply to the interface
        mi_from_fem.apply_displacements(p.mf_u, U);

        // Then convert to LCC
        auto lcc = static_cast<LCC>(mi_from_fem);
        //draw(lcc);

        // Interface from LCC
        mesh_interface<point3, tetrahedron> mi_from_lcc(lcc);
        draw(static_cast<LCC>(mi_from_lcc));

        if (p.PARAM.int_value("VTK_EXPORT")) {
            p.vtk_export(U);
        }
    }
    GMM_STANDARD_CATCH_ERROR;

    GETFEM_MPI_FINALIZE;

    return 0;
}
