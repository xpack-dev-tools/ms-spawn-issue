/*
 * This file is part of the xPack project.
 *   (https://xpack.github.io)
 * Copyright (c) 2021 Liviu Ionescu.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>

// ----------------------------------------------------------------------------

// Print all environment variables.

int main(int argc, char *argv[], char *envp[])
{
  char **p;
  int i;

  printf("\nDump process environment:\n");
  
  for (i = 0, p = envp; *p; p++, i++)
  {
    printf("envp[%d]='%s'\n", i, *p);
  }

  for (i = 0, p = argv; *p; p++, i++)
  {
    printf("argv[%d]='%s'\n", i, *p);
  }

  return 0;
}

// ----------------------------------------------------------------------------
