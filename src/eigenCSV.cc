#include "eigenCSV.h"

#include <Eigen/Dense>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iostream>

namespace EigenCSV {

using boost::lexical_cast;

Eigen::MatrixXd readCSV(std::string file_name, int rows, int cols) {
  std::ifstream in(file_name);
  std::string line;
  int row = 0;
  int col = 0;
  Eigen::MatrixXd res = Eigen::MatrixXd(rows, cols);
  if (in.is_open()) {
    while (std::getline(in, line)) {
      char *ptr = (char *)line.c_str();
      int len = line.length();
      col = 0;
      char *start = ptr;
      for (int i = 0; i < len; i++) {
        if (ptr[i] == ',') {
          res(row, col++) = atof(start);
          start = ptr + i + 1;
        }
      }
      res(row, col) = atof(start);
      row++;
    }
    in.close();
  }
  return res;
}

void writeToCSVfile(std::string file_name, Eigen::MatrixXd matrix) {
  std::ofstream file(file_name.c_str());

  for (int i = 0; i < matrix.rows(); i++) {
    for (int j = 0; j < matrix.cols(); j++) {
      std::string str = lexical_cast<std::string>(matrix(i, j));
      if (j + 1 == matrix.cols()) {
        file << str;
      } else {
        file << str << ',';
      }
    }
    file << '\n';
  }
}
}  // namespace EigenCSV