import sys
import networkx as nx
from networkx.algorithms import isomorphism as iso
from sklearn import svm
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.metrics import f1_score
from subprocess import call

def create_all_graph_dictionary(filename, pos_set, neg_set, outfile):
    f = open(filename, 'r')
    data = f.readlines()
    li = 0
    graph_dict = {}
    pos_list = []
    neg_list = []
    while li < len(data):
        if len(data[li].strip()) == 0:
            li+=1
            continue
        graph_id = int(data[li].strip().split(' ')[2])
        if graph_id in pos_set:
            pos_list.append(graph_id)
            start = li
        elif graph_id in neg_set:
            neg_list.append(graph_id)
            start = li
        else:
            li+=1
            while li<len(data) and len(data[li].strip())!=0 and data[li][0] != 't':
                li+=1
            continue

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
        for line in data[start:li]:
            outfile.write(line)
        graph_dict[graph_id] = G
    f.close()
    return [graph_dict, pos_list, neg_list]

def create_frequent_graph_dictionary(filename):
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

graph_file = sys.argv[1].strip()
pfile = sys.argv[2].strip()
nfile = sys.argv[3].strip()
support = sys.argv[4].strip()
frequent_graphs = graph_file+'.temp.fp'
current_dir='/'.join(sys.argv[0].split('/')[:-1])
if current_dir == '':
    current_dir = '.'
print("Reading positive graph ids")
positive_set = set(get_ids(pfile))
print("Reading negative graph ids")
negative_set = set(get_ids(nfile))

print("Generating dictionary for all graphs in dataset")
new_graph_file = open(graph_file+'.temp', 'w')
[all_graphs, positive_list, negative_list] = create_all_graph_dictionary(graph_file, positive_set, negative_set, new_graph_file)
new_graph_file.close()
print("Total graphs read:", len(all_graphs))
print("Total positive ids:", len(positive_list))
print("Total negative ids:", len(negative_list))

print("Creating frequent subgraphs using gSpan, support:", support)
call([(current_dir+"/libraries/gSpan"), "-f", (graph_file+'.temp'), "-s", support.strip(), "-o"])

print("Reading feature graphs")
feature_graphs = list(create_frequent_graph_dictionary(frequent_graphs).values())
feature_length = len(feature_graphs)
print("Total Features:", feature_length)

print("Generating positive graphs with features")
positive_features = {}
count = 0
for graph_id in positive_list:
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
features = list(positive_features.values()) 
features_neg = list(negative_features.values())
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
        if freq == 0:
            f_w.append(0)
        else:
            f_w.append(float(total_labels)/freq)
    for ft in features:
        for i in range(feature_length):
            ft[i] = float(ft[i]* f_w[i])


features = np.array(features).reshape((len(features), feature_length))
labels = np.array(labels)
X_train, X_test, Y_train, Y_test = train_test_split(features,labels, test_size = 0.3, random_state = 69)
clf = svm.SVC()
clf.fit(X_train,Y_train)
Y_pred = clf.predict(X_test)
print("f1 score is ", f1_score(Y_test, Y_pred))
