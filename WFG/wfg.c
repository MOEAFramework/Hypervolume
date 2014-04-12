/*

 This program is free software (software libre); you can redistribute
 it and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2 of the
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, you can obtain a copy of the GNU
 General Public License at:
                 http://www.gnu.org/copyleft/gpl.html
 or by writing to:
           Free Software Foundation, Inc., 59 Temple Place,
                 Suite 330, Boston, MA 02111-1307 USA

 ----------------------------------------------------------------------

*/

// To do: 
// - can we sort less often or reduce/optimise dominance checks? 
// - should we use FPL's data structure? 
// - two changes in read.c 
// - heuristics 

// opt:  0 = basic, 1 = sorting, 2 = slicing to 2D, 3 = slicing to 3D 

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "wfg.h"
#include "avl.h"

#define MAXIMISING true

#if MAXIMISING
#define BEATS(x,y)   (x >  y) 
#define BEATSEQ(x,y) (x >= y) 
#else
#define BEATS(x,y)   (x <  y) 
#define BEATSEQ(x,y) (x <= y) 
#endif

#define WORSE(x,y)   (BEATS(y,x) ? (x) : (y)) 
#define BETTER(x,y)  (BEATS(y,x) ? (y) : (x)) 

int n;     // the number of objectives 
POINT ref; // the reference point 

FRONT *fs;      // memory management stuff 
int fr = 0;     // current depth 
int frmax = -1; // max depth malloced so far (for opt = 0) 
int maxm = 0;   // identify the biggest fronts in the file 
int maxn = 0;


static avl_tree_t *tree;
double hv(FRONT);

static int compare_tree_asc( const void *p1, const void *p2)
{
    const double x1= *((const double *)p1+1);
    const double x2= *((const double *)p2+1);

    if (x1 != x2) return (x1 > x2) ? -1 : 1;
    else          return 0;
}


int greater(const void *v1, const void *v2)
// this sorts points improving in the last objective
{
  POINT p = *(POINT*)v1;
  POINT q = *(POINT*)v2;
  #if opt == 1
  for (int i = n - fr - 1; i >= 0; i--)
  #else
  for (int i = n - 1; i >= 0; i--)
  #endif
    if BEATS(p.objectives[i],q.objectives[i]) return  1;
    else
    if BEATS(q.objectives[i],p.objectives[i]) return -1;
  return 0;
}


int dominates2way(POINT p, POINT q)
// returns -1 if p dominates q, 1 if q dominates p, 2 if p == q, 0 otherwise 
{
  // domination could be checked in either order 
  #if opt == 1
  for (int i = n - fr - 1; i >= 0; i--)
  #else
  for (int i = n - 1; i >= 0; i--)
  #endif
    if BEATS(p.objectives[i],q.objectives[i]) 
      {for (int j = i - 1; j >= 0; j--) 
         if BEATS(q.objectives[j],p.objectives[j]) return 0; 
       return -1;}
    else
    if BEATS(q.objectives[i],p.objectives[i]) 
      {for (int j = i - 1; j >= 0; j--) 
         if BEATS(p.objectives[j],q.objectives[j]) return 0; 
       return  1;}
  return 2;
}


void makeDominatedBit(FRONT ps, int p)
// creates the front ps[p+1 ..] in fs[fr], with each point bounded by ps[p] and dominated points removed 
{
  // when opt = 0 each new frame is allocated as needed, because the worst-case needs #frames = #points 
  #if opt == 0
  if (fr > frmax)
    {frmax = fr;
     fs[fr].points = malloc(sizeof(POINT) * maxm);
     for (int j = 0; j < maxm; j++) 
     {
       fs[fr].points[j].objectives = malloc(sizeof(OBJECTIVE) * maxn);
     }
    }
  #endif

  int z = ps.nPoints - 1 - p;
  for (int i = 0; i < z; i++)
    for (int j = 0; j < n; j++) 
      fs[fr].points[i].objectives[j] = WORSE(ps.points[p].objectives[j],ps.points[p + 1 + i].objectives[j]); 
  POINT t;
  fs[fr].nPoints = 1;
  for (int i = 1; i < z; i++)
    {int j = 0;
     bool keep = true;
     while (j < fs[fr].nPoints && keep)
       switch (dominates2way(fs[fr].points[i], fs[fr].points[j]))
	 {case -1: t = fs[fr].points[j];
                   fs[fr].nPoints--; 
                   fs[fr].points[j] = fs[fr].points[fs[fr].nPoints]; 
                   fs[fr].points[fs[fr].nPoints] = t; 
                   break;
          case  0: j++; break;
          // case  2: printf("Identical points!\n");
	  default: keep = false;
	 }
     if (keep) {t = fs[fr].points[fs[fr].nPoints]; 
                fs[fr].points[fs[fr].nPoints] = fs[fr].points[i]; 
                fs[fr].points[i] = t; 
                fs[fr].nPoints++;}
    }
  fr++;
}


