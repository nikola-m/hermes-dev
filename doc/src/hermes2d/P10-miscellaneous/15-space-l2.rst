Space L2 (15-space-l2)
----------------------

**Git reference:** Tutorial example `15-space-l2 <http://git.hpfem.org/hermes.git/tree/HEAD:/hermes2d/tutorial/P10-miscellaneous/15-space-l2>`_. 

We already saw the $L^2$ space in the `Navier-Stokes example 
<http://hpfem.org/hermes/doc/src/hermes2d/timedep/navier-stokes.html>`_ where 
it was used for pressure to keep the velocity discreetely divergence-free. This example 
shows how to create an $L^2$ space, visualize 
finite element basis functions, and perform an orthogonal $L^2$-projection of a continuous 
function onto the FE space::

    const int INIT_REF_NUM = 1;                       // Number of initial uniform mesh refinements.
    const int P_INIT = 3;                             // Polynomial degree of mesh elements.
    MatrixSolverType matrix_solver = SOLVER_UMFPACK;  // Possibilities: SOLVER_AMESOS, SOLVER_AZTECOO, SOLVER_MUMPS,
                                                      // SOLVER_PETSC, SOLVER_SUPERLU, SOLVER_UMFPACK.
    // Projected function.
    scalar F(double x, double y, double& dx, double& dy)
    {
      return - pow(x, 4) * pow(y, 5); 
      dx = 0; // not needed for L2-projection
      dy = 0; // not needed for L2-projection
    }

    int main(int argc, char* argv[])
    {
      // Load the mesh.
      Mesh mesh;
      H2DReader mloader;
      mloader.load("square.mesh", &mesh);

      // Perform uniform mesh refinements.
      for (int i = 0; i<INIT_REF_NUM; i++) mesh.refine_all_elements();

      // Enter boundary markers (default is Neumann boundary).
      BCTypes bc_types;

      // Enter Dirichlet boundary values (not relevant here).
      BCValues bc_values;

      // Create an L2 space with default shapeset.
      L2Space space(&mesh, &bc_types, &bc_values, P_INIT);

      // View basis functions.
      BaseView bview("BaseView", new WinGeom(0, 0, 600, 500));
      bview.show(&space);
      // View::wait(H2DV_WAIT_KEYPRESS);

      // Assemble and solve the finite element problem.
      WeakForm wf_dummy;

      // Initialize the exact and projected solution.
      Solution sln;
      Solution sln_exact(&mesh, F);

      OGProjection::project_global(&space, &sln_exact, &sln, matrix_solver, HERMES_L2_NORM);

      // Visualize the solution.
      ScalarView view1("Projection", new WinGeom(610, 0, 600, 500));
      view1.show(&sln);

      // Wait for all views to be closed.
      View::wait();
      return 0;
    }

Sample basis functions:

.. image:: 15-space-l2/fn0.png
   :align: center
   :width: 400
   :alt: Sample basis function

.. image:: 15-space-l2/fn1.png
   :align: center
   :width: 400
   :alt: Sample basis function

.. image:: 15-space-l2/fn2.png
   :align: center
   :width: 400
   :alt: Sample basis function

.. image:: 15-space-l2/fn3.png
   :align: center
   :width: 400
   :alt: Sample basis function

The projection (note that this is a discontinuous function):

.. image:: 15-space-l2/sol.png
   :align: center
   :width: 400
   :alt: Projection
