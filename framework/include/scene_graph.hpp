#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP


#include "node.hpp"

class SceneGraph {
public:

	SceneGraph::SceneGraph();
	SceneGraph::SceneGraph(std::string _name, std::shared_ptr<Node> _root);

	std::string getName();
	void setName(std::string _name);
	std::shared_ptr<Node> getRoot();
	void setRoot(std::shared_ptr<Node> _root);
	void printGraph();



private:
	std::string name;
	std::shared_ptr<Node> root;
};

#endif