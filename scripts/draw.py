import sys
import argparse
import sqlite3
import matplotlib
import matplotlib.pyplot as plt

from common import *

def plot(ax,name,right,times):
    count = range(1,len(times)+1)

    x = [0]
    x.extend(t for t in times for _ in (0,1))
    y = [0,0]
    y.extend(c for c in count for _ in (0,1))
    y.pop()
    
    color = ax.plot(x,y,label=name)[0].get_color()
    if right > times[-1]:
        ax.plot([x[-1],right],[y[-1],y[-1]],color=color,linestyle='dashed')

def main():
    parser = make_argument_parser()
    # TODO add argument '--since'
    parser.add_argument('-f','--file')
    args = parser.parse_args()

    DATABASE_PATH = args.database
    # TIMEOUT = args.timeout
    FILE_PATH = args.file

    prog_paths = args.prog

    examples = [line.split() for line in sys.stdin]

    con = sqlite3.connect(DATABASE_PATH)
    con.row_factory = sqlite3.Row

    progs = []
    for prog_path in prog_paths:
        with open(prog_path,'rb') as prog_f:
            prog_digest = digest(prog_f.read())
        prog = get_prog(con,prog_digest)
        progs.append(prog)

    progs_times = dict()
    max_time = 0
    for prog in progs:
        prog_times = []
        for pattern,target in examples:
            min_execution_time,max_timeout = con.execute(
                '''SELECT MIN(execution_time),MAX(timeout)
                   FROM results
                   WHERE prog=? AND pattern=? AND target=?
                ''',
                (prog['id'],pattern,target)
            ).fetchone()
            if min_execution_time is not None:
                prog_times.append(min_execution_time)
                if min_execution_time > max_time:
                    max_time =  min_execution_time
            elif max_timeout is not None:
                print('{} {} {} takes longer than {} s.'.format(prog['path'],pattern,target,max_timeout), file=sys.stderr)
            else:
                print('No result for {} {} {}'.format(prog['path'],pattern,target), file=sys.stderr)
        prog_times.sort()
        progs_times[prog['path']] = prog_times

    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
    for prog_path,prog_times in progs_times.items():
        if prog_times:
            plot(ax,prog_path,1.01*max_time,prog_times)
    plt.legend()
    plt.show()



if __name__ == '__main__':
    main()
