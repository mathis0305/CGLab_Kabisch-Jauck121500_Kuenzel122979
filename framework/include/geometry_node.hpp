#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include "node.hpp"
#include "point_light_node.hpp"

class GeometryNode : public Node {
 public:
	 //constructors of geometry nodes
	 GeometryNode::GeometryNode();
	 GeometryNode::GeometryNode(std::shared_ptr<Node> _parent, std::vector<std::shared_ptr<Node>> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation, model_object _geometry, std::string _type, glm::vec3 _color, texture_object _texture);
	 GeometryNode::GeometryNode(std::shared_ptr<Node> _parent, std::vector<std::shared_ptr<Node>> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation, model_object _geometry, std::string _type, glm::vec3 _color);
	 GeometryNode::GeometryNode(std::shared_ptr<Node> _parent, std::string _name, int _depth,model_object _geometry,std::string _type, glm::vec3 _color, texture_object _texture);
	 GeometryNode::GeometryNode(std::shared_ptr<Node> _parent, std::string _name, int _depth,model_object _geometry,std::string _type, glm::vec3 _color);
	 
	 //member function of geometry nodes
	 model_object getGeometry();
	 void setGeometry(model_object _model);
	 glm::vec3 getColor();
	 void setColor(glm::vec3 _color);
	 std::string getType();
	 void render(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform,bool cellShading) override;
	 void starRender(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform);
	 void planetRender(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform, bool cellShading);
	 void orbitRender(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform);

 //member variable of geometry node
 private:
	 model_object geometry;
	 glm::vec3 color;
	 //specifies if it is a planet, orbit or stars
	 std::string type;
	 //the displayed texture as png
	 texture_object texture;

};

#endif