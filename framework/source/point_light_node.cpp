#include "point_light_node.hpp"


PointLightNode::PointLightNode(std::shared_ptr<Node> _parent, std::vector<std::shared_ptr<Node>> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation, glm::vec3 _lightColor, float _lightIntensity) :
	Node(_parent,_children, _name, _path,  _depth,  _localTransformation,  _worldTransformation, 1.0f),
	lightColor(_lightColor),
	lightIntensity(_lightIntensity)
	{}

PointLightNode::PointLightNode()
{
	Node();
}

glm::vec3 PointLightNode::getLightColor()
{
	return lightColor;
}

void PointLightNode::setLightColor(glm::vec3 _lightColor)
{
	lightColor = _lightColor;
}

float PointLightNode::getLightIntensity()
{
	return lightIntensity;
}

void PointLightNode::setLightIntensity(float _lightIntensity)
{
	lightIntensity = _lightIntensity;
}