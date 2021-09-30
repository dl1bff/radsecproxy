/* Copyright (c) 2011, NORDUnet A/S */
/* See LICENSE for licensing information. */

#ifdef SYS_SOLARIS9
#include <sys/inttypes.h>
#else
#include <stdint.h>
#endif

#include "tlv-rewrite.h"

int radsecproxy_main(int argc, char **argv);

int main(int argc, char **argv)
{
  tlv_rewrite_opendb();
  return radsecproxy_main(argc, argv);
}

/* Local Variables: */
/* c-file-style: "stroustrup" */
/* End: */
