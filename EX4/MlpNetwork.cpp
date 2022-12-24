#include "MlpNetwork.h"

#define FIRST_FUNC 0
#define SECOND_FUNC 1
#define THIRD_FUNC 2
#define FOURTH_FUNC 3
#define DIGIT_NUM 10
#define LEN_MSG "The values are problematic"

using std::length_error;

MlpNetwork::MlpNetwork (Matrix *weight_mat, Matrix *bias_mat)
{
  for (int i = 0; i < MLP_SIZE; ++i)
    {
      int r1 = weights_dims[i].rows;
      int r2 = bias_dims[i].rows;
      int c1 = weights_dims[i].cols;
      int c2 = bias_dims[i].cols;
      if (r1 != weight_mat[i].get_rows () || c1 != weight_mat[i].get_cols ()
          || r2 != bias_mat[i].get_rows () || c2 != bias_mat[i].get_cols ())
        {
          throw length_error (LEN_MSG);
        }
      weights[i] = weight_mat[i];
      bias[i] = bias_mat[i];
    }
}

digit MlpNetwork::operator() (Matrix &m) const
{
  Dense d1 (weights[FIRST_FUNC], bias[FIRST_FUNC], activation::relu);
  Matrix r1 = d1 (m);
  Dense d2 (weights[SECOND_FUNC], bias[SECOND_FUNC], activation::relu);
  Matrix r2 = d2 (r1);
  Dense d3 (weights[THIRD_FUNC], bias[THIRD_FUNC], activation::relu);
  Matrix r3 = d3 (r2);
  Dense d4 (weights[FOURTH_FUNC], bias[FOURTH_FUNC], activation::softmax);
  Matrix r4 = d4 (r3);

  digit final_arr[DIGIT_NUM];
  float max_digit = 0;
  int index = 0;
  for (int i = 0; i < DIGIT_NUM; ++i)
    {
      final_arr[i].value = i;
      final_arr[i].probability = r4[i];
      if (r4[i] > max_digit)
        {
          max_digit = r4[i];
          index = i;
        }
    }
  return final_arr[index];
}