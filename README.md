# Hypervolume Calculation

**Newer versions of the MOEA Framework, starting with version 3.4, include a significantly faster hypervolume implementation.  This project is being archived and may not receive further updates.**

This project contains a collection of open-source codes for computing the hypervolume of a Pareto set.  The codes may have been modified
from their original sources in order to standardize their interfaces.  All codes posted here are compatible with the
[MOEA Framework](http://www.moeaframework.org).  Refer to section 10.1 in the MOEA Framework Beginner's Guide for additional information
on using external hypervolume calculators.

## WFG Setup

Fast hypervolume calculation published by the Walking Fish Group (WFG) and described in:

> While, Ronald Lyndon et al. “A Fast Way of Calculating Exact Hypervolumes.” IEEE Transactions on Evolutionary Computation 16 (2012): 86-95.

Run `make` from the `WFG/` subdirectory to compile.  Copy `wfg2` to the MOEA Framework root directory.  Modify `moeaframework.properties` to include the following lines:

```
org.moeaframework.core.indicator.hypervolume = ./wfg2 {2}
org.moeaframework.core.indicator.hypervolume_inverted = true
```

## HV Setup

Improved dimension-sweep hypervolume calculation as described in:

> Carlos M. Fonseca, Luís Paquete, and Manuel López-Ibáñez. An improved dimension-sweep algorithm for the hypervolume indicator. In IEEE Congress on Evolutionary Computation, pages 1157-1163, Vancouver, Canada, July 2006.

Run `make` from the `HV/` subdirectory to compile.  Copy `fpli_hv` to the MOEA Framework root directory.  Modify `moeaframework.properties` to include the following lines:

```
org.moeaframework.core.indicator.hypervolume = ./fpli_hv {2}
org.moeaframework.core.indicator.hypervolume_inverted = true
```

## HOY Setup

Hypervolume calculation based on Klee's measure problem as described in:

> Nicola Beume and Guenter Rudolph. Faster S-Metric Calculation by Considering Dominated Hypervolume 
as Klee's Measure Problem. In: B. Kovalerchuk (ed.): Proceedings of the Second IASTED Conference on Computational Intelligence (CI 2006), pp. 231-236. ACTA Press: Anaheim, 2006. 

Run `make` from the `HOY/` subdirectory to compile.  Copy `hoy` to the MOEA Framework root directory.  Modify `moeaframework.properties` to include the following lines:

```
org.moeaframework.core.indicator.hypervolume = ./hoy {0} {1} {2} {3} 
org.moeaframework.core.indicator.hypervolume_inverted = false
```

## Building on Windows

If compiling on Windows, please note the `.exe` extension might be added to the executable name.  Be sure to include the correct extension when updating `moeaframework.properties`.

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
