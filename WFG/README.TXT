Lyndon While, Lucas Bradstreet, Luigi Barone

Email {lyndon,lucas,luigi}@csse.uwa.edu.au for any queries regarding
usage/bugs/improvements.

This code includes a high performance implementation of the WFG algorithm, 
used to calculate the hypervolume indicator for a set of non-dominated points.

CREDIT:

Code used to calculate the 3D hypervolume base case was taken from hv-1.2 available at
http://iridia.ulb.ac.be/~manuel/hypervolume and proposed by:

Carlos M. Fonseca, Luís Paquete, and Manuel López-Ibáñez.  An improved
dimension-sweep algorithm for the hypervolume indicator. In IEEE
Congress on Evolutionary Computation, pages 1157-1163, Vancouver,
Canada, July 2006.

Please find the copyright notice below.


COMPILING:

run make march=processortype, e.g. make march=pentium4

USAGE:
where X = 1-3

wfgX FRONTFILE
# calculates hypervolume for frontfile using reference point 0, 0, ..., 0
wfgX FRONTFILE r1 r2 .. rd
# calculates hypervolume for frontfile using reference point r1, r2, .., rd

Code currently performs minimisation hypervolume calculations relative to the 
reference point. However, it can be easily transformed to allow maximisation calculations.


FILE FORMAT:

A file can contain any number of fronts, laid out as follows:

#
0.598 0.737 0.131 0.916 6.745
0.263 0.740 0.449 0.753 6.964
0.109 8.483 0.199 0.302 8.872
#
0.598 0.737 0.131 0.916 6.745
0.263 0.740 0.449 0.753 6.964
0.109 8.483 0.199 0.302 8.872
#

Notes:

- objective values are separated by spaces.
- one point per line.
- fronts are separated by #s.

- all fronts use the same reference point, therefore all points in all
  fronts must have the same number of objectives.


COPYRIGHT:

This software, aside from the hv3_AVL function contained in hv3d.c, 
is Copyright (C) 2010 Lyndon While, Lucas Bradstreet.

This program is free software (software libre); you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

--------

Code for the hv3_AVL function in wfg.c is Copyright (C) 2006-2010 Carlos M. Fonseca, Manuel
López-Ibáñez and Luís Paquete.

Original code contains the following notice:

########
This program is free software (software libre); you can redistribute
it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

IMPORTANT NOTE: Please be aware that the fact that this program is
released as Free Software does not excuse you from scientific
propriety, which obligates you to give appropriate credit! If you
write a scientific paper describing research that made substantive use
of this program, it is your obligation as a scientist to (a) mention
the fashion in which this software was used in the Methods section;
(b) mention the algorithm in the References section. The appropriate
citation is:

 Carlos M. Fonseca, Luís Paquete, and Manuel López-Ibáñez. An improved
 dimension-sweep algorithm for the hypervolume indicator. In IEEE
 Congress on Evolutionary Computation, pages 1157-1163, Vancouver,
 Canada, July 2006.

Moreover, as a personal note, I would appreciate it if you would email
manuel.lopez-ibanez@ulb.ac.be with citations of papers referencing this
work so I can mention them to my funding agent and tenure committee.
#########
