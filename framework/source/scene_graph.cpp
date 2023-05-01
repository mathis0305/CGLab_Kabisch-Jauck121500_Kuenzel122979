#include "scene_graph.hpp"

SceneGraph::SceneGraph(std::string _name, std::shared_ptr<Node> _root) :
	name(_name),
	root(_root)
	{}

SceneGraph::SceneGraph() {
	name = "";
}


std::string Node::getName()
{
	return name;
}

void SceneGraph::setName(std::string _name)
{
	name = _name;
}

std::shared_ptr<Node> SceneGraph::getRoot()
{
	return root;
}

void SceneGraph::setRoot(std::shared_ptr<Node> _root)
{
	root = _root;
}

void SceneGraph::printGraph()
{
	root -> printGraph();
}