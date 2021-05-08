import numpy as np

rec = np.array([np.inf, np.inf, np.inf])
in_file = open("bunny.obj", "r")
out_file = open("bunny.rot.obj", "w")
line = in_file.readline()
out_file.write(line)

while True:
    line = in_file.readline()
    if not line:
        break
    if not line.startswith("f"):
        out_file.write(line)
        continue
    splits = line.split()
    print(splits)
    out_file.write(' '.join([splits[0], splits[1], splits[3], splits[2]]) + '\n')

in_file.close()
out_file.close()