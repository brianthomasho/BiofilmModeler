//
//  ComputeMatrix.c
//  BiofilmModeler
//
//  Created by Mekalanos Lab on 12/6/18.
//  Copyright © 2018 Brian Thomas Ho. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ErrorHandler.h"
#include "BiofilmModeler.h"
#include "ComputeMatrix.h"

matrix_t *update_matrix (matrix_t *m, settings_t *s) {
  int i, j, k, index, target;
  int a, t;
  int neighbor[4], t_neighbor[4];
  double grow_sum, grow_total; // values to determine which neightbor
  double rr[5]; // up to 5 random numbers needed for T6A/T6S/T6R or growth
  double adj_t6a, adj_t6r;
  
  /* set default change as "no change". This is simply a computation safety net as
   each value will be overridden later. */
  for (i = 0; i < m->rows; i++) {
    for (j = 0; j < m->cols; j++) {
      index = i + m->rows * j;
      m->next_cells[index].label = m->cells[index].label;
      m->next_cells[index].PARA_flag = m->cells[index].PARA_flag;
      m->next_cells[index].PARA_timer = m->cells[index].PARA_timer;
    }
  }
  
  // evaluate actions for each cell
  for (i = 0; i < m->rows; i++) {
    for (j = 0; j < m->cols; j++) {
      index = i + m->rows * j;
      neighbor[0] = get_up(index, m);
      neighbor[1] = get_right(index, m);
      neighbor[2] = get_down(index, m);
      neighbor[3] = get_left(index, m);
      
      // choose target for T6 attacks
      target = neighbor[rand() % 4];
      // store target infomation for each (for potential implementations in future versions)
      m->target[index] = target;
      
      for (k = 0; k < 5; k++) {
        /* rr = "random ratio" is random number on interval [0,1]
         rr[0] roll for growth
         rr[1] roll for "is there a T6 attack?"
         rr[2] roll for "is the T6 attack successful (sensitivity)?"
         rr[3] roll for "is there a T6 retaliation?"
         rr[4] roll for "is the T6 retaliation successful (sensitivity)?"
         Each random number is generated separately to avoid reusing numbers */
        rr[k] = (double) rand() / (double) RAND_MAX;
      }
      
      /* Replace Empty/Dead cells with a random neighbor. Higher grow rate of the
       neighbor cell means proportionally higher chance to fill into empty cell.
       Chance for each neighbor to "grow" into this empty cell is weighted by
       their respective "growth rate." Calculation is done by assigning each cell
       type space on the interval from [0,1] with relative abundance determining
       the size of its space. Then use RNG on the interval [0,1] to determine cell
       type for each matrix position. */
      if (m->cells[index].label == 0) { // label == 0 means cell is dead/empty
        grow_sum = grow_total = 0.0;
        for (k = 0; k < 4; k++) {
          grow_total += s->grow_rate[m->cells[neighbor[k]].label];
        }
        if (grow_total == 0.0) continue; // if all neighbors are empty, stay empty
        
        for (k = 0; k < 4; k++) {
          grow_sum += s->grow_rate[m->cells[neighbor[k]].label];
          if (rr[0] < grow_sum / grow_total) {
            m->next_cells[index].label = m->cells[neighbor[k]].label;
            m->next_cells[index].PARA_flag = m->cells[index].PARA_flag;
            m->next_cells[index].PARA_timer = m->cells[index].PARA_timer;
            break;
          }
        }
        continue;
      }
      
      /* Evaluate T6 attacks for each cell. If PARA is active, alter the T6A for
       the attacker. If PARA is active in the target, alter the T6R for the target.
       Then evaluate T6 attacks and retaliations. */
      a = m->cells[index].label;
      t = m->cells[target].label;
      if (m->cells[index].PARA_flag == 1 && m->cells[index].PARA_timer == 0) {
        adj_t6a = 1.0 - pow ( 1.0 - s->T6A_rate[a], s->T6P_mult[a]);
      } else {
        adj_t6a = s->T6A_rate[a];
      }
      if (m->cells[target].PARA_flag == 1 && m->cells[target].PARA_timer == 0) {
        adj_t6r = 1.0 - pow ( 1.0 - s->T6R_rate[t], s->T6P_mult[t]);
      } else {
        adj_t6r = s->T6R_rate[t];
      }
      
      // is there a t6 attack?
      if (rr[1] < adj_t6a) {
        // is the attack successful?
        if (rr[2] < s->T6S_rate[t * MAX_CELL_TYPES + a]) {
          m->next_cells[target].label = 0;
          
          // if killed target has PARA
          if (m->cells[target].PARA_flag == 1) {
            t_neighbor[0] = get_up(target, m);
            t_neighbor[1] = get_right(target, m);
            t_neighbor[2] = get_down(target, m);
            t_neighbor[3] = get_left(target, m);
            
            // activate PARA in neighbors of killed target
            for (k = 0; k < 4; k++) {
              // if a neighbor of the "killed" target is the same cell type as the target
              if (m->cells[t_neighbor[k]].label == t) {
                // if PARA has not already been activated already
                if (m->cells[t_neighbor[k]].PARA_timer < 0) {
                  /* start the PARA timer; +1 is added since increments are evaluated at
                   the end of the iteration, so if delay is zero, timer will be zero during
                   the next iteration*/
                  m->next_cells[t_neighbor[k]].PARA_timer = s->PARA_delay[t] + 1;
                }
              }
            }
          }
        }
        
        // Will the target retaliate?
        if (rr[3] < adj_t6r) {
          // is the retaliatory attack successful? NOTE: a and t are swapped compared to above
          if (rr[4] < s->T6S_rate[a * MAX_CELL_TYPES + t]) {
            m->next_cells[index].label = 0;
            
            // if now dead attacker has PARA
            if (m->cells[index].PARA_flag == 1) {
              // activate PARA in each neighbor
              for (k = 0; k < 4; k++) {
                // if neighbor is same type as attacker
                if (m->cells[neighbor[k]].label == a) {
                  // if PARA is not already active
                  if (m->cells[neighbor[k]].PARA_timer < 0) {
                    /* start the PARA timer; +1 is added since increments are evaluated at
                     the end of the iteration, so if delay is zero, timer will be zero during
                     the next iteration*/
                    m->next_cells[neighbor[k]].PARA_timer = s->PARA_delay[a] + 1;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  
  /* update current matrix with next_cells*/
  for (i = 0; i < m->rows; i++) {
    for (j = 0; j < m->cols; j++) {
      index = i + m->rows * j;
      m->cells[index].label = m->next_cells[index].label;
      
      if (m->cells[index].label == 0) {
        m->cells[index].PARA_timer = -1;
        m->cells[index].PARA_flag = 0;
      } else {
        m->cells[index].PARA_flag = m->next_cells[index].PARA_flag;
        if (m->cells[index].PARA_timer > 0) {
          // increment PARA_timer
          m->cells[index].PARA_timer = m->next_cells[index].PARA_timer - 1;
        } else {
          // if timer is -1 (no activation) or 0 (fully activated), then timer stays the same
          m->cells[index].PARA_timer = m->next_cells[index].PARA_timer;
        }
      }
    }
  }
  
  // increment the iteration counter for the simulation
  m->time++;
  
  return m;
}



// return index of cell to the left
int get_left (int index, matrix_t *mat) {
  return (index + mat->rows * mat->cols - 1) % (mat->rows * mat->cols);
}

// return index of cell to the right
int get_right (int index, matrix_t *mat) {
  return (index + 1) % (mat->rows * mat->cols);
}

// return index of cell above
int get_up (int index, matrix_t *mat) {
  return (index + mat->rows) % (mat->rows * mat->cols);
}

// return index of cell below
int get_down (int index, matrix_t *mat) {
  return (index + mat->rows * mat->cols - mat->rows) % (mat->rows * mat->cols);
}

