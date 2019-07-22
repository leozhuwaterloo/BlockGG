ground_earth = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10, "ground_earth")

cube = gr.mesh('cube', 'cube')
cube:set_material(ground_earth)

return cube
