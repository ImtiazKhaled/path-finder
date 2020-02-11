// Imtiaz Mujtaba Khaled
// 1001551928

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
        
        friend bool operator>(const PathState& p1, const PathState& p2) {
            return p1.total_distance > p2.total_distance;
        }
};

// Calculates First Uninformed Path To The Destination Node
void GetUninformedPath(TreeNode* root, std::string destination) {
    
    std::priority_queue<PathState, std::vector<PathState>, std::greater<PathState>> queue;
    std::unordered_map<std::string, int> visited;
    std::vector<std::string> paths;
    PathState root_node(root, 0); 
    PathState curr_node(root, -1); 
    PathState found_node(root, -1);
    queue.push(root_node);
    int path = 0, max_nodes = 0, nodes_expanded = 0, nodes_generated = 0;
    bool flag_optimal = false, flag_found = false;
    ++visited[root->start];

    // uniform cost search with visited
    while(!queue.empty() && !flag_optimal) {
        
        max_nodes = max(queue.size(), max_nodes);
        curr_node = queue.top();
        queue.pop();
        ++nodes_expanded;

        if(curr_node.last_node->start == destination)  {
            found_node = curr_node;
            flag_found = true;
            continue;
        }

        if(flag_found && found_node.total_distance < curr_node.total_distance) {
            flag_optimal = true;
        }

        if(flag_found) {
            for(auto child : curr_node.last_node->children) {
                if(visited[child.first->start] == 0 && 
                    found_node.total_distance < curr_node.total_distance + child.second) {
                    // std::cout << curr_node.last_node->start << " to " << child.first->start << ": " << curr_node.total_distance + child.second << std::endl;
                    ++nodes_generated;
                    child.first->prev_node = curr_node.last_node;
                    PathState next_node(child.first, curr_node.total_distance + child.second);
                    queue.push(next_node);
                    ++visited[child.first->start];
                }
            }
        } else {
            for(auto child : curr_node.last_node->children) {
                if(visited[child.first->start] == 0) {
                    // std::cout << curr_node.last_node->start << " to " << child.first->start << ": " << curr_node.total_distance + child.second << std::endl;
                    ++nodes_generated;
                    child.first->prev_node = curr_node.last_node;
                    PathState next_node(child.first, curr_node.total_distance + child.second);
                    queue.push(next_node);
                    ++visited[child.first->start];
                }
            }
        } 
    }

    TreeNode * curr_tree_node = NULL;
    if(found_node.total_distance != -1) {
        curr_tree_node = found_node.last_node;
        while(curr_tree_node->prev_node !=  NULL && flag_found) {
            std::ostringstream path_stream;
            path_stream << curr_tree_node->prev_node->start << " to "<< curr_tree_node->start
                        << ", " << curr_tree_node->prev_node->children[curr_tree_node] << ".0 km\n";
            std::string curr_path = path_stream.str();
            paths.push_back(curr_path);
            path += curr_tree_node->prev_node->children[curr_tree_node]; 
            curr_tree_node = curr_tree_node->prev_node;
        }
    }
    
    std::cout << "nodes expanded: " << nodes_expanded << "\n"
              << "nodes generated: " << nodes_generated << "\n"
              << "max nodes in memory: " << max_nodes << "\n"
              << "distance: " << path << "\n"
              << "route: " << "\n";

    if(paths.empty()) {
        std::cout << "none" << std::endl;
    } else {
        for(int i = paths.size() - 1; i >= 0; --i) {
            std::cout << paths[i];
        }
    }
}


// Calculates Optimal Path To The Destination Node By Checking Results With Heuristic 
void GetInformedPath(TreeNode* root, std::string destination, std::unordered_map<std::string, int> heuristic) {
    
    std::priority_queue<PathState, std::vector<PathState>, std::greater<PathState>> queue;
    std::unordered_map<std::string, int> visited;
    std::vector<std::string> paths;
    PathState root_node(root, 0); 
    PathState curr_node(root, -1); 
    PathState found_node(root, -1);
    queue.push(root_node);
    int path = 0, max_nodes = 0, nodes_expanded = 0, nodes_generated = 0;
    bool flag_optimal = false, flag_found = false;
    ++visited[root->start];

    // uniform cost search with visited
    while(!queue.empty() && !flag_found) {
        
        max_nodes = max(queue.size(), max_nodes);
        curr_node = queue.top();
        queue.pop();
        ++nodes_expanded;

        if(curr_node.last_node->start == destination)  {
            found_node = curr_node;
            flag_found = true;
            continue;
        }

        for(auto child : curr_node.last_node->children) {
            if(visited[child.first->start] == 0) {
                ++nodes_generated;
                child.first->prev_node = curr_node.last_node;
                int path_distance = curr_node.total_distance == 0 ? child.second + heuristic[child.first->start] : 
                    curr_node.total_distance + child.second + heuristic[child.first->start] - 
                    heuristic[curr_node.last_node->start];
                PathState next_node(child.first, path_distance);
                queue.push(next_node);
                ++visited[child.first->start];
            }
        }
    }

    TreeNode * curr_tree_node = NULL;
    if(found_node.total_distance != -1) {
        curr_tree_node = found_node.last_node;
        while(curr_tree_node->prev_node !=  NULL && flag_found) {
            std::ostringstream path_stream;
            path_stream << curr_tree_node->prev_node->start << " to "<< curr_tree_node->start
                        << ", " << curr_tree_node->prev_node->children[curr_tree_node] << ".0 km\n";
            std::string curr_path = path_stream.str();
            paths.push_back(curr_path);
            path += curr_tree_node->prev_node->children[curr_tree_node]; 
            curr_tree_node = curr_tree_node->prev_node;
        }
    }
    
    std::cout << "nodes expanded: " << nodes_expanded << "\n"
              << "nodes generated: " << nodes_generated << "\n"
              << "max nodes in memory: " << max_nodes << "\n"
              << "distance: " << path << "\n"
              << "route: " << "\n";

    if(paths.empty()) {
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
