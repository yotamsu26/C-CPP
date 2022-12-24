#include "cipher.h"
#include <ctype.h>
#include <string.h>

/// IN THIS FILE, IMPLEMENT EVERY FUNCTION THAT'S DECLARED IN cipher.h.
#define LOWER_START 97
#define LOWER_END 122
#define LOWER_UP 123
#define LOWER_DOWN 96
#define UPPER_START 65
#define UPPER_END 90
#define UPPER_UP 91
#define UPPER_DOWN 64

// the function change the string according to the shifting value k
void encode (char s[], int k)
{
  unsigned int n = strlen (s);
  if (k == 0)
    {
      return;
    }
  else
    {
      p_n_k (s, k, n);
    }
}

// this function runs on all the different letter and change the
// according to the shifting number
void p_n_k (char s[], int k, unsigned int str_len)
{
  for (unsigned int i = 0; i < str_len; i++)
    {
      if (isalpha (s[i]) > 0)
        {
          if (islower (s[i]) != 0)
            {
              if (k > 0)
                {
                  lower_case_p (&s[i], k);
                }
              else if (k < 0)
                {
                  lower_case_n (&s[i], k);
                }
            }
          else if (isupper (s[i]) != 0)
            {
              if (k > 0)
                {
                  upper_case_p (&s[i], k);
                }
              else if (k < 0)
                {
                  upper_case_n (&s[i], k);
                }
            }
        }
    }
}

// this function runs on a single letter if it lower case and change
// it (and the shifting is negative)
void lower_case_n (char *s, int k)
{
  while (k < 0)
    {
      if (*s == LOWER_START)
        {
          *s = LOWER_END;
          k++;
        }
      else if (LOWER_START < *s && *s < LOWER_UP)
        {
          *s -=  1;
          k++;
        }
    }
}

// this function runs on a single letter if it lower case and change
// it (and the shifting is positive)
void lower_case_p (char *s, int k)
{
  while (k > 0)
    {
      if (*s == LOWER_END)
        {
          *s = LOWER_START;
          k--;
        }
      else if (LOWER_DOWN < *s && *s < LOWER_END)
        {
          *s += 1;
          k--;
        }
    }
}

// this function runs on a single letter if it is upper case and change
// it (and the shifting is positive)
void upper_case_p (char *s, int k)
{
  while (k > 0)
    {
      if (*s == UPPER_END)
        {
          *s = UPPER_START;
          k--;
        }
      else if (UPPER_DOWN < *s && *s < UPPER_END)
        {
          *s += 1;
          k--;
        }
    }
}

void upper_case_n (char *s, int k)
// this function runs on a single letter if it is upper case and change
// it (and the shifting is negative)
{
  while (k < 0)
    {
      if (*s == UPPER_START)
        {
          *s = UPPER_END;
          k++;
        }
      else if (UPPER_START < *s && *s < UPPER_UP)
        {
          *s -= 1;
          k++;
        }
    }
}

// this function clear a code by shifting it back to place.
void decode (char s[], int k)
{
  k = k * -1;
  encode (s, k);
}
