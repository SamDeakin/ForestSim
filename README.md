# Compilation:
Use CMAKE. Good luck!

jk, run these commands

First run cmake to generate the build directory and makefile like this:
```cmake cmake-build-debug```

Now cd into the ```cmake-build-debug``` directory and run make to execute the makefile.

Running from here should be like ```./ForestSim Assets/<Script>```

# Running
This was built to work in 1080p. Other resolutions will get weird, so don't resize the window or try it on a smaller screen.

# Interaction
Interaction is as specified. The z, x and c buttons have also been made aliases of the 3 mouse buttons.

# Extra keys:
    Q - Quit
    R - Reset
    F - Enable/Disable FXAA
    +/- - Adjust FXAA render mode

# Lua
The lua functions for controlling the scene have been changed to being passed 3 arguments in place of a table in the specifications. This change is to save development time.

Note that getting models to work with the random rotation and scaling is really hard. To work with scaling and y-axis rotation, they have to lie with the plane of x=0, z=0 cutting through the model. Otherwise the random movements make them fly everywhere.

Rotations in x and z-axes are even more difficult. The model must contain the point {0,0,0} inside it. I'm too lazy to perfect each model like that, and I only plan on using y-axis rotations anyways, so I've disabled them.

#TODO
5. Textures
7. Shadow Mapping
9. Crepuscular Rays
8. SSAO
10. Tune FXAA to find that harsh edge bug
11. Make good scene

#Credits for assets:
    SkyboxSet1 is from Heiko Irrgang, https://93i.de/p/free-skybox-texture-set/
    GreenPine - http://tf3dm.com/3d-model/green-pine-26327.html
    BroadLeafStraightTrunk - http://tf3dm.com/3d-model/broad-leaf-straight-trunk-39747.html
    
