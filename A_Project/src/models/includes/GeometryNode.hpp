#pragma once

#include "SceneNode.hpp"

class GeometryNode : public SceneNode {
public:
	GeometryNode(
		const std::string & meshId,
		const std::string & name
	);

	Material material;
	glm::vec3 pos;

	// Mesh Identifier. This must correspond to an object name of
	// a loaded .obj file.
	std::string meshId;
};