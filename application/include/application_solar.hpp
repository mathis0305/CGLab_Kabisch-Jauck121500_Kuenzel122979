#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP
#define _USE_MATH_DEFINES

#include <glm/gtc/random.hpp>
#include <math.h>

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"
//#include "node.hpp"
#include "geometry_node.hpp"
#include "camera_node.hpp"
#include "scene_graph.hpp"
#include "point_light_node.hpp"

// gpu representation of model
class ApplicationSolar : public Application {
 public:
  // allocate and initialize objects
  ApplicationSolar(std::string const& resource_path);
  // free allocated objects
  ~ApplicationSolar();

  // react to key input
  void keyCallback(int key, int action, int mods);
  //handle delta mouse movement input
  void mouseCallback(double pos_x, double pos_y);
  //handle resizing
  void resizeCallback(unsigned width, unsigned height);

  // draw all objects
  void render() const;


 protected:
  void initializeShaderPrograms();
  void initializeGeometry();
  void initializeFramebuffer();

  //start function for planet initialization
  void initializeSceneGraph();
  // update uniform values
  void uploadUniforms();
  // upload projection matrix
  void uploadProjection();
  // upload view matrix
  void uploadView();

  //Root of the scene graph
  std::shared_ptr<Node> root;

  // cpu representation of model
  model_object planet_object;  
  model_object star_object;
  model_object orbit_object;
  // camera transform matrix
  glm::fmat4 m_view_transform;
  // camera projection matrix
  glm::fmat4 m_view_projection;

  bool cellShading = false;

  // framebuffer
  unsigned int fbo;
  // texture
  unsigned int texture;
  // render buffer
  unsigned int rbo;
  // screen size
  unsigned int width = 800;
  unsigned int height = 600;
};

#endif