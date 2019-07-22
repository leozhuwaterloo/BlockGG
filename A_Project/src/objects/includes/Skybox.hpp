#include "GameObject.hpp"
#include "../../renderers/includes/SkyboxRenderer.hpp"

class Skybox : GameObject {
public:
  Skybox();
  virtual ~Skybox();
  virtual void tick() override;
  virtual void render(SkyboxRenderer *renderer);
};
