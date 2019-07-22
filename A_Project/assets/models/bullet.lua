cyan = gr.material({0.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10, "")

bullet = gr.mesh('cube', 'bullet')
bullet:scale(0.05, 0.05, 0.8)
bullet:rotate('z', 45.0)
bullet:set_material(cyan)

return bullet
