Hypervolume Calculation
=======================

This project contains a collection of open-source codes for computing the hypervolume of a Pareto set.  The codes may have been modified from their original sources in order to standardize their interfaces.  All codes posted here are compatible with the [MOEA Framework](http://www.moeaframework.org).  Refer to section 10.1 in the MOEA Framework User Manual for additional information on using external hypervolume calculators.

WFG Setup
---------
Run `make` to compile.  Copy `wfg2` to the MOEA Framework root directory.  Modify `global.properties` to include the following lines:

```
org.moeaframework.core.indicator.hypervolume = ./wfg2 {2}
org.moeaframework.core.indicator.hypervolume_inverted = true
```

HOY Setup
---------
Run `make` to compile.  Copy `hoy` to the MOEA Framework root directory.  Modify `global.properties` to include the following lines:

```
org.moeaframework.core.indicator.hypervolume = ./hoy {0} {1} {2} {3} 
org.moeaframework.core.indicator.hypervolume_inverted = false
```

Building on Windows
-------------------
These codes have been tested on Windows using the MinGW compilers.  Some compilers automatically add the `.exe` extension, so be sure to update the `global.properties` file with the extension if necessary.
