#include "./includes/MeshObjects.hpp"
#include "cs488-framework/GlErrorCheck.hpp"
#include <stack>
#include "../managers/includes/GameManager.hpp"

MeshObject::MeshObject(const ObjectType &type, const glm::vec3 &pos, const glm::vec3 &rot)
  : GameObject(type, pos, rot) {}

MeshObject::MeshObject(const ObjectType &type, const glm::vec3 &pos)
  : MeshObject(type, pos, glm::vec3(0)) {}

void MeshObject::tick() {}



MeshInstance::MeshInstance(const ObjectType &type, const glm::vec3 &pos, const glm::vec3 &rot)
  : MeshObject(type, pos, rot) {}

MeshInstance::MeshInstance(const ObjectType &type, const glm::vec3 &pos)
  : MeshInstance(type, pos, glm::vec3(0)) {}

void MeshInstance::getNodes(SceneNode *model, std::vector<std::unique_ptr<GeometryNode>> &ret_nodes) {
  std::stack<SceneNode*> stack;
  stack.push(model);
  while(!stack.empty()) {
    SceneNode *node = stack.top();
    stack.pop();
    for (SceneNode *child : node->children) {
      stack.push(child);
      child->parent = node;
    }
    if (node->m_nodeType == NodeType::GeometryNode) {
      std::unique_ptr<GeometryNode> n_copy = std::unique_ptr<GeometryNode>(
        new GeometryNode(*static_cast<GeometryNode*>(node)));
      if (node->parent != nullptr) {
        n_copy->trans = node->parent->trans * n_copy->trans;
      }
      ret_nodes.push_back(std::move(n_copy));
    }
  }
}

void MeshInstance::render(MasterRenderer *renderer) {
  glUniform1i(renderer->m_shader->using_offset_loc, 0);
  glDisableVertexAttribArray(renderer->m_shader->pos_offset_attr_loc);
  std::vector<std::unique_ptr<GeometryNode>> nodes;
  getNodes(nodes);
  renderer->updateGlobalShaderUniforms();
  for (const auto &node : nodes){
    const BatchInfo &batchInfo = resource_manager->batch_info_map.at(node->meshId);
    node->rotate('x', rot[0]);
    node->rotate('z', rot[2]);
    node->rotate('y', rot[1]);
    node->translate(pos);
    renderer->updateShaderUniforms(*node);
    glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
  }
}
void MeshInstance::render(ShadowRenderer *renderer) {
  glUniform1i(renderer->s_shader->using_offset_loc, 0);
  glDisableVertexAttribArray(renderer->s_shader->pos_offset_attr_loc);
  std::vector<std::unique_ptr<GeometryNode>> nodes;
  getNodes(nodes);
  for (const auto &node : nodes){
    const BatchInfo &batchInfo = resource_manager->batch_info_map.at(node->meshId);
    node->rotate('x', rot[0]);
    node->rotate('z', rot[2]);
    node->rotate('y', rot[1]);
    node->translate(pos);
    renderer->updateShaderUniforms(*node);
    CHECK_GL_ERRORS;
    glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
  }
}

// -----------------------------------------------------------------------------------
MeshGroup::MeshGroup(const ObjectType &type, const glm::vec3 &pos,
  MeshInstance *instance)
  : MeshObject(type, pos), instance(std::unique_ptr<MeshInstance>(instance)) {}

