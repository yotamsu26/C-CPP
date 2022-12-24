//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"

#define MLP_SIZE 4

/**
 * @struct digit
 * @brief Identified (by Mlp network) digit with
 *        the associated probability.
 * @var value - Identified digit value
 * @var probability - identification probability
 */
typedef struct digit {
	unsigned int value;
	float probability;
} digit;

const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
									{64,  128},
									{20,  64},
									{10,  20}};
const matrix_dims bias_dims[] = {{128, 1},
								 {64,  1},
								 {20,  1},
								 {10,  1}};

class MlpNetwork
{
 private:

  Matrix weights[MLP_SIZE]; // array of weights matrices.
  Matrix bias[MLP_SIZE]; // array of bias matrices.

 public:

  MlpNetwork(Matrix wei[], Matrix bi[]); //constructor
  /**
   *
   * @param m matrix.
   * @return the digit with the max probability.
   */
  digit operator()(Matrix& m) const;
};

#endif // MLPNETWORK_H