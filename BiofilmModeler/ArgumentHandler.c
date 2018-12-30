//
//  ArgumentHandler.c
//  BiofilmModeler
//
//  Created by Mekalanos Lab on 12/6/18.
//  Copyright © 2018 Brian Thomas Ho. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArgumentHandler.h"
#include "ErrorHandler.h"
#include "BiofilmModeler.h"

settings_t *arguments_handler (settings_t *s, int argc, const char *argv[]) {
  //index variables
  int i, j;
  int cell_type, t_cell_type, a_cell_type, flag;
  double value;

  // Define default values
  s->rows = 100;                                // length of array rows
  s->cols = 100;                                // length of array cols
  s->iterations = 500;                         // length of simulation
  s->used_cell_types = MAX_CELL_TYPES;         // number of cell types
  for (i = 0; i <= MAX_CELL_TYPES; i++) {
    s->abundance[i] = 0.0;                     // relative abundance of each cell type
    s->T6A_rate[i] = 0.0;                      // t6 attack rate for each cell type
    s->T6R_rate[i] = 0.0;                      // t6 retaliate rate for each cell type
    s->T6P_mult[i] = 4.0;                      // PARA activation multiplier
    s->grow_rate[i] = 1.0;                     // proliferation rate for each cell type
    s->PARA_flag[i] = 0;                       // flag for whether PARA occurs
    s->PARA_delay[i] = 0;                      // number of iterations before PARA activation
    for (j = 0; j <= MAX_CELL_TYPES; j++) {
      if (i == j) {
        s->T6S_rate[i * MAX_CELL_TYPES + j] = 0.0; // Fully immune to all "self" attacks
      }
      else {
        s->T6S_rate[i * MAX_CELL_TYPES + j] = 1.0; // Fully sensitive to all "non-self" attacks
      }
    }
  }
  s->grow_rate[0] = 0;                         // empty matrix points/dead cells do not grow
  s->cen_fp = stdout;                          // default all output to stdout
  s->mat_fp = NULL;                            // default all output to stdout
  s->par_fp = NULL;                          // default all output to stdout
  
  for (i = 1; i < argc; i++) {
    // ignore non-flags
    if (strncmp(argv[i], "-", 1)) continue;

    // help flag - if present, will exit program with list of possible flags and value ranges
    if (!strncmp(argv[i], "-h", 2) || !strncmp(argv[i], "?", 1))
      error_handler('H', argv[0]);

    
    // Matrix Initial State Flags
    // - Matrix format flags will determine the configuration of the cell seeding.
    // - Rows & cols define matrix dimensions.
    // - Iterations determine length of the simulation.
    // - Used cell types are the number different cell types used in the simulation
    
    // Matrix rows [-x] [50 (default), integer value from 2 to MAX_MATRIX_WIDTH]
    if (!strncmp(argv[i], "-x", 2)) {
      s->rows = atoi(argv[i+1]);
      if (s->rows < 1 || s->rows > MAX_MATRIX_WIDTH)
        error_handler('R', "[-x] Invalid rows length. Use [-h] flag for list of options.");
      continue;
    }
    // Matrix columns [-y] [50 (default), integer value from 2 to MAX_MATRIX_HEIGHT]
    if (!strncmp(argv[i], "-y", 2)) {
      s->cols = atoi(argv[i+1]);
      if (s->cols < 2 || s->cols > MAX_MATRIX_HEIGHT)
        error_handler('R', "[-y] Invalid column length. Use [-h] flag for list of options.");
      continue;
    }
    // Iterations [-i] [500 (default), integer value from 1 to MAX_ITERATIONS]
    if (!strncmp(argv[i], "-i", 2)) {
      s->iterations = atoi(argv[i+1]);
      if (s->iterations < 0 || s->iterations > MAX_ITERATIONS)
        error_handler('R', "[-i] Invalid number of iterations. Use [-h] flag for list of options.");
      continue;
    }
    // Used Cell types [-n] [integer from 1 to MAX_CELL_TYPES]
    // default is MAX_CELL_TYPES... providing this number simply increases program/output efficiency
    if (!strncmp(argv[i], "-n", 2)) {
      s->used_cell_types = atoi(argv[i+1]);
      if (s->used_cell_types < 1 || s->used_cell_types > MAX_CELL_TYPES)
        error_handler('R', "[-n] Invalid number of used cell types.");
      continue;
    }

    // Cell Type Parameters
    //  - These parameters determine the relative rates of different properties of each cell type.
    //  - Cell types are integer values (e.g. 1, 2, 3, etc.). 0 is reserved for empty/dead cells.
    //  - Values are assigned based on the two fields following each flag with the first field
    //     defining the cell type and the second determining the value.
    //  - T6 sensitivity is uses three fields following the flag with the first two defning cell
    //     types and the third beign the value.
    //  - Initial abundance determines the relative ratios of different cell types.

    // Initial abundance of each cell type [-a] [CELL TYPE] [RELATIVE ABUNDANCE]
    if (!strncmp(argv[i], "-a", 2)) {
      cell_type = atoi(argv[i+1]);
      if (cell_type < 0 || cell_type > MAX_CELL_TYPES)
        error_handler('R', "[-a] Invalid cell type. Use -h flag for list of options.");
      value = atof(argv[i+2]);
      if (value < 0 || value > 1000)
        error_handler('R', "[-a] Initial abundance exceeds bounds [0, 1000]. Use -h flag for list of options.");
      else
        s->abundance[cell_type] = value;
      continue;
    }
    
    // PARA flag [-p] [CELL TYPE] [0 or 1]
    if (!strncmp(argv[i], "-p", 2)) {
      cell_type = atoi(argv[i+1]);
      if (cell_type < 0 || cell_type > MAX_CELL_TYPES)
        error_handler('R', "[-p] Invalid cell type. Use -h flag for list of options.");
      flag = atoi(argv[i+2]);
      if (flag != 0 && flag != 1)
        error_handler('R', "[-p] PARA state must be 0 (no) or 1 (yes). Use -h flag for list of options.");
      s->PARA_flag[cell_type] = flag;
      continue;
    }
    
    // PARA delay [-d] [CELL TYPE] [DELAY BEFORE PARA ACTIVATION (integer from 0 to MAX_ITERATIONS]
    if (!strncmp(argv[i], "-d", 2)) {
      cell_type = atoi(argv[i+1]);
      if (cell_type < 0 || cell_type > MAX_CELL_TYPES)
        error_handler('R', "[-d] Invalid cell type. Use -h flag for list of options.");
      s->PARA_delay[cell_type] = atoi(argv[i+2]);
      if (s->PARA_delay[cell_type] < 0 || s->PARA_delay[cell_type] > MAX_ITERATIONS)
        error_handler('R', "[-d] Invalid PARA delay. Use -h flag for list of options.");
      continue;
    }
    
    // T6 attack rates [-t6a] [CELL TYPE] [T6 ATTACK RATE]
    if (!strncmp(argv[i], "-t6a", 4)) {
      cell_type = atoi(argv[i+1]);
      if (cell_type < 1 || cell_type > MAX_CELL_TYPES)
        error_handler('R', "[-t6a] Invalid cell type. Use -h flag for list of options.");
      value = atof(argv[i+2]);
      if (value < 0 || value > 1)
        error_handler('R', "[-t6a] Rate outside bounds [0, 1]. Use -h flag for list of options.");
      else
        s->T6A_rate[cell_type] = value;
      continue;
    }

    // T6 PARA multiplier [-t6p] [CELL TYPE] [T6 rate multiplier]
    if (!strncmp(argv[i], "-t6p", 4)) {
      cell_type = atoi(argv[i+1]);
      if (cell_type < 1 || cell_type > MAX_CELL_TYPES)
        error_handler('R', "[-t6p] Invalid cell type. Use -h flag for list of options.");
      value = atof(argv[i+2]);
      if (value < 0 || value > 10000)
        error_handler('R', "[-t6p] Rate outside bounds [0, 10000]. Use -h flag for list of options.");
      else
        s->T6P_mult[cell_type] = value;
      continue;
    }

    // T6 sensitivity rates [-t6s] [TARGET CELL TYPE] [ATTACKER CELL TYPE] [sensitivity to T6 ATTACK]
    if (!strncmp(argv[i], "-t6s", 4)) {
      t_cell_type = atoi(argv[i+1]);
      a_cell_type = atoi(argv[i+2]);
      if (t_cell_type < 1 || t_cell_type > MAX_CELL_TYPES)
        error_handler('R', "[-t6s] Invalid recipient cell type. Use -h flag for list of options.");
      if (a_cell_type < 1 || a_cell_type > MAX_CELL_TYPES)
        error_handler('R', "[-t6s] Invalid attacker cell type. Use -h flag for list of options.");
      value = atof(argv[i+3]);
      if (value < 0 || value > 1)
        error_handler('R', "[-t6s] Rate outside bounds [0, 1]. Use -h flag for list of options.");
      else
        s->T6S_rate[t_cell_type * MAX_CELL_TYPES + a_cell_type] = value;
      continue;
    }
    
    // T6 retaliation rates [-t6r] [CELL TYPE] [T6 COUNTERATTACK RATE]
    if (!strncmp(argv[i], "-t6r", 4)) {
      cell_type = atoi(argv[i+1]);
      if (cell_type < 1 || cell_type > MAX_CELL_TYPES)
        error_handler('R', "[-t6r] Invalid cell type. Use -h flag for list of options.");
      value = atof(argv[i+2]);
      if (value < 0 || value > 1)
        error_handler('R', "[-t6r] Rate outside bounds [0, 1]. Use -h flag for list of options.");
      else
        s->T6R_rate[cell_type] = value;
      continue;
    }

    // growth rates [-g] [CELL TYPE] [RELATIVE GROWTH RATE]
    if (!strncmp(argv[i], "-g", 2)) {
      cell_type = atoi(argv[i+1]);
      if (cell_type < 0 || cell_type > MAX_CELL_TYPES)
        error_handler('R', "[-g] Invalid cell type. Use -h flag for list of options.");
      value = atof(argv[i+2]);
      if (value < 0)
        error_handler('R', "[-g] Rate outside bounds [0, 1]. Use -h flag for list of options.");
      else
        s->grow_rate[cell_type] = value;
      continue;
    }
    
    // FILE HANDLES
    //  - Matrix output will contain tab delimited matrix with each cell type at each matrix point.
    //  - Census file will contain a list of the relative abundance of each cell type in the matrix.
    // Census output file [-oc] [string]
    if (!strncmp(argv[i], "-oc", 3)) {
      if (NULL == (s->cen_fp = fopen(argv[i+1], "w")))
        error_handler('R', "[-o] Cannot create census output file.");
      continue;
    }
    
    // Matrix output file [-om] [string]
    if (!strncmp(argv[i], "-om", 3)) {
      if (NULL == (s->mat_fp = fopen(argv[i+1], "w")))
        error_handler('R', "[-m] Cannot create matrix output file.");
      continue;
    }

    // Matrix output file [-op] [string]
    if (!strncmp(argv[i], "-op", 3)) {
      if (NULL == (s->par_fp = fopen(argv[i+1], "w")))
        error_handler('R', "[-m] Cannot create PARA output file.");
      continue;
    }

  }
  
  return s;
}
