import sys
import tempfile
import shutil

filepath = sys.argv[1]
with open(filepath, 'r') as f:
  fd, tempfilepath = tempfile.mkstemp(dir='./')
  with open(fd, 'w') as tf:
    tf.write(next(f))
    line = next(f)
    tf.write(line)
    n = int(line.strip())
    for i in range(n):
      tf.write(next(f))
    edge_count = int(next(f).strip())
    edges = set(tuple(sorted(map(int, line.split()))) for line in f.read().splitlines()[:edge_count])
    print(len(edges), file=tf)
    for u, v in sorted(edges):
      print(u, v, file=tf)
    
shutil.move(tempfilepath, filepath)
