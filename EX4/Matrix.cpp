#include "Matrix.h"
#include <cmath>
#include <iostream>
#include <string>
#include <stdexcept>


#define DOUBLE_ASTERISK "**"
#define DOUBLE_SPACE "  "
#define LEN_MSG "The values are problematic"
#define RANGE_MSG "The index is out of range"
#define RUNTIME_MSG "The file is not valid"
#define INIT_SIZE 1
#define PROB_NUM 0.1

using std::length_error;
using std::out_of_range;
using std::runtime_error;
using std::cout;
using std::endl;
using std::sqrt;
using std::istream;
using std::ostream;
using std::ifstream;

Matrix::Matrix (int rows, int cols)
{
  if (rows <= 0 || cols <= 0)
    {
      throw length_error(LEN_MSG);
    }
  dimension.rows = rows;
  dimension.cols = cols;
  int size = rows * cols;
  values = new float[size];
  int i = 0;
  while (i < size)
    {
      values[i] = 0;
      i++;
    }
}

Matrix::Matrix ()
{
  dimension.rows = INIT_SIZE;
  dimension.cols = INIT_SIZE;
  values = new float[INIT_SIZE];
  values[0] = 0;
}

Matrix::Matrix (const Matrix &m)
{
  this->dimension.rows = m.dimension.rows;
  this->dimension.cols = m.dimension.cols;
  int size = (m.get_cols()) * (m.get_rows());
  this->values = new float[size];
  int i = 0;

  while (i < size)
    {
      this->values[i] = m.values[i];
      i ++;
    }
}

Matrix::~Matrix ()
{
  delete[] values;
}

int Matrix::get_rows () const
{
  return dimension.rows;
}

int Matrix::get_cols () const
{
  return dimension.cols;
}

Matrix& Matrix::vectorize ()
{
  int size = dimension.rows*dimension.cols;
  dimension.cols = 1;
  dimension.rows = size;
  return (*this);
}

Matrix &Matrix::transpose ()
{
  int size = dimension.rows*dimension.cols;
  auto *m_transpose = new float[size];
  int r = dimension.rows;
  int c = dimension.cols;
  for (int i = 0; i < c; i++)
    {
      for (int j = 0; j < r; j++)
        {
          m_transpose[i * r + j] = values[j * c + i];
        }
    }
  int k = 0;
  while (k < size)
    {
      values[k] = m_transpose[k];
      k++;
    }

  delete[] m_transpose;
  dimension.rows = c;
  dimension.cols = r;
  return (*this);
}

void Matrix::plain_print () const
{
  int r = dimension.rows;
  int c = dimension.cols;
  for (int i = 0; i < r; i++)
    {
      for (int j = 0; j < c; j++)
        {
          cout << values[i * c + j] << " ";
        }
      cout << endl;
    }
}

Matrix Matrix::dot (const Matrix &m)
{
  int r1 = this->dimension.rows;
  int r2 = m.dimension.rows;
  int c1 = this->dimension.cols;
  int c2 = m.dimension.cols;
  if (c1 != c2 || r1 != r2)
    {
      throw length_error (LEN_MSG);
    }
  Matrix m_dot (*this);
  for (int i = 0; i < r1; i++)
    {
      for (int j = 0; j < c2; j++)
        {
          m_dot[i * c1 + j] =
              (m.values[i * c1 + j]) * (this->values[i * c1 + j]);
        }
    }
  return m_dot;
}

float Matrix::norm () const
{
  float sum = 0;
  int i = 0;
  int size = dimension.rows*dimension.cols;

  while (i < size)
    {
      sum += (values[i]) * (values[i]);
      i++;
    }

  return sqrt (sum);
}

Matrix operator+ (const Matrix &m, const Matrix &n)
{
  int size = (n.dimension.rows)*(n.dimension.cols);
  if (n.dimension.cols != m.dimension.cols
      || n.dimension.rows != m.dimension.rows)
    {
      throw length_error (LEN_MSG);
    }
  Matrix operator_mat (n);
  int i = 0;
  while (i < size)
    {
      operator_mat.values[i] = n.values[i] + m.values[i];
      i++;
    }

  return operator_mat;
}

