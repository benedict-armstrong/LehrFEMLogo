#include <lf/assemble/assemble.h>
#include <lf/io/io.h>
#include <lf/mesh/hybrid2d/hybrid2d.h>
#include <lf/mesh/utils/utils.h>
#include <lf/uscalfe/uscalfe.h>

#include <Eigen/Core>
#include <iostream>
#include <memory>

#include "eigenCSV.h"
#include "radauthreetimestepping.h"

using namespace RadauThreeTimestepping;

int main(int argc, char** argv) {
  int x_dim = 200;
  int y_dim = 75;

  /* Solving the parabolic heat equation */
  // Create a Lehrfem++ square tensor product mesh
  lf::mesh::utils::TPTriagMeshBuilder builder(
      std::make_unique<lf::mesh::hybrid2d::MeshFactory>(2));

  // Set mesh parameters following the Builder pattern
  // Domain is the unit square
  builder.setBottomLeftCorner(Eigen::Vector2d{0, 0})
      .setTopRightCorner(Eigen::Vector2d{x_dim, y_dim})
      .setNumXCells(x_dim - 1)  // one less than image size in both dimensions
      .setNumYCells(y_dim - 1);

  auto mesh_p = builder.Build();

  // Generate the linear lagrange FE data
  // Finite element space
  auto fe_space =
      std::make_shared<lf::uscalfe::FeSpaceLagrangeO1<double>>(mesh_p);
  // Obtain local->global index mapping for current finite element space
  const lf::assemble::DofHandler& dofh{fe_space->LocGlobMap()};
  // Dimension of finite element space
  const lf::uscalfe::size_type N_dofs(dofh.NumDofs());

  //   Eigen::MatrixXd mapping = Eigen::MatrixXd::Ones(30, 76);

  //   for (auto* node : (*mesh_p).Entities(2)) {
  //     auto index = mesh_p->Index(*node);
  //     auto coords = lf::geometry::Corners(*node->Geometry());
  //     auto x = (int)coords(0);
  //     auto y = (int)coords(1);

  //     mapping(y, x) = index;
  //   }
  //   EigenCSV::writeToCSVfile("mapping.csv", mapping);

  std::cout << "Numer of Dof: " << N_dofs << std::endl;

  Eigen::VectorXd initial_condition =
      EigenCSV::readCSV("initial_condition.csv", N_dofs, 1);

  //   Solve heat evolution with zero initial and boundary
  //       conditions
  double final_time = 1.0;
  unsigned int m = 30;
  auto rec = [m, x_dim, y_dim](Eigen::VectorXd x, int i) -> void {
    EigenCSV::writeToCSVfile(fmt::format("out/solution_{}.csv", i),
                             x.reshaped<Eigen::RowMajor>(y_dim, x_dim));
    std::cout << "timestep: " << i << "/" << m << std::endl;
  };

  Eigen::VectorXd discrete_heat_solution =
      solveHeatEvolution(dofh, m, final_time, initial_condition, rec);

  return 0;
}
