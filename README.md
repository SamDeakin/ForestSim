# Compilation:
Use CMAKE. Good luck

# Interaction
Interaction is as specified. The z, x and c buttons have also been made aliases of the 3 mouse buttons.

# Extra keys:
    Q - Quit
    R - Reset

# Lua
The lua functions for controlling the scene have been changed to being passed 3 arguments in place of a table in the specifications. This change is to save development time.
Note that getting models to work with the random rotation and scaling is really hard. To work with scaling and y-axis rotation, they have to lie with the plane of x=0, z=0 cutting through the model. Otherwise the random movements make them fly everywhere.
Rotations in x and z-axes are even more difficult. The model must contain the point {0,0,0} inside it. I'm too lazy to perfect each model like that, and I only plan on using y-axis rotations anyways, so I've disabled them.

#TODO
###Tuesday
2. Skybox
    Skybox
4. Movement
    Rotation
2. Skybox
    Ground
4. Movement
    Finish
###Wednesday
1. Load objects in w/ LUA
4. Check move values, add shift
6. Shading
###Thursday
3. Variations
5. Textures
7. Shadow Mapping
###Friday
10. FXAA
###Saturday
9. Crepuscular Rays
###Sunday
8. SSAO

#Credits for assets:
    SkyboxSet1 is from Heiko Irrgang, https://93i.de/p/free-skybox-texture-set/
    GreenPine - http://tf3dm.com/3d-model/green-pine-26327.html
    BroadLeafStraightTrunk - http://tf3dm.com/3d-model/broad-leaf-straight-trunk-39747.html
    
