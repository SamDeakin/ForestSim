list = gr.make_list()

object2 = gr.make_object("DownyOak/EU55_1.obj", 25)
object2:rotate('x', -90)
object2:scale(100, 100, 100)
object2:translate(0, 0, 0.05)

list:add_object(object2)

-- This one is left commented out cause it's way too detailed
-- It kinda murders performance
--object3 = gr.make_object("DownyOak/EU55_2.obj", 1)
--object3:rotate('x', -90)
--object3:scale(100, 100, 100)
--object3:translate(0, 0, 0.05)
--
--list:add_object(object3)

object4 = gr.make_object("DownyOak/EU55_3.obj", 25)
object4:rotate('x', -90)
object4:scale(100, 100, 100)
object4:translate(0, 0, 0.05)

list:add_object(object4)

object5 = gr.make_object("DownyOak/EU55_4.obj", 25)
object5:rotate('x', -90)
object5:scale(100, 100, 100)
object5:translate(0, 0, 0.05)

list:add_object(object5)

return list
