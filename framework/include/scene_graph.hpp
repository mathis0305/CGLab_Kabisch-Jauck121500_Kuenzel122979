#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP


#include "node.hpp"
#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class SceneGraph {
 public:
	 
	 std::string getName();
	 Node getRoot();
	 void printGraph();


 
 private:
	 std::string name;
	 Node root;

	 void setName(std::string _name);
	 void setRoot(Node _root);

protected:
	SceneGraph::SceneGraph();
	SceneGraph::SceneGraph(std::string _name, Node _root);
};

#endif