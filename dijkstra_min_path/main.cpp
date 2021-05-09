#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h> 
#include <algorithm>
#include <numeric>
using namespace std;

class Graph{
    public:

        //void constructor
        Graph(){}
        
        //constructor
        Graph(int s, double degree, int weight){
            num_vertex = s;
            num_edge = 0;
            max_weight = weight;
            graph_matrix.resize(num_vertex, vector<int>(num_vertex, 0));;
            node_value.resize(num_vertex,0);
            generate_graph(degree);
        }

        //random number generator for graph generation based on graph density
        double generate_prob(){
            return static_cast<double>(rand())/RAND_MAX;
        }

        //random graph generator
        void generate_graph(double degree){
            for (int i = 0; i < num_vertex; ++i){
                for (int j = i; j < num_vertex; ++j){
                    if (i == j){
                        graph_matrix[i][j] = 0;
                    }
                    else if (generate_prob() < degree){
                        graph_matrix[i][j] = graph_matrix[j][i] = rand()%max_weight+1;
                        num_edge += 1;
                    }else{
                        graph_matrix[i][j] = graph_matrix[j][i] = 0;
                    }
                }
            }
        }

        //print out graph
        void print(){
            for (int i = 0; i < num_vertex; ++i){
                for (int j = 0; j < num_vertex; ++ j){
                    cout << " " << graph_matrix[i][j] << " ";
                }
            cout << " " << endl;
            }
        }

        //return number of nodes in the graph
        int V(){return num_vertex;}

        //return total number of edges in the graph
        int E(){return num_edge;}

        //return if node x is adjacent of node y
        bool adjacent(int x, int y){
            if (graph_matrix[x][y] > 0) {return true;}
            else {return false;}
        }

        //return a list of reachable neighbor nodes of node x
        vector<int> neighbors(int x){
            vector<int> neighbors_list;
            for (int i = 0; i< graph_matrix[x].size(); i++){
                if (graph_matrix[x][i] > 0){
                    neighbors_list.push_back(i);
                }
            }
            return neighbors_list;
        }

        //add an edge with random weight into the graph
        void add_edge(int x,int y){
            graph_matrix[x][y] = rand()%max_weight+1;
        }

        //delete an edge in the graph
        void delete_edge(int x,int y){
            graph_matrix[x][y] = 0;
        }

        //return node value to the graph
        int get_node_value(int x){
            return node_value[x];
        }

        //set node value in the graph
        void set_node_value(int x, int a){
            node_value[x]=a;
        }

        //return the edge weight between node x and node y
        int get_edge_value(int x, int y){
            return graph_matrix[x][y];
        }

        //set the edge weight between node x and node y
        void set_edge_value(int x, int y, int w){
            graph_matrix[x][y] = w;
        }

    private:
        int num_vertex;
        int num_edge;
        int max_weight;
        vector< vector<int> > graph_matrix;
        vector<int> node_value;

};

class node{
    public:
        int n_index;
        int n_weight;
        vector<int> n_ancestor;

        node(){
            n_index = -1;
            n_weight = -1;
        }
        
        node(int index, int weight){
            n_index =  index;
            n_weight = weight;
        }

};

bool operator==(const node a, const node b){
    if (a.n_index == b.n_index){
        return true;
    }
    return false;
}

class PriorityQueue{
    private:
        int priority;
        int q_size;
        int capacity;
        vector<node> queue;

        //parent index of i
        int parent(int i){
            return ( i - 1 ) / 2;
        }
        //left index of i
        int left_child(int i){
            return 2 * i + 1;
        }
        //right index of i
        int right_child(int i){
            return 2 * i + 2;
        }
        //swap position of index x and inndex y
        void swap(int x, int y){
            node temp = queue[x];
            queue[x] = queue[y];
            queue[y] = temp;
        }
        //heapify up
        void heapifyUp(int index){
            while (parent(index)>=0 && queue[parent(index)].n_weight > queue[index].n_weight && queue[parent(index)].n_weight != -1 && queue[index].n_weight != -1){
                swap(parent(index), index);
                index = parent(index);
            }
        }
        //heapify down
        void heapifyDown(int index = 0){
            int smaller = index;
            if (left_child(index)<q_size && queue[left_child(index)].n_weight<queue[index].n_weight
                && queue[left_child(index)].n_weight != -1 && queue[index].n_weight != -1){
                smaller = left_child(index);
            }
            if (right_child(index)<q_size && queue[right_child(index)].n_weight<queue[smaller].n_weight
                && queue[right_child(index)].n_weight != -1 && queue[smaller].n_weight != -1){
                smaller = right_child(index);
            }
            if (smaller != index){
                swap(smaller, index);
                heapifyDown(smaller);
            }
        }
        

    public:

        //default constructor
        PriorityQueue(){}

        //constructor
        PriorityQueue(int capacity){
            node n;
            queue.resize(capacity,n);
            q_size = 0;
            this->capacity = capacity;
        }

        //changes the priority (node value) of queue element
        void chgPrioirity(node priority){
            int queue_index = 0;
            int old_weight = -1;
            if (contains(priority) == true){
                for (int i = 0; i < q_size; ++i){
                    if (queue[i].n_index == priority.n_index){
                        old_weight = queue[i].n_weight;
                        queue[i].n_weight = priority.n_weight;
                        queue_index = i;
                        break;
                    }
                }
            }
            if (old_weight < priority.n_weight){
                heapifyDown(queue_index);
            }else if(old_weight > priority.n_weight){
                heapifyUp(queue_index);
            }
        }

