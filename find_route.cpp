#include <iostream>
#include <utility>
#include <unordered_map>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#define max(a,b) ( a > b ? a : b)

// Tree Node Class
class TreeNode {
    public:
        std::string start;
        std::unordered_map<TreeNode*, int> children;
        TreeNode* prev_node;

        TreeNode(std::string _start) {
            start = _start;
        } 

        void AddChild(TreeNode* _destination, int _distance) {
            children[_destination] = _distance;
        }
};

class PathState {
    public:
        int total_distance;
        TreeNode* last_node;

        PathState(TreeNode* last_node, int total_distance) : last_node(last_node), total_distance(total_distance) {}

        friend bool operator<(const PathState& p1, const PathState& p2) {
            return p1.total_distance < p2.total_distance;
        }
};

// Calculates First Uninformed Path To The Destination Node
void GetUninformedPath(TreeNode* root, std::string destination) {
    
    std::queue<TreeNode*> queue;
    std::unordered_map<std::string, int> visited;
    std::vector<std::string> paths;
    queue.push(root);
    TreeNode* curr_node = NULL;
    int path = 0, max_nodes = 0, nodes_expanded = 0, nodes_generated = 0;
    bool flag = false;
    ++visited[root->start];

    // BFS visited
    while(!queue.empty() && !flag) {
        
        max_nodes = max(queue.size(), max_nodes);
        curr_node = queue.front();
        queue.pop();
        ++nodes_expanded;

        if(curr_node->start == destination)  flag = true;

        for(auto child : curr_node->children) {
            if(visited[child.first->start] == 0) {
                ++nodes_generated;
                child.first->prev_node = curr_node;
                queue.push(child.first);
                ++visited[child.first->start];
            }
        }

    }

    while(curr_node->prev_node !=  NULL && flag) {
        std::ostringstream path_stream;
        path_stream << curr_node->prev_node->start << " to "<< curr_node->start
                    << ", " << curr_node->prev_node->children[curr_node] << ".0 km\n";
        std::string curr_path = path_stream.str();
        paths.push_back(curr_path);
        path += curr_node->prev_node->children[curr_node]; 
        curr_node = curr_node->prev_node;
    }

    std::cout << "nodes expanded: " << nodes_expanded << "\n"
              << "nodes generated: " << nodes_generated << "\n"
              << "max nodes in memory: " << max_nodes << "\n"
              << "distance: " << path << "\n"
              << "route: " << "\n";
              
    if(!flag) {
        std::cout << "none" << std::endl;
    } else {
        for(int i = paths.size() - 1; i >= 0; --i) {
            std::cout << paths[i];
        }
    }
}


// Calculates Optimal Path To The Destination Node By Checking Results With Heuristic 
void GetInformedPath(TreeNode* root, std::string destination, 
std::unordered_map<std::string, int> heuristic_map) {

    std::priority_queue<PathState*> queue;
    std::unordered_map<std::string, int> visited;
    std::vector<std::string> paths;
    PathState* root_state = new PathState(root,0);
    PathState* curr_state = NULL;
    queue.push(root_state);
    int path = 0, max_nodes = 0, nodes_expanded = 0, nodes_generated = 0;
    bool flag = false;
    ++visited[root->start];

    // BFS visited
    while(!queue.empty() && !flag) {
        
        max_nodes = max(queue.size(), max_nodes);
        curr_state = queue.top();
        queue.pop();
        ++nodes_expanded;

        if(curr_state->last_node->start == destination)  flag = true;

        for(auto child : curr_state->last_node->children) {
            if(visited[child.first->start] == 0 && 
            heuristic_map[curr_state->last_node->start] > heuristic_map[child.first->start]) {
                ++nodes_generated;
                child.first->prev_node = curr_state->last_node;
                PathState* new_state = new PathState(child.first, curr_state->total_distance + child.second);
                queue.push(new_state);
                ++visited[child.first->start];
            }
        }
    }

    TreeNode* curr_node = curr_state->last_node;
    while(curr_node->prev_node !=  NULL && flag) {
        std::ostringstream path_stream;
        path_stream << curr_node->prev_node->start << " to "<< curr_node->start
                    << ", " << curr_node->prev_node->children[curr_node] << ".0 km\n";
        std::string curr_path = path_stream.str();
        paths.push_back(curr_path);
        path += curr_node->prev_node->children[curr_node]; 
        curr_node = curr_node->prev_node;
    }

    std::cout << "nodes expanded: " << nodes_expanded << "\n"
              << "nodes generated: " << nodes_generated << "\n"
              << "max nodes in memory: " << max_nodes << "\n"
              << "distance: " << path << "\n"
              << "route: " << "\n";
              
    if(!flag) {
        std::cout << "none" << std::endl;
    } else {
        for(int i = paths.size() - 1; i >= 0; --i) {
            std::cout << paths[i];
        }
    }
}


int main(int argc, char** argv) {
    
    int distance;
    std::string start, dest;
    std::unordered_map<std::string, TreeNode*> in_map;
    std::unordered_map<std::string, int> heuristic_map;
    TreeNode* start_node = NULL;
    std::ifstream tree_source (argv[1]);

    // construct a Tree based on input file 
    if (tree_source.is_open()) {
        while ( tree_source >> start >> dest >> distance ) {
            TreeNode* node_start = in_map[start];
            TreeNode* node_dest = in_map[dest];
            
            if(in_map[dest] == 0) {
                node_dest = new TreeNode(dest);
                in_map[dest] = node_dest;
            }
            
            if(in_map[start] == 0) {
                node_start = new TreeNode(start);
                in_map[start] = node_start;
            }
            
            if(argv[2] == start && start_node == NULL) {
                start_node = node_start;
            } else if(argv[2] == dest && start_node == NULL) {
                start_node = node_dest;
            }

            node_start->AddChild(node_dest, distance);
            node_dest->AddChild(node_start, distance);
        }

        tree_source.close();
    }

    else std::cout << "Unable to open file"; 
    
    if(argv[4] != NULL) {
        
        std::ifstream heuristic (argv[4]);
        if (heuristic.is_open()) {
            while ( heuristic >> dest >> distance ) {
                heuristic_map[dest] = distance;
            }
            
            heuristic.close();
        }

        else std::cout << "Unable to open file"; 

    }


    if(argv[4] == NULL) {
        GetUninformedPath(start_node, argv[3]);
    } else {
        GetInformedPath(start_node, argv[3], heuristic_map);
    }

    return 0; 
}
