Core:
Use the core button to load a single flock

Completion:
Use the completion button to load 2-5 flocks. There is a slider under Boid Numbers to increase the number of predators which is 
initally set to 0. 

Challenge:
Use the challenge button to load in a single flock, aswell as 3 spheres. The flock will avoid the spheres and go around them. 


In order to make the IMGUI sliders easier to read, the sliders have been placed into various tabs, as outlined below.

Movement:
A slider for Cohesion, Alignment and Avoidance weight. 
Inside the movement tab is a seperate tab for speed, to control the min, max velocity and max acceleration of the boids

Boid Numbers:
There are sliders to control the total number of boids, boid flocks and predator count.
Note that these sliders only take place when the scene is reset, e.g Core, Completion or Challenge is pushed.

Predators:
There are sliders to control how boids avoid predators, aswell as how close predators need to be to 'eat' boids.
There are also sliders to control the max velocity and acceleration of a predator. 
Once a boid has been eaten, it will turn purple and predators will no longer chase them. 

Spheres:
The spheres tab let you control how much the boids avoid the spheres. If 0, then the boids will pass through the spheres effectively disabling
them. Also note, that the spheres are placed based on the boundries. Therefore increasing/decreasing the bounds will move the spheres.
There is also a slider to increase the size of the boid. Note that included is a new sphere model as the one provided was not very round. 

Presets:
Use the covid mode button to have the boids avoid each other at all cost, and maintain social distancing rules.
Use the reset button to reset most values.

There is a screenshot of all the settings open incase I forgot any. 