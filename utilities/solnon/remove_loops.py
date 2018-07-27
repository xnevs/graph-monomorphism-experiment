import sys
import tempfile
import shutil

filepath = sys.argv[1]
with open(filepath, 'r') as f:
  fd, tempfilepath = tempfile.mkstemp()
  with open(fd, 'w') as tf:
    tf.write(next(f))

    for u, line in enumerate(f):
      adj = [v for v in line.split()[1:] if int(v) != u]
      print(len(adj), ' '.join(adj), file=tf)

shutil.move(tempfilepath, filepath)
