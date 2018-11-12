import sys
input_file = sys.argv[1]
f = open(input_file, 'r')
lines = f.readlines()
current = 0
map = {}
mapid = 0
while current < len(lines):
    id = lines[current].strip().replace('#', '').strip()
    print ("t #",id)
    numv = int(lines[current+1].strip())
    vcount = 0
    for vert in lines[current+2:current+2+numv]:
        vertex = vert.strip()
        if vertex in map:
            print ("v", vcount, map[vertex])
        else:
            print ("v", vcount, mapid)
            map[vertex] = mapid
            mapid += 1

        vcount += 1
    current += (2+numv)
    nume = int(lines[current].strip())
    for edge in lines[current+1:current+1+nume]:
        print ("e", edge.strip())
    current += (1+nume)
f.close()
