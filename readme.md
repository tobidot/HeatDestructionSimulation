Force Pressure -> Wanted Result

1 1 1   2 0 0		0 0 0
1 1 1   0 1 0	-> 	2 0 0
1 1 1   0 0 0		0 1 0





World boundries

Impuls 
000
?x?
000

Mass 
000
?x?
nnn

Pressure
000
?x?
---


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

