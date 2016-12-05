list = gr.make_list()

object1 = gr.make_object("Pine_4m/Pine.obj", 100);
object1:scale(0.5, 0.5, 0.5)
object1:translate(0, 0, 100)

object1:set_scale_variance(2, 2, 2)
object1:set_rot_variance(0, 0, 0, 360, 360, 360)

list:add_object(object1)

return list
