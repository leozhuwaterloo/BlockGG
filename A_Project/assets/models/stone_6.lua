stone_6 = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10, "stone_6")

cube = gr.mesh('cube', 'cube')
cube:set_material(stone_6)

return cube
