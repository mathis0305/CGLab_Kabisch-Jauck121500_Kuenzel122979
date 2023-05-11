#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include "node.hpp"

class GeometryNode : public Node {
 public:
	 //constructors of geometry nodes
	 GeometryNode::GeometryNode();
	 GeometryNode::GeometryNode(std::shared_ptr<Node> _parent, std::vector<std::shared_ptr<Node>> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation, model_object _geometry);
	 GeometryNode::GeometryNode(std::shared_ptr<Node> _parent, std::string _name, int _depth,model_object _geometry);
	 
	 //member function of geometry nodes
	 model_object getGeometry();
	 void setGeometry(model_object _model);
	 void render(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform) override;
	 void starRender(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform);
	 void planetRender(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform);

 //member variable of geometry node
 private:
	 model_object geometry;

};

#endif