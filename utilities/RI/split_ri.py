import sys
import os

def split_populate(filename, path):
  with open(filename) as f:
    count = 0
    crnt_file = None
    for line in f:
      if line[0] == '#':
        if crnt_file is not None:
          crnt_file.close()
          count += 1
        new_filepath = os.path.join(path, '{:05d}.txt'.format(count))
        crnt_file = open(new_filepath, 'x')
        print(line.strip(), file=crnt_file)
      else:
        print(line.strip(), file=crnt_file)

root = sys.argv[1]

new_root = sys.argv[2]

for dirpath, dirnames, filenames in os.walk(root):
  new_path = os.path.join(new_root, dirpath[len(root):])
  os.mkdir(new_path)
  for filename in filenames:
    new_file_dirname = os.path.join(new_path, filename)
    os.mkdir(new_file_dirname)
    split_populate(os.path.join(dirpath, filename), new_file_dirname)
