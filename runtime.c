#include <stdio.h>
#include <stdlib.h>
#include <bottomert.h>
#include <string.h>
#include <stdint.h>

/* Oat Internal Functions --------------------------------------------------- */

int64_t* oat_alloc_array (int64_t size) {
  bottomert (size >= 0);
  int64_t *arr = (int64_t*)calloc(size+1, sizeof(int64_t));
  arr[0] = size;
  return arr;
}

/* Oat Builtin Functions ---------------------------------------------------- */

int64_t* array_of_string (char *str) {
  int64_t len, i, *arr;

  bottomert (NULL != str);

  len = strlen(str);
  bottomert (len >= 0);

  arr = (int64_t*)malloc(sizeof(int64_t) * (len+1));
  arr[0] = len;
  for (i=0; i<len; i++) {
    arr[i+1]=(int64_t)str[i];
  }

  return arr; 
}

char* string_of_array (int64_t *arr) {
  int64_t len, i;
  char *str;

  bottomert (NULL != arr);

  len = arr[0];
  bottomert (len >= 0);

  str = malloc(sizeof(char) * (len+1));
  
  for (i=0; i<len; i++) {
    str[i] = (char)arr[i+1];
    bottomert (0 != str[i]);
  }
  str[len] = 0;

  return str;
}

int64_t length_of_string (char *str) {
  bottomert (NULL != str);
  return strlen(str);
}

char* string_of_int(int64_t i) {
  static char buf[128];
  static int len;
  len = sprintf(buf,"%ld",(long)i);
  char* str = (char*) malloc(sizeof(char) * (len + 1));
  memcpy(str, buf, len);
  str[len] = 0;
  return (char*)str;
}

char* string_cat(char* l, char* r) {
  size_t ll = strlen(l);
  size_t lr = strlen(r);
  char* new = (char*) malloc(sizeof(char) * (ll + lr + 1));
  memcpy(new, l, ll);
  memcpy(new + ll, r, lr);
  new[ll + lr] = 0;
  return new;
}

void print_string (char* str) {
  bottomert (NULL != str);
  printf ("%s", str);
}

void print_int (int64_t i) {
  printf ("%ld", (long)i);
}

void print_bool (int64_t i) {
  if (i == 0) {
    printf ("false");
  } else {
    printf ("true");
  }
}

extern int64_t program(int64_t argc, int64_t* oargv);

/* 
 * Convert the argv array into an Oat array of 
 * type string[]
 * Invoke the Oat 'program' entry point after
 * initializing the global variables.
 * Prints the results of the Oat program call 
 * to the terminal.
 */
int main(int argc, char* argv[]) {
  int64_t *oargv, i, result;

  oargv = oat_alloc_array(argc); 

  /* Copy the string pointers to the correct places. */
  for (i=0; i<argc; i++){
    oargv[i+1] = (int64_t)argv[i];
  }

  /* Call the initialization code. */
  result = program(argc, oargv);
  return result;
}
