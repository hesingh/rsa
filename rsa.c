/*
Copyright 2019 MNK Labs & Consulting, LLC.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

#define STRIDE 15

void encrypt(char *plaintext, mpz_t pt, mpz_t ct, mpz_t e, mpz_t n)
{
  gmp_printf("\n%Zd len %d\n", pt, strlen(plaintext));
  mpz_import(pt, strlen(plaintext), 1, 1, 0, 0, plaintext);
  if (mpz_cmp(pt, n) > 0)
    abort();
  mpz_powm(ct, pt, e, n); // ct = pt^e mod n
}

void decrypt(mpz_t pt, mpz_t ct, mpz_t d, mpz_t n)
{
  mpz_powm(pt, ct, d, n); // pt = ct^e mod n
}

int test_crypto(char *plaintext)
{
    mpz_t n, d, e, pt, ct;

    mpz_init(pt);
    mpz_init(ct);
    mpz_init_set_str(n, "9516311845790656153499716760847001433441357", 10);
    mpz_init_set_str(e, "65537", 10);
    mpz_init_set_str(d, "5617843187844953170308463622230283376298685", 10);

    encrypt(plaintext, pt, ct, e, n);
    gmp_printf("Encoded:   %Zd\n", ct);

    decrypt(pt, ct, d, n);
    gmp_printf("Decoded:   %Zd\n", pt);

    char buffer[64];
    memset(buffer, 0, 64);
    mpz_export(buffer, NULL, 1, 1, 0, 0, pt);
    printf("As String: %s\n", buffer);

    mpz_clears(pt, ct, n, e, d, NULL);
    return 0;
}

int main(void)
{
  char *plaintext = "The honey badger and the snake are locked in mortal combat when a jackal enters the picture. The snake appears to be strangling the badger, but the mammal slips free and gets the upper hand on the python, dragging it.";
  char buf[16];
  int len = strlen(plaintext);

  // To encode larger plaintext, break it up into blocks.
  for (int i = 0; i < len; i += STRIDE) {
    if (plaintext) {
	  int plen = strlen(plaintext);
	  if (plen < STRIDE) {
		memset(buf, 0, STRIDE+1);
		memcpy(buf, plaintext, plen);
	  } else {
		memcpy(buf, plaintext, STRIDE);
		buf[STRIDE+1] = '\0';
	  }
	  test_crypto(buf);
	}
	plaintext += STRIDE;
  }
}
