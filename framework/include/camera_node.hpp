#ifndef CAMERA_NODE_HPP
#define CAMERA_NODE_HPP

#include "node.hpp"
#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class CameraNode: public Node {
 public:
	 CameraNode::CameraNode();
	 CameraNode::CameraNode(Node* _parent, std::vector<Node> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation, bool _isPerspective, bool _isEnabled, glm::mat4 _projectionMatrix);
	 bool getPerspective();
	 bool getEnabled();
	 void setEnabled(bool _isEnabled);
	 glm::mat4 getProjectionMatrix();
	 void setProjectionMatrix(glm::mat4 _projectionMatrix);


 
 private:
	 bool isPerspective;
	 bool isEnabled;
	 glm::mat4 projectionMatrix;

};

#endif