#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class Node {
 public:
	 Node::Node();
	 Node::Node(Node* _parent, std::vector<Node> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation);
	 Node getParent();
	 void setParent(Node* _parent);
	 Node getChildren(std::string child);
	 std::vector<Node> getChildrenList();
	 std::string getName();
	 std::string getPath();
	 int getDepth();
	 glm::mat4 getLocalTransformation();
	 glm::mat4 getWorldTransformation();
	 void setLocalTransformation(glm::mat4 _localTransformation);
	 void setWorldTransformation(glm::mat4 _worldTransformation);
	 void addChildren(Node newChild);
	 std::string removeChildren(Node childToDelete);
	 void printGraph();

 
 private:
	 Node *parent;
	 std::vector<Node> children;
	 std::string name;
	 std::string path;
	 int depth;
	 glm::mat4 localTransformation;
	 glm::mat4 worldTransformation;
};

#endif