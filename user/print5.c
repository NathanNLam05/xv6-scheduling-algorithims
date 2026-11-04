#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#include <stdarg.h>

static void
print5()
{
  // print5 to stdout
  char five = '5';
  write(0, &five, 1);
  char newline = '\n';
  write(0, &newline, 1);
}

int
main(int argc, char *argv[])
{
  print5();
  return 0;
}
