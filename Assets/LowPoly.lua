list = gr.make_list()

tree1 = gr.make_object("LowPoly/Tree1.obj", 70)
tree1:scale(25, 25, 25)
tree1:set_scale_variance(2, 2, 2)
tree1:set_rot_variance(0, 0, 0, 360, 360, 360)
list:add_object(tree1)

tree2 = gr.make_object("LowPoly/Tree2.obj", 70)
tree2:scale(25, 25, 25)
tree2:set_scale_variance(2, 2, 2)
tree2:set_rot_variance(0, 0, 0, 360, 360, 360)
list:add_object(tree2)

tree3 = gr.make_object("LowPoly/Tree3.obj", 70)
tree3:scale(25, 25, 25)
tree3:set_scale_variance(2, 2, 2)
tree3:set_rot_variance(0, 0, 0, 360, 360, 360)
list:add_object(tree3)

tree4 = gr.make_object("LowPoly/Tree4.obj", 70)
tree4:scale(25, 25, 25)
tree4:set_scale_variance(2, 2, 2)
tree4:set_rot_variance(0, 0, 0, 360, 360, 360)
list:add_object(tree4)

grass1 = gr.make_object("LowPoly/Grass1.obj", 10)
grass1:scale(25, 25, 25)
grass1:set_scale_variance(2, 2, 2)
grass1:set_rot_variance(0, 0, 0, 360, 360, 360)
list:add_object(grass1)

grass2 = gr.make_object("LowPoly/Grass2.obj", 100)
grass2:scale(25, 25, 25)
grass2:set_scale_variance(2, 2, 2)
grass2:set_rot_variance(0, 0, 0, 360, 360, 360)
list:add_object(grass2)

rocks = gr.make_object("LowPoly/Rocks.obj", 100)
rocks:scale(0.2, 0.2, 0.2)
rocks:set_scale_variance(2, 2, 2)
rocks:set_rot_variance(0, 0, 0, 360, 360, 360)
list:add_object(rocks)

bush = gr.make_object("LowPoly/Bush.obj", 100)
bush:scale(25, 25, 25)
bush:set_scale_variance(2, 2, 2)
bush:set_rot_variance(0, 0, 0, 360, 360, 360)
list:add_object(bush)

return list
