import sys
import os

def split_populate(filename, path):
  with open(filename) as f:
    crnt_file = None
    for line in f:
      if line[0] == '#':
        if crnt_file is not None:
          crnt_file.close()
        new_filepath = os.path.join(path, '{}.txt'.format(line[1:].strip()))
        crnt_file = open(new_filepath, 'x')
        print(line.strip(), file=crnt_file)
      else:
        print(line.strip(), file=crnt_file)

filename = sys.argv[1]
dirname = sys.argv[2]
os.mkdir(dirname)
split_populate(filename, dirname)
