
#include "Dense.h"

#define LEN_MSG "The values are problematic"

using std::length_error;

Dense::Dense (const Matrix& we, const Matrix& bi, active_func ActiveFunc)
{
  weights = we;
  bias = bi;
  ActiveFunction = ActiveFunc;
}

Matrix Dense::get_weights () const
{
  return weights;
}

Matrix Dense::get_bias () const
{
  return bias;
}

active_func Dense::get_activation() const
{
  return ActiveFunction;
}

Matrix Dense::operator() (const Matrix &m)
{
  Matrix layer_matrix = ActiveFunction(weights*m + bias);
  return layer_matrix;
}