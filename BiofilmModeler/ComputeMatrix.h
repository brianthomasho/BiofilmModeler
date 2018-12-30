//
//  ComputeMatrix.h
//  BiofilmModeler
//
//  Created by Mekalanos Lab on 12/6/18.
//  Copyright © 2018 Brian Thomas Ho. All rights reserved.
//

#ifndef ComputeMatrix_h
#define ComputeMatrix_h

#include "BiofilmModeler.h"

matrix_t *update_matrix (matrix_t *, settings_t *);
int get_left (int, matrix_t *);
int get_right (int, matrix_t *);
int get_up (int, matrix_t *);
int get_down (int, matrix_t *);

#endif /* ComputeMatrix_h */
