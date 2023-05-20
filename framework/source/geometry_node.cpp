#include "geometry_node.hpp"

//full constructor for geometry node
GeometryNode::GeometryNode(std::shared_ptr<Node> _parent, std::vector<std::shared_ptr<Node>> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation, model_object _geometry, std::string _type, glm::vec3 _color) :
	Node(_parent, _children, _name, _path, _depth, _localTransformation, _worldTransformation, 1.0f),
	geometry(_geometry),
	type(_type),
	color(_color)
{}

//constructor of geometry node for important variabes
GeometryNode::GeometryNode(std::shared_ptr<Node> _parent, std::string _name, int _depth, model_object _geometry, std::string _type, glm::vec3 _color) :
	Node(_parent, _name, _depth, 0.0f),
	geometry(_geometry),
	type(_type),
	color(_color)
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

//getter for color of geometry node
glm::vec3 GeometryNode::getColor()
{
	return color;
}

//setter for color of geometry node
void GeometryNode::setColor(glm::vec3 _color)
{
	color = _color;
}

//getter for type of geometry node
std::string GeometryNode::getType()
{
	return type;
}
void GeometryNode::planetRender(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform) {
	// bind shader to upload uniforms
	glUseProgram(m_shaders.at("planet").handle);

	glUniform3f(m_shaders.at("planet").u_locs.at("PlanetColor"), getColor().x / 255.f, getColor().y / 255.f, getColor().z / 255.f);
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
	// bind shader to upload uniforms
	glUseProgram(m_shaders.at("orbit").handle);

	//set position around sun for every planet orbit
	glm::fmat4 model_matrix = getLocalTransformation();

	//set position around earth for moon orbit
	if (getName() == "moon_orbit")
		model_matrix = getWorldTransformation() * getLocalTransformation();

	glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ModelMatrix"),1, GL_FALSE, glm::value_ptr(model_matrix));

	// bind the VAO to draw
	glBindVertexArray(geometry.vertex_AO);

	// draw bound vertex array using bound shader
	glDrawArrays(geometry.draw_mode, GLint(0), geometry.num_elements);

	
}

void GeometryNode::starRender(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform) {
	// bind shader to upload uniforms
	glUseProgram(m_shaders.at("stars").handle);

	// bind the VAO to draw
	glBindVertexArray(geometry.vertex_AO);

	// draw bound vertex array using bound shader
	glDrawArrays(geometry.draw_mode, GLint(0), geometry.num_elements);
}

void GeometryNode::render(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform) {

	//call specific render function for every type of geometry node
	if (getType() == "stars")
		starRender(m_shaders,m_view_transform);

	else if(getType() == "orbit")
		orbitRender(m_shaders, m_view_transform);

	else
		planetRender(m_shaders, m_view_transform);

	//recursevely call function on child nodes
	for (auto child : children)
		child->render(m_shaders, m_view_transform);
}
