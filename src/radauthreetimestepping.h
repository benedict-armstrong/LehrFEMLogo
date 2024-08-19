#ifndef NPDECODES_RADAUTHREETIMESTEPPING_RADAUTHREETIMESTEPPING_H_
#define NPDECODES_RADAUTHREETIMESTEPPING_RADAUTHREETIMESTEPPING_H_

/** @file radauthreetimestepping.h
 * @brief Based on NPDE homework RadauThreeTimestepping
 * @author Benedict Armstrong based on work by Erick Schulz and Oliver Rietmann
 * @copyright Developed at ETH Zurich
 */

#include <lf/assemble/assemble.h>
#include <lf/base/base.h>
#include <lf/geometry/geometry.h>
#include <lf/mesh/mesh.h>

#include <Eigen/Core>
#include <Eigen/SparseLU>
#include <functional>

namespace RadauThreeTimestepping {

/**
 * @brief time depedent heat source
 */
Eigen::VectorXd rhsVectorheatSource(const lf::assemble::DofHandler &dofh,
                                    double time);

/**
 * @brief solve heat equation with rhsVectorHeat source as source
 */
Eigen::VectorXd solveHeatEvolution(
    const lf::assemble::DofHandler &dofh, unsigned int m, double final_time,
    Eigen::VectorXd initial_condition,
    std::function<void(Eigen::VectorXd, int)> recorder);

/**
 * @brief This class implements a Lehrfem++ matrix provider defining a
 * LinFEMassMatrixProvider::Eval function returning the local MASS matrix for
 * linear first-order lagrange FE bases over triangular mesh (only!).
 * Integration over the triangular cells is performed using the trapezoidal
 * rule.
 */
class LinFEMassMatrixProvider {
 public:
  /**
   * @brief default constructor
   */
  explicit LinFEMassMatrixProvider() = default;

  /**
   * @brief Default implement: all cells are active
   */
  virtual bool isActive(const lf::mesh::Entity & /*cell*/) { return true; }

  /**
   * @brief Main method for computing the element vector
   * @param cell refers to current cell for which the element vector is desired
   * @returns The mass matrix corresponding to the given cell
   *
   * The implementation uses an analytic formula defined over triangular cells
   **/
  Eigen::Matrix3d Eval(const lf::mesh::Entity &tria);
};

/**
 * @brief class providing timestepping for heat equation
 */
class Radau3MOLTimestepper {
 public:
  // Disabled constructors
  Radau3MOLTimestepper() = delete;
  Radau3MOLTimestepper(const Radau3MOLTimestepper &) = delete;
  Radau3MOLTimestepper(Radau3MOLTimestepper &&) = delete;
  Radau3MOLTimestepper &operator=(const Radau3MOLTimestepper &) = delete;
  Radau3MOLTimestepper &operator=(const Radau3MOLTimestepper &&) = delete;

  // Main constructor; precomputations are done here
  Radau3MOLTimestepper(const lf::assemble::DofHandler &dofh);

  // Destructor
  virtual ~Radau3MOLTimestepper() = default;

  /* Class member functions */
  // Discrete evolution operator for Radau IIA 3rd order
  Eigen::VectorXd discreteEvolutionOperator(double time, double tau,
                                            const Eigen::VectorXd &mu) const;

 private:
  double tau_;
  const lf::assemble::DofHandler &dofh_;  // dangerous
  // Matrices in triplet format holding Galerkin matrices
  Eigen::SparseMatrix<double> A_;     // Element matrix
  Eigen::SparseMatrix<double> A_Kp_;  // Element Kronecker product matrix
  Eigen::SparseMatrix<double> M_Kp_;  // Mass Kronecker product matrix
  // Butcher tableau of the Runge-Kutta RADAU-2 method
  Eigen::Matrix2d U_;
  Eigen::Vector2d b_;
  Eigen::Vector2d c_;
  // For fixed step-size in time, the linear system of equations implicitely
  // defining the Runge-Kutta increments is independent of time. We can thus
  // precompute the LU decomposition for more efficiency.
  Eigen::SparseLU<Eigen::SparseMatrix<double>> solver_;
};

}  // namespace RadauThreeTimestepping

#endif
