#!/usr/bin/env python3
import re
import sys

WORKLOAD = [10_000_000, 1_000, 500_000_000, 0, 1_240,
            843, 21_111, 4_320, 10_010_000, 32]

PATTERN = re.compile(
    r"arrival=(\d+)\s+first_run=(\d+)\s+duration=(\d+)\s+"
    r"completion=(\d+)\s+context_switches=(\d+)"
)

def main(path):
    rows = []

    with open(path) as f:
        for line_number, line in enumerate(f, 1):
            match = PATTERN.search(line)
            if match:
                arrival, first_run, duration, completion, switches = map(
                    int, match.groups()
                )
                rows.append((arrival, first_run, duration, completion, switches))
            elif line.strip():
                print(f"Skipping unrecognized line {line_number}: {line.rstrip()}",
                      file=sys.stderr)

    if len(rows) != len(WORKLOAD):
        print(f"Found {len(rows)} records but expected {len(WORKLOAD)}.",
              file=sys.stderr)
        sys.exit(1)

    print("PID  Workload  Runtime  Response  Turnaround  Waiting  Switches  Run%")
    for i, ((arrival, first_run, duration, completion, switches), workload) \
            in enumerate(zip(rows, WORKLOAD), 1):
        response = first_run - arrival
        turnaround = completion - arrival
        waiting = turnaround - duration
        run_pct = 100 * duration / turnaround if turnaround else 0

        print(f"{i:>3}  {workload:>8}  {duration:>7}  {response:>8}  "
              f"{turnaround:>10}  {waiting:>7}  {switches:>8}  "
              f"{run_pct:>5.1f}%")

if __name__ == "__main__":
    main(sys.argv[1] if len(sys.argv) > 1 else "logfile.txt")

