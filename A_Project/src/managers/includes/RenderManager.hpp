#pragma once
#include "../../managers/includes/ResourceManager.hpp"
#include "../../renderers/includes/MasterRenderer.hpp"
#include "../../renderers/includes/ShadowRenderer.hpp"
#include "../../renderers/includes/GuiRenderer.hpp"
#include "../../renderers/includes/ParticleRenderer.hpp"
#include "../../renderers/includes/NormalRenderer.hpp"
#include "../../renderers/includes/SkyboxRenderer.hpp"
#include "../../objects/includes/Crate.hpp"
#include "../../objects/includes/Skybox.hpp"
#include <memory>

class RenderManager {
public:
	RenderManager();
	virtual ~RenderManager();
	void init();
  void render();

	std::unique_ptr<MasterRenderer> m_renderer;
	std::unique_ptr<ShadowRenderer> s_renderer;
	std::unique_ptr<GuiRenderer> g_renderer;
	std::unique_ptr<ParticleRenderer> p_renderer;
	std::unique_ptr<NormalRenderer> n_renderer;
	std::unique_ptr<SkyboxRenderer> b_renderer;
	std::unique_ptr<Crate> crate_no_bump;
	std::unique_ptr<Crate> crate_bump;
	std::unique_ptr<Skybox> skybox;
private:
	ResourceManager *resource_manager;
};
