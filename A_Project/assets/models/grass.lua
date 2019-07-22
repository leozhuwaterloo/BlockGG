mat = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10, "grass")

grass = gr.mesh('grass', 'grass')
grass:set_material(mat)
grass:translate(0.0, 0.5, 0.0)
return grass
