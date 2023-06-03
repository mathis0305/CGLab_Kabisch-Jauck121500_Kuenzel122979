#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <map>
#include <regex>
#include <structs.hpp>
#include "model.hpp"

#define GLFW_INCLUDE_NONE
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <GLFW/glfw3.h>


class Node {
public:
	//constructors of node
	Node::Node();
	Node::Node(std::shared_ptr<Node> _parent, std::string _name, int _depth, float _rotationSpeed);
	Node::Node(std::shared_ptr<Node> _parent, std::vector <std::shared_ptr<Node>> _children, std::string _name, std::string _path, int _depth, glm::mat4 _localTransformation, glm::mat4 _worldTransformation, float _rotationSpeed);

	//member functions of node
	std::shared_ptr<Node> getParent();
	void setParent(std::shared_ptr<Node> _parent);
	std::shared_ptr<Node> getChildren(std::string child);
	std::vector<std::shared_ptr<Node>> getChildrenList();
	std::string getName();
	std::string getPath();
	float getRotationSpeed();
	int getDepth();
	glm::mat4 getLocalTransformation() const;
	glm::mat4 getWorldTransformation() const;
	void setLocalTransformation(glm::mat4 _localTransformation);
	void setWorldTransformation(glm::mat4 _worldTransformation);
	void addChildren(std::shared_ptr<Node> newChild);
	std::string removeChildren(Node childToDelete);
	void printGraph();
	virtual void render(std::map<std::string, shader_program> m_shaders, glm::fmat4 m_view_transform, bool cellShading);
	virtual glm::vec3 getLightColor();
	virtual void setLightColor(glm::vec3 _lightColor);
	virtual float getLightIntensity();
	virtual void setLightIntensity(float _lightIntensity);


	//member variables of node
protected:
	std::vector < std::shared_ptr<Node>> children;


private:
	std::shared_ptr<Node> parent;
	std::string name;
	std::string path;
	int depth;
	glm::mat4 localTransformation;
	glm::mat4 worldTransformation;
	float rotationSpeed;

};

#endif