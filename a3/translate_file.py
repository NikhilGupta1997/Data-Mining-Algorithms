import sys
input_files = []
for i in range(len(sys.argv)-1):
    input_files.append(sys.argv[i+1])
map = {}
mapid = 0
for input_file in input_files:
    f = open(input_file, 'r')
    f_out = open((input_file+'.translated'), 'w')
    lines = f.readlines()
    current = 0
    while current < len(lines):
        id = lines[current].strip().replace('#', '').strip()
        f_out.write("t # "+str(id)+'\n')
        numv = int(lines[current+1].strip())
        vcount = 0
        for vert in lines[current+2:current+2+numv]:
            vertex = vert.strip()
            if vertex in map:
                f_out.write("v "+str(vcount)+' '+str(map[vertex])+'\n')
            else:
                f_out.write("v "+str(vcount)+' '+str(mapid)+'\n')
                map[vertex] = mapid
                mapid += 1

            vcount += 1
        current += (2+numv)
        nume = int(lines[current].strip())
        for edge in lines[current+1:current+1+nume]:
            f_out.write("e "+ edge.strip()+'\n')
        current += (1+nume)
    f.close()
    f_out.close()
