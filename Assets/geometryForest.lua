list = gr.make_list()

object = gr.make_object("BroadLeafStraightTrunk/Broad Leaf Straight Trunk.obj", 0)

list:add_object(object)

object:scale(1, 2, 3)
object:rotate('x', 45)
object:translate(5.5, 6, 7)

return list
