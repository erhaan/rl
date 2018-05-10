/*
;; Lab: Markov Decision Processes
;; CSC 261 - S18
;;
;; File
;;   value_iteration.c
;;
;; Summary
;;   Our implementation of value_iteration procedure for Lab:11
;;   + instructors implemented "main" 
;;
;; Provides
;;   value_iteration 
     (const mdp * p_mdp, double epsilon, double gamma,
                  double * utilities)
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "utilities.h"
#include "mdp.h"

/* Process command-line arguments, verifying usage */
void
process_args (int argc, char* argv[], double * gamma, double * epsilon,
              mdp ** p_mdp );


void
value_iteration ( const mdp * p_mdp, double epsilon, double gamma,
                  double * utilities)
{

  int num_states = p_mdp->numStates;
  // initialize utilities
  double * utilities_prime = (double *)calloc(num_states, sizeof(double));
  if (utilities_prime == NULL) {
    perror("Calloc returned NULL");
    exit(2);
  }
   
  double delta;
  double meu;
  unsigned int action;
  
  do {
    delta = 0.0;
    //update utils
    memmove(utilities, utilities_prime, num_states * (sizeof (double)));
    for (int state = 0; state < num_states; state++){
      //do terminal state check
      if (p_mdp->terminal[state] == true) {
        utilities_prime[state] = p_mdp->rewards[state];
      }
      else {
        //if not terminal calculate best action
        calc_meu(p_mdp, state, utilities_prime, &meu, &action);
        utilities_prime[state] = p_mdp->rewards[state] +
          gamma * meu;
        double util_diff = fabs(utilities_prime[state] - utilities[state]);
        if (util_diff > delta) {
          delta = util_diff;
        }

      }
    }
  } while (!(delta < (epsilon * (1 - gamma)/ gamma)));

  free(utilities_prime);
           
} 



/*
 * Main: value_iteration gamma epsilon mdpfile
 *
 * Runs value_iteration algorithm using gamma and with max
 * error of epsilon on utilities of states using MDP in mdpfile.
 *
 * Author: Jerod Weinman
 */
int
main (int argc, char* argv[])
{
  // Read and process configurations
  double gamma, epsilon;
  mdp *p_mdp;

  process_args (argc,argv,&gamma,&epsilon,&p_mdp);

  // Allocate utility array
  double * utilities = malloc ( sizeof(double) * p_mdp->numStates );

  if (NULL == utilities) // Verify we have memory for utility array
  {
    fprintf(stderr,
	    "%s: Unable to allocate utilities (%s)",
	    argv[0], strerror (errno));
    exit (EXIT_FAILURE);
  }

  // Run value iteration!
  value_iteration ( p_mdp, epsilon, gamma, utilities);

  // Print utilities
  unsigned int state;
  for ( state=0 ; state < p_mdp->numStates ; state++)
    if (p_mdp->numAvailableActions[state] > 0 || p_mdp->terminal[state])
      printf ("%1.3f\n", utilities[state]);
    else
      printf("X\n");

  
  // Clean up
  free (utilities);
  mdp_free (p_mdp);
} // main


/* Process command-line arguments, verifying usage */
void
process_args  (int argc, char * argv[], double * gamma, double * epsilon,
               mdp ** p_mdp )
{ 
  if (argc != 4)
  {
    fprintf (stderr,"Usage: %s gamma epsilon mdpfile\n",argv[0]);
    exit (EXIT_FAILURE);
  }

  char * endptr; // String End Location for number parsing
  
  *gamma = strtod(argv[1], &endptr); // Read gamma, the discount factor
  
  if ( (endptr - argv[1]) < strlen(argv[1]) ) 
  { // Error: The entire argument was not consumed by the conversion
    fprintf (stderr, "%s: Illegal non-numeric value in argument gamma=%s\n",
             argv[0], argv[1]);
    exit (EXIT_FAILURE);
  }
  
  // Read epsilon, maximum allowable state utility error
  *epsilon = strtod(argv[2], &endptr); 
  
  if ( (endptr - argv[2]) < strlen(argv[2]) )
  { // Error: The entire argument was not consumed by the conversion
    fprintf (stderr, "%s: Illegal non-numeric value in argument epsilon=%s\n",
             argv[0], argv[2]);
    exit (EXIT_FAILURE);
  }
  
  *p_mdp = mdp_read (argv[3]); // Read MDP file (exits with message if error)

  if (NULL == *p_mdp)
  { // mdp_read prints a message
      exit (EXIT_FAILURE);
  }
} // process_args


