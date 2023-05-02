#ifndef POINT_LIGHT_NODE_HPP
#define POINT_LIGHT_NODE_HPP

#include "node.hpp"

class PointLightNode : public Node {
 public:
	 PointLightNode::PointLightNode();
     PointLightNode::PointLightNode(std::shared_ptr<Node> _parent, std::vector<std::shared_ptr<Node>> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation, glm::vec3 _lightColor, float _lightIntensity);
	
	 glm::vec3 getLightColor();
	 void setLightColor(glm::vec3 _lightColor);
	 float getLightIntensity();
	 void setLightIntensity(float _lightIntensity);

 private:
	 glm::vec3 lightColor;
	 float lightIntensity;
};

#endif