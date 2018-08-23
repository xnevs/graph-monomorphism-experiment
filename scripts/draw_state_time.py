import sys
from collections import defaultdict
import argparse
import shlex
import sqlite3
import numpy as np
import matplotlib
import matplotlib.pyplot as plt

from common import *

def plot(ax, name, x, C):
  ax.plot(x, C/x, marker='o', label=name)

def plot_rule(ax, C):
  _, xmax = ax.get_xlim()
  _, ymax = ax.get_ylim()
  
  xmin = C / ymax
  
  x = np.linspace(xmin, xmax, 300)
  y = C / x
  ax.plot(x, y, color='gray', linewidth=0.5)

def parse_results(con):
  result = con.execute('''
      SELECT MIN(execution_time) AS execution_time,
             MIN(state_count) AS state_count,
             progs.id AS prog_id,
             results.pattern AS pattern,
             results.target AS target
      FROM results
      JOIN progs ON progs.id = results.prog
      GROUP BY progs.id,results.pattern,results.target''')
  return {(row['prog_id'],row['pattern'],row['target']): (row['execution_time'], row['state_count']) for row in result}

def main():
  parser = make_argument_parser()
  # TODO add argument '--since'
  parser.add_argument('-f','--file')
  args = parser.parse_args()

  DATABASE_PATH = args.database
  # TIMEOUT = args.timeout
  FILE_PATH = args.file

  prog_paths = args.prog

  examples = [shlex.split(line) for line in sys.stdin]

  con = sqlite3.connect(DATABASE_PATH)
  con.row_factory = sqlite3.Row

  progs = []
  for prog_path in prog_paths:
    with open(prog_path,'rb') as prog_f:
      prog_digest = digest(prog_f.read())
    prog = get_prog(con,prog_digest)
    progs.append(prog)
    
  results = parse_results(con)
  
  con.close()

  print(results)

  filtered_examples = []
  for pattern, target in examples:
    for prog in progs:
      print(prog['path'])
      result = results.get((prog['id'], pattern, target), None)
      if result is None or result[0] is None or result[0] < 0 or result[1] is None:  # no result or did not finish
        print('No result or timeout or crash for {0} {1} {2}. Excluded.'.format(prog['path'], pattern, target), file=sys.stderr)
        break
    else:
      filtered_examples.append((pattern, target))
  
  data = dict()
  for prog in progs:
    all_time = 0
    all_states = 0
    for pattern,target in filtered_examples:
      min_execution_time, min_state_count = results[(prog['id'],pattern,target)]
      all_time += min_execution_time
      all_states += min_state_count
    data[prog['path']] = (all_time/len(filtered_examples), all_states/len(filtered_examples))

  fig = plt.figure()
  ax = fig.add_subplot(1,1,1)
  
  for prog_path, (avg_time, avg_states) in data.items():
    plot(ax, prog_path, avg_states, avg_time)
    
  ax.autoscale()
  ax.set_xlim(xmin=0)
  ax.set_ylim(ymin=0)
  
  for prog_path, (avg_time, avg_states) in data.items():
    plot_rule(ax, avg_time)
  
  plt.legend()
  plt.show()

if __name__ == '__main__':
    main()
