#include "./includes/Crate.hpp"

Crate::Crate(const glm::vec3 &pos)
  : MeshInstance(ObjectType::Crate, pos) {}
Crate::~Crate() {}
void Crate::getNodes(std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) {
  MeshInstance::getNodes(resource_manager->model_map.at("crate").get(), ret_nodes);
}
void Crate::render(NormalRenderer *renderer) {
  glUniform1i(renderer->n_shader->using_offset_loc, 0);
  glDisableVertexAttribArray(renderer->n_shader->pos_offset_attr_loc);
  glActiveTexture(GL_TEXTURE0 + renderer->n_shader->normal_map_index);
	glBindTexture(GL_TEXTURE_2D, resource_manager->texture_map.at("crate_normal"));
  std::vector<std::unique_ptr<GeometryNode>> nodes;
  getNodes(nodes);
  renderer->updateGlobalShaderUniforms();
  for (const auto &node : nodes){
    const BatchInfo &batchInfo = resource_manager->batch_info_map.at(node->meshId);
    node->translate(pos);
    renderer->updateShaderUniforms(*node);
    glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
  }
}
void Crate::render(MasterRenderer *renderer) {
  MeshInstance::render(renderer);
}
void Crate::render(ShadowRenderer *renderer) {
  MeshInstance::render(renderer);
}
