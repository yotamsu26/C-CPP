#ifndef CIPHER_H
#define CIPHER_H

/// DO NOT CHANGE ANYTHING IN THIS FILE.

/**
 * Encodes the given string according to the given shift value - k.
 * @param s - given string.
 * @param k - given shift value.
 */
void encode (char s[], int k);

/**
 * Decodes the given string according to the given shift value - k.
 * @param s - given string.
 * @param k - given shift value.
 */
void decode (char s[], int k);

void lower_case_p (char *s, int k);
// this function runs on a single letter if it is lower case and change
// it (and the shifting is positive)
// k - shifting s - letter
void lower_case_n (char *s, int k);
// this function runs on a single letter if it is lower case and change
// it (and the shifting is negative)
// k - shifting s - letter
void upper_case_n (char *s, int k);
// this function runs on a single letter if it is upper case and change
// it (and the shifting is negative)
// k - shifting s - letter
void upper_case_p (char *s, int k);
// this function runs on a single letter if it is upper case and change
// it (and the shifting is positive)
// k - shifting s - letter
void p_n_k (char s[], int k, unsigned int str_len);
// runs on all the string.
// s[] - the string, k - shifting number , n - size of the string
#endif //CIPHER_H