        //removes the top element of the queue
        node minPrioirty(){
            if (q_size == 0){
                cout << "The queue is empty" << endl;
                node nan_node;
                return nan_node;
            }else{
                node out = queue[0];
                queue[0] = queue[q_size-1];
                queue[q_size-1].n_index = -1;
                queue[q_size-1].n_weight = -1;
                heapifyDown();
                q_size -= 1;
                return out;
            }
        }

        //does the queue contain queue_element
        bool contains(node queue_element){
            for (int i = 0; i < q_size; ++i){
                if (queue[i].n_index == queue_element.n_index){
                    return true;
                }
            }
            return false;
        }
        //if the queue contains queue_element, return queue element
        node get_element(int index){
            for (int i = 0; i < q_size; ++i){
                if (queue[i].n_index == index){
                    return queue[i];
                }
            }
            node n;
            return n;
        }

        //insert queue_element into queue
        void insert(node queue_element){
            if (q_size == capacity){
                cout << "The queue is full" << endl;
            }else{
                queue[q_size] = queue_element;
                q_size += 1;
                heapifyUp(q_size-1);
            }
        }

        //returns the top element of the queue
        node get_top(){
            return queue[0];
        }

        //returns the number of queue_elements
        int get_size(){
            return q_size;
        }

        //print queue
        void print(){
            for (int i = 0; i < capacity; ++i){
                cout << "(" << queue[i].n_index << "," << queue[i].n_weight  << ") ";
            }
            cout << "" << endl;
        }

};

class ShortestPath{
    public:
        //Constructor
        ShortestPath(Graph graph){
            s_graph = graph;
            min_cost = 0;
        }

        //list of vertices in G(V,E)
        vector<int> vertices(){
            vector<int> vertice_list;
            for (int i = 0; i < s_graph.V(); ++i){
                vertice_list.push_back(i);
            }
            return vertice_list;
        }

        //find shortest path between u-w and returns the sequence of vertices representing shorest path u-v1-v2-…-vn-w.
        vector<int> path(int u, int w){
            dijkstra_min_path(u,w);
            return min_path;
        }
        //return the path cost associated with the shortest path.
        int path_size(int u, int w){
            dijkstra_min_path(u,w);
            return min_cost;
        }
    private:
        vector<int> min_path;
        int min_cost;
        Graph s_graph;

        //Dijkstra's Algorithm
        void dijkstra_min_path(int u, int w){
            PriorityQueue open_set(s_graph.V());
            vector<int> close_set(s_graph.V(), 0);
            //initialize starting position
            node n(u, 0);
            close_set[n.n_index]=1;
            int curr_cost = 0;
            bool initial_state = true;
            //if it is not initial_state and open set is empty, no path exists
            cout << "check while loop: " << (open_set.get_size() != 0 || initial_state == true) << endl;
            while (open_set.get_size() != 0 || initial_state == true){
                //if find end point, stop
                cout << "check index：" << n.n_index << ", end index: " << w << endl;
                if (n.n_index == w){
                    min_path = n.n_ancestor;
                    min_cost = n.n_weight;
                    break;
                }
                initial_state = false;
                //check neighbors and updating the open set
                vector<int> neighbor_set = s_graph.neighbors(close_set[0]);
                for (int i = 0; i < neighbor_set.size(); ++i){
                    cout << neighbor_set[i] << endl;
                    if (close_set[neighbor_set[i]] == 0){
                        //calculate the cost for neighbor given existing path
                        node neighbor(neighbor_set[i], s_graph.get_edge_value(u,neighbor_set[i])+curr_cost);
                        //update open set
                        if (open_set.contains(neighbor)){
                            //if open set contains neighbor node
                            //check if new cost is smaller than old cost, if true replace with new cost, else do nothing
                            node curr_node = open_set.get_element(neighbor.n_index);
                            if (curr_node.n_weight > neighbor.n_weight){
                                //update ancestor list
                                neighbor.n_ancestor.push_back(n.n_index);
                                neighbor.n_ancestor.insert(neighbor.n_ancestor.end(), n.n_ancestor.begin(), n.n_ancestor.end());
                                open_set.chgPrioirity(neighbor);
                            }
                        }else{ //if open set doesn't contains the neighbor node, add it to the set
                            //update ancestor list
                            neighbor.n_ancestor.push_back(n.n_index);
                            neighbor.n_ancestor.insert(neighbor.n_ancestor.end(), n.n_ancestor.begin(), n.n_ancestor.end());
                            open_set.insert(neighbor);
                        }
                    }
                }
                //pop the node with minimum cost
                n = open_set.minPrioirty();
                //mark the new node as visited
                close_set[n.n_index]=1;
                //update to new cost
                curr_cost = n.n_weight;
                
            }
        }

};

int main(){
    //seed random number generator to be different each time
    srand (time(NULL)); 
    //Graph(int number of nodes, double degree, int max weight in a path)
    Graph rand_graph(5, 0.5, 5);
    rand_graph.print();
    ShortestPath cal_path(rand_graph);
    cout << cal_path.path_size(0,rand_graph.V()-1) << " " << endl;
    vector<int> min_path_out = cal_path.path(0,rand_graph.V()-1);
    for (int i = 0; i < min_path_out.size(); ++i){
        cout << min_path_out[i] << " ";
    }
}