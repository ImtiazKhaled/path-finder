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

// Calculates First Uninformed Path To The Destination Node
void GetUninformedPath(TreeNode* root, std::string destination) {
    
    std::queue<TreeNode*> queue;
    std::unordered_map<std::string, int> visited;
    std::vector<std::string> paths;
    queue.push(root);
    TreeNode* CurrNode = NULL;
    int path = 0, max_nodes = 0, nodes_expanded = 0, nodes_generated = 0;
    bool flag = false;
    ++visited[root->start];

    // BFS visited
    while(!queue.empty() && !flag) {
        
        max_nodes = max(queue.size(), max_nodes);
        CurrNode = queue.front();
        queue.pop();
        ++nodes_expanded;

        if(CurrNode->start == destination)  flag = true;

        for(auto child : CurrNode->children) {
            if(visited[child.first->start] == 0) {
                ++nodes_generated;
                child.first->prev_node = CurrNode;
                queue.push(child.first);
                ++visited[child.first->start];
            }
        }

    }

    while(CurrNode->prev_node !=  NULL && flag) {
        std::ostringstream path_stream;
        path_stream << CurrNode->prev_node->start << " to "<< CurrNode->start
                    << ", " << CurrNode->prev_node->children[CurrNode] << ".0 km\n";
        std::string curr_path = path_stream.str();
        paths.push_back(curr_path);
        path += CurrNode->prev_node->children[CurrNode]; 
        CurrNode = CurrNode->prev_node;
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
    std::ifstream myfile (argv[1]);
    std::unordered_map<std::string, TreeNode*> in_map;
    TreeNode* StartNode = NULL;
    
    // construct a Tree based on input file 
    if (myfile.is_open())
    {
        while ( myfile >> start >> dest >> distance ) {
            TreeNode* NodeStart = in_map[start];
            TreeNode* NodeDest = in_map[dest];
            
            if(in_map[dest] == 0) {
                NodeDest = new TreeNode(dest);
                in_map[dest] = NodeDest;
            }
            
            if(in_map[start] == 0) {
                NodeStart = new TreeNode(start);
                in_map[start] = NodeStart;
            }
            
            if(argv[2] == start && StartNode == NULL) {
                StartNode = NodeStart;
            } else if(argv[2] == dest && StartNode == NULL) {
                StartNode = NodeDest;
            }

            NodeStart->AddChild(NodeDest, distance);
            NodeDest->AddChild(NodeStart, distance);
        }

        myfile.close();
    }

    else std::cout << "Unable to open file"; 
    
    GetUninformedPath(StartNode, argv[3]);
    return 0; 
}
