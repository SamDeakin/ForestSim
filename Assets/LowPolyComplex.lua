list = gr.make_list()

tree1 = gr.make_object("LowPolyComplex/Tree1.obj", 70)
tree1:scale(0.35, 0.35, 0.35)
tree1:set_scale_variance(2, 2, 2)
tree1:set_rot_variance(0, 0, 0, 360, 360, 360)
list:add_object(tree1)

tree2 = gr.make_object("LowPolyComplex/Tree2.obj", 70)
tree2:scale(0.35, 0.35, 0.35)
tree2:set_scale_variance(2, 2, 2)
tree2:set_rot_variance(0, 0, 0, 360, 360, 360)
list:add_object(tree2)

tree3 = gr.make_object("LowPolyComplex/Tree3.obj", 70)
tree3:scale(0.35, 0.35, 0.35)
tree3:set_scale_variance(2, 2, 2)
tree3:set_rot_variance(0, 0, 0, 360, 360, 360)
list:add_object(tree3)

tree4 = gr.make_object("LowPolyComplex/Tree4.obj", 70)
tree4:scale(0.35, 0.35, 0.35)
tree4:set_scale_variance(2, 2, 2)
tree4:set_rot_variance(0, 0, 0, 360, 360, 360)
list:add_object(tree4)

tree5 = gr.make_object("LowPolyComplex/Tree5.obj", 70)
tree5:scale(0.35, 0.35, 0.35)
tree5:set_scale_variance(2, 2, 2)
tree5:set_rot_variance(0, 0, 0, 360, 360, 360)
list:add_object(tree5)

trunk = gr.make_object("LowPolyComplex/trunk.obj", 30)
trunk:scale(0.35, 0.35, 0.35)
trunk:set_scale_variance(2, 2, 2)
trunk:set_rot_variance(0, 0, 0, 360, 360, 360)
list:add_object(trunk)

return list

