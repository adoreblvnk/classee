#include "../../include/utils/str_utils.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// our version of POSIX strcasecmp (case-insensitive string comparison)
int util_strcasecmp(const char *s1, const char *s2) {
  const unsigned char *p1 = (const unsigned char *)s1;
  const unsigned char *p2 = (const unsigned char *)s2;

  while (*p1 && (tolower(*p1) == tolower(*p2))) {
    p1++;
    p2++;
  }
  return tolower(*p1) - tolower(*p2);
}

// our version of POSIX strcasestr (case-insensitive substring search)
char *util_strcasestr(const char *haystack, const char *needle) {
  if (!*needle) { return (char *)haystack; } // empty needle matches everything

  for (; *haystack; haystack++) {
    const char *h = haystack;
    const char *n = needle;
    while (tolower((unsigned char)*h) == tolower((unsigned char)*n) && *n) {
      h++;
      n++;
    }
    if (!*n) { return (char *)haystack; } // needle is exhausted, we found a match
  }
  return NULL;
}

// POSIX strsep for parsing strings
char *util_strsep(char **stringp, const char *delim) {
  char *start = *stringp; // save starting value of *stringp (an address) as value of start
  char *p;                // to find address of next delim

  if (start == NULL) { return NULL; } // reached the end or stringp is NULL

  // p points to address of 1st delim
  p = strpbrk(start, delim); // string pointer break

  if (p == NULL) {
    *stringp = NULL; // set the address of stringp to NULL to end loop
  } else {
    // replace delimiter with \0 & move *stringp to next char
    *p = '\0';
    *stringp = p + 1;
  }

  return start; // old value of *stringp aka the start
}
