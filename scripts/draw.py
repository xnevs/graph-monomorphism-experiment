import sys
from collections import defaultdict
import argparse
import shlex
import sqlite3
import matplotlib
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1.inset_locator import zoomed_inset_axes
from mpl_toolkits.axes_grid1.inset_locator import mark_inset

from common import *

def onclick(ax):
  def handler(event):
    for line in ax.get_lines():
      if line.contains(event)[0]:
        print(line.get_label())
    print()
    print()
  return handler

def plot(ax,name,right,times):
  count = range(1,len(times)+1)

  x = [0]
  x.extend(t for t in times for _ in (0,1))
  y = [0,0]
  y.extend(c for c in count for _ in (0,1))
  y.pop()

  label = name
  if 'RI-DS' in name:
    label = 'RI-Ds'
  elif 'RI' in name:
    label = 'RI'
  elif 'VF3' in name:
    label = 'VF3'
  elif 'glasgow' in name:
    label = 'Glasgow'
  elif 'backtracking_parent_degreeprune_adjacentconsistency_forwardcount_ind' in name:
    label = 'SICS BT+'
  elif 'lazyforwardcheckingbackjumping_low_bitset_degreeprune_ind' in name:
    label = 'SICS LFCBJ+'
  elif 'forwardchecking_bitset_mrv_degreesequenceprune_ind' in name:
    label = 'SICS FCMRV+'
  elif 'forwardchecking_bitset_mrv_degreesequenceprune_countingalldifferent_ind' in name:
    label = 'SICS FCMRV+ Hall'
    
  color = ax.plot(x,y,label=label)[0].get_color()
  if right > times[-1]:
    ax.plot([x[-1],right],[y[-1],y[-1]],color=color,linestyle='dashed',label='_'+name)

def parse_results(con):
  result = con.execute('''
      SELECT MIN(execution_time) AS execution_time,
             MIN(state_count) AS state_count,
             MAX(timeout) AS timeout,
             progs.id AS prog_id,
             results.pattern AS pattern,
             results.target AS target
      FROM results
      JOIN progs ON progs.id = results.prog
      GROUP BY progs.id,results.pattern,results.target''')
  return {(row['prog_id'],row['pattern'],row['target']): (row['execution_time'], row['state_count'], row['timeout']) for row in result}

def main():
  parser = make_argument_parser()
  # TODO add argument '--since'
  parser.add_argument('-f','--file')
  parser.add_argument('-z','--zoom',default=None)
  args = parser.parse_args()

  DATABASE_PATH = args.database
  # TIMEOUT = args.timeout
  FILE_PATH = args.file

  prog_paths = args.prog

  zoom_limits = args.zoom
  if zoom_limits is not None:
    zoom_limits = zoom_limits.split(',')
    zoom_scale = float(zoom_limits.pop(0))
    zoom_guides = int(zoom_limits.pop(0))
    zoom_limits = [int(x) for x in zoom_limits]

  examples = [shlex.split(line) for line in sys.stdin]

  con = sqlite3.connect(DATABASE_PATH)
  con.row_factory = sqlite3.Row

  progs = []
  for prog_path in prog_paths:
    if prog_path != 'null':
      with open(prog_path,'rb') as prog_f:
        prog_digest = digest(prog_f.read())
      prog = get_prog(con,prog_digest)
      progs.append(prog)
    
  results = parse_results(con)
  
  con.close()

  filtered_examples = []
  for pattern, target in examples:
    for prog in progs:
      if (prog['id'], pattern, target) not in results:
        print('No result for {0} {1} {2}. Excluded.'.format(prog['path'], pattern, target), file=sys.stderr)
        break
    else:
      filtered_examples.append((pattern, target))
  
  progs_times = dict()
  max_time = 0
  for prog in progs:
    prog_times = []
    for pattern,target in filtered_examples:
      min_execution_time, min_state_count, max_timeout = results[(prog['id'],pattern,target)]
      if min_execution_time is not None:
        if min_execution_time >= 0:
          prog_times.append(min_execution_time)
          if min_execution_time > max_time:
            max_time =  min_execution_time
        else:
          print('{} {} {} crashed.'.format(prog['path'], pattern, target), file=sys.stderr)
      else:
        print('{} {} {} takes longer than {} s.'.format(prog['path'], pattern, target, max_timeout), file=sys.stderr)
    prog_times.sort()
    progs_times[prog['path']] = prog_times

  fig = plt.figure(figsize=(4.5,4.5))
  ax = fig.add_subplot(1,1,1)
  if zoom_limits is not None:
    axins = zoomed_inset_axes(ax, zoom_scale, loc=10)
    mark_inset(ax, axins, loc1=1+zoom_guides, loc2=3+zoom_guides, fc="none", ec="0.5")

  fig.canvas.mpl_connect('button_release_event', onclick(ax))
  
  for prog_path in prog_paths:
    if prog_path == 'null':
      ax.plot([], [])
      if zoom_limits is not None:
        axins.plot([], [])
    else:
      prog_times = progs_times.get(prog_path, None)
      if prog_times:
        plot(ax,prog_path,1.01*max_time,prog_times)
        if zoom_limits is not None:
          plot(axins,prog_path,1.01*max_time,prog_times)

  ax.legend()

  if zoom_limits is not None:
    axins.set_xlim(zoom_limits[0], zoom_limits[1])
    axins.set_ylim(zoom_limits[2], zoom_limits[3])

    axins.set_xticks([])
    axins.set_yticks([])

  plt.tight_layout()
  plt.show()

if __name__ == '__main__':
    main()
