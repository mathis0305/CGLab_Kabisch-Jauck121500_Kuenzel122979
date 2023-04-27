#ifndef POINT_LIGHT_NODE_HPP
#define POINT_LIGHT_NODE_HPP

#include "node.hpp"
#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class PointLightNode : public Node {
 public:
	 PointLightNode::PointLightNode();
     PointLightNode::PointLightNode(Node _parent, std::vector<Node> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation, glm::vec3 _lightColor, float _lightIntensity);
	
	 glm::vec3 getLightColor();
	 void setLightColor(glm::vec3 _lightColor);
	 float getLightIntensity();
	 void setLightIntensity(float _lightIntensity);

 private:
	 static glm::vec3 lightColor;
	 static float lightIntensity;
};

#endif