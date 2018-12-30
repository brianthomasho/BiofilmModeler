//
//  ErrorHandler.c
//  BiofilmModeler
//
//  Created by Mekalanos Lab on 12/6/18.
//  Copyright © 2018 Brian Thomas Ho. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "BiofilmModeler.h"

void error_handler (int status, const char *message) {
  switch (status){
    case 'H':
      printf("Usage: %s\n", message);
      printf(" [-h]  Print list of options (overrides all other flags)\n\n\n");
      
      printf(" Simulation set-up flags:\n");
      printf(" [-x] [1, 2, 3, ..., %d]\n", MAX_MATRIX_WIDTH);
      printf("    Matrix row length. Maximum value of %d.\n", MAX_MATRIX_HEIGHT);
      printf("    (default = 100)\n\n");
      printf(" [-y] [1, 2, 3, ..., %d]\n", MAX_MATRIX_HEIGHT);
      printf("    Matrix column length. Maximum value of %d.\n", MAX_MATRIX_HEIGHT);
      printf("    (default = 100)\n\n");
      printf(" [-i] [1, 2, 3, ..., %d]\n", MAX_ITERATIONS);
      printf("    Nummber of iterations to run the simulation, i.e. simulation duration.\n");
      printf("    Simulation capped at %d iterations.\n", MAX_ITERATIONS);
      printf("    (default = 500)\n\n");
      printf(" [-n] [cell type] [integer from 1 to %d]\n", MAX_CELL_TYPES);
      printf("    Number of used cell types (excluding cell type 0, which is reserved for\n ");
      printf("    empty matrix positions and dead cells)\n");
      printf("    Extra cell types information for cell types beyond this number are ignored.\n ");
      printf("    (default value is %d.\n\n", MAX_CELL_TYPES);
      
      printf(" Cell type parameters:\n\n");
      printf(" [-a] [cell type] [number [0, 1000]]\n");
      printf("    Initial relative abundance of the indicated cell type.\n");
      printf("    The matrix is seeded with cells with relative abundances based on their\n");
      printf("     initial relative abundance values.\n");
      printf("    (default value is 1.0 for cell type '0'; all others set to 0.0)\n\n");
      printf(" [-t6a] [cell type] [rational number [0,1]]\n");
      printf("    Rate of T6SS attack for the indicated cell type.\n");
      printf("    Cells with a T6SS will attempt to kill neighboring cells by attacking them.\n");
      printf("     This parameter will determine whether the T6 attack successfully hits an\n");
      printf("     adjacent target cell. The target cells is chosen randomly from the four\n");
      printf("     immediate neighbors (left, right, up and down). Whether the target dies\n");
      printf("     or retaliates is determined by the -t6s and -t6r parameters for the target.\n");
      printf("    (default value is 0.0 for all cell types.)\n\n");
      printf(" [-t6s] [target cell type] [attacker cell type] [rational number [0,1]]\n");
      printf("    Sensitivty of the recipient cell type to T6SS attack from the attacker cell\n");
      printf("     type. Cells with a T6SS will attempt to kill neighboring cells by attacking\n");
      printf("     them. This value will deterimine the rate at which a successful T6 hit from\n");
      printf("     the attacker cell type will result in the death of the recipient cell type.\n");
      printf("     when a cell dies, its death is not registered until the start of the next\n");
      printf("     iteration meaning that actions taken by the attacked cell (i.e. T6 attacks\n");
      printf("     or retaliatory actions can still take place even if the recipient cell will\n");
      printf("     ultimately die. By default, cells are immune to attacks from the same cell\n");
      printf("     type, and fully sensitive to attacks from different cell types.\n");
      printf("    (default value is 1.0 for all non-self attacks and 0.0 for all self attacks.)\n\n");
      printf(" [-t6p] [cell type] [rational number [0,10000]]\n");
      printf("    If cells undergo PARA activation, their rate of T6SS activity increases.\n");
      printf("     This value is the \"fold activation\" of the T6 activity. The simulation\n");
      printf("     will effectively re-roll t6a and t6r rolls this many times. The values of\n");
      printf("     t6a and t6r will be modified as follows:\n");
      printf("       t6a_new = 1 - (1 - t6a)^t6p\n");
      printf("       t6r_new = 1 - (1 - t6r)^t6p\n");
      printf("     This value is only relevant if the -p flag is active for this cell type.\n");
      printf("    (default value is 4.0 for all cell types.)\n\n");
      printf(" [-t6r] [cell type] [rational number [0,1]]\n");
      printf("    Rate of T6SS retaliation for the indicated cell type.\n");
      printf("    Some cell types have the ability to sense exogenous attacks and use their\n");
      printf("     T6SS to attack the source of the attack. T6SS retaliation rate is the\n");
      printf("     probability that an indicated cell type will retaliate after receiving\n");
      printf("     a T6SS attack. This retaliation is independent of whether the initial\n");
      printf("     attack will result in the death of the retaliator cell.\n");
      printf("    (default value is 0.0 for all cell types.)\n\n");
      printf(" [-p] [cell type] [0 or 1]\n");
      printf("    Flag whether PARA activation occurs in this cell type.\n");
      printf("      1 for YES and 0 for NO.\n");
      printf("    (default value is 0 for all cell types.)\n");
      printf(" [-d] [cell type] [interger from 0 to %d]\n", MAX_ITERATIONS);
      printf("    Delay before actiavtion of T6 via PARA.\n");
      printf("    When a neighboring cell of the same cell type dies, T6 will be activated.\n");
      printf("     This value determines how many iterations before T6SS becomes activated.\n");
      printf("     This value will be ignored in cell types where the PARA flag is 0.\n");
      printf("    (default value is 0 for all cell types, indicating immediate activation.)\n\n");
      printf(" [-g] [cell type] [rational number [0,1]\n");
      printf("    Relative growth rate of the indicated cell type. Empty or dead cells\n");
      printf("     are replaced randomly with one of their non-dead neighboring cell types.\n");
      printf("     The relative value of the growth rates of the different cell types\n");
      printf("      determine the likelihood that a given cell type will be chosen.\n");
      printf("    (default value is 1.0 for all cell types.)\n\n\n");
      
      printf(" Output file flags:\n\n");
      printf(" [-oc] [file name]\n");
      printf("    Output file to which cell type census information will be sent.\n");
      printf("    (default will output to stdout.)\n\n");
      printf(" [-om] [file name]\n");
      printf("    Output file to which matrix representations for each iteration will be sent.\n");
      printf("    The format will be as tab-delimited text images.\n");
      printf("    If no file is specified, no matrix file will be created.\n\n");
      printf(" [-op] [file name]\n");
      printf("    Output file to which PARA activations for each iteration will be sent.\n");
      printf("    Each point in the matrix will be the PARA timer for each position:\n");
      printf("     -1 --> no activation:\n");
      printf("      0 --> fully active:\n");
      printf("     >0 --> number of iterations until fully active:\n");
      printf("    The format will be as tab-delimited text images.\n");
      printf("    If no file is specified, no PARA file will be created.\n\n");
      exit(1);
      break;
    case 'M':
      printf("Error allocating memory:\n");
      printf("%s\n",message);
      exit(1);
      break;
    case 'I':
      printf("Error during maxtrix initialization:\n");
      printf("%s\n",message);
      exit(1);
      break;
    case 'R':
      printf("Invalid argument:\n");
      printf("%s\n",message);
      exit(1);
      break;
    default:
      printf("%s\n",message);
  }
  
  exit(0); // exit status 0 if reached end via unknown error
}
