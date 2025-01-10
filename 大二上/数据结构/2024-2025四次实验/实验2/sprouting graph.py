import networkx as nx
import matplotlib.pyplot as plt
G=nx.Graph()
nodes=list(range(1,21))
G.add_nodes_from(nodes)
edges = [  
    (1, 2),(1, 3),(1, 4),(2, 5),(2, 6),(3, 7),(3, 8),(4, 9),(4, 10),(5, 11),(5, 12),(6, 13),(6, 14),(7, 15),
    (7, 16),(8, 17),(8, 18),(9, 19),(9, 20),(10, 15),(10, 16),(11, 17),(11, 19),(12, 18),(12, 20),(13, 17),
    (13, 19),(14, 18),(14, 20),(15, 18),(16, 17) ]  
G.add_edges_from(edges) 
pos=nx.spring_layout(G,k=0.5)
nx.draw(G, pos, with_labels=True, node_color='lightblue', node_size=500, font_size=10, font_weight='bold')  
plt.show(block=True)