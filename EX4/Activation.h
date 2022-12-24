
#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"

typedef Matrix (*active_func) (const Matrix&); // typedef for
// activation functions

namespace activation
{
    /**
     *
     * @param m matrix.
     * @return updated matrix without negative values.
     */
    Matrix relu (const Matrix& m);
    /**
     *
     * @param m matrix.
     * @return new matrix with the probability values.
     */
    Matrix softmax (const Matrix& m);
}



#endif //ACTIVATION_H