double hv2(FRONT ps)
// returns the hypervolume of ps[0 ..] in 2D 
// assumes that ps is sorted improving
{
  double volume = fabs((ps.points[0].objectives[0] - ref.objectives[0]) * 
                       (ps.points[0].objectives[1] - ref.objectives[1])); 
  for (int i = 1; i < ps.nPoints; i++) 
    volume += fabs((ps.points[i].objectives[0] - ref.objectives[0]) * 
                   (ps.points[i].objectives[1] - ps.points[i - 1].objectives[1]));
  return volume;
}

double hv3_AVL(FRONT ps)
/* hv3_AVL: 3D algorithm code taken from version hv-1.2 available at
http://iridia.ulb.ac.be/~manuel/hypervolume and proposed by:

Carlos M. Fonseca, Luís Paquete, and Manuel López-Ibáñez.  An improved
dimension-sweep algorithm for the hypervolume indicator. In IEEE
Congress on Evolutionary Computation, pages 1157-1163, Vancouver,
Canada, July 2006.

                     Copyright (c) 2009
                Carlos M. Fonseca <cmfonsec@ualg.pt>
           Manuel Lopez-Ibanez <manuel.lopez-ibanez@ulb.ac.be>
                  Luis Paquete <paquete@dei.uc.pt>
*/

// returns the hypervolume of ps[0 ..] in 3D 
// assumes that ps is sorted improving
{
  avl_init_node(ps.points[ps.nPoints-1].tnode,ps.points[ps.nPoints-1].objectives);
  avl_insert_top(tree,ps.points[ps.nPoints-1].tnode);

  double hypera = (ref.objectives[0] - ps.points[ps.nPoints-1].objectives[0]) *
    (ref.objectives[1] - ps.points[ps.nPoints-1].objectives[1]);

  double height;
  if (ps.nPoints == 1)
    height = ref.objectives[2] - ps.points[ps.nPoints-1].objectives[2];
  else
    height = ps.points[ps.nPoints-2].objectives[2] - ps.points[ps.nPoints-1].objectives[2];

  double hyperv = hypera * height;

  for (int i = ps.nPoints - 2; i >= 0; i--)
  {
    if (i == 0)
      height = ref.objectives[2] - ps.points[i].objectives[2];
    else
      height = ps.points[i-1].objectives[2] - ps.points[i].objectives[2];

      // search tree for point q to the right of current point
      const double * prv_ip, * nxt_ip;
      avl_node_t *tnode;

      avl_init_node(ps.points[i].tnode, ps.points[i].objectives);

      if (avl_search_closest(tree, ps.points[i].objectives, &tnode) <= 0) {
          nxt_ip = (double *)(tnode->item);
          tnode = tnode->prev;
      } else {
          nxt_ip = (tnode->next!=NULL)
              ? (double *)(tnode->next->item)
              : ref.objectives;
      }
                // if p is not dominated
                if (nxt_ip[0] > ps.points[i].objectives[0]) {

                  // insert p in tree
                    avl_insert_after(tree, tnode, ps.points[i].tnode);

                    if (tnode !=NULL) {
                        prv_ip = (double *)(tnode->item);

                        if (prv_ip[0] > ps.points[i].objectives[0]) {
                            const double * cur_ip;

                            tnode = ps.points[i].tnode->prev;
                            // cur_ip = point dominated by pp with highest [0]-coordinate
                            cur_ip = (double *)(tnode->item);

                            // for each point in s in tree dominated by p
                            while (tnode->prev) {
                                prv_ip = (double *)(tnode->prev->item);
                                // decrease area by contribution of s
                                hypera -= (prv_ip[1] - cur_ip[1])*(nxt_ip[0] - cur_ip[0]);
                                if (prv_ip[0] < ps.points[i].objectives[0])
                                    break; // prv is not dominated by pp
                                cur_ip = prv_ip;
                                // remove s from tree
                                avl_unlink_node(tree,tnode);
                                tnode = tnode->prev;
                            }

                            // remove s from tree
                            avl_unlink_node(tree,tnode);

                            if (!tnode->prev) {
                                // decrease area by contribution of s
                                hypera -= (ref.objectives[1] - cur_ip[1])*(nxt_ip[0] - cur_ip[0]);
                                prv_ip = ref.objectives;
                            }
                        }
                    } else
                        prv_ip = ref.objectives;

                    // increase area by contribution of p
                    hypera += (prv_ip[1] -
                        ps.points[i].objectives[1])*(nxt_ip[0] -
                          ps.points[i].objectives[0]);

                }

                if (height > 0)
                    hyperv += hypera * height;
        }
        avl_clear_tree(tree);
        return hyperv;
  }


