import sys

with open(sys.argv[1]) as f1:
  f1_lines = list(map(lambda s: s.strip(), f1.read().splitlines()))
  with open(sys.argv[2]) as f2:
    for f2_line in f2:
      for f1_line in f1_lines:
        print(f1_line, f2_line.strip())
