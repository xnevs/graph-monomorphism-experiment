import sys
import argparse
import shlex
import subprocess
import sqlite3

from common import *

def test(prog_path,pattern,target,timeout):
  with subprocess.Popen([prog_path,pattern,target],stdout=subprocess.PIPE,stderr=subprocess.PIPE,universal_newlines=True) as process:
    try:
      process.wait(timeout=timeout)
      output = process.stdout.read()
      errput = process.stderr.read()
      if process.returncode == 0:
        lines = output.strip().splitlines()
        solution, execution_time = lines[0].strip().split(',')
        state_count = lines[1].strip() if len(lines) > 1 else None
        return {
            'status': 'OK',
            'solution': solution,
            'execution_time': execution_time,
            'state_count': state_count}
      else:
        return {
            'status': 'ERROR',
            'returncode': process.returncode,
            'stdout': output,
            'stderr': errput}
    except subprocess.TimeoutExpired:
      process.kill()
      return {'status': 'TIMEOUT'}

def should_run(con, prog_id, pattern, target, timeout, repeat):
  all_cnt = count_results(con, prog_id, pattern, target)
  if all_cnt == 0:
    return True
  else:
    successfull_cnt = count_successfull_results(con, prog_id, pattern, target)
    if successfull_cnt > 0:
      return repeat
    else:
      max_timeout = get_max_timeout(con, prog_id, pattern, target)
      if max_timeout < timeout:
        return True
      else:
        return repeat

def main():
  parser = make_argument_parser()
  parser.add_argument('-r','--repeat',action='store_true',default=False)
  args = parser.parse_args()

  DATABASE_PATH = args.database
  TIMEOUT = args.timeout
  REPEAT = args.repeat

  prog_paths = args.prog

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

  for line in sys.stdin:
    pattern, target = shlex.split(line)
    for prog in progs:
      if should_run(con, prog['id'], pattern, target, TIMEOUT, REPEAT):
        result = test(prog['path'], pattern, target, TIMEOUT)
        print(result)
        if result['status'] == 'OK':
          insert_result(con, (prog['id'], pattern, target, result['solution'], result['execution_time'], result['state_count'], TIMEOUT))
        elif result['status'] == 'TIMEOUT':
          insert_result(con, (prog['id'], pattern, target, None, None, None, TIMEOUT))
        elif result['status'] == 'ERROR':
          insert_result(con, (prog['id'], pattern, target, None, -1, TIMEOUT))
      else:
        print('SKIPPED')

if __name__ == '__main__':
  main()