double inclhv(POINT p)
// returns the inclusive hypervolume of p
{
  double volume = 1;
  for (int i = 0; i < n; i++) 
    volume *= fabs(p.objectives[i] - ref.objectives[i]);
  return volume;
}


double exclhv(FRONT ps, int p)
// returns the exclusive hypervolume of ps[p] relative to ps[p+1 ..] 
{
  double volume = inclhv(ps.points[p]);
  if (ps.nPoints > p + 1) 
    {
     makeDominatedBit(ps, p);
     volume -= hv(fs[fr - 1]);
     fr--;
    }
  return volume;
}


double hv(FRONT ps)
// returns the hypervolume of ps[0 ..] 
{
  #if opt > 0
  qsort(ps.points, ps.nPoints, sizeof(POINT), greater);
  #endif

  #if opt == 2
  if (n == 2) return hv2(ps);
  #elif opt == 3
  if (n == 3) return hv3_AVL(ps);
  #endif

  double volume = 0;

  #if opt <= 1
  for (int i = 0; i < ps.nPoints; i++) volume += exclhv(ps, i);
  #else
  n--;
  for (int i = ps.nPoints - 1; i >= 0; i--)
    // we can ditch dominated points here, 
    // but they will be ditched anyway in dominatedBit 
    volume += fabs(ps.points[i].objectives[n] - ref.objectives[n]) * exclhv(ps, i);

  n++; 
  #endif

  return volume;
}


int main(int argc, char *argv[]) 
// processes each front from the file 
{
  FILECONTENTS *f = readFile(argv[1]);

  // find the biggest fronts
  for (int i = 0; i < f->nFronts; i++)
    {if (f->fronts[i].nPoints > maxm) maxm = f->fronts[i].nPoints;
     if (f->fronts[i].n       > maxn) maxn = f->fronts[i].n;
    }

  // allocate memory
  #if opt == 0
  fs = malloc(sizeof(FRONT) * maxm);
  #else

  // slicing (opt > 1) saves a level of recursion
  int maxd = maxn - (opt / 2 + 1); 
  fs = malloc(sizeof(FRONT) * maxd);

  // 3D base (opt = 3) needs space for the sentinels
  int maxp = maxm + 2 * (opt / 3);
  //int maxp = 100000;
  for (int i = 0; i < maxd; i++) 
    {fs[i].points = malloc(sizeof(POINT) * maxp); 
     for (int j = 0; j < maxp; j++) 
     {
       fs[i].points[j].tnode = malloc(sizeof(avl_node_t));
       // slicing (opt > 1) saves one extra objective at each level
       fs[i].points[j].objectives = malloc(sizeof(OBJECTIVE) * (maxn - (i + 1) * (opt / 2)));
     }
    }
  #endif

  tree = avl_alloc_tree ((avl_compare_t) compare_tree_asc,
                         (avl_freeitem_t) free);

  // initialise the reference point
  ref.objectives = malloc(sizeof(OBJECTIVE) * maxn);
  ref.tnode = malloc(sizeof(avl_node_t));
  if (argc == 2)
    {printf("No reference point provided: using the origin\n");
     for (int i = 0; i < maxn; i++) ref.objectives[i] = 0;
    }
  else if (argc - 2 != maxn)
    {printf("Your reference point should have %d values\n", maxn);
     return 0;
    }
  else 
  for (int i = 2; i < argc; i++) ref.objectives[i - 2] = atof(argv[i]);

  for (int i = 0; i < f->nFronts; i++) 
    {      
      struct timeval tv1, tv2;
      struct rusage ru_before, ru_after;
      getrusage (RUSAGE_SELF, &ru_before);
 
      n = f->fronts[i].n;
      #if opt >= 3
      if (n == 2)
        {qsort(f->fronts[i].points, f->fronts[i].nPoints, sizeof(POINT), greater);
         printf("hv(%d) = %1.10f\n", i+1, hv2(f->fronts[i])); 
        }
      else
      #endif
      printf("hv(%d) = %1.10f\n", i+1, hv(f->fronts[i])); 

      getrusage (RUSAGE_SELF, &ru_after);
      tv1 = ru_before.ru_utime;
      tv2 = ru_after.ru_utime;
      printf("Time: %f (s)\n", tv2.tv_sec + tv2.tv_usec * 1e-6 - tv1.tv_sec - tv1.tv_usec * 1e-6);
    }

  return 0;
}
