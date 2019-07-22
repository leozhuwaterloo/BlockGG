stone_4 = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10, "stone_4")

cube = gr.mesh('cube', 'cube')
cube:set_material(stone_4)

return cube
