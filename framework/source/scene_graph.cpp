#include "scene_graph.hpp"

SceneGraph::SceneGraph(std::string _name, Node _root) :
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

Node SceneGraph::getRoot()
{
	return root;
}

void SceneGraph::setRoot(Node _root)
{
	root = _root;
}

void SceneGraph::printGraph()
{
	root.printGraph();
}