#include "cipher.h"
#include "stdio.h"
#include "tests.h"
#include "string.h"
#include "stdlib.h"

#define TEST "test"
#define ENCODE "encode"
#define DECODE "decode"
#define CASE_1 5
#define CASE_2 2
#define BUFFER 1023
#define SHIFTING 2
#define COMMAND 1
#define FILE_IN 3
#define FILE_OUT 4
#define DEMICAL 10

int test_checker ();

int test_checker ()
{
  // does all the test from the test.c file and collects the results.
  // return 1 if something file and 0 if all been succeed
  int test_1 = test_encode_cyclic_upper_case_positive_k ();
  int test_2 = test_encode_non_cyclic_lower_case_positive_k ();
  int test_3 = test_encode_cyclic_lower_case_special_char_positive_k ();
  int test_4 = test_encode_non_cyclic_lower_case_special_char_negative_k ();
  int test_5 = test_encode_cyclic_lower_case_negative_k ();
  int test_6 = test_decode_non_cyclic_lower_case_positive_k ();
  int test_7 = test_decode_cyclic_lower_case_negative_k ();
  int test_8 = test_decode_cyclic_upper_case_positive_k ();
  int test_9 = test_decode_cyclic_lower_case_special_char_positive_k ();
  int test_10 = test_decode_non_cyclic_lower_case_special_char_negative_k ();
  int tests_check =
      test_1 + test_2 + test_3 + test_4 + test_5 + test_6 + test_7 + test_8 +
      test_9 + test_10;

  if (tests_check > 0)
    {
      return 1;
    }
  return 0;
}

int main (int argc, char *argv[])
// the first argument is the name of the function.
// the second is the value of the shifting.
// the third is the name of the source file.
// the fourth is the name of the output file.
{
  if (argc != CASE_1 && argc != CASE_2)
    {
      fprintf (stderr, "The program receives 1 or 4 arguments only.\n");
      return EXIT_FAILURE;
    }
  else if (argc == CASE_2 && strcmp (argv[COMMAND], TEST) != 0)
    {
      fprintf (stderr, "Usage: cipher test\n");
      return EXIT_FAILURE;
    }
  else if (argc == CASE_1 && (strcmp (ENCODE, argv[COMMAND]) != 0)
           && strcmp (DECODE, argv[COMMAND]) != 0)
    {
      fprintf (stderr, "The given command is invalid.\n");
      return EXIT_FAILURE;
    }

  if (argc == CASE_1)
    {
      char *k_modelu;
      int k = strtol (argv[SHIFTING], &k_modelu, DEMICAL);
      //decimal base
      //checking the k value if valid
      if (strcmp ("", k_modelu) != 0)
        {
          fprintf (stderr, "The given shift value is invalid.\n");
          return EXIT_FAILURE;
        }
      FILE *uncoded_file = fopen (argv[FILE_IN], "r");
      FILE *coded_file = fopen (argv[FILE_OUT], "w");
      if (uncoded_file == 0)
        {
          fclose (coded_file);
          fprintf (stderr, "The given file is invalid.\n");
          return EXIT_FAILURE;
        }
      // when the cml is equal to 5 and valid
      char single_char[BUFFER]; // 2 - is the buffer size
      while (fgets (single_char, BUFFER, uncoded_file))
        {
          if (strcmp (ENCODE, argv[COMMAND]) == 0)
            {
              encode (single_char, k);
            }
          if (strcmp (DECODE, argv[COMMAND]) == 0)
            {
              decode (single_char, k);
            }
          fprintf (coded_file, "%s", single_char);
        }
      fclose (coded_file);
      fclose (uncoded_file);
      return EXIT_SUCCESS;
    }
  // when the cml is equal to 2 and valid
  int result = test_checker ();
  if (result == 1)
    {
      return EXIT_FAILURE;
    }
  else
    {
      return EXIT_SUCCESS;
    }
}

