import sys
import argparse
import subprocess
import sqlite3

from common import *

def test(prog_path,pattern,target,timeout):
    with subprocess.Popen([prog_path,pattern,target],stdout=subprocess.PIPE,universal_newlines=True) as process:
        try:
            process.wait(timeout=timeout)
            solution,execution_time = process.stdout.read().strip().split(',')
            return (solution,execution_time)
        except subprocess.TimeoutExpired:
            process.kill()
            return (None,None)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('prog',nargs='+')
    parser.add_argument('-d','--database',required=True)
    parser.add_argument('-t','--timeout',type=int,default=5)
    parser.add_argument('-r','--repeat',action='store_true',default=False)
    args = parser.parse_args()

    DATABASE_PATH = args.database
    TIMEOUT = args.timeout
    REPEAT = args.repeat

    prog_paths = args.prog

    examples = [line.split() for line in sys.stdin]

    con = sqlite3.connect(DATABASE_PATH)
    con.row_factory = sqlite3.Row
    create_tables(con)

    progs = []
    for prog_path in prog_paths:
        with open(prog_path,'rb') as prog_f:
            prog_digest = digest(prog_f.read())
        prog = get_prog(con,prog_digest)
        if prog is None:
            insert_prog(con,(prog_digest,prog_path))
            prog = get_prog(con,prog_digest)
        progs.append(prog)


    fresh_tests = []
    timeout_tests = []
    known_tests = []
    for prog in progs:
        for pattern,target in examples:
            all_cnt = con.execute(
                '''SELECT COUNT(*) FROM results
                   WHERE prog=? AND pattern=? AND target=?
                ''',
                (prog['id'],pattern,target)
            ).fetchone()[0]
            if all_cnt == 0:
                fresh_tests.append((prog,pattern,target))
            else:
                successful_cnt = con.execute(
                    '''SELECT COUNT(*) FROM results
                       WHERE prog=? AND pattern=? AND target=?
                       AND execution_time IS NOT NULL
                    ''',
                    (prog['id'],pattern,target)
                ).fetchone()[0]
                if successful_cnt > 0:
                    known_tests.append((prog,pattern,target))
                else:
                    max_timeout = con.execute(
                        '''SELECT MAX(timeout) FROM results
                           WHERE prog=? AND pattern=? AND target=?
                        ''',
                        (prog['id'],pattern,target)
                    ).fetchone()[0]
                    if max_timeout < TIMEOUT:
                        timeout_tests.append((prog,pattern,target))
                    else:
                        known_tests.append((prog,pattern,target))

    test_groups = [fresh_tests,timeout_tests]
    if REPEAT:
        test_groups.append(known_tests)

    for i,test_group in enumerate(test_groups):
        print('Running test group {}'.format(i))
        for prog,pattern,target in test_group:
            sys.stdout.write('.')
            sys.stdout.flush()
            solution,execution_time = test(prog['path'],pattern,target,TIMEOUT)
            insert_result(con,(prog['id'],pattern,target,solution,execution_time,TIMEOUT))
        if test_group:
            print()

if __name__ == '__main__':
    main()
