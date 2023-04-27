#include "node.hpp"

Node::Node(Node _parent, std::vector<Node> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation) {
	parent = _parent;
	children = _children;
	name = _name;
	path = _path;
	depth = _depth;
	localTransformation = _localTransformation;
	worldTransformation = _worldTransformation;
}

Node::Node() {
	name = "";
}

Node Node::getParent()
{
	return parent;
}

void Node::setParent(Node _parent)
{
	parent = _parent;
}

Node Node::getChildren(std::string child)
{
	for (Node currentChild : children)
		if (currentChild.name == child)
			return currentChild;
	std::cout << "Child not found" << std::endl;
}

std::vector<Node> Node::getChildrenList()
{
	return children;
}

std::string Node::getName()
{
	return name;
}

std::string Node::getPath()
{
	return path;
}

int Node::getDepth()
{
	return depth;
}

glm::mat4 Node::getLocalTransformation()
{
	return localTransformation;
}

glm::mat4 Node::getWorldTransformation()
{
	return worldTransformation;
}

void Node::setLocalTransformation(glm::mat4 _localTransformation)
{
	localTransformation = _localTransformation;
}

void Node::setWorldTransformation(glm::mat4 _worldTransformation)
{
	worldTransformation = _worldTransformation;
}

void Node::addChildren(Node newChild)
{
	children.push_back(newChild);
}

std::string Node::removeChildren(Node childToDelete)
{
	std::string currentName = "";
	for (int i = 0; i < children.size(); i++)
	{
		currentName = children[i].name;
		if (currentName == childToDelete.name)
		{
			children.erase(children.begin() + i);
			return currentName;
		}
	}
	return "";
}

void Node::printGraph()
{
	std::cout << name << std::endl;
	for (Node child : children)
		child.printGraph();
}