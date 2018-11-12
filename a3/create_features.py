import sys
import networkx as nx
from networkx.algorithms import isomorphism as iso

def create_graph_dictionary(filename):
    f = open(filename, 'r')
    data = f.readlines()
    li = 0
    graph_dict = {}
    while li < len(data):
        if len(data[li].strip()) == 0:
            li+=1
            continue
        graph_id = int(data[li].strip().split(' ')[2])
        G = nx.Graph()
        li+=1
        while li<len(data) and len(data[li].strip())!=0 and data[li][0] != 't':
            if data[li][0] == 'v': # Found a vertex
                data_list = data[li].strip().split(' ')
                G.add_node(int(data_list[1]), node_id=int(data_list[2]))
            elif data[li][0] == 'e': # Found an edge
                data_list = data[li].strip().split(' ')
                G.add_edge(int(data_list[1]), int(data_list[2]), edge_id=int(data_list[3]))
            li+=1
        graph_dict[graph_id] = G
    f.close()
    return graph_dict

def get_ids(filename):
    f = open(filename, 'r')
    ids = [ int(x.strip()) for x in f.readlines() ]
    f.close()
    return ids

graph_file = sys.argv[1]
pfile = sys.argv[2]
nfile = sys.argv[3]
frequent_graphs = sys.argv[4]

print("Generating dictionary for all graphs in dataset")
all_graphs = create_graph_dictionary(graph_file)
print("Total graphs read:", len(all_graphs))

print("Reading positive graph ids")
positive_list = [ x for x in get_ids(pfile) if (x in all_graphs) ]
print("Total positive ids:", len(positive_list))
print("Reading negative graph ids")
negative_list = [ x for x in get_ids(nfile) if (x in all_graphs) ]
print("Total negative ids:", len(negative_list))

print("Reading feature graphs")
feature_graphs = list(create_graph_dictionary(frequent_graphs).values())
feature_length = len(feature_graphs)
print("Total Features:", feature_length)

print("Generating positive graphs with features")
positive_features = {}
for graph_id in positive_list:
    f_v = []
    for feature_graph in feature_graphs:
        graph_match = iso.GraphMatcher(
                all_graphs[graph_id],
                feature_graph,
                node_match = lambda x, y: x['node_id'] == y['node_id'],
                edge_match = lambda x, y: x['edge_id'] == y['edge_id']
                )
        if(graph_match.subgraph_is_isomorphic()):
            f_v.append(1)
        else:
            f_v.append(0)
    positive_features[graph_id] = f_v

print("Generating negative graphs with features")
negative_features = {}
count = 0
for graph_id in negative_list:
    if count%100 == 0:
        print (count)
    count+=1
    f_v = []
    for feature_graph in feature_graphs:
        graph_match = iso.GraphMatcher(
                all_graphs[graph_id],
                feature_graph,
                node_match = lambda x, y: x['node_id'] == y['node_id'],
                edge_match = lambda x, y: x['edge_id'] == y['edge_id']
                )
        if(graph_match.subgraph_is_isomorphic()):
            f_v.append(1)
        else:
            f_v.append(0)
    negative_features[graph_id] = f_v

labels = [ 1 for x in range(len(positive_list)) ]
labels_neg = [ 0 for x in range(len(negative_list)) ]
labels.extend(labels_neg)
features = [ x for x in list(positive_features.values()) ]
features_neg = [ x for x in list(negative_features.values()) ]
features.extend(features_neg)

IDF = True
if IDF:
    print("Performing IDF")
    f_w = []
    total_labels = len(labels)
    for i in range(feature_length):
        freq = 0
        for ft in features:
            freq += ft[i]
        f_w.append(float(total_labels)/freq)
    for ft in features:
        for i in range(feature_length):
            ft[i] = float(ft[i]* f_w[i])

for i in range(100):
    print(features[i])
