import numpy as np
import os

os.makedirs('data', exist_ok=True)

db_size, queries_size, dim = 10000, 100, 128

db = np.random.rand(db_size, dim).astype(np.float32)
queries = np.random.rand(queries_size, dim).astype(np.float32)

db.tofile('data/baza.bin')
queries.tofile('data/zapytania.bin')
print("Dane wygenerowane w folderze data/")