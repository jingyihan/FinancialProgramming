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
#define SIM_NUM 1000

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
            min_cost = 0;
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

        //list of vertices in G(V,E)
        vector<int> vertices(){
            vector<int> vertice_list;
            for (int i = 0; i < V(); ++i){
                vertice_list.push_back(i);
            }
            return vertice_list;
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
            move_cnt = 0;
            pied = false;
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
                    pied = true;
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
            //cout << "Check win: " << convert_s(col) << endl;
            //print_node();
            if (col == RED){
                //RED North to South
                for (int i = 0; i < num_vertex_sqrt; i++){
                    if (get_node_value(i) == static_cast<int>(col)){
                        //cout << i << ", " << bfs(i, col) << endl;
                        if (bfs(i, col)){
                            return true;
                        }
                    } 
                }
            }

            if (col == BLUE){
                //BLUE East to West
                for (int i = 0; i < num_vertex_sqrt; i++){
                    if (get_node_value(i*num_vertex_sqrt) == static_cast<int>(col)){
                        //cout << i*num_vertex_sqrt << ", " << bfs(i*num_vertex_sqrt, col) << endl;
                        if (bfs(i*num_vertex_sqrt, col)){
                            return true;
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

        string convert_s(Color col){
            if (col == RED){
                return "RED";
            }else{
                return "BLUE";
            }
        }

        void get_user_color(){
            string user_color_s;
            cout << "Choice your color: RED or BLUE (BLUE play first): " << endl;
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
                user_last = last_i*num_vertex_sqrt + last_j;
            }
        }

        void random_play(){
            int last_move_cnt = move_cnt;
            while (last_move_cnt == move_cnt){
                int n = rand()%num_vertex;
                update_broad(n/num_vertex_sqrt, n%num_vertex_sqrt, my_color);
                my_last = n;
            }
        }

        void random_play(int num){
            int last_move_cnt = move_cnt;
            while (last_move_cnt == move_cnt){
                int n = rand()%num;
                update_broad(n/static_cast<int>(sqrt(num)), n%static_cast<int>(sqrt(num)), my_color);
                my_last = n;
            }
        }

        //play on one of the neighours of the last played location
        void my_strategy_simple(){
            if ((my_color == RED) & (move_cnt == 1)){
                update_broad(last_i, last_j, RED);
            }else if ((my_color == BLUE) & (move_cnt == 0)){
                random_play();
            }else{
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

        int mc_helper(vector<int> nodes){
            random_shuffle(nodes.begin(), nodes.end());
            Color first = BLUE;
            Color second = RED;
            if(pied){
                first = RED;
                second = BLUE;
            }
            for (int i=0; i<nodes.size(); i++){
                if (i < nodes.size()/2){
                    if (get_node_value(nodes[i])==0){
                        set_node_value(nodes[i], static_cast<int>(second));
                    }
                }else{
                    if (get_node_value(nodes[i])==0){
                        set_node_value(nodes[i], static_cast<int>(first));
                    }
                }
            }
            if(check_win(BLUE)){
                return 1;
            }else{
                return 0;
            }
        }

        //calcuate win rate
        double monte_carlo(Color c){
            //save current board
            vector<int> node_value_save = node_value;
            //initialize shuffle vector
            vector<int> nodes;
            for (int i=0; i<num_vertex; i++){
                nodes.push_back(i);
            }
            int sum = 0;
            for (int i=0; i<SIM_NUM; i++){
                //reset the board for each simulation
                node_value = node_value_save;
                sum += mc_helper(nodes);
            }
            //cout << "Monte Carlo sum: " << sum << endl;
            node_value = node_value_save;
            if (c==BLUE){
                return static_cast<double>(sum)/SIM_NUM;
            }else{
                return 1.0 - static_cast<double>(sum)/SIM_NUM;
            }
        }

        //use monte carlo simulation to calculate win rate
        void my_strategy_mc(){
            
            clock_t start, end;
            start = clock();
            vector<double> win_rates(num_vertex, 0.0);
            //cout << "move_cnt: " << move_cnt << endl;

            if (move_cnt == 0){
                //if start first, play randomly within inner board
                random_play(num_vertex/2);
                move_cnt += 1;
            }else if (move_cnt == 1){
                //if play second, take the center
                update_broad(num_vertex_sqrt/2, num_vertex_sqrt/2, my_color);
                my_last = num_vertex_sqrt/2 * num_vertex_sqrt + num_vertex_sqrt/2;
            }else{
                //check around my last play 
                vector<int> my_n = neighbors(my_last);
                for (int i = 0; i< my_n.size(); i++){
                    if ((get_node_value(my_n[i]) == 0)){
                        set_node_value(my_n[i],my_color);
                        win_rates[my_n[i]] = max(monte_carlo(my_color), win_rates[my_n[i]]);
                        set_node_value(my_n[i], 0);
                    }
                }
                
                //check around opponent last play
                vector<int> user_n = neighbors(user_last);
                for (int i = 0; i< user_n.size(); i++){
                    if ((get_node_value(user_n[i]) == 0) & (win_rates[user_n[i]] == 0)){
                        set_node_value(user_n[i],my_color);
                        win_rates[user_n[i]] = max(monte_carlo(my_color), win_rates[user_n[i]]);
                        set_node_value(user_n[i], 0);
                    }
                }

                int max_play = max_element(win_rates.begin(),win_rates.end()) - win_rates.begin();
                cout << "max play is: " << max_play << " win rate: " << win_rates[max_play] << endl;

                //if no chance of winning, play randomly; else, play max win position
                if (win_rates[max_play]==0.0){
                    random_play(num_vertex/2);
                }else{
                    update_broad(max_play/num_vertex_sqrt, max_play%num_vertex_sqrt, my_color);
                    my_last = max_play;
                }
            }
            
            end = clock();
            double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
            cout << "Time taken by program is : " << fixed 
                << time_taken << setprecision(5);
            cout << " sec " << endl;
        }

        void play(){
            bool cancel = false;
            //print();
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
                    //cout << "Check win: " << convert_s(user_color) << " - " << check_win(user_color) << endl;
                    if (check_win(user_color)){
                        cout << "You Win!" << endl;
                        cancel = true;
                        break;
                    }

                    my_strategy_mc();
                    //update status
                    print_broad();
                    //check if i win
                    //cout << "Check win: " << convert_s(my_color) << " - " << check_win(my_color) << endl;
                    if (check_win(my_color)){
                        cout << "Computer Win!" << endl;
                        cancel = true;
                        break;
                    }

                }else{

                    my_strategy_mc();
                    //update status
                    print_broad();
                    //check if i win
                    //cout << "Check win: " << convert_s(my_color) << " - " << check_win(my_color) << endl;
                    if (check_win(my_color)){
                        cout << "Computer Win!" << endl;
                        cancel = true;
                        break;
                    }

                    get_user_input();
                    //update status
                    print_broad();
                    //check if user win
                    //cout << "Check win: " << convert_s(user_color) << " - " << check_win(user_color) << endl;
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
        Color my_color, user_color;
        int last_i, last_j;
        int my_last, user_last;
        int num_vertex_sqrt;
        bool pied;

        // BFS
        bool bfs(int u, Color c){

            vector<int> queue;
            vector<int> visited(num_vertex, 0);

            queue.push_back(u);
            visited[u] = 1;

            //cout << "BFS: " << endl;

            while (queue.size() != 0){

                //cout << "Print queue: ";
                //for (int i = 0; i < queue.size(); i++){
                //    cout << queue[i] << " ";
                //}

                //pop the top of queue
                int s = queue[0];
                queue.erase(queue.begin());

                //cout << "" << endl;
                //cout << "Top queue poped: ";
                //cout << s << " , " << static_cast<int>(c) << endl;

                //check top of the queue
                if ((c == RED) & (s/num_vertex_sqrt == num_vertex_sqrt-1)){
                    return true;
                }else if ((c == BLUE) & (s%num_vertex_sqrt == (num_vertex_sqrt-1)))
                {
                    return true;
                }

                //check neighbors and updating the queue
                vector<int> neighbor_set = neighbors(s);
                //cout << "Print neighbors of : " << s << endl;
                for (int i = 0; i < neighbor_set.size(); ++i){
                    //cout << "(" << neighbor_set[i] << ", " << get_node_value(neighbor_set[i]) << ") " << endl;
                    if (visited[neighbor_set[i]] == 0 & get_node_value(neighbor_set[i]) == static_cast<int>(c)){
                        //mark the node as visited
                        visited[neighbor_set[i]] = 1;
                        //update queue
                        queue.push_back(neighbor_set[i]);
                    }
                }
                
            }

            return false;

        }

};


int main(){
    //seed random number generator to be different each time
    srand (time(NULL)); 

    Hex test(11);
    test.play();

}