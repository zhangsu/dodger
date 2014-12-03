# Dodger

A simple 3D game rendered through OpenGL as a mean to demonstrate simple terrain rendering from heightmaps, particle systems, shadow mapping, multitexturing, Phong lighting and Fog rendering.

The game can be run from the command line. To run the game, simply cd to the Dodger project root directory, then type `bin/dodger` to execute the pre-compiled binary. There are no command line arguments.

Once the window loads, the game starts in a hilly terrain surrounded by mountains. The player moves a spherical blue spirit (represented by thousands of blue particles) on the ground and tries to dodge his enemies -- the red spirits. When the blue spirit collide with any red spirit, the game resets and all the spirits are moved back to the starting positions. The term “player” is used to refer to the blue spirit in various places down below.

![screenshot](https://cloud.githubusercontent.com/assets/743034/5257059/4a602e5e-79a0-11e4-9433-17b9bb92ee6b.png)

The window can be freely resize and maximized; the OpenGL viewport is updated dynamically to always have a 60 degree field of view. After clicking on the X button of the window, the game application exits.

## Input
The game uses a keyboard and a mouse (and potentially another kind of scrolling device too) for input.

### W or UP arrow key
Move the player and camera forward, or move just the camera in God mode.

### A or LEFT arrow key
Move the player and camera to the left, or move just the camera in God mode.

### D or RIGHT arrow key
Move the player and camera to the right, or move just the camera in God mode.

### S or DOWN arrow key
Move the player and camera backward, or move just the camera in God mode.

### Space
Move the camera upwards in God mode.

### CTRL-Space
Move the camera downwards in God mode.

### G
Switch between God mode and first person mode (the default is first person mode). In God mode, the camera moves and rotates freely, independent from the player. The starting position and orientation are whatever they were before hitting “G” and are reset back to the states before going into God mode after switching back to first person mode. In God mode, there is no gravity or collision detection as the camera is not a physical objects, thus allowing it to pierce through 3D objects.

### 1
Toggle wireframe mode.

### 2
Toggle backface culling.

### 3
Toggle lighting.

### 4
Toggle shadow rendering.

### 5
Toggle terrain shadow rendering (if shadow rendering is disabled, so is terrain shadow rendering).

### 6
Toggle fog rendering.

### 7
Toggle audio.

### TAB
Cycle through different spirits’ views. The camera is set to look at the forward direction of the cycled spirit. The red spirits are controlled by AI and often rotate abruptly, therefore a “shaking” of the camera can be seen.

### =
Increase the moving speed of the player.

### -
Decrease the moving speed of the player.
Left mouse button
Rotate camera only around x or y axis. Mouse motions in the x direction are used for rotation along the y axis, and mouse motions in the y direction are used for rotation along the x axis. No rotation is done on the z axis. When the mouse button is pressed, the cursor is temporarily disabled so that it doesn’t go out of the window frame (unless the window is maximized). The cursor is re-enabled when the mouse button is released.

The camera rotation can be done even when the player is moving (e.g., holding down W to move forward while at the same time hold down left mouse button to rotate the camera). The rotation along the x axis is bounded by an interval so that the camera keeps on top of the ground and not going over the player’s head. The rotation is originated at the player. When the left mouse button is released, the view is reset to the forward direction of the player. In God mode, there is no bound to the rotation along the x axis.
Right mouse button
Similar to the left mouse button, but also rotate the player’s orientation along with the camera. In God mode, it behaves the same as the left mouse button.

### Scrolling
Zoom in the camera if scrolling up and zoom out if scrolling down. This input is typically triggered by a mouse wheel, but can also be triggered by other scrolling devices. Initially the camera is a few units away behind the player. There is a limit on how much the camera can be zoomed in and out. The closest distance is where the player is just behind the camera, making the view a true first person view.
