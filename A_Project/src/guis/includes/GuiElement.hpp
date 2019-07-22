#pragma once
#include <glm/glm.hpp>
#include "../../managers/includes/ResourceManager.hpp"
#include "../../renderers/includes/GuiRenderer.hpp"

enum class ElementType {
	StaticTexturedGui,
	ColoredGui,
	FadeColoredGui
};
static std::string ELEMENT_TYPE_STR[] = {
	"StaticTexturedGui",
	"ColoredGui",
	"FadeColoredGui"
};

class GuiElement {
public:
  GuiElement(const ElementType &type, const glm::vec2 &pos,
    const glm::vec2 &size);
  virtual ~GuiElement() = 0;

  virtual void draw(GuiRenderer *renderer) = 0;
	virtual void tick();

  unsigned int id;
  ElementType type;
  glm::vec2 pos;
  glm::vec2 size;
  GuiElement *next;
  GuiElement *parent;

	glm::mat4 m_transformation;
	void computeTransformation();
protected:
  ResourceManager *resource_manager;
};
