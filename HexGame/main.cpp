#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h> 
#include <algorithm>
#include <numeric>
#include <fstream>
#include <string>
#include <iterator>
using namespace std;

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
                        queue[i].n_ancestor = priority.n_ancestor;
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

class Graph{
    public:

        //void constructor
        Graph(){}
        
        //constructor
        Graph(int s, double degree = 0, int weight = 0){
            num_vertex = s;
            num_edge = 0;
            max_weight = weight;
            graph_matrix.resize(num_vertex, vector<int>(num_vertex, 0));
            node_value.resize(num_vertex,0);
            generate_graph(degree);
            min_cost = 0;
        }

        //read input file
        Graph(string filename){

            //initialize num of edge to 0
            num_edge = 0;

            //read input file
            ifstream data_file(filename);
            istream_iterator<string> start(data_file), end;
            vector<string> data(start, end);

            //initialize counter and edge info
            int counter = 0;
            int i = 0, j = 0;

            //start looping through the input file
            for (auto e: data){
                //the first number sets the number of vertexs in the graph
                if (counter == 0){
                    num_vertex = stoi(e);
                    graph_matrix.resize(num_vertex, vector<int>(num_vertex, 0));
                    node_value.resize(num_vertex,0);
                    cout << "number of vertex: " << num_vertex << endl;
                }
                else if ((counter % 3 == 1)){
                    i = stoi(e);
                    //cout << stoi(e) << " ";
                } 
                else if ((counter % 3 == 2)){
                    j = stoi(e);
                    //cout << stoi(e) << " ";
                } 
                else {
                    graph_matrix[i][j] = stoi(e);
                    //cout << stoi(e) << endl;    
                }
                counter++;
            }

        }

        //random number generator for graph generation based on graph density
        double generate_prob(){
            return static_cast<double>(rand())/RAND_MAX;
        }

