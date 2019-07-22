#include "MeshConsolidator.hpp"
using namespace glm;
using namespace std;

#include <iostream>
#include "cs488-framework/Exception.hpp"
#include "cs488-framework/ObjFileDecoder.hpp"


//----------------------------------------------------------------------------------------
// Default constructor
MeshConsolidator::MeshConsolidator()
{

}

//----------------------------------------------------------------------------------------
// Destructor
MeshConsolidator::~MeshConsolidator()
{

}

//----------------------------------------------------------------------------------------
template <typename T>
static void appendVector (
		std::vector<T> & dest,
		const std::vector<T> & source
) {
	// Increase capacity to hold source.size() more elements
	dest.reserve(dest.size() + source.size());

	dest.insert(dest.end(), source.begin(), source.end());
}


//----------------------------------------------------------------------------------------
MeshConsolidator::MeshConsolidator(
		std::initializer_list<ObjFilePath> objFileList
) {

	MeshId meshId;
	vector<vec3> positions;
	vector<vec3> normals;
  vector<vec2> uv_coords;
	vector<vec3> tangents;
	BatchInfo batchInfo;
	unsigned long indexOffset(0);

    for(const ObjFilePath & objFile : objFileList) {
	    ObjFileDecoder::decode(objFile.c_str(), meshId, positions, normals,
				uv_coords);

	    uint numIndices = positions.size();

			if (numIndices % 3 != 0) {
				throw Exception("Error within MeshConsolidator: "
					"positions.size() % 3 != 0\n");
			}

	    if (numIndices != normals.size()) {
		    throw Exception("Error within MeshConsolidator: "
					"positions.size() != normals.size()\n");
	    }

			if (uv_coords.size() != 0 && numIndices != uv_coords.size()) {
				throw Exception("Error within MeshConsolidator: "
					"positions.size() != uv_coords.size()\n");
			}

	    batchInfo.startIndex = indexOffset;
	    batchInfo.numIndices = numIndices;
	    m_batchInfoMap[meshId] = batchInfo;

			// Calculate tangent
			tangents.clear();
			for(int i = 0; i < positions.size(); i += 3) {
					glm::vec3 p0 = positions[i];
					glm::vec3 p1 = positions[i + 1];
					glm::vec3 p2 = positions[i + 2];
					glm::vec2 t0 = uv_coords[i];
					glm::vec2 t1 = uv_coords[i + 1];
					glm::vec2 t2 = uv_coords[i + 2];
					glm::vec3 delta_p1 = p1 - p0;
					glm::vec3 delta_p2 = p2 - p0;
					glm::vec2 delta_uv1 = t1 - t0;
					glm::vec2 delta_uv2 = t2 - t0;
					float r = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);
					glm::vec3 tangent = (delta_p1 * delta_uv2.y - delta_p2 * delta_uv1.y) * r;
					tangent = glm::normalize(tangent);
					tangents.push_back(tangent);
					tangents.push_back(tangent);
					tangents.push_back(tangent);
			}
	    appendVector(m_vertexPositionData, positions);
	    appendVector(m_vertexNormalData, normals);
      appendVector(m_vertexUVCoordData, uv_coords);
			appendVector(m_vertexTangentData, tangents);

	    indexOffset += numIndices;
    }

}

//----------------------------------------------------------------------------------------
void MeshConsolidator::getBatchInfoMap (
		BatchInfoMap & batchInfoMap
) const {
	batchInfoMap = m_batchInfoMap;
}

//----------------------------------------------------------------------------------------
// Returns the starting memory location for vertex position data.
const float * MeshConsolidator::getVertexPositionDataPtr() const {
	return &(m_vertexPositionData[0].x);
}

//----------------------------------------------------------------------------------------
// Returns the starting memory location for vertex normal data.
const float * MeshConsolidator::getVertexNormalDataPtr() const {
    return &(m_vertexNormalData[0].x);
}

//----------------------------------------------------------------------------------------
// Returns the starting memory location for vertex uv coord data.
const float * MeshConsolidator::getVertexUVCoordDataPtr() const {
    return &(m_vertexUVCoordData[0].x);
}

//----------------------------------------------------------------------------------------
// Returns the starting memory location for vertex tangent data.
const float * MeshConsolidator::getVertexTangentDataPtr() const {
    return &(m_vertexTangentData[0].x);
}


//----------------------------------------------------------------------------------------
// Returns the total number of bytes of all vertex position data.
size_t MeshConsolidator::getNumVertexPositionBytes() const {
	return m_vertexPositionData.size() * sizeof(vec3);
}

//----------------------------------------------------------------------------------------
// Returns the total number of bytes of all vertex normal data.
size_t MeshConsolidator::getNumVertexNormalBytes() const {
	return m_vertexNormalData.size() * sizeof(vec3);
}

//----------------------------------------------------------------------------------------
// Returns the total number of bytes of all vertex uv coord data.
size_t MeshConsolidator::getNumVertexUVCoordBytes() const {
	return m_vertexUVCoordData.size() * sizeof(vec2);
}

//----------------------------------------------------------------------------------------
// Returns the total number of bytes of all vertex tangent data.
size_t MeshConsolidator::getNumVertexTangentBytes() const {
	return m_vertexTangentData.size() * sizeof(vec3);
}
