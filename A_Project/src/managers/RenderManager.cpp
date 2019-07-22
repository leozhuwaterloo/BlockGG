#include "./includes/RenderManager.hpp"
#include "./includes/GameManager.hpp"
#include "../objects/includes/MeshObjects.hpp"
#include "../objects/includes/Particles.hpp"
#include "../guis/includes/StaticTexturedGui.hpp"
#include "cs488-framework/GlErrorCheck.hpp"


RenderManager::RenderManager()
	: resource_manager(ResourceManager::getInstance()) {}
RenderManager::~RenderManager() {}

void RenderManager::init() {
	m_renderer = std::unique_ptr<MasterRenderer>(
		new MasterRenderer(resource_manager->m_shader.get()));
	s_renderer = std::unique_ptr<ShadowRenderer>(
		new ShadowRenderer(resource_manager->s_shader.get()));
	g_renderer = std::unique_ptr<GuiRenderer>(
		new GuiRenderer(resource_manager->g_shader.get()));
	p_renderer = std::unique_ptr<ParticleRenderer>(
		new ParticleRenderer(resource_manager->p_shader.get()));
	n_renderer = std::unique_ptr<NormalRenderer>(
		new NormalRenderer(resource_manager->n_shader.get()));
	b_renderer = std::unique_ptr<SkyboxRenderer>(
		new SkyboxRenderer(resource_manager->b_shader.get()));


	crate_no_bump = std::unique_ptr<Crate>(
		new Crate(glm::vec3(0.0, 100.0, -15.0)));
	crate_bump = std::unique_ptr<Crate>(
		new Crate(glm::vec3(0.0, 100.0, 15.0)));
	skybox = std::unique_ptr<Skybox>(new Skybox());

	// Render only the front face of geometry.
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
  glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setup depth testing
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	glEnable(GL_DEPTH_CLAMP);

	glClearDepth(1.0f);
  glClearColor(0.85, 0.85, 0.85, 1.0);
}

void RenderManager::render() {
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(resource_manager->m_mesh->vao);
	CHECK_GL_ERRORS;
	if (resource_manager->game->command_manager->render_skybox) {
		b_renderer->enable();
		skybox->render(b_renderer.get());
		b_renderer->disable();
	}

	s_renderer->enable();
	if (resource_manager->game->command_manager->render_shadow) {
		for (GameObject *object : resource_manager->game->objects) {
			MeshObject *mesh_object = dynamic_cast<MeshObject*>(object);
			if (mesh_object != nullptr) {
				mesh_object->render(s_renderer.get());
			}
		}
		if (resource_manager->game->command_manager->bump_test) {
			crate_no_bump->render(s_renderer.get());
			crate_bump->render(s_renderer.get());
		}
	}
	s_renderer->disable();

	m_renderer->enable();
	glActiveTexture(GL_TEXTURE0 + m_renderer->m_shader->shadow_map_index);
	glBindTexture(GL_TEXTURE_2D, s_renderer->depth_map);
	for (GameObject *object : resource_manager->game->objects) {
		MeshObject *mesh_object = dynamic_cast<MeshObject*>(object);
		if (mesh_object != nullptr) {
			mesh_object->render(m_renderer.get());
		}
  }
	if (resource_manager->game->command_manager->bump_test) {
		crate_no_bump->render(m_renderer.get());
		if (!resource_manager->game->command_manager->render_bump) {
			crate_bump->render(m_renderer.get());
		}
	}
	m_renderer->disable();

	if (resource_manager->game->command_manager->render_bump) {
		n_renderer->enable();
		glActiveTexture(GL_TEXTURE0 + n_renderer->n_shader->shadow_map_index);
		glBindTexture(GL_TEXTURE_2D, s_renderer->depth_map);
		for (GameObject *object : resource_manager->game->objects) {
			MeshGroups *mesh_groups = dynamic_cast<MeshGroups*>(object);
			if (mesh_groups != nullptr) {
				mesh_groups->render(n_renderer.get());
			}
	  }
		if (resource_manager->game->command_manager->bump_test) {
			crate_bump->render(n_renderer.get());
		}
		n_renderer->disable();
	}

	glBindVertexArray(resource_manager->m_mesh_2d->vao);
	if (resource_manager->game->command_manager->render_particle) {
		p_renderer->enable();
		for (GameObject *object : resource_manager->game->objects) {
			ParticleSystem *particle_group = dynamic_cast<ParticleSystem*>(object);
			if (particle_group != nullptr) {
				particle_group->render(p_renderer.get());
			}
	  }
		p_renderer->disable();
	}

	if (resource_manager->game->command_manager->render_gui) {
		g_renderer->enable();
		for (GuiElement *element : resource_manager->game->elements) {
			element->draw(g_renderer.get());
	  }
		g_renderer->disable();
	}
	CHECK_GL_ERRORS;
}
