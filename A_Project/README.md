### Compilation
Please make sure the following package is installed
```
sudo apt-get install libalut-dev
```
Computer in the graphics labs seems to already have it installed.
(Tested on the second computer by the door)


To compile the program run
```
premake4 gmake
make
```
from root
(Note that this steps has to be taken since code in `shared` is modified)


then run
```
premake4 gmake
make
```
inside A_Project


Then run the program using `./Main`

#### Inputs

 - W, A, S, D: for movement control
 - Q, E: for moving up and down (god mode only, for how to enter god mode, read forward)
 - Space: for jumping
 - Left Click: for shooting

type `/help` in game for a list of features that you can toggle on or off.
(To execute a command, press "Enter", then type the command that you want to execute)
Here is a list of commands available. (These can also be found in game wtih `/help`)

#### Objective related

 - `/toggle texture`: Toggle texture mapping on/off
 - `/toggle shadow`: Toggle shadow on/off
 - `/toggle s_col`: Toggle static collision on/off (Note: you will fail through floors)
 - `/toggle d_col`: Toggle dynamic collision on/off
 - `/toggle particle`: Toggle Particle on/off
 - `/toggle bump`: Toggle bump mapping on/off
 - `/toggle bump_test`: Show bump mapping crates (This will teleport you to two crates which clearly shows the effect of bump mapping, it also execute "god")
- `/toggle gui`: Toggle gui on/off
- `/set gravity <float>`: Set gravity multiplier (default 1.0)
- `/set friction <float>`: Set player friction multiplier (default 1.0, set to for example 0.5 to become more slippery)
- `/set sound_bg <float>`: Set background sound volume
- `/set sound_effect <float>`: Set sound effect volume (this includes slime jump, slime death and shooting sound effects)
- `/god`: Toggle god mode on/off (Jumping will be disabled. Use Q, E to go up and down)


#### Extra commands
 - `/help`: List of commands
 - `/toggle skybox`: Toggle skybox on/off
 - `/quit`: Quit the game
 - `/toggle shadow_map`: Show shadow map (this will shows the actual shadow map)
 - `/set s_width <float>`: Set shadow map width (for lower or higher resolution shadows, default 3000)
 - `/set s_height <float>`: Set shadow map height (for lower or higher resolution shadows, default 3000)
 - `/respawn`: Respawn
 - `/killall`: Kill all enemies
 - `/set speed <float>`: Set player speed multiplier
 - `/set render <float>`: Set sound effect volume


### Objectives:
1. Scene and character is modelled correctly using hierarchical model.
2. Texture mapping is supported, and there are at least two different texture mapped objects visible during the gameplay.
3. An user interfaces is created, and it at least includes one button to quit the game.
4. Static collision detection is implemented. Player is not able to walk into a solid wall.
5. Dynamic collision detection is implemented. Bullets is able to kill moving enemies precisely.
6. Particle system is implemented. There is at least one clearly visible particle effect.
7. Physics Engine is implemented. At least 2 physics forces are applied on the player, such that gravity and friction.
8. Synchronized sound is implemented. Synchronized sound is played when a bullet hits an enemy.
9. Bump mapping is implemented. Two of the same object with and without bump mapping are put inside the start scene for comparison.
10. Shadow mapping is implemented using the depth map. Objects in the scene cast shadows.
