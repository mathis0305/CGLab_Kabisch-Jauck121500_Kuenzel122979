#include "scene_graph.hpp"

//full contructor of scene graph
SceneGraph::SceneGraph(std::string _name, std::shared_ptr<Node> _root) :
	name(_name),
	root(_root)
	{}

//default constructor of scene graph
SceneGraph::SceneGraph() {
	name = "";
}

//getter for name of scene graph
std::string SceneGraph::getName()
{
	return name;
}

//setter for name of scene graph
void SceneGraph::setName(std::string _name)
{
	name = _name;
}

//getter for root node
std::shared_ptr<Node> SceneGraph::getRoot()
{
	return root;
}

//setter for root node
void SceneGraph::setRoot(std::shared_ptr<Node> _root)
{
	root = _root;
}

//print the name of every child recursively
void SceneGraph::printGraph()
{
	root -> printGraph();
}