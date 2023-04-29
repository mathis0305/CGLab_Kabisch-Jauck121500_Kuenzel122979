#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include "node.hpp"
#include "model.hpp"
#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class GeometryNode : public Node {
 public:
	 GeometryNode::GeometryNode();
	 GeometryNode::GeometryNode(Node* _parent, std::vector<Node> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation, model _geometry);
	 GeometryNode::GeometryNode(model _geometry);
	 model getGeometry();
	 void setGeometry(model _model);

 
 private:
	 model geometry;

};

#endif