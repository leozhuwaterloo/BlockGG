#include "./includes/GuiElement.hpp"
#include <glm/gtc/matrix_transform.hpp>

static unsigned int _id = 0;

GuiElement::GuiElement(const ElementType &type, const glm::vec2 &pos,
  const glm::vec2 &size)
  : id(++_id),
    type(type),
    pos(pos),
    size(size),
    next(nullptr),
    parent(nullptr),
    resource_manager(ResourceManager::getInstance())
{
  computeTransformation();
}
GuiElement::~GuiElement() {}

void GuiElement::computeTransformation() {
  m_transformation = glm::scale(glm::translate(glm::mat4(1.0f),
    glm::vec3(pos, 0.0f)), glm::vec3(size, 0.0f));
}

void GuiElement::tick() {}
