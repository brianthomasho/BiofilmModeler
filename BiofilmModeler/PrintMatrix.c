//
//  PrintMatrix.c
//  BiofilmModeler
//
//  Created by Mekalanos Lab on 12/29/18.
//  Copyright © 2018 Brian Thomas Ho. All rights reserved.
//

#include <stdio.h>
#include "BiofilmModeler.h"
#include "ErrorHandler.h"
#include "PrintMatrix.h"

matrix_t *print_matrix (matrix_t *m, settings_t *s) {
  // declare variables
  int i, j, index;
  
  if (s->mat_fp == NULL) return m;
  
  // print matrix
  fprintf(s->mat_fp, "\n");
  for (j = 0; j < m->cols; j++) {
    for (i = 0; i < m->rows; i++) {
      index = i + m->rows * j;
      fprintf(s->mat_fp, "\t%d", m->cells[index].label);
    }
    fprintf(s->mat_fp, "\n");
  }
  fprintf(s->mat_fp, "\n");
  
  // flush file stream
  fflush(s->mat_fp);
  
  return m;
}

matrix_t *print_para (matrix_t *m, settings_t *s) {
  // declare variables
  int i, j, index;
  
  if (s->par_fp == NULL) return m;
  
  // print matrix
  fprintf(s->par_fp, "\n");
  for (j = 0; j < m->cols; j++) {
    for (i = 0; i < m->rows; i++) {
      index = i + m->rows * j;
      fprintf(s->par_fp, "\t%d", m->cells[index].PARA_timer);
    }
    fprintf(s->par_fp, "\n");
  }
  fprintf(s->par_fp, "\n");
  
  // flush file stream
  fflush(s->par_fp);
  
  return m;
}


matrix_t *census_matrix (matrix_t *m, settings_t *s) {
  // declare variables
  int i, j, index;
  int census[MAX_CELL_TYPES + 1];
  static int first_time_flag = 0;
  
  if (first_time_flag == 0) {
    fprintf(s->cen_fp,"# Dimensions: %d x %d\n", s->rows, s->cols);
    fprintf(s->cen_fp,"# Max iterations: %d\n", s->iterations);
    fprintf(s->cen_fp,"# of starting cell types: %d\n", s->used_cell_types);
    for (i = 1; i <= s->used_cell_types; i++) {
      fprintf(s->cen_fp,"# cell type %d parameters:\n", i);
      fprintf(s->cen_fp,"#   initial abundance = %g\n", s->abundance[i]);
      fprintf(s->cen_fp,"#   grow_rate = %g\n", s->grow_rate[i]);
      fprintf(s->cen_fp,"#   t6a_rate = %g\n", s->T6A_rate[i]);
      fprintf(s->cen_fp,"#   t6r_rate = %g\n", s->T6R_rate[i]);
      fprintf(s->cen_fp,"#   PARA flag = %d\n", s->PARA_flag[i]);
      fprintf(s->cen_fp,"#   PARA delay = %d\n", s->PARA_delay[i]);
      fprintf(s->cen_fp,"#   PARA multiplier = %g\n", s->T6P_mult[i]);
      for (j = 1; j <= s-> used_cell_types; j++) {
        fprintf(s->cen_fp,"#   Sensitivity to cell type %d = %g\n", j, s->T6S_rate[i * MAX_CELL_TYPES + j]);
      }
    }
    
    fprintf(s->cen_fp, "\nEmpty");
    for (i = 1; i <= s->used_cell_types; i++) {
      fprintf(s->cen_fp, "\t%d", i);
    }
    fprintf(s->cen_fp, "\n");
    first_time_flag++;
  }
  
  // initialize census
  for (i = 0; i <= MAX_CELL_TYPES; i++) census[i] = 0;
  
  // census matrix
  for (i = 0; i < m->rows; i++) {
    for (j = 0; j < m->cols; j++) {
      index = i + m->rows * j;
      census[m->cells[index].label] ++;
    }
  }
  
  // print census information
  fprintf(s->cen_fp, "%d", census[0]);
  for (i = 1; i <= s->used_cell_types; i++) fprintf(s->cen_fp, "\t%d", census[i]);
  fprintf(s->cen_fp, "\n");
  
  // flush file stream
  fflush(s->cen_fp);
  
  return m;
}
