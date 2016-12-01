list = gr.make_list()

object1 = gr.make_object("Geometry/smstdodeca.obj", 0)
object1:scale(5, 5, 5)
object1:rotate('x', 45)
object1:translate(5.5, 6, 7)

list:add_object(object1)

object2 = gr.make_object("Geometry/buckyball.obj", 0)
object2:scale(10, 10, 10)
object2:translate(-10, 1, 1)

list:add_object(object2)

return list
