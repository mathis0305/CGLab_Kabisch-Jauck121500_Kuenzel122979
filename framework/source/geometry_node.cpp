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
void GeometryNode::planetRender(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform,bool cellShading) {
	std::shared_ptr<Node> root = getParent();

	//Assign root
	while (root->getName() != "root")
	{
		root = root->getParent();
	}

	//Get sunlight
	auto point_light = root->getChildren("point_light");

	// bind shader to upload uniforms
	glUseProgram(m_shaders.at("planet").handle);

	//Assign different color for each planet
	glUniform3f(m_shaders.at("planet").u_locs.at("PlanetColor"), getColor().x, getColor().y, getColor().z);

	//Assign ambient color
	glUniform3f(m_shaders.at("planet").u_locs.at("AmbientColor"), getColor().x, getColor().y, getColor().z);

	//Assign ambient light intensity
	float ambient_intensity = 0.15f;

	//Give sun more ambient light
	if (getName() == "sun_geometry")
		ambient_intensity = 1.0f;

	//Set ambient light
	glUniform1f(m_shaders.at("planet").u_locs.at("AmbientIntensity"), ambient_intensity);

	//Set light position
	auto pos = point_light->getLocalTransformation() * glm::vec4{0, 0, 0, 1};
	glUniform3fv(m_shaders.at("planet").u_locs.at("LightPosition"), 1, glm::value_ptr(pos));

	//Set light intensity
	glUniform1f(m_shaders.at("planet").u_locs.at("LightIntensity"), point_light->getLightIntensity());

	//Set light color
	glm::vec3 light_color = point_light->getLightColor();
	glUniform3f(m_shaders.at("planet").u_locs.at("LightColor"), light_color.x, light_color.y, light_color.z);
	

	//enabling and disabeling cell shading shader
	auto loc_cel = glGetUniformLocation(m_shaders.at("planet").handle, "Cel");
	glUniform1i(loc_cel, cellShading);

	glUniform3fv(m_shaders.at("planet").u_locs.at("CameraPosition"), 1,
		glm::value_ptr(m_view_transform * glm::vec4{0, 0, 0, 1}));

	//get initial location in world
	//set new local transformation and world transformation of children
	//multiply local transformation to matrix for further use 
	glm::fmat4 model_matrix = getWorldTransformation();
	setLocalTransformation(glm::rotate(glm::mat4(1), glm::radians(0.1f), glm::fvec3{ 0.0f, 1.0f, 0.0f }) * getLocalTransformation());
	model_matrix = model_matrix * getLocalTransformation();


	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
		1, GL_FALSE, glm::value_ptr(model_matrix));

	// extra matrix for normal transformation to keep them orthogonal to surface
	glm::fmat4 normal_matrix = glm::inverseTranspose(model_matrix);
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

void GeometryNode::render(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform, bool cellShading) {

	//call specific render function for every type of geometry node
	if (getType() == "stars")
		starRender(m_shaders,m_view_transform);

	else if(getType() == "orbit")
		orbitRender(m_shaders, m_view_transform);

	else
		planetRender(m_shaders, m_view_transform,cellShading);

	//recursevely call function on child nodes
	for (auto child : children)
		child->render(m_shaders, m_view_transform, cellShading);
}
