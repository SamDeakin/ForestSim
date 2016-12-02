list = gr.make_list()

object1 = gr.make_object("Geometry/smstdodeca.obj", 150)
object1:scale(0.8, 0.8, 0.8)
object1:translate(0, -150.0, 0)
--object1:rotate('x', 45)

list:add_object(object1)

object2 = gr.make_object("Geometry/buckyball.obj", 250)
object2:scale(10, 10, 10)
object2:translate(10, 2.2, 0)

list:add_object(object2)

-- The smallest object, we will attempt to make this one act like grass or small shrubs
object3 = gr.make_object("Geometry/smstdodeca.obj", 10000)
object3:scale(0.05, 0.05, 0.05)
object3:translate(0, -130, 0)

list:add_object(object3)

return list