Matrix &Matrix::operator= (const Matrix &m)
{
  if (this == &m)
    {
      return (*this);
    }
  delete[] this->values;

  int size = (m.dimension.rows)*(m.dimension.cols);
  this->dimension.rows = m.dimension.rows;
  this->dimension.cols = m.dimension.cols;
  this->values = new float[size];

  int i = 0;
  while (i < size)
    {
      this->values[i] = m.values[i];
      i++;
    }

  return (*this);
}

Matrix &Matrix::operator+= (const Matrix &m)
{
  if (this->dimension.cols != m.dimension.cols
      || this->dimension.rows != m.dimension.rows)
    {
      throw length_error (LEN_MSG);
    }
  int size = dimension.rows*dimension.cols;
  int i = 0;
  while (i < size)
    {
      this->values[i] += m.values[i];
      i++;
    }

  return (*this);
}

Matrix Matrix::operator* (float scalar) const
{
  int size = dimension.rows*dimension.cols;
  Matrix multi (*this);
  int i = 0;
  while (i < size)
    {
      multi.values[i] = (this->values[i]) * scalar;
      i++;
    }
  return multi;
}

Matrix Matrix::operator* (const Matrix &m) const
{
  int r1 = this->dimension.rows;
  int c1 = this->dimension.cols;
  int r2 = m.dimension.rows;
  int c2 = m.dimension.cols;
  if (c1 != r2)
    {
      throw length_error (LEN_MSG);
    }
  Matrix multi_mat (r1, c2);
  for (int i = 0; i < r1; i++)
    {
      for (int j = 0; j < c2; j++)
        {
          float sum = 0;
          for (int k = 0; k < c1; k++)
            {
              sum += (this->values[i * c1 + k]) * (m.values[k * c2 + j]);
            }
          multi_mat.values[i * c2 + j] = sum;
        }
    }
  return multi_mat;
}

Matrix operator* (float scalar, const Matrix &m)
{
  return (m * scalar);
}

float Matrix::operator() (int r, int c) const
{
  if (r >= dimension.rows || c >= dimension.cols || r < 0 || c < 0)
    {
      throw out_of_range (RANGE_MSG);
    }
  return (*this)[(r) * (dimension.cols) + c];
}

float& Matrix::operator() (int r, int c)
{
  if (r >= dimension.rows || c >= dimension.cols || r < 0 || c < 0)
    {
      throw out_of_range (RANGE_MSG);
    }
  return (*this)[(r) * (dimension.cols) + c];
}

float Matrix::operator[] (int index) const
{
  int size = dimension.rows*dimension.cols;
  if (index >= size || index < 0)
    {
      throw out_of_range (RANGE_MSG);
    }
  return values[index];
}

float& Matrix::operator[] (int index)
{
  int size = dimension.rows*dimension.cols;
  if (index >= size || index < 0)
    {
      throw out_of_range (RANGE_MSG);
    }
  return values[index];
}

istream &operator>> (istream &s, Matrix &m)
{
  int size = (m.dimension.rows)*(m.dimension.cols);
  if (!s.good())
    {
      throw runtime_error (RUNTIME_MSG);
    }
  s.seekg (0, ifstream::end);
  long int total_size = size * sizeof(float);
  if(s.tellg() != total_size)
    {
      throw runtime_error (RUNTIME_MSG);
    }

  s.seekg(0, std::ios_base::beg);
  s.read ((char *) m.values, size* sizeof (float));

  return s;
}

ostream &operator<< (ostream &s, Matrix &m)
{
  for (int i = 0; i < m.dimension.rows; i++)
    {
      for (int j = 0; j < m.dimension.cols; j++)
        {
          m (i, j) > PROB_NUM ? s << DOUBLE_ASTERISK : s << DOUBLE_SPACE;
        }
      s << endl;
    }
  if (s.bad())
    {
      throw runtime_error (RUNTIME_MSG);
    }
  return s;
}


