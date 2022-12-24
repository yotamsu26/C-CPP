// Matrix.h
#include <iostream>
#include <string>
#include <stdexcept>
#include <cmath>
#include <fstream>


#ifndef MATRIX_H
#define MATRIX_H

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
	int rows, cols;
} matrix_dims;

class Matrix
{
 private:

  matrix_dims dimension{};
  float *values;

 public:

  //// constructors.
  Matrix(int rows, int cols);
  Matrix();
  Matrix(const Matrix& m);
  ~Matrix();

  int get_rows() const;
  int get_cols() const;
  /**
   *
   * @return one row matrix.
   */
  Matrix& vectorize();
  /**
   *
   * @return transposed matrix.
   */
  Matrix& transpose();
  /**
   * print the current matrix.
   */
  void plain_print() const;
  /**
   *
   * @param m matrix.
   * @return multiple 2 matrix by indexes. Aij X Bij.
   */
  Matrix dot(const Matrix& m);
  /**
   *
   * @return the distance of the matrix from 0.
   */
  float norm() const;

  //// operators between two matrices.
  friend Matrix operator+(const Matrix& m, const Matrix& n);
  Matrix& operator=(const Matrix& m);
  Matrix& operator+=(const Matrix& m);
  Matrix operator*(const Matrix& m) const;
  Matrix operator*(float scalar) const;
  friend Matrix operator*(float scalar,const Matrix& m);
  float operator()(int r, int c) const;
  float operator[](int index) const;
  float& operator()(int r, int c);
  float& operator[](int index);

  //// stream operators.
  friend std::istream& operator>>(std::istream& s, Matrix& m);
  friend std::ostream& operator<<(std::ostream& s, Matrix& m);

};

#endif //MATRIX_H