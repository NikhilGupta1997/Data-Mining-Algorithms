import networkx as nx
import sys

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
        print(graph_id)
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

graph_file = sys.argv[1]
#positive_ids = sys.argv[2]
#negative_ids = sys.argv[3]
#frequent_graphs = sys.argv[4]
all_graphs = create_graph_dictionary(graph_file)
feature_graphs = create_graph_dictionary(frequent_graphs)
