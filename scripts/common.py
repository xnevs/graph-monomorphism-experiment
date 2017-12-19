import hashlib

def create_tables(con):
    with con:
        con.execute('''
            CREATE TABLE IF NOT EXISTS progs (
                id INTEGER PRIMARY KEY,
                digest BLOB NOT NULL, -- the digest of the binary
                path TEXT NOT NULL,   -- path to the binary as passed on the cmd line
                timestamp INTEGER NOT NULL DEFAULT CURRENT_TIMESTAMP, -- when the program was first inserted
                CONSTRAINT digest_unique UNIQUE (digest)
            )
        ''')
        con.execute('''
            CREATE TABLE IF NOT EXISTS results (
                id INTEGER PRIMARY KEY,
                prog INTEGER NOT NULL,  -- prog.id of the program
                pattern TEXT NOT NULL,  -- path to the pattern file as passed on the cmd line
                target TEXT NOT NULL,   -- path to the target file as passed on the cmd line
                solution INTEGER,       -- the reported solution
                execution_time INTEGER, -- the reported execution time in milliseconds
                timeout INTEGER,        -- the set timeout in seconds
                timestamp INTEGER NOT NULL DEFAULT CURRENT_TIMESTAMP, -- when the test was run
                FOREIGN KEY(prog) REFERENCES progs(id)
            )
        ''')

def get_prog(con,digest):
    prog = con.execute('SELECT * FROM progs WHERE digest=?',(digest,)).fetchone()
    return prog
def insert_prog(con,data):
    with con:
        con.execute('INSERT INTO progs (digest,path) VALUES (?,?)',data)
def get_results(con,prog_id,pattern,target):
    results = con.execute('SELECT * FROM results WHERE prog=? AND pattern=? AND target=?',(prog_id,pattern,target)).fetchall()
    return results
def insert_result(con,data):
    with con:
        con.execute('INSERT INTO results (prog,pattern,target,solution,execution_time,timeout) VALUES (?,?,?,?,?,?)',data)

def digest(x):
    h = hashlib.md5()
    h.update(x)
    return h.hexdigest()
