/* */
#include <math.h>

int main(int argc, char** argv)
{
  (void)argv;
#ifndef ceilf
  return ((int*)(&ceilf))[argc];
#else
  (void)argc;
  return 0;
#endif
}
