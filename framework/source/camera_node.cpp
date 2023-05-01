#include "camera_node.hpp"

CameraNode::CameraNode(std::shared_ptr<Node> _parent, std::vector<std::shared_ptr<Node>> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation, bool _isPerspective, bool _isEnabled, glm::mat4 _projectionMatrix) :
	Node(_parent, _children,  _name, _path, _depth, _localTransformation, _worldTransformation),
	isPerspective(_isPerspective),
	isEnabled(_isEnabled),
	projectionMatrix(_projectionMatrix)
	{}

CameraNode::CameraNode() {
	Node();
}

bool CameraNode::getPerspective()
{
	return isPerspective;
}

bool CameraNode::getEnabled()
{
	return isEnabled;
}

void CameraNode::setEnabled( bool _isEnabled)
{
	isEnabled = _isEnabled;
}

glm::mat4 CameraNode::getProjectionMatrix()
{
	return projectionMatrix;
}


void CameraNode::setProjectionMatrix(glm::mat4 _projectionMatrix)
{
	projectionMatrix = _projectionMatrix;
}

