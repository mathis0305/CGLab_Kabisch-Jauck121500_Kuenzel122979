#include "node.hpp"


Node::Node(std::shared_ptr<Node> _parent, std::vector<std::shared_ptr<Node>> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation) :
	parent(_parent),
	children(_children),
	name(_name),
	path(_path),
	depth(_depth),
	localTransformation(_localTransformation),
	worldTransformation(_worldTransformation)
	{}

Node::Node() {
	name = "";
}

Node Node::getParent()
{
	return *parent;
}

void Node::setParent(std::shared_ptr<Node> _parent)
{
	parent = _parent;
}

std::shared_ptr<Node> Node::getChildren(std::string child)
{
	for (auto currentChild : children)
		if (currentChild -> name == child)
			return currentChild;
	std::cout << "Child not found" << std::endl;
	return nullptr;
}

std::vector<std::shared_ptr<Node>> Node::getChildrenList()
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

glm::mat4 Node::getWorldTransformation() const
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

void Node::addChildren(std::shared_ptr<Node> newChild)
{
	children.push_back(newChild);
}

std::string Node::removeChildren(Node childToDelete)
{
	std::string currentName = "";
	for (int i = 0; i < children.size(); i++)
	{
	    currentName = children[i] -> name;
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
	for (auto child : children)
		child -> printGraph();
}

void Node::render(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform) const {
	std::cout << children.size() << std::endl;
	for (auto& child : children) {
		
		child -> render(m_shaders, m_view_transform);
	}
}
