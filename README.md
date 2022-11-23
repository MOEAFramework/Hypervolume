# Hypervolume Calculation

This project contains a collection of open-source codes for computing the hypervolume of a Pareto set.  The codes may have been modified
from their original sources in order to standardize their interfaces.  All codes posted here are compatible with the
[MOEA Framework](http://www.moeaframework.org).  Refer to section 10.1 in the MOEA Framework Beginner's Guide for additional information
on using external hypervolume calculators.

## WFG Setup

Run `make` to compile.  Copy `wfg2` to the MOEA Framework root directory.  Modify `moeaframework.properties` to include the following lines:

```
org.moeaframework.core.indicator.hypervolume = ./wfg2 {2}
org.moeaframework.core.indicator.hypervolume_inverted = true
```

## HOY Setup

Run `make` to compile.  Copy `hoy` to the MOEA Framework root directory.  Modify `moeaframework.properties` to include the following lines:

```
org.moeaframework.core.indicator.hypervolume = ./hoy {0} {1} {2} {3} 
org.moeaframework.core.indicator.hypervolume_inverted = false
```

## Building on Windows

These codes have been tested on Windows using the MinGW compilers.  Some compilers automatically add the `.exe` extension, so be sure to update the `moeaframework.properties` file with the extension if necessary.

## Performance

The performance of any hypervolume calculation is dependent on the number of objectives (dimensions).  Below is a very rough comparison of the performance of each approach, starting with the 4-objective DTLZ2 reference set:

    Built-in:  0.618 seconds
    WFG:       0.576 seconds
    HOY:       0.561 seconds
    
And for the 6-objective DTLZ2 reference set:

    Built-in:  64.854 seconds
    WFG:        0.660 seconds
    HOY:        2.430 seconds
    
Note: the built-in calculation is the hypervolume calculation included in the MOEA Framework by default.  Due to differences in licensing, the WFG
and HOY codes can not be distributed within the MOEA Framework.  However, following the instructions above, you can enable the WFG or HOY
calculations for your personal use.
