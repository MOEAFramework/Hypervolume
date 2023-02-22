# Hypervolume Calculation

**Newer versions of the MOEA Framework, starting with version 3.4, include a significantly faster hypervolume implementation.  This project is being archived and may not receive further updates.**

This project contains a collection of open-source codes for computing the hypervolume of a Pareto set.  The codes may have been modified
from their original sources in order to standardize their interfaces.  Please refer to the documentation and license information included with
each implementation for additional details.

The setup instructions below show how to compile and configure each implementation with the [MOEA Framework](http://www.moeaframework.org).  
Refer to section 10.1 in the MOEA Framework Beginner's Guide for additional information.

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

Run `make` from the `HV/` subdirectory to compile.  Copy `hv` to the MOEA Framework root directory.  Modify `moeaframework.properties` to include the following lines:

```
org.moeaframework.core.indicator.hypervolume = ./hv {2}
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

The performance of any hypervolume calculation is dependent on the number of objectives (dimensions) and the number of points in the Pareto front.
Below is a very rough comparison of the performance of these implementations:

Data Set    | Objectives | Built-in | ./wfg2  | ./hv
----------- | ---------- | -------- | ------- | ------
DTLZ2.2D.pf | 2          | 252 ms   | 275 ms  | 273 ms
DTLZ2.3D.pf | 3          | 251 ms   | 293 ms  | 280 ms
DTLZ2.4D.pf | 4          | 296 ms   | 322 ms  | 303 ms
DTLZ2.6D.pf | 6          | 468 ms   | 359 ms  | 682 ms
DTLZ2.8D.pf | 8          | 5.8 sec  | 1.5 sec | 101 sec

This leads to a few conclusions:
1. The built-in implementation is close in performance to its natively compiled variant WFG.  We only begin to see a difference with 8+ objectives.
2. Users performing many hypervolume calculations with 8+ more objectives could still benefit from using the compiled WFG implementation
