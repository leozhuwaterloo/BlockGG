green = gr.material({0.096, 0.8, 0.042}, {0.1, 0.1, 0.1}, 10, "")
brown = gr.material({0.269, 0.138, 0.059}, {0.1, 0.1, 0.1}, 10, "")

root = gr.node('root')
root:scale(2.0, 2.0, 2.0)

tree1leaf2 = gr.mesh('tree1leaf2', 'tree1leaf2')
tree1leaf2:set_material(green)
root:add_child(tree1leaf2)

tree1leaf1 = gr.mesh('tree1leaf1', 'tree1leaf1')
tree1leaf1:set_material(green)
root:add_child(tree1leaf1)

tree1leaf3 = gr.mesh('tree1leaf3', 'tree1leaf3')
tree1leaf3:set_material(green)
root:add_child(tree1leaf3)

tree1main = gr.mesh('tree1main', 'tree1main')
tree1main:set_material(brown)
root:add_child(tree1main)

return root
