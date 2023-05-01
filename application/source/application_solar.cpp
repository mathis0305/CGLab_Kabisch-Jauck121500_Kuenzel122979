#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 
using namespace gl;
#include <iostream>
//dont load gl bindings from glfw
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
//
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/matrix_inverse.hpp>
//#include <glm/gtc/type_ptr.hpp>



ApplicationSolar::ApplicationSolar(std::string const& resource_path)
	:Application{ resource_path }
	, planet_object{}
	, m_view_transform{ glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f}) }
	, m_view_projection{ utils::calculate_projection_matrix(initial_aspect_ratio) }
{
	
	initializeGeometry();
	initializeSceneGraph();
	initializeShaderPrograms();
}

ApplicationSolar::~ApplicationSolar() {
	glDeleteBuffers(1, &planet_object.vertex_BO);
	glDeleteBuffers(1, &planet_object.element_BO);
	glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

void ApplicationSolar::render() const {
	root -> render(m_shaders, m_view_transform);
	/*// bind shader to upload uniforms
	glUseProgram(m_shaders.at("planet").handle);

	glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{ 0.0f, 1.0f, 0.0f });
	model_matrix = glm::translate(model_matrix, glm::fvec3{ 0.0f, 0.0f, -1.0f });
	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
		1, GL_FALSE, glm::value_ptr(model_matrix));

	// extra matrix for normal transformation to keep them orthogonal to surface
	glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
		1, GL_FALSE, glm::value_ptr(normal_matrix));

	// bind the VAO to draw
	glBindVertexArray(planet_object.vertex_AO);

	// draw bound vertex array using bound shader
	glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);*/
}

void ApplicationSolar::uploadView() {
	// vertices are transformed in camera space, so camera transform must be inverted
	glm::fmat4 view_matrix = glm::inverse(m_view_transform);
	// upload matrix to gpu
	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
		1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {
	// upload matrix to gpu
	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
		1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() {
	// bind shader to which to upload unforms
	glUseProgram(m_shaders.at("planet").handle);
	// upload uniform values to new locations
	uploadView();
	uploadProjection();
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
	// store shader program objects in container
	m_shaders.emplace("planet", shader_program{ {{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
											 {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}} });
	// request uniform locations for shader program
	m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
	m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
	m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
	m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
}

// load models
void ApplicationSolar::initializeGeometry() {
	std::cout << "InitializeGeometry" << std::endl;
	
	model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

	// generate vertex array object
	glGenVertexArrays(1, &planet_object.vertex_AO);
	// bind the array for attaching buffers
	glBindVertexArray(planet_object.vertex_AO);

	// generate generic buffer
	glGenBuffers(1, &planet_object.vertex_BO);
	// bind this as an vertex array buffer containing all attributes
	glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
	// configure currently bound array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

	// activate first attribute on gpu
	glEnableVertexAttribArray(0);
	// first attribute is 3 floats with no offset & stride
	glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
	// activate second attribute on gpu
	glEnableVertexAttribArray(1);
	// second attribute is 3 floats with no offset & stride
	glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

	// generate generic buffer
	glGenBuffers(1, &planet_object.element_BO);
	// bind this as an vertex array buffer containing all attributes
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
	// configure currently bound array buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

	// store type of primitive to draw
	planet_object.draw_mode = GL_TRIANGLES;
	// transfer number of indices to model object 
	planet_object.num_elements = GLsizei(planet_model.indices.size());
}

///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		m_view_transform = glm::translate(m_view_transform, glm::fvec3{ 0.0f, 0.0f, -0.1f });
		uploadView();
	}
	else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		m_view_transform = glm::translate(m_view_transform, glm::fvec3{ 0.0f, 0.0f, 0.1f });
		uploadView();
	}
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
	// mouse handling
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
	// recalculate projection matrix for new aspect ration
	m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
	// upload new projection matrix
	uploadProjection();
}

void ApplicationSolar::initializeSceneGraph()
{
	root = std::make_shared<Node>();
	//SceneGraph sceneGraph{"sceneGraph", root};

	std::shared_ptr<PointLightNode> point_light = std::make_shared<PointLightNode>(root, std::vector<std::shared_ptr<Node>>{}, "point_light", "models/sphere.obj", 1, glm::fmat4{}, glm::fmat4{}, glm::vec3{1, 1, 1}, 1);
	std::shared_ptr<GeometryNode> sun_geometry = std::make_shared<GeometryNode>(point_light, std::vector<std::shared_ptr<Node>>{}, "sun_geometry", "models/sphere.obj", 5, glm::fmat4{}, glm::fmat4{}, planet_object);

	std::shared_ptr<Node> mercury_holder = std::make_shared<Node>(root, std::vector<std::shared_ptr<Node>>{}, "mercury_holder", "models/sphere.obj", 1, glm::fmat4{}, glm::fmat4{});
	std::shared_ptr<Node> venus_holder = std::make_shared<Node>(root, std::vector<std::shared_ptr<Node>>{}, "venus_holder", "models/sphere.obj", 1, glm::fmat4{}, glm::fmat4{});
	std::shared_ptr<GeometryNode> mercury_geometry = std::make_shared<GeometryNode>(mercury_holder, std::vector<std::shared_ptr<Node>>{}, "mercury_geometry", "models/sphere.obj", 5, glm::fmat4{}, glm::fmat4{}, planet_object);
	std::shared_ptr<GeometryNode> venus_geometry = std::make_shared<GeometryNode>(venus_holder, std::vector<std::shared_ptr<Node>>{}, "venus_geometry", "models/sphere.obj", 5, glm::fmat4{}, glm::fmat4{}, planet_object);
	
	sun_geometry -> setLocalTransformation(glm::translate(glm::mat4(1), glm::fvec3{0.0f, 0.0f, 0.0f}));
	
	mercury_geometry -> setLocalTransformation(glm::translate(glm::fmat4(1), glm::fvec3{4.0f, 0.0f, 0.0f}));
	
	venus_geometry -> setLocalTransformation(glm::translate(glm::fmat4(1), glm::fvec3{8.0f, 0.0f, 0.0f}));

	/*auto t1 = glm::translate(glm::mat4(1), glm::vec3{ 0, 0, 0 });
	auto r = glm::rotate(glm::mat4(1), 180.0f, glm::vec3{ 0, 1, 0 });
	auto t2 = glm::translate(glm::mat4(1), glm::vec3{ 0, 0, 0 });
	glm::mat4 newMat = (t2 * r * t1) * glm::translate(glm::mat4(1), glm::vec3{ 10, 0, 0 });*/

	root->addChildren(point_light);
	root->addChildren(mercury_holder);
	root->addChildren(venus_holder);
	point_light->addChildren(sun_geometry);
	mercury_holder->addChildren(mercury_geometry);
	venus_holder->addChildren(venus_geometry);
}


// exe entry point
int main(int argc, char* argv[]) {
	Application::run<ApplicationSolar>(argc, argv, 3, 2);
}