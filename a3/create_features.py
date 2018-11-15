import sys
import networkx as nx
from networkx.algorithms import isomorphism as iso
from sklearn import svm
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.metrics import f1_score
from subprocess import call
from concurrent import futures

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

def create_graph_list(filename):
    f = open(filename, 'r')
    data = f.readlines()
    li = 0
    graph_list = []
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
        graph_list.append(G)
    f.close()
    return graph_list

def IDF(features, feature_graphs):
    print("Performing IDF")
    total_labels = len(features)
    if total_labels == 0:
        return
    feature_length = len(features[0])
    f_w = []
    for i in range(feature_length):
        freq = 0
        for ft in features:
            freq += ft[i]
        if freq == 0:
            f_w.append(0)
        else:
            wt = float(total_labels)/freq
            if wt < 1.4:
                wt = 0
            f_w.append(wt)
    print(f_w)
    new_features = [[] for x in range(total_labels)]
    new_feature_graphs = []
    for i in range(feature_length):
        if f_w[i]!=0:
            for j in range(total_labels):
                new_features[j].append(features[j][i])
            new_feature_graphs.append(feature_graphs[i])
    print("New number of features:", len(new_feature_graphs))
    return [new_features, new_feature_graphs]

def get_ids(filename):
    f = open(filename, 'r')
    ids = [ int(x.strip()) for x in f.readlines() ]
    f.close()
    return ids

graph_file = sys.argv[1].strip()
pfile = sys.argv[2].strip()
nfile = sys.argv[3].strip()
testset_file = sys.argv[4].strip()
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
print("Total train graphs read:", len(all_graphs))
print("Total positive ids:", len(positive_list))
print("Total negative ids:", len(negative_list))
test_graphs = create_graph_list(testset_file)

support_list = []#[ 0.05*(x+8) for x in range(3) ]
support_list_2 = [ 0.025*(x+21) for x in range(18) ]
support_list.extend(support_list_2)
MAX_FEATURES = 100
feature_graphs = []
feature_length = 0
for support in support_list:
    print("Creating frequent subgraphs using gSpan, support:", support)
    call([(current_dir+"/libraries/gSpan"), "-f", (graph_file+'.temp'), "-s", str(support), "-o"])
 
    print("Reading feature graphs")
    feature_graphs = create_graph_list(frequent_graphs)
    feature_length = len(feature_graphs)
    print("Total Features:", feature_length, ", Support:", support)
    if feature_length < MAX_FEATURES:
        print("Chosen Feature Length:", feature_length, ", Support:", support)
        break

def run_parallel(item_list, feature_graphs, graphs_dict, pid):
    features_dict = {}
    count = 0
    for graph_id in item_list:
        if count%100 == 0:
            print (pid, count)
        count+=1
        f_v = []
        for feature_graph in feature_graphs:
            graph_match = iso.GraphMatcher(
                    graphs_dict[graph_id],
                    feature_graph,
                    node_match = lambda x, y: x['node_id'] == y['node_id'],
                    edge_match = lambda x, y: x['edge_id'] == y['edge_id']
                    )
            if(graph_match.subgraph_is_isomorphic()):
                f_v.append(1)
            else:
                f_v.append(0)
        features_dict[graph_id] = f_v
    return features_dict

print("Generating positive graphs with features")
positive_features = {}
pieces = int(len(positive_list)/4)
pos_lists = []
pos_lists.append(positive_list[0:pieces])
pos_lists.append(positive_list[pieces:2*pieces])
pos_lists.append(positive_list[2*pieces:3*pieces])
pos_lists.append(positive_list[3*pieces:])
executor = futures.ProcessPoolExecutor(4)
my_futures = [executor.submit(run_parallel,pos_lists[pid], feature_graphs, all_graphs, pid) for pid in range(4) ]
futures.wait(my_futures)
for ft in my_futures:
    positive_features.update(ft.result())
print("Generating negative graphs with features")
negative_features = {}
pieces = int(len(negative_list)/4)
neg_lists = []
neg_lists.append(negative_list[0:pieces])
neg_lists.append(negative_list[pieces:2*pieces])
neg_lists.append(negative_list[2*pieces:3*pieces])
neg_lists.append(negative_list[3*pieces:])
executor = futures.ProcessPoolExecutor(4)
my_futures = [executor.submit(run_parallel,neg_lists[pid], feature_graphs, all_graphs, pid) for pid in range(4) ]
futures.wait(my_futures)
for ft in my_futures:
    negative_features.update(ft.result())

labels = [ 1 for x in range(len(positive_list)) ]
labels_neg = [ 0 for x in range(len(negative_list)) ]
labels.extend(labels_neg)
features = list(positive_features.values())
features_neg = list(negative_features.values())
features.extend(features_neg)
[features, feature_graphs] = IDF(features, feature_graphs)
feature_length = len(feature_graphs)

def run_test_parallel(test_graphs, feature_graphs, pid):
    test_features = []
    count = 0
    for test_graph in test_graphs:
        if count%100 == 0:
            print (pid,count)
        count+=1
        f_v = []
        for feature_graph in feature_graphs:
            graph_match = iso.GraphMatcher(
                    test_graph,
                    feature_graph,
                    node_match = lambda x, y: x['node_id'] == y['node_id'],
                    edge_match = lambda x, y: x['edge_id'] == y['edge_id']
                    )
            if(graph_match.subgraph_is_isomorphic()):
                f_v.append(1)
            else:
                f_v.append(0)
        test_features.append(f_v)
    return test_features

print("Generating features for test graphs")
test_features = []
pieces = int(len(test_graphs)/4)
print(pieces)
t_lists = []
t_lists.append(test_graphs[0:pieces])
t_lists.append(test_graphs[pieces:2*pieces])
t_lists.append(test_graphs[2*pieces:3*pieces])
t_lists.append(test_graphs[3*pieces:])
executor = futures.ProcessPoolExecutor(4)
my_futures = [executor.submit(run_test_parallel,t_lists[pid], feature_graphs, pid) for pid in range(4) ]
futures.wait(my_futures)
for ft in my_futures:
    test_features.extend(ft.result())

print('Writing train file')
with open('train.txt', 'w') as f:
    for i in range(len(features)):
        f.write(str(int(labels[i])*2-1))
        f_v = features[i]
        for j in range(len(f_v)):
            if f_v[j] == 0:
                continue
            f.write(' ')
            f.write(str(j))
            f.write(':')
            f.write(str(f_v[j]))
        f.write('\n')

print('Writing test file')
with open('test.txt', 'w') as f:
    for i in range(len(test_features)):
        f_v = test_features[i]
        first = True
        for j in range(len(f_v)):
            if f_v[j] == 0:
                continue
            if not first:
                f.write(' ')
            first = False
            f.write(str(j))
            f.write(':')
            f.write(str(f_v[j]))
        f.write('\n')

test_labels = []
with open(sys.argv[5], 'r') as f:
    test_labels = [ 2-int(x.strip()) for x in f.readlines() ]
features = np.array(features).reshape((len(features), feature_length))
test_features = np.array(test_features).reshape((len(test_features), feature_length))
labels = np.array(labels)
test_labels = np.array(test_labels)
clf = svm.SVC()
clf.fit(features,labels)
Y_pred = clf.predict(test_features)
print (Y_pred)
print( test_labels)
print("f1 score is ", f1_score(test_labels, Y_pred))
