#include "geometry_node.hpp"


GeometryNode::GeometryNode(std::shared_ptr<Node> _parent, std::vector<std::shared_ptr<Node>> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation, model_object _geometry) :
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

void GeometryNode::render(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform) const {
	// bind shader to upload uniforms
	glUseProgram(m_shaders.at("planet").handle);

	//glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{ 0.0f, 1.0f, 0.0f });
	//model_matrix = glm::translate(model_matrix, glm::fvec3{ 0.0f, 0.0f, -1.0f });
	glm::mat4 model_matrix = getWorldTransformation();
	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
		1, GL_FALSE, glm::value_ptr(model_matrix));

	// extra matrix for normal transformation to keep them orthogonal to surface
	glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
		1, GL_FALSE, glm::value_ptr(normal_matrix));

	// bind the VAO to draw
	glBindVertexArray(geometry.vertex_AO);

	// draw bound vertex array using bound shader
	glDrawElements(geometry.draw_mode, geometry.num_elements, model::INDEX.type, NULL);

	for (auto child : children)
		child -> render(m_shaders, m_view_transform);
}
