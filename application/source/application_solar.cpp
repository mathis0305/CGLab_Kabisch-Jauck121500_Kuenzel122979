#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include <glbinding/gl/gl.h>

using namespace gl;
#include <iostream>

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
	//call render() on root node which then recursively calls render() on every child
	root->render(m_shaders, m_view_transform);
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
	//W for negative z direction
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		m_view_transform = glm::translate(m_view_transform, glm::fvec3{ 0.0f, 0.0f, -0.1f });
		uploadView();
	}
	//S for positive z direction
	else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		m_view_transform = glm::translate(m_view_transform, glm::fvec3{ 0.0f, 0.0f, 0.1f });
		uploadView();
	}
	//A for negative x direction
	else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		m_view_transform = glm::translate(m_view_transform, glm::fvec3{ -0.1f, 0.0f, 0.0f });
		uploadView();
	}
	//D for positive x direction
	else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		m_view_transform = glm::translate(m_view_transform, glm::fvec3{ 0.1f, 0.0f, 0.0f });
		uploadView();
	}
	//left shift for negative y direction
	else if (key == GLFW_KEY_LEFT_SHIFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		m_view_transform = glm::translate(m_view_transform, glm::fvec3{ 0.0f, -0.1f, 0.0f });
		uploadView();
	}
	//spacebar for positive y direction
	else if (key == GLFW_KEY_SPACE && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		m_view_transform = glm::translate(m_view_transform, glm::fvec3{ 0.0f, 0.1f, 0.0f });
		uploadView();
	}
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
	//slower mouse movement
	pos_x /= 150;
	pos_y /= 150;

	//give mouse position to m_view_transform matrix
	m_view_transform = glm::translate(m_view_transform, glm::fvec3{ pos_x, -pos_y, 0.0f });
	uploadView();
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
	//initialize root as parent node for planet holder, point light and camera
	root = std::make_shared<Node>();

	//initialize sceneGraph with root
	std::shared_ptr<SceneGraph> sceneGraph = std::make_shared<SceneGraph>("sceneGraph", root);

	//initializing celestial body holder as node and celestial body geometry as GeometryNode
	//Camera
	std::shared_ptr<CameraNode> camera = std::make_shared<CameraNode>(root, std::vector<std::shared_ptr<Node>>{}, "camera", "models/sphere.obj", 1, glm::fmat4{}, glm::fmat4{}, true, true, glm::mat4{});
	//Sun
	std::shared_ptr<PointLightNode> point_light = std::make_shared<PointLightNode>(root, std::vector<std::shared_ptr<Node>>{}, "point_light", "models/sphere.obj", 1, glm::fmat4{}, glm::fmat4{}, glm::vec3{ 1, 1, 1 }, 1);
	std::shared_ptr<GeometryNode> sun_geometry = std::make_shared<GeometryNode>(point_light, std::vector<std::shared_ptr<Node>>{}, "sun_geometry", "models/sphere.obj", 3, glm::fmat4{}, glm::fmat4{}, planet_object);
	//Mercury
	std::shared_ptr<Node> mercury_holder = std::make_shared<Node>(root, "mercury_holder", 2, 0.048f);
	std::shared_ptr<GeometryNode> mercury_geometry = std::make_shared<GeometryNode>(mercury_holder, "mercury_geometry", 3, planet_object);
	//Venus
	std::shared_ptr<Node> venus_holder = std::make_shared<Node>(root, "venus_holder", 2, 0.035f);
	std::shared_ptr<GeometryNode> venus_geometry = std::make_shared<GeometryNode>(venus_holder, "venus_geometry", 3, planet_object);
	//Earth
	std::shared_ptr<Node> earth_holder = std::make_shared<Node>(root, "earth_holder", 2, 0.030f);
	std::shared_ptr<GeometryNode> earth_geometry = std::make_shared<GeometryNode>(earth_holder, "earth_geometry", 3, planet_object);
	//Mars
	std::shared_ptr<Node> mars_holder = std::make_shared<Node>(root, "mars_holder", 2, 0.024f);
	std::shared_ptr<GeometryNode> mars_geometry = std::make_shared<GeometryNode>(mars_holder, "mars_geometry", 3, planet_object);
	//Jupiter
	std::shared_ptr<Node> jupiter_holder = std::make_shared<Node>(root, "jupiter_holder", 2, 0.013f);
	std::shared_ptr<GeometryNode> jupiter_geometry = std::make_shared<GeometryNode>(jupiter_holder, "jupiter_geometry", 3, planet_object);
	//Saturn
	std::shared_ptr<Node> saturn_holder = std::make_shared<Node>(root, "saturn_holder", 2, 0.010f);
	std::shared_ptr<GeometryNode> saturn_geometry = std::make_shared<GeometryNode>(saturn_holder, "saturn_geometry", 3, planet_object);
	//Uranus
	std::shared_ptr<Node> uranus_holder = std::make_shared<Node>(root, "uranus_holder", 2, 0.068f);
	std::shared_ptr<GeometryNode> uranus_geometry = std::make_shared<GeometryNode>(uranus_holder, "uranus_geometry", 3, planet_object);
	//Neptune
	std::shared_ptr<Node> neptune_holder = std::make_shared<Node>(root, "neptune_holder", 2, 0.054f);
	std::shared_ptr<GeometryNode> neptune_geometry = std::make_shared<GeometryNode>(neptune_holder, "neptune_geometry", 3, planet_object);
	//Moon of Earth
	std::shared_ptr<Node> moon_holder = std::make_shared<Node>(mercury_holder, "moon_holder", 2, 0.08f);
	std::shared_ptr<GeometryNode> moon_geometry = std::make_shared<GeometryNode>(moon_holder, "moon_geometry", 3, planet_object);

	//set distance to center of the scene with the x coordinate
	point_light->setLocalTransformation(glm::translate(glm::mat4(1), glm::fvec3{ 0.0f, 0.0f, 0.0f }));

	mercury_holder->setLocalTransformation(glm::translate(glm::fmat4(1), glm::fvec3{ 2.0f, 0.0f, 0.0f }));

	venus_holder->setLocalTransformation(glm::translate(glm::fmat4(1), glm::fvec3{ 4.0f, 0.0f, 0.0f }));

	earth_holder->setLocalTransformation(glm::translate(glm::fmat4(1), glm::fvec3{ 6.0f, 0.0f, 0.0f }));

	mars_holder->setLocalTransformation(glm::translate(glm::fmat4(1), glm::fvec3{ 8.0f, 0.0f, 0.0f }));

	jupiter_holder->setLocalTransformation(glm::translate(glm::fmat4(1), glm::fvec3{ 12.0f, 0.0f, 0.0f }));

	saturn_holder->setLocalTransformation(glm::translate(glm::fmat4(1), glm::fvec3{ 16.0f, 0.0f, 0.0f }));

	uranus_holder->setLocalTransformation(glm::translate(glm::fmat4(1), glm::fvec3{ 20.0f, 0.0f, 0.0f }));

	neptune_holder->setLocalTransformation(glm::translate(glm::fmat4(1), glm::fvec3{ 22.0f, 0.0f, 0.0f }));

	//set size of celestial bodies
	mercury_holder->setLocalTransformation(mercury_holder->getLocalTransformation() * glm::scale(glm::fmat4(1), glm::fvec3{ 0.1f, 0.1f, 0.1f }));

	venus_holder->setLocalTransformation(venus_holder->getLocalTransformation() * glm::scale(glm::fmat4(1), glm::fvec3{ 0.1f, 0.1f, 0.1f }));

	earth_holder->setLocalTransformation(earth_holder->getLocalTransformation() * glm::scale(glm::fmat4(1), glm::fvec3{ 0.1f, 0.1f, 0.1f }));

	mars_holder->setLocalTransformation(mars_holder->getLocalTransformation() * glm::scale(glm::fmat4(1), glm::fvec3{ 0.08f, 0.08f, 0.08f }));

	jupiter_holder->setLocalTransformation(jupiter_holder->getLocalTransformation() * glm::scale(glm::fmat4(1), glm::fvec3{ 0.5f, 0.5f, 0.5f }));

	saturn_holder->setLocalTransformation(saturn_holder->getLocalTransformation() * glm::scale(glm::fmat4(1), glm::fvec3{ 0.45f, 0.45f, 0.45f }));

	uranus_holder->setLocalTransformation(uranus_holder->getLocalTransformation() * glm::scale(glm::fmat4(1), glm::fvec3{ 0.3f, 0.3f, 0.3f }));

	neptune_holder->setLocalTransformation(neptune_holder->getLocalTransformation() * glm::scale(glm::fmat4(1), glm::fvec3{ 0.3f, 0.3f, 0.3f }));

	point_light->setLocalTransformation(point_light->getLocalTransformation() * glm::scale(glm::fmat4(1), glm::fvec3{ 1.0f, 1.0f, 1.0f }));

	//size of moon
	//moonMat is generated since location is inhereted by earth
	auto moonMat = glm::translate(glm::fmat4(1), glm::fvec3{ 1.6f, 0.0f, 0.0f });
	moonMat = moonMat * glm::scale(glm::fmat4(1), glm::fvec3{ 0.3f, 0.3f, 0.3f });
	moon_holder->setLocalTransformation(moonMat);

	//add holder and point light to root node 
	root->addChildren(camera);
	root->addChildren(point_light);
	root->addChildren(mercury_holder);
	root->addChildren(venus_holder);
	root->addChildren(earth_holder);
	root->addChildren(mars_holder);
	root->addChildren(jupiter_holder);
	root->addChildren(saturn_holder);
	root->addChildren(uranus_holder);
	root->addChildren(neptune_holder);

	//add Moon_holder to Earth_holder
	//Moon holder is child of earth holder
	earth_holder->addChildren(moon_holder);

	//add geometry to holder
	point_light->addChildren(sun_geometry);
	mercury_holder->addChildren(mercury_geometry);
	venus_holder->addChildren(venus_geometry);
	earth_holder->addChildren(earth_geometry);
	mars_holder->addChildren(mars_geometry);
	jupiter_holder->addChildren(jupiter_geometry);
	saturn_holder->addChildren(saturn_geometry);
	uranus_holder->addChildren(uranus_geometry);
	neptune_holder->addChildren(neptune_geometry);
	moon_holder->addChildren(moon_geometry);

	//sceneGraph->printGraph();
}


// exe entry point
int main(int argc, char* argv[]) {
	Application::run<ApplicationSolar>(argc, argv, 3, 2);
}