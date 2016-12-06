# Compilation:
Use CMAKE. Good luck!

jk, run these commands

First make a build directory (```mkdir cmake-build-debug```) and then cd into it.

From this directory run ```cmake ..``` to execute cmake, and then ```make``` to build.

If make complains about missing lua functions then you have to build lua manually. cd to the ```shared/lua-5.3.1/src``` directory and do ```make <platform>``` where platform is ```linux``` or whatever you're running on, then try making from the cmake-build-debug directory again.

Running from here should be like ```./ForestSim Assets/<Script>```

### Common errors:
If you encounter errors about files not being found, then make sure the Assets/ directory is symbolically linked to a directory of the same name inside the build directory.

If the program exits on an unspecified exception try lowering the shadow map resolution. It is defined inside Forest.cpp in the macro DEPTH_TEXTURE_SIZE.

# Running
This was built to work in 1080p. Other resolutions will get weird, so don't resize the window or try it on a smaller screen. Theres an explanation of why this is a bug in documentation/implementation.pdf.

# Interaction
Interaction is as specified in Documentation/revised.pdf. The z, x and c buttons have also been made aliases of the 3 mouse buttons. The extra functions and interactions are described further in Documentation/Manual.pdf

# Extra keys:
    Q - Quit
    R - Reset
    F - Enable/Disable FXAA
    +/- - Adjust FXAA render mode
    B - Toggle skybox
    M - Toggle shadows
    N - Show the shadow texture

# Lua
The lua functions for controlling the scene have been changed to being passed 3 arguments in place of a table in the specifications. This change is to save development time.

Note that getting models to work with the random rotation and scaling is really hard. To work with scaling and y-axis rotation, they have to lie with the plane of x=0, z=0 cutting through the model. Otherwise the random movements make them fly everywhere.

Rotations in x and z-axes are even more difficult. The model must contain the point {0,0,0} inside it. I'm too lazy to perfect each model like that, and I only plan on using y-axis rotations anyways, so I've disabled them.

# Implementation
Implementation details can be found in documentation/Implementation.pdf

#TODO
5. Textures
9. Crepuscular Rays
8. SSAO
11. Make good scene

# Credits for assets:
    SkyboxSet1 is from Heiko Irrgang, https://93i.de/p/free-skybox-texture-set/
    GreenPine - http://tf3dm.com/3d-model/green-pine-26327.html
    BroadLeafStraightTrunk - http://tf3dm.com/3d-model/broad-leaf-straight-trunk-39747.html
    Pine_4m - https://www.cgtrader.com/free-3d-models/plant-tree/conifer/free-pine-trees-sample-model
    DownyOak - https://www.cgtrader.com/free-3d-models/plant-tree/leaf-tree/xfrogplants-downy-oak
    LowPoly - http://www.turbosquid.com/FullPreview/Index.cfm/ID/986631
    LowPolyToon - http://www.turbosquid.com/FullPreview/Index.cfm/ID/999137

# Objectives
1. The program should take input from the script and produce multiples of every object specified in the script to generate a ”forest” of objects.
2. The program should generate an interesting non-trivial skybox. 4
3. The program should vary objects size and rotation based on the provided limitations in the script
4. The program should allow the user to move the camera using the move- ment controls outlined above
5. The program should use the textures specified inside object files, or place a default white texture when there is no linked texture.
6. A Phong shading model should be implemented with one directional light imitating a sun.
7. The program should produce shadows using a shadow mapping technique
8. The program should enhance shadows using the ambient occlusion tech- niques outlined above
9. The program should show crepuscular rays imitating the media refracting some but not all sunlight that passes through it.
10. The program should implement the Fast Approximate AntiAliasing (FXAA) algorithm.
