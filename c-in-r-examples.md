# Simple examples of C code in R sources

## Session 1

Code examples that (mainly) use concepts from Session 1

### If, while

Finding the number of the active device, starting from `from`

https://github.com/r-devel/r-svn/blob/2c0ed92eae8a9845b42dfcd984b9942e751e57a6/src/main/devices.c#L192-L209

```c
int nextDevice(int from)
{
    if (R_NumDevices == 1)
	return 0;
    else {
	int i = from;
	int nextDev = 0;
	while ((i < (R_MaxDevices-1)) && (nextDev == 0))
	    if (active[++i]) nextDev = i;
	if (nextDev == 0) {
	    /* start again from 1 */
	    i = 0;
	    while ((i < (R_MaxDevices-1)) && (nextDev == 0))
		if (active[++i]) nextDev = i;
	}
	return nextDev;
    }
}
```

### If, for

Creating a 3 x 3 identity matrix (part of wider code base related to [transformation matrices](https://en.wikipedia.org/wiki/Transformation_matrix)). 

https://github.com/r-devel/r-svn/blob/2c0ed92eae8a9845b42dfcd984b9942e751e57a6/src/library/grid/src/matrix.c#L62-L71

```c
void identity(LTransform m) 
{
    int i, j;
    for (i=0; i<3; i++) 
	for (j=0; j<3; j++)
	    if (i == j)
		m[i][j] = 1;
	    else
		m[i][j] = 0;
}
```

`LTransfrom` type defined here: https://github.com/r-devel/r-svn/blob/2c0ed92eae8a9845b42dfcd984b9942e751e57a6/src/library/grid/src/grid.h#L153

### Switch 

Compute a "padj" value, which is 0 if the axis label should be horizontal to the axis and 1 otherwise. (See `?par` for the definition of `las` in R; 
`side' numbers the sides of the plot from the bottom, in a clockwise direction.

```c
static double ComputePAdjValue(double padj, int side, int las)
{
    if (!R_FINITE(padj)) {
    switch(las) {
    case 0:/* parallel to axis */
	padj = 0.0; break;
    case 1:/* horizontal */
	switch(side) {
	case 1:
	case 3: padj = 0.0; break;
	case 2:
	case 4: padj = 0.5; break;
	}
	break;
    case 2:/* perpendicular to axis */
	padj = 0.5; break;
    case 3:/* vertical */
	switch(side) {
	case 1:
	case 3: padj = 0.5; break;
	case 2:
	case 4: padj = 0.0; break;
	}
	break;
    }
    }
    return padj;
}
```

## Session 2

### Operators, arithmetic 

Generate a random value from an $F_{n1, n2}$ distribution (by definition, the ratio of two independent Chi-squared 
distributions divided by their degrees of freedom, where the degrees of freedom should be positive). 

https://github.com/r-devel/r-svn/blob/acf386967eb8607fb7967eafec64720e84ad1b65/src/nmath/rf.c#L36-L45

```c
double rf(double n1, double n2)
{
    double v1, v2;
    if (ISNAN(n1) || ISNAN(n2) || n1 <= 0. || n2 <= 0.)
        ML_WARN_return_NAN;

    v1 = R_FINITE(n1) ? (rchisq(n1) / n1) : 1;
    v2 = R_FINITE(n2) ? (rchisq(n2) / n2) : 1;
    return v1 / v2;
}
```
