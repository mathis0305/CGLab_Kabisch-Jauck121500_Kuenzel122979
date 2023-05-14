#include "geometry_node.hpp"

//full constructor for geometry node
GeometryNode::GeometryNode(std::shared_ptr<Node> _parent, std::vector<std::shared_ptr<Node>> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation, model_object _geometry, std::string _type) :
	Node(_parent, _children, _name, _path, _depth, _localTransformation, _worldTransformation, 1.0f),
	geometry(_geometry),
	type(_type)
{}

//constructor of geometry node for important variabes
GeometryNode::GeometryNode(std::shared_ptr<Node> _parent, std::string _name, int _depth, model_object _geometry, std::string _type) :
	Node(_parent, _name, _depth, 0.0f),
	geometry(_geometry),
	type(_type)
{}

//default constructor for geometry node
GeometryNode::GeometryNode() {
	Node();
}

//getter for model_object of geometry node
model_object GeometryNode::getGeometry()
{
	return geometry;
}

//setter for model_object of geometry node
void GeometryNode::setGeometry(model_object _geometry)
{
	geometry = _geometry;
}

std::string GeometryNode::getType()
{
	return type;
}
void GeometryNode::planetRender(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform) {
	// bind shader to upload uniforms
	glUseProgram(m_shaders.at("planet").handle);

	//get initial location in world
	//set new local transformation and world transformation of children
	//multiply local transformation to matrix for further use 
	glm::fmat4 model_matrix = getWorldTransformation();
	setLocalTransformation(glm::rotate(glm::mat4(1), glm::radians(0.1f), glm::fvec3{ 0.0f, 1.0f, 0.0f }) * getLocalTransformation());
	model_matrix = model_matrix * getLocalTransformation();


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

}


void GeometryNode::orbitRender(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform) {
	glUseProgram(m_shaders.at("orbit").handle); 
	glm::fmat4 model_matrix = getWorldTransformation();
	glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ModelViewMatrix"),1, GL_FALSE, glm::value_ptr(model_matrix));
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertex_BO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * getGeometry().data.size(), getGeometry().data.data(), GL_STATIC_DRAW);
	glBindVertexArray(geometry.vertex_AO);
	glDrawArrays(geometry.draw_mode, GLint(0), geometry.num_elements);

	
}

void GeometryNode::starRender(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform) {
	glUseProgram(m_shaders.at("stars").handle);
	glBindVertexArray(geometry.vertex_AO);
	glDrawArrays(geometry.draw_mode, GLint(0), geometry.num_elements);
}

void GeometryNode::render(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform) {

	if (getType() == "stars")
	{
		starRender(m_shaders,m_view_transform);
	}
	else if(getType() == "orbit")
	{
		orbitRender(m_shaders, m_view_transform);
	}
	else
	{
		planetRender(m_shaders, m_view_transform);
	}
	//recursevely call function on child nodes
	for (auto child : children)
		child->render(m_shaders, m_view_transform);
}
