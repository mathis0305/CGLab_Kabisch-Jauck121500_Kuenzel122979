#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include "node.hpp"
#include "model.hpp"
#include "structs.hpp"
#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class GeometryNode : public Node {
 public:
	 GeometryNode::GeometryNode();
	 GeometryNode::GeometryNode(Node* _parent, std::vector<Node> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation, model_object _geometry);
	 GeometryNode::GeometryNode(model_object _geometry);
	 model getGeometry();
	 void setGeometry(model_object _model);

 
 private:
	 model_object geometry;

};

#endif