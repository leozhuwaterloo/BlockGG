mat = gr.material({1.0, 1.0, 1.0}, {10.0, 10.0, 10.0}, 500, "crate")

crate = gr.mesh('crate', 'crate')
crate:set_material(mat)
crate:scale(10, 10, 10)
return crate
