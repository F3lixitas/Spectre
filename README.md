# Spectre
 A Vulkan based game engine
## For Windows systems:
You'll need to configure a config.txt file for the GLM, GLFW paths and wether you are using visual studio or not. A template file can be found here : [config.txt](https://github.com/F3lixitas/Spectre/files/9033297/config.txt)
Put this file at the root directory of the project and add something like [-C ../config.txt] to cmake's build arguments. The path towards the config file is set relative to the output binary.
## Dependencies :
You'll need GLM and Vulkan for Windows users. For Linux users, you will need in addition the xcb and xcb-util package (used for window communication with the X server). Also, the program won't work if you use a Wayland windowing system.
