## Simulation

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
The mass is a value defined by 
> base_mass * pressure

This mass acts upon a block simply multipling the gravity with its mass.
> gravity_impuls = {0, mass * consts::gravity }

#### Type
Type defines some extra behaviours when beeig next to other blocks.
Will be polished and defined later.

#### Heat
This attribute is the only thing the user can manipulate.
Heat is trying to even out throughout the whole world.
In one step the current heat is the median of all neighbouring and the current block.

#### Impuls
This value mostly determines wether something happens or not.
If this value is bigger than a certain threshold,
the blocks values will copy towards the targets block and the target block moves to this block.


## Example

##### Pressure Map

| | | |
|---|---|:-:|
| 2m 1p (0,0)i | 1m 1p (0,0)i | 0m 0p (0,0)i |
| 0m 0p (0,0)i | 0m 0p (0,0)i | 0m 0p (0,0)i |
| 1m 1p (0,0)i | 0m 0p (0,0)i | 0m 0p (0,0)i |

###### Thoughts

World boundries

Impuls 
000
?x?
000

Mass 
000
?x?
nnn

#### World boundries

At the borders of the world some unchanging blocks are placed.
Since the world is repeating in horizontal direction,
they are only necessary above and below the world.

| |
|---|
| 0m 0p (0,0)i | 
| World | 
| 1m 1p (0,0)i | 


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

