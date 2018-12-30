//
//  InitializeMatrix.c
//  BiofilmModeler
//
//  Created by Mekalanos Lab on 12/6/18.
//  Copyright © 2018 Brian Thomas Ho. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "BiofilmModeler.h"
#include "ErrorHandler.h"

matrix_t *initialize_matrix(matrix_t *m, settings_t *s) {
  /* Take information contained in settings (s) and create matrix (m) given those
   parameters. information about the behavior of each cell is stored within the cell
   itself (cell_t). Information can be recalled by m->cells.*/
  
  int x, y;
  int i, j, k;
  int index;
  double rand_ratio, abundance_total, sum;
  
  // matrix dimensions
  x = s->rows;
  y = s->cols;
  
  // allocate memory to store matrix contents
  if (NULL == (m->cells = (cell_t *) malloc(x * y * sizeof(cell_t)))) return NULL;
  if (NULL == (m->next_cells = (cell_t *) malloc(x * y * sizeof(cell_t)))) return NULL;
  if (NULL == (m->target = (int *) malloc(x * y * sizeof(int)))) return NULL;
  
  // Record matrix dimensions
  m->rows = x;
  m->cols = y;
  m->time = 0;
  
  for (i = 0; i < x; i++) {
    for (j = 0; j < y; j++) {
      index = i + x * j;
      
      /* Initialize the matrix with the initial state based on input arguments.
       Assign each cell type space on the interval from [0,1] with relative
       abundance determining the size of its space. Then use RNG on the interval
       [0,1] to determine cell type for each matrix position. */

      rand_ratio = (double) rand() / (double) RAND_MAX;
      abundance_total = 0;
      for (k = 0; k < MAX_CELL_TYPES; k++) {
        abundance_total += s->abundance[k];
      }
      
      // if no cells are present exit program
      if (abundance_total == 0) error_handler('I', "Abundance of all cell types totals to 0.\n");
      
      sum = 0;
      for (k = 0; k < MAX_CELL_TYPES; k++) {
        // k is the current cell type
        sum += s->abundance[k];
        if (rand_ratio < sum / abundance_total) {
          // current cell (index) is assigned cell type k
          m->cells[index].label = k;
          m->cells[index].PARA_timer = -1; // default PARA_timer is -1, meaning inactive
          m->cells[index].PARA_flag = s->PARA_flag[k];
          break;
        }
      }
      
      /* initialize the next_cells matrix; actual values will be assigned as
       the simulation progresses. These values should never actually utilized. */
      m->next_cells[index].label = 0;
      m->next_cells[index].PARA_timer = -1;
      m->next_cells[index].PARA_flag = 0;
      m->target[index] = index;
    }
  }
  
  return m;
}
