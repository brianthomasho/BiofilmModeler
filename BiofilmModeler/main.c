//
//  main.c
//  BiofilmModeler
//
//  Created by Mekalanos Lab on 12/6/18.
//  Copyright © 2018 Brian Thomas Ho. All rights reserved.
//


#include <stdio.h>     // input/outputs
#include <time.h>      // for seeding RNG
#include <stdlib.h>    // memory management
#include "BiofilmModeler.h"
#include "ArgumentHandler.h"
#include "ErrorHandler.h"
#include "InitializeMatrix.h"
#include "ComputeMatrix.h"
#include "PrintMatrix.h"

int main(int argc, const char * argv[]) {
  
  // variable declaration
  int i;
  settings_t settings;
  matrix_t biofilm;
  
  // read input arguments
  if (NULL == arguments_handler(&settings, argc, argv))       // define values of settings
    error_handler('A', "Error reading arguments");
  
  
  // initialize environment
  srand((unsigned int) time(NULL));                           // seed RNG with system time
  if (NULL == initialize_matrix(&biofilm, &settings))         // initialize matrix
    error_handler('I', "Error initializing matrix\n");
  
  print_matrix(&biofilm, &settings);
  census_matrix(&biofilm, &settings);
  
  for (i = 0; i < settings.iterations; i++) {
    update_matrix(&biofilm, &settings);
    print_matrix(&biofilm, &settings);
    print_para(&biofilm, &settings);
    census_matrix(&biofilm, &settings);
  }
  
  return 0;
}
