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


then run
```
premake4 gmake
make
```
inside A_Project


Then run the program using `./Main`


type `/help` in game for a list of features that you can toggle on or off.


Objectives:
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
