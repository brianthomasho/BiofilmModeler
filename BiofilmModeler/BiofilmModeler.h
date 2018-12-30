//
//  BiofilmModeler.h
//  BiofilmModeler
//
//  Created by Mekalanos Lab on 12/6/18.
//  Copyright © 2018 Brian Thomas Ho. All rights reserved.
//

#include <stdio.h>

#ifndef Biofilm_modeler_h
#define Biofilm_modeler_h

#define MAX_CELL_TYPES 20       // hard coded total number of cell types
#define MAX_MATRIX_WIDTH 1000   // hard coded max matrix width
#define MAX_MATRIX_HEIGHT 1000  // hard coded max matrix height
#define MAX_ITERATIONS 3000     // hard coded max number of iterations
#define MAX_STRING_LENGTH 1000  // hard coded max input string length

// define cell_t struct
typedef struct {
  int label;                // cell type of the given cell
  int PARA_flag;            // PARA_flag == 1 if PARA activation of occurs
  int PARA_timer;           /* used to calculate time elapsed post PARA activation
                             PARA_timer will be defaulted to -1. When timer is started,
                             it will be set to a positive integer. Each iteration will
                             incremented timer by -1. When timer reaches 0, the state
                             will become "active". Checks will be for
                             PARA_flag == 1 and PARA_timer == 0 */
} cell_t;

// define matrix_t struct
typedef struct {
  cell_t *cells;             // cell array
  cell_t *next_cells;        // next iteration of cell array
  int *target;               // array of action targets for each cell (index of target)
  int rows;                  // length of array rows
  int cols;                  // length of array columns
  int time;                  // iteration counter
} matrix_t;

typedef struct {
  int rows;                               // length of array rows
  int cols;                               // length of array cols
  int iterations;                         // length of simulation
  int used_cell_types;                    // number of cell types that will be used in simulation
  int PARA_flag[MAX_CELL_TYPES + 1];      // flag for whether PARA occurs in the given cell type
  int PARA_delay[MAX_CELL_TYPES + 1];     // number of iterations before PARA activation occurs
  double abundance[MAX_CELL_TYPES + 1];   // relative number of each cell type
  double T6A_rate[MAX_CELL_TYPES + 1];    // t6 attack rate for each cell type
  double T6R_rate[MAX_CELL_TYPES + 1];    // t6 retaliate rate for each cell type
  double grow_rate[MAX_CELL_TYPES + 1];   // proliferation rate for each cell type
  double T6S_rate[MAX_CELL_TYPES * (MAX_CELL_TYPES + 1) + 1]; /* sensitivity to the t6 attacks
                                                               of each cell type */
  double T6P_mult[MAX_CELL_TYPES + 1];    // T6a rate multiplier after PARA activation
  
  FILE *cen_fp;                           // census file handle
  FILE *mat_fp;                           // matrix output file handle
  FILE *par_fp;                           // para activation output file handle
} settings_t;


#endif /* BiofilmModeler_h */
