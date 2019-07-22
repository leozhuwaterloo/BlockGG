ground_snow = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10, "ground_snow")

cube = gr.mesh('cube', 'cube')
cube:set_material(ground_snow)

return cube
