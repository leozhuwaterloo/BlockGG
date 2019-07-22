mat = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10, "slime")

slime = gr.mesh('slime', 'slime')
slime:set_material(mat)
slime:scale(0.5, 0.5, 0.5)
slime:translate(0.0, -0.7, 0.0)

return slime
