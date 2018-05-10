/* start.c
 *
 * A small test program to load and print info on an MDP file
 *
 * Jerod Weinman
 */
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <inttypes.h>

#include "mdp.h"



////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])

{
  if (argc != 4)
  {
    fprintf(stderr,"Usage: %s mdpfile\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  unsigned int state, action;
  state = (unsigned int)strtol(argv[2], NULL, 10);
  action = (unsigned int)strtol(argv[3], NULL, 10);
  
  // Pointer for the MDP struct
  mdp *p_mdp;

  // Read the MDP file
  p_mdp = mdp_read(argv[1]);

  if (NULL == p_mdp)
    // mdp_read prints a message upon failure
    exit(EXIT_FAILURE);
    
	// Print information about the markov decision process (MDP)
  
  
  /*
  for (state=0 ; state < p_mdp->numStates ; state++)
  {
    printf("State %u:", state);
    printf("\t[%u] ", p_mdp->numAvailableActions[state]);
    unsigned int action;
    for (action = 0; action < p_mdp->numAvailableActions[state]; action++){
      printf("%u ", p_mdp->actions[state][action]);
    }
    printf("\n");
    
  }
  */

  for (int possible_state = 0; possible_state < p_mdp->numStates; possible_state++) {
    printf("State %d\t%lf\n", possible_state,
           p_mdp->transitionProb[possible_state][state][action]);
      }

  

  // Clean up
  mdp_free(p_mdp);

}

