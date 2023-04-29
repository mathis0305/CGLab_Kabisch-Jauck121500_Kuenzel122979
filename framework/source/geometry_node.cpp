#include "geometry_node.hpp"


GeometryNode::GeometryNode(Node* _parent, std::vector<Node> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation, model_object _geometry) :
	Node(_parent,_children, _name, _path,  _depth,  _localTransformation,  _worldTransformation),
	geometry(_geometry)
	{}

GeometryNode::GeometryNode() {
	Node();
}

model_object GeometryNode::getGeometry()
{
	return geometry;
}

void GeometryNode::setGeometry(model_object _geometry)
{
	geometry = _geometry;
}

