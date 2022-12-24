#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"


class Dense
{
 private:

    Matrix weights; // weight matrix.
    Matrix bias; // bia matrix.
    active_func ActiveFunction; // relu or softmax function.

 public:

  /**
   * dense constructor.
   * @param we weight matrix.
   * @param bi bia matrix.
   * @param ActiveFunc relu or softmax function.
   */
    Dense(const Matrix& we, const Matrix& bi, active_func ActiveFunc);
    Matrix get_weights() const;
    Matrix get_bias() const;
    active_func get_activation() const;

    /**
     *
     * @param m matrix by reference.
     * @return updated matrix according to the relevant matrices.
     */
    Matrix operator()(const Matrix& m);
};


#endif //DENSE_H
