mat = gr.material({1.0, 1.0, 1.0}, {0.4, 0.4, 0.4}, 10, "gun_smg")

gun_smg = gr.mesh('gun_smg', 'gun_smg')
gun_smg:set_material(mat)
gun_smg:scale(0.5, 0.5, 0.5)
gun_smg:rotate('y', 90.0)
return gun_smg
