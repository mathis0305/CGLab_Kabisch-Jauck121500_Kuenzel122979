#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP


#include "node.hpp"
#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class SceneGraph {
 public:
	 
	 std::string getName();
	 std::shared_ptr<Node> getRoot();
	 void printGraph();


 
 private:
	 std::string name;
	 std::shared_ptr<Node> root;

	 void setName(std::string _name);
	 void setRoot(std::shared_ptr<Node> _root);

protected:
	SceneGraph::SceneGraph();
	SceneGraph::SceneGraph(std::string _name, std::shared_ptr<Node> _root);
};

#endif