void MeshGroup::render(MasterRenderer *renderer) {
  glUniform1i(renderer->m_shader->using_offset_loc, 1);
  glEnableVertexAttribArray(renderer->m_shader->pos_offset_attr_loc);
	glBindBuffer(GL_ARRAY_BUFFER, pos_offset);
	glVertexAttribPointer(renderer->m_shader->pos_offset_attr_loc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	CHECK_GL_ERRORS;
  renderer->updateGlobalShaderUniforms();
  std::vector<std::unique_ptr<GeometryNode>> nodes;
  instance->getNodes(nodes);
  for (const auto &node : nodes){
    const BatchInfo &batchInfo = resource_manager->batch_info_map.at(node->meshId);
    renderer->updateShaderUniforms(*node);
    glDrawArraysInstanced(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices, data_count);
  }
}
void MeshGroup::render(ShadowRenderer *renderer) {
  glUniform1i(renderer->s_shader->using_offset_loc, 1);
  glEnableVertexAttribArray(renderer->s_shader->pos_offset_attr_loc);
  glBindBuffer(GL_ARRAY_BUFFER, pos_offset);
  glVertexAttribPointer(renderer->s_shader->pos_offset_attr_loc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  CHECK_GL_ERRORS;
  std::vector<std::unique_ptr<GeometryNode>> nodes;
  instance->getNodes(nodes);
  for (const auto &node : nodes){
    const BatchInfo &batchInfo = resource_manager->batch_info_map.at(node->meshId);
    renderer->updateShaderUniforms(*node);
    glDrawArraysInstanced(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices, data_count);
  }
}




// -------------------------------------------------------------------------------------------
MeshGroups::MeshGroups(const ObjectType &type, const glm::vec3 &pos)
  : MeshObject(type, pos) {}

void MeshGroups::render(MasterRenderer *renderer) {
  glUniform1i(renderer->m_shader->using_offset_loc, 1);
  glEnableVertexAttribArray(renderer->m_shader->pos_offset_attr_loc);
  renderer->updateGlobalShaderUniforms();
  for (int i = 0; i < instance_list.size(); ++i) {
    ObjectType i_type = instance_list[i]->type;
    if (i_type == ObjectType::Grass) {
      if (resource_manager->game->command_manager->render_bump) continue;
      glDisable(GL_CULL_FACE);
    }
    glBindBuffer(GL_ARRAY_BUFFER, pos_offset_list[i]);
    glVertexAttribPointer(renderer->m_shader->pos_offset_attr_loc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    std::vector<std::unique_ptr<GeometryNode>> nodes;
    instance_list[i]->getNodes(nodes);
    for (const auto &node : nodes){
      const BatchInfo &batchInfo = resource_manager->batch_info_map.at(node->meshId);
      renderer->updateShaderUniforms(*node);
      glDrawArraysInstanced(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices, data_count_list[i]);
    }
    if (i_type == ObjectType::Grass) glEnable(GL_CULL_FACE);
  }
}
void MeshGroups::render(ShadowRenderer *renderer) {
  glUniform1i(renderer->s_shader->using_offset_loc, 1);
  glEnableVertexAttribArray(renderer->s_shader->pos_offset_attr_loc);
  for (int i = 0; i < instance_list.size(); ++i) {
    if (instance_list[i]->type == ObjectType::Grass) continue;
    glBindBuffer(GL_ARRAY_BUFFER, pos_offset_list[i]);
    glVertexAttribPointer(renderer->s_shader->pos_offset_attr_loc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    std::vector<std::unique_ptr<GeometryNode>> nodes;
    instance_list[i]->getNodes(nodes);
    for (const auto &node : nodes){
      const BatchInfo &batchInfo = resource_manager->batch_info_map.at(node->meshId);
      renderer->updateShaderUniforms(*node);
      glDrawArraysInstanced(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices, data_count_list[i]);
    }
  }
}
void MeshGroups::render(NormalRenderer *renderer) {
  glUniform1i(renderer->n_shader->using_offset_loc, 1);
  glEnableVertexAttribArray(renderer->n_shader->pos_offset_attr_loc);
  glActiveTexture(GL_TEXTURE0 + renderer->n_shader->normal_map_index);
	glBindTexture(GL_TEXTURE_2D, resource_manager->texture_map.at("grass_normal"));
  renderer->updateGlobalShaderUniforms();
  glDisable(GL_CULL_FACE);
  for (int i = 0; i < instance_list.size(); ++i) {
    if (instance_list[i]->type != ObjectType::Grass) continue;
    glBindBuffer(GL_ARRAY_BUFFER, pos_offset_list[i]);
    glVertexAttribPointer(renderer->n_shader->pos_offset_attr_loc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    std::vector<std::unique_ptr<GeometryNode>> nodes;
    instance_list[i]->getNodes(nodes);
    for (const auto &node : nodes){
      const BatchInfo &batchInfo = resource_manager->batch_info_map.at(node->meshId);
      renderer->updateShaderUniforms(*node);
      glDrawArraysInstanced(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices, data_count_list[i]);
    }
  }
  glEnable(GL_CULL_FACE);
}
