#include "Activation.h"

#define LEN_MSG "The values are problematic"

using std::exp;
using std::length_error;

Matrix activation::relu (const Matrix& mat)
{
  int c = mat.get_cols();
  int r = mat.get_rows();
  auto& relu_mat(const_cast<Matrix &>(mat));
  for(int i = 0; i < c*r; i ++)
    {
      relu_mat[i] < 0? relu_mat [i] = 0: relu_mat[i] = relu_mat[i];
    }
  return relu_mat;
}

Matrix activation::softmax (const Matrix& mat)
{
  int c = mat.get_cols();
  int r = mat.get_rows();
  Matrix softmax_mat(mat);
  float sum = 0;
  for(int i = 0; i < c*r; i ++)
    {
      softmax_mat[i] = exp (mat[i]);
      sum += softmax_mat[i];
    }

  softmax_mat = (1/sum)*softmax_mat;
  return softmax_mat;
}

