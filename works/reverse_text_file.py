fin = open("input.txt", 'r')
fout = open("output.txt", 'w')

for i in fin.readlines()[::-1]:
    fout.write(i)

fin.close()
fout.close()