        //random graph generator
        void generate_graph(double degree){
            for (int i = 0; i < num_vertex; ++i){
                for (int j = i; j < num_vertex; ++j){
                    double prob = generate_prob();
                    if (i == j){
                        graph_matrix[i][j] = 0;
                    }
                    else if (prob < degree){
                        cout << degree << endl;
                        cout << prob << endl;
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

        void print_node(){
            for (int i = 0; i < num_vertex; ++i){
                cout << " " << node_value[i] << " ";
            }
            cout << " " << endl;
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

        //set all edge weight to negative 
        void reverse_edge_weight(){
            for (int i = 0; i < num_vertex; i++){
                for (int j = 0; j < num_vertex; j++){
                    graph_matrix[i][j] *= -1;
                }
            }
        }

        //list of vertices in G(V,E)
        vector<int> vertices(){
            vector<int> vertice_list;
            for (int i = 0; i < V(); ++i){
                vertice_list.push_back(i);
            }
            return vertice_list;
        }

        //find shortest path between u-w and returns the sequence of vertices representing shorest path u-v1-v2-…-vn-w.
        vector<int> shortest_path(int u, int w = -100){
            dijkstra_min_path(u,w);
            return min_path;
        }

        //find longest path between u-w and returns the sequence of vertices representing longest path u-v1-v2-…-vn-w.
        vector<int> longest_path(int u, int w = -100){
            reverse_edge_weight();
            dijkstra_min_path(u,w);
            reverse_edge_weight();
            return min_path;
        }

        //return the path cost associated with the shortest path.
        int path_size(int u, int w = -100){
            dijkstra_min_path(u,w);
            return min_cost;
        }

    protected:
        int num_vertex;
        int num_edge;
        int max_weight;
        vector< vector<int> > graph_matrix;
        vector<int> node_value;

        //shortest path
        vector<int> min_path;
        int min_cost;

        //Dijkstra's Algorithm
        bool dijkstra_min_path(int u, int w = -100){
            PriorityQueue open_set(num_vertex);
            vector<int> close_set(num_vertex, 0);
            //initialize starting position
            node n(u, 0);
            close_set[n.n_index]=1;
            int curr_cost = 0;
            bool initial_state = true;
            //if it is not initial_state and open set is empty, no path exists
            //cout << "check while loop: " << (open_set.get_size() != 0 || initial_state == true) << endl;
            while (open_set.get_size() != 0 || initial_state == true){
                //if find end point, stop
                //cout << "check index：" << n.n_index << ", end index: " << w << endl;

                initial_state = false;
                //check neighbors and updating the open set
                vector<int> neighbor_set = neighbors(n.n_index);
                for (int i = 0; i < neighbor_set.size(); ++i){
                    if (close_set[neighbor_set[i]] == 0){
                        //calculate the cost for neighbor given existing path
                        node neighbor(neighbor_set[i], get_edge_value(n.n_index,neighbor_set[i])+curr_cost);
                        //update open set
                        if (open_set.contains(neighbor)){
                            //if open set contains neighbor node
                            //check if new cost is smaller than old cost, if true replace with new cost, else do nothing
                            node curr_node = open_set.get_element(neighbor.n_index);
                            if (curr_node.n_weight > neighbor.n_weight){
                                //update ancestor list
                                neighbor.n_ancestor.insert(neighbor.n_ancestor.end(), n.n_ancestor.begin(), n.n_ancestor.end());
                                neighbor.n_ancestor.push_back(n.n_index);
                                open_set.chgPrioirity(neighbor);
                            }
                        }else{ //if open set doesn't contains the neighbor node, add it to the set
                            //update ancestor list
                            neighbor.n_ancestor.insert(neighbor.n_ancestor.end(), n.n_ancestor.begin(), n.n_ancestor.end());
                            neighbor.n_ancestor.push_back(n.n_index);
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
                
                //cout << "check n: "<< n.n_index << endl;
                if (n.n_index == w){
                    min_path = n.n_ancestor;
                    min_cost = n.n_weight;
                    return true;
                }
            }
            min_path = n.n_ancestor;
            min_cost = n.n_weight;
            return false;
        }

};

enum Color { RED = -1, BLUE = 1 };

class Hex: Graph{
    public:
        //default constructor
        Hex(){}
        //constructor
        Hex(int s, double degree = 0, int weight = 0){
            num_vertex = s * s;
            num_vertex_sqrt = s;
            num_edge = 0;
            max_weight = weight;
            graph_matrix.resize(num_vertex * num_vertex, vector<int>(num_vertex * num_vertex, 0));
            node_value.resize(num_vertex * num_vertex,0);
            generate_graph(degree);
            move_cnt = 0;
            generate_board();
        }

        void generate_board(){
            for (int i = 0; i < num_vertex_sqrt; ++i){
                for (int j = 0; j < num_vertex_sqrt; ++j){

                    if (j + num_vertex * i + 1 < num_vertex_sqrt * (i + 1)){
                        set_edge_value(j + num_vertex_sqrt * i, j + num_vertex_sqrt * i + 1, 1);
                        set_edge_value(j + num_vertex_sqrt * i + 1, j + num_vertex_sqrt * i, 1);
                    }
                    if (j + num_vertex_sqrt * i - 1 >= num_vertex_sqrt * i){
                        set_edge_value(j +num_vertex_sqrt * i, j +num_vertex_sqrt * i - 1, 1);
                        set_edge_value(j +num_vertex_sqrt * i - 1, j +num_vertex_sqrt * i, 1);
                    }
                    if (j +num_vertex_sqrt * i + num_vertex_sqrt < num_vertex){
                        set_edge_value(j +num_vertex_sqrt * i, j +num_vertex_sqrt * i + num_vertex_sqrt, 1);
                        set_edge_value(j +num_vertex_sqrt * i + num_vertex_sqrt, j +num_vertex_sqrt * i, 1);
                    }
                    if ( (j +num_vertex_sqrt * i +num_vertex_sqrt - 1 >num_vertex_sqrt * (i + 1)-1) & (j + num_vertex_sqrt * i + num_vertex_sqrt - 1 < num_vertex)){
                        set_edge_value(j + num_vertex_sqrt * i, j + num_vertex_sqrt * i +num_vertex_sqrt - 1, 1);
                        set_edge_value(j +num_vertex_sqrt * i +num_vertex_sqrt - 1, j +num_vertex_sqrt * i, 1);
                    }
                }
            }
            
        }
        void update_broad(int i, int j, Color r){
            int input = 0;
            switch(r){
                case RED  : input = RED; break;
                case BLUE : input = BLUE; break;
            }
            if ((i*num_vertex_sqrt+j >= num_vertex) | (i*num_vertex_sqrt+j < 0)){
                cout << "(" << i << "," << j << ") not on board" << endl;
            }
            if (get_node_value(i*num_vertex_sqrt+j) == 0){
                set_node_value(i*num_vertex_sqrt+j,input); move_cnt++;
                cout << "set " << i*num_vertex_sqrt+j << " node value: " << get_node_value(i*num_vertex_sqrt+j) << endl;
            }else if (get_node_value(i*num_vertex_sqrt+j) == input){
                cout << "position already played" << endl;
            }else{
                if(move_cnt == 1){
                    set_node_value(i*num_vertex_sqrt+j,input);
                    move_cnt++;
                }else{
                    cout << "position played by opponent" << endl;
                }
            }
        }

        void print_broad(){
            string space = "";
            string space2 = "";
            for (int i = 0; i < num_vertex_sqrt; ++i){
                // 1st row
                cout << space;
                space += "  ";
                for (int j = 0; j < num_vertex_sqrt ; ++j){
                    if (get_node_value(i*num_vertex_sqrt+j) == static_cast<int>(RED)){
                        cout << " X " ;
                    }else if (get_node_value(i*num_vertex_sqrt+j) == 1){
                        cout << " O ";
                    }else{
                        cout << " . ";
                    }
                    if (j < num_vertex_sqrt-1)
                        cout << "-";
                }
                cout << "" << endl;
                
                // 2nd row
                cout << space2;
                space2 += "  ";
                cout << " ";
                if (i < num_vertex_sqrt-1){
                    for (int j = 0; j < num_vertex_sqrt ; ++j){
                        cout << " \\";
                        if (j < num_vertex_sqrt-1)
                            cout << " /";
                    }
                }
                
                cout << "" << endl;
            }
            cout << "" << endl;
        }

        bool check_win(Color col){
            cout << "Check win" << endl;
            print_node();
            if (col == RED){
                //RED North to South
                for (int i = 0; i < num_vertex_sqrt; i++){
                    for (int j = 0; j < num_vertex_sqrt; j++){
                        if ((get_node_value(i) == -1) & (get_node_value((num_vertex_sqrt-1)*num_vertex_sqrt+j) == -1)){
                            cout << i << "," << (num_vertex_sqrt-1)*num_vertex_sqrt+j << endl;
                            if (dijkstra_min_path(i, (num_vertex_sqrt-1)*num_vertex_sqrt+j)){
                                return true;
                            }
                        }
                    }   
                }
            }

            if (col == BLUE){
                //BLUE East to West
                for (int i = 0; i < num_vertex_sqrt; i++){
                    for (int j = 0; j < num_vertex_sqrt; j++){
                        if ((get_node_value(i*num_vertex_sqrt) == 1) & (get_node_value(j*num_vertex_sqrt+(num_vertex_sqrt-1)) == 1)){
                            cout << i*num_vertex_sqrt << "," << (j*num_vertex_sqrt+(num_vertex_sqrt-1)) << endl;
                            cout << dijkstra_min_path(i*num_vertex_sqrt, j*num_vertex_sqrt+(num_vertex_sqrt-1)) << endl;
                            if (dijkstra_min_path(i*num_vertex_sqrt, j*num_vertex_sqrt+(num_vertex_sqrt-1))){
                                return true;
                            }
                        }
                    } 
                }
            }

            return false;
        }

        Color convert(string& col){
            if (col == "RED"){
                return RED;
            }else{
                return BLUE;
            }
        }

        void get_user_color(){
            string user_color_s;
            cout << "Choice your color: RED of BLUE (BLUE play first): " << endl;
            cin >> user_color_s;
            user_color = convert(user_color_s);
            if (user_color == RED){
                my_color = BLUE;
            }else{
                my_color = RED;
            }
        }

        void get_user_input(){
            int last_move_cnt = move_cnt;
            while (last_move_cnt == move_cnt){
                cout << "Input your position: " << endl;
                cin >> last_i >> last_j;
                update_broad(last_i, last_j, user_color);
            }
        }

        void random_play(){
            int last_move_cnt = move_cnt;
            while (last_move_cnt == move_cnt){
                int n = rand()%num_vertex;
                update_broad(n/num_vertex_sqrt, n%num_vertex_sqrt, my_color);
            }
        }

        void my_strategy(){
            if ((my_color == RED) & (move_cnt == 1)){
                update_broad(last_i, last_j, RED);
            }else if ((my_color == BLUE) & (move_cnt == 0)){
                random_play();
            }else{
                //vector<int> path = longest_path(last_i*num_vertex_sqrt+last_j);
                //vector<int> n = neighbors(path[path.size() - 1]);
                vector<int> n = neighbors(last_i*num_vertex_sqrt+last_j);
                if (n.size() == 0){
                    random_play();
                }else{
                    int last_move_cnt = move_cnt;
                    for (int i= 0; i<n.size(); i++){
                        update_broad(n[i]/num_vertex_sqrt, n[i]%num_vertex_sqrt, my_color);
                        if (move_cnt > last_move_cnt){
                            break;
                        }
                    }
                }
            }
        }

        void play(){
            bool cancel = false;
            print();
            //get color
            get_user_color();

            if (user_color == RED){
                cout << "You are RED (X), RED play North to South" << endl;
            }else{
                cout << "You are BLUE (O), BLUE play East to West" << endl;
            }

            while (cancel != true){
                //play
                if (user_color == BLUE){

                    get_user_input();
                    //update status
                    print_broad();
                    //check if user win
                    if (check_win(user_color)){
                        cout << "You Win!" << endl;
                        cancel = true;
                        break;
                    }

                    my_strategy();
                    //update status
                    print_broad();
                    //check if i win
                    if (check_win(my_color)){
                        cout << "Computer Win!" << endl;
                        cancel = true;
                        break;
                    }

                }else{

                    my_strategy();
                    //update status
                    print_broad();
                    //check if i win
                    if (check_win(my_color)){
                        cout << "Computer Win!" << endl;
                        cancel = true;
                        break;
                    }

                    get_user_input();
                    //update status
                    print_broad();
                    //check if user win
                    if (check_win(user_color)){
                        cout << "You Win!" << endl;
                        cancel = true;
                        break;
                    }
                }
                
                //check if keep playing
                cout << "keep playing? (please enter 'y' or 'n'):" << endl;
                string answer;
                cin >> answer;
                if (answer == "n"){
                    cancel = true;
                    break;
                }
            }      
        }

    private:
        int move_cnt;
        Color my_color;
        Color user_color;
        int last_i;
        int last_j;
        int num_vertex_sqrt;
};


int main(){
    //seed random number generator to be different each time
    //srand (time(NULL)); 

    Hex test(5);
    //test.generate_board();
    test.play();

}