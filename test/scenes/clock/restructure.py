in_file = open("Clock_obj.obj", "r")
out_file = open("Clock.obj", "w")

for line in in_file.readlines():
    line = line.strip()
    if line.startswith("f "):
        splits = line.split()
        out_file.write(' '.join(splits[:-1]) + '\n')
        out_file.write(' '.join(splits[:2] + splits[3:]) + '\n')
    else:
        out_file.write(line + "\n")

in_file.close()
out_file.close()