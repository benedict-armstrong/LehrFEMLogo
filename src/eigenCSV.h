#ifndef EIGEN_CSV_H_
#define EIGEN_CSV_H_

#include <Eigen/Dense>
#include <fstream>
#include <iostream>

namespace EigenCSV {

Eigen::MatrixXd readCSV(std::string file_name, int rows, int cols);
void writeToCSVfile(std::string file_name, Eigen::MatrixXd matrix);

}  // namespace EigenCSV

#endif