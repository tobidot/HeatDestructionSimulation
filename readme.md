
## Simulation

### Controls

Currently you can only switch the render modes.

| Key | Action |
|:-:|:-:|
| I | Visualizes the impulses |
| H | Visualizes the heatmap |
| M | Visualizes the masses |
| P | Visualizes the pressure |



### The World

The world is stored as a simple grid of blocks,
with a fixed width and height.

And some constants define some of the worlds behaviour.
+ gravity : int

### Blocks
Every block has 4 attributes.
+ mass : int
+ pressure : int 
+ heat : int
+ type : enum
+ impuls  : vector


#### Pressure
The pressure value essentialy tells how many blocks are compressed into this one.
If two neighbouring blocks merge together their pressure value adds up.
A block always tries to minimize its pressure value as much as possible.
This is achived by adding an impuls towards a block with a lower pressure.

#### Mass
This is the basemass of a single block without pressure.

This mass acts upon a block simply multipling the gravity with its mass.
> gravity_impuls = {0, mass * pressure * consts::gravity }

#### Heat
This attribute is the only thing the user can manipulate.
Heat is trying to even out throughout the whole world.
In one step the current heat is the median of all neighbouring and the current block.

#### Impuls
This value mostly determines wether something happens or not.
If this value is bigger than a certain threshold,
the blocks values will copy towards the targets block and the target block moves to this block.

#### Type
Type defines some extra behaviours when beeig next to other blocks.
Will be polished and defined later.


### World Bounds
The world is simulated in a relatively small box.  
*Currently 50x50*  
In the horizontal axis the world is repeating.
So what gets out to the right, comes in from the far left.
But in the verticals the world has set boundries defined by its height.
These boundries exist of never changeing blocks.
They keep the same impuls, pressure, heat , etc., no matter the influence.
They even have their special block types.
At the bottom HELLSTONE has fixed values of

| TYPE | Mass | Pressure | Heat | Impuls |
|:-:|:-:|:-:|:-:|:-:|:-:|
| HELLSTONE | 100 | 1 | 100000 | (0,0) |

Above the top of the world HEAVENSTONE exists.

| TYPE | Mass | Pressure | Heat | Impuls |
|:-:|:-:|:-:|:-:|:-:|:-:|
| HEAVENSTONE | 0 | 0 | 0 | (0,0) |


### Movements

In pr0gress



## Example

~Too much will change here so example is not in sync with the code~

##### Map

| | | |
|---|---|:-:|
| 2m 1p (0,0)i | 1m 1p (0,0)i | 0m 0p (0,0)i |
| 0m 0p (0,0)i | 0m 0p (0,0)i | 0m 0p (0,0)i |
| 1m 1p (0,0)i | 0m 0p (0,0)i | 0m 0p (0,0)i |




## Thoughts and Upcumming Features

### Step 1

Impuls
(0,0) (0,0) (0,0)
(0,0) (0,0) (0,0)
(0,0) (0,0) (0,0)

Mass 
5 1 0
0 0 1
1 0 0

Pressure
1 1 0
0 0 1
1 0 0

--- Step 2

ForceGravity = ( 0, Mass * Gravity )
ForcePressure = ( left - right , top - bottom )
ForceImpuls = |+left| + |-right| + |+top| + |-right|					// *1
Result = OldImpuls + ForceGravity + ForcePressure + ForceImpuls
(-1,6) (1,1) (0,-1)
(1,0) (-1,0) (0,1)
(0,0) (1,0) (-1,1)

Mass 
2 1 0
0 0 1
1 0 0

Pressure
1 1 0
0 0 1
1 0 0

### Conditions

If  (Impuls - (target-impuls)) > 10 * ((target-mass)+(mass)) => swap places
If  :same-block-type: (Impuls) > 10 * ((target-mass)+(mass)) => merge
	impuls = (mine * mine-pressure + target * target-pressure) / (min-pressure + target-pressure)
	pressure = mine + target











|+x| nur der positve anteil zählt
|-x| nur der negative anteil zählt


x value of worldblock
? overlapping value
- infinity

