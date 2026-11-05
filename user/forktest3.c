// Test that fork fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NUM_PROCESS 10

struct procstats {
  int arrival;
  int first_run;
  int duration;
  int completion;
  int context_switches;
};
void
print(const char *s)
{
  write(1, s, strlen(s));
}

void
forktest3(void)
{
  int pids[NUM_PROCESS];
  int workload[NUM_PROCESS] = {10000000, 1000,  500000000, 0,        1240,
                               843,      21111, 4320,      10010000, 32};
  for (int n = 0; n < NUM_PROCESS; n++) {
    pids[n] = fork();
    if (pids[n] < 0) {
      print("Fork Failed\n");
      break;
    }
    if (pids[n] == 0) {
      for (int m = 0; m < workload[n]; m++) {
      }
      print("Child done\n");
      exit(0);
    }
  }
  pause(100);
  struct procstats results[10]; // = malloc(sizeof(struct procstats));

  int fd = open("logfile.txt", -1);
  if (fd < 0) {
    exit(-1); // bad
  }
  for (int i = 0; i < NUM_PROCESS; i++) {
    childstats(pids[i], &results[i]);
    wait(&pids[i]);
    struct procstats result = results[i];
    fprintf(
      fd,
      "arrival=%d first_run=%d duration=%d completion=%d context_switches=%d\n",
      result.arrival, result.first_run, result.duration, result.completion,
      result.context_switches);
  }
  close(fd);
  return;
}

int
main(void)
{
  forktest3();
  exit(0);
}
