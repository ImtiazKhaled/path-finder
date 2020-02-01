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
            
            if(argv[2] == start && StartNode == NULL) {
                StartNode = NodeStart;
            } else if(argv[2] == dest && StartNode == NULL) {
                StartNode = NodeDest;
            }
            
            if(in_map[dest] == 0) {
                NodeDest = new TreeNode(dest);
                in_map[dest] = NodeDest;
            }
            
            if(in_map[start] == 0) {
                NodeStart = new TreeNode(start);
                in_map[start] = NodeStart;
            }
            
            NodeStart->AddChild(NodeDest, distance);
            NodeDest->AddChild(NodeStart, distance);
        }

        myfile.close();
    }

    else std::cout << "Unable to open file"; 
    
    return 0;
}
