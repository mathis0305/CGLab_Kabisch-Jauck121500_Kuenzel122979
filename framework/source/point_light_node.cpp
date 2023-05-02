#include "point_light_node.hpp"

//full constructor for point light node
PointLightNode::PointLightNode(std::shared_ptr<Node> _parent, std::vector<std::shared_ptr<Node>> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation, glm::vec3 _lightColor, float _lightIntensity) :
	Node(_parent, _children, _name, _path, _depth, _localTransformation, _worldTransformation, 1.0f),
	lightColor(_lightColor),
	lightIntensity(_lightIntensity)
{}

//default constructor for point light node
PointLightNode::PointLightNode()
{
	Node();
}

//getter for color of light
glm::vec3 PointLightNode::getLightColor()
{
	return lightColor;
}

//setter for color of light
void PointLightNode::setLightColor(glm::vec3 _lightColor)
{
	lightColor = _lightColor;
}

//getter of light intensity
float PointLightNode::getLightIntensity()
{
	return lightIntensity;
}

//setter of light intensity
void PointLightNode::setLightIntensity(float _lightIntensity)
{
	lightIntensity = _lightIntensity;
}