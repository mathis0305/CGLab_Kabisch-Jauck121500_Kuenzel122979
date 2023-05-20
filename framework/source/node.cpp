#include "node.hpp"

//full constructor of node
Node::Node(std::shared_ptr<Node> _parent, std::vector<std::shared_ptr<Node>> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation, float _rotationSpeed) :
	parent(_parent),
	children(_children),
	name(_name),
	path(_path),
	depth(_depth),
	localTransformation(_localTransformation),
	worldTransformation(_worldTransformation),
	rotationSpeed(_rotationSpeed)
{}

//constructor of node for important variables
Node::Node(std::shared_ptr<Node> _parent, std::string _name, int _depth, float _rotationSpeed) :
	parent(_parent),
	children(std::vector<std::shared_ptr<Node>>{}),
	name(_name),
	path("models/sphere.obj"),
	depth(_depth),
	localTransformation(glm::fmat4{}),
	worldTransformation(glm::fmat4{}),
	rotationSpeed(_rotationSpeed)
{}

//default constructor of node
Node::Node() {
	name = "root";
	rotationSpeed = 0.0f;
}

//getter for parent node
std::shared_ptr<Node> Node::getParent()
{
	return parent;
}

//setter for parent node
void Node::setParent(std::shared_ptr<Node> _parent)
{
	parent = _parent;
}

//getter for specific child node selected by name
std::shared_ptr<Node> Node::getChildren(std::string child)
{
	for (auto currentChild : children)
		if (currentChild->name == child)
			return currentChild;
	std::cout << "Child not found" << std::endl;
	return nullptr;
}

//getter for vector of child nodes
std::vector<std::shared_ptr<Node>> Node::getChildrenList()
{
	return children;
}

//getter for name of node
std::string Node::getName()
{
	return name;
}

//getter for path of node
std::string Node::getPath()
{
	return path;
}

//getter for depth in scene graph of node
int Node::getDepth()
{
	return depth;
}

//getter for rotation speed of node
float Node::getRotationSpeed()
{
	return rotationSpeed;
}

//getter for local transformation of node
glm::mat4 Node::getLocalTransformation() const
{
	return localTransformation;
}

//getter for world transformation for node
glm::mat4 Node::getWorldTransformation() const
{
	return worldTransformation;
}

//set local transformation for node and world transformation for child node
void Node::setLocalTransformation(glm::mat4 _localTransformation)
{
	localTransformation = _localTransformation;

	for (auto& child : children) {
		child->setWorldTransformation(worldTransformation * localTransformation);
	}
}

//set world transformation for node and child nodes
void Node::setWorldTransformation(glm::mat4 _worldTransformation)
{
	worldTransformation = _worldTransformation;
	for (auto& child : children) {
		child->setWorldTransformation(worldTransformation * localTransformation);
	}
}

//add child node to vector of children
void Node::addChildren(std::shared_ptr<Node> newChild)
{
	children.push_back(newChild);
}

//iterates through vector of children
//if child node is found it gets erased and the name will be returned
//if child node is not found, empty string is returned
std::string Node::removeChildren(Node childToDelete)
{
	std::string currentName = "";
	for (int i = 0; i < children.size(); i++)
	{
		currentName = children[i]->name;
		if (currentName == childToDelete.name)
		{
			children.erase(children.begin() + i);
			return currentName;
		}
	}
	return "";
}
//prints name of node and calls itself iteratively
void Node::printGraph()
{
	std::cout << name << std::endl;
	for (auto child : children)
		child->printGraph();
}

glm::vec3 Node::getLightColor() {
	return glm::vec3{};
}

void Node::setLightColor(glm::vec3 _lightColor) {
	return;
}

float Node::getLightIntensity() {
	return 0.0f;
}

void Node::setLightIntensity(float _lightIntensity) {
	return;
}

void Node::render(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform) {

	//rotate planet holder around sun
	glm::fmat4 model_matrix = getWorldTransformation();
	setLocalTransformation(glm::rotate(glm::mat4(1), glm::radians(getRotationSpeed() / 10), glm::fvec3{ 0.0f, 1.0f, 0.0f }) * getLocalTransformation());
	model_matrix = model_matrix * getLocalTransformation();

	//recursively call function on child nodes
	for (auto& child : children)
		child->render(m_shaders, m_view_transform);
}
