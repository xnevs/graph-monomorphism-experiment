import hashlib
import argparse

def create_tables(con):
  with con:
    con.execute('''
        CREATE TABLE IF NOT EXISTS progs (
            id INTEGER PRIMARY KEY,
            digest BLOB NOT NULL, -- the digest of the binary
            path TEXT NOT NULL,   -- path to the binary as passed on the cmd line
            timestamp INTEGER NOT NULL DEFAULT CURRENT_TIMESTAMP, -- when the program was first inserted
            CONSTRAINT digest_unique UNIQUE (digest))''')
    con.execute('''
        CREATE TABLE IF NOT EXISTS results (
            id INTEGER PRIMARY KEY,
            prog INTEGER NOT NULL,  -- prog.id of the program
            pattern TEXT NOT NULL,  -- path to the pattern file as passed on the cmd line
            target TEXT NOT NULL,   -- path to the target file as passed on the cmd line
            solution INTEGER,       -- the reported solution
            execution_time INTEGER, -- the reported execution time in milliseconds, or -1 if crashed
            state_count INTEGER,    -- the reported search space state count, NULL if not available
            timeout INTEGER,        -- the set timeout in seconds
            timestamp INTEGER NOT NULL DEFAULT CURRENT_TIMESTAMP, -- when the test was run
            FOREIGN KEY(prog) REFERENCES progs(id))''')

def get_prog(con, digest):
  prog = con.execute('SELECT * FROM progs WHERE digest=?', (digest,)).fetchone()
  return prog

def insert_prog(con, data):
  with con:
    con.execute('INSERT INTO progs (digest,path) VALUES (?,?)', data)

def get_results(con, prog_id, pattern, target):
  results = con.execute(
      'SELECT * FROM results WHERE prog=? AND pattern=? AND target=?',
      (prog_id, pattern, target)).fetchall()
  return results

def count_results(con, prog_id, pattern, target):
  count = con.execute(
      'SELECT COUNT(*) FROM results WHERE prog=? AND pattern=? AND target=?',
      (prog_id, pattern, target)).fetchone()[0]
  return count

def count_successfull_results(con, prog_id, pattern, target):
  count = con.execute('''
      SELECT COUNT(*) FROM results
      WHERE prog=? AND pattern=? AND target=?
      AND execution_time IS NOT NULL
      AND execution_time != -1''',
      (prog_id, pattern, target)).fetchone()[0]
  return count

def get_max_timeout(con, prog_id, pattern, target):
  max_timeout = con.execute('''
      SELECT MAX(timeout) FROM results
      WHERE prog=? AND pattern=? AND target=?''',
      (prog_id, pattern, target)).fetchone()[0]
  return max_timeout
  
def insert_result(con,data):
  with con:
    con.execute(
        'INSERT INTO results (prog,pattern,target,solution,execution_time,state_count,timeout) VALUES (?,?,?,?,?,?,?)',
        data)

def digest(x):
  h = hashlib.md5()
  h.update(x)
  return h.hexdigest()

def make_argument_parser():
  parser = argparse.ArgumentParser()
  parser.add_argument('prog',nargs='+')
  parser.add_argument('-d','--database',required=True)
  parser.add_argument('-t','--timeout',type=int,default=5)
  return parser
