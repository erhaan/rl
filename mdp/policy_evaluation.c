/*
;; Lab: Markov Decision Processes
;; CSC 261 - S18
;;
;; File
;;   policy_evaluation.c
;;
;; Summary
;;   Implementation of policy_evaluation procedure for Lab:11
;;
;; Provides
;;   (policy_evaluation (const unsigned int* policy, const mdp* p_mdp,
			double epsilon, double gamma,
			double* utilities))
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "utilities.h"
#include "mdp.h"

/*  Procedure
 *    policy_evaluation
 *
 *  Purpose
 *    Iteratively estimate state utilities under a fixed policy
 *
 *  Parameters
 *   policy
 *   p_mdp
 *   epsilon
 *   gamma
 *   utilities
 *
 *  Produces,
 *   [Nothing.]
 *
 *  Preconditions
 *    policy points to a valid array of length p_mdp->numStates
 *    Each policy entry respects 0 <= policy[s] < p_mdp->numActions
 *       and policy[s] is an entry in p_mdp->actions[s]
 *    p_mdp is a pointer to a valid, complete mdp
 *    epsilon > 0
 *    0 < gamma < 1
 *    utilities points to a valid array of length p_mdp->numStates
 *
 *  Postconditions
 *    utilities[s] has been updated according to the simplified Bellman update
 *    so that no update is larger than epsilon
 */
void policy_evaluation( const unsigned int* policy, const mdp* p_mdp,
			double epsilon, double gamma,
			double* utilities)
{

  double delta;
  int num_states = p_mdp->numStates;
  //allocate space for utils
  double * utilities_prime = (double *)calloc(num_states, sizeof(double));
  if (utilities_prime == NULL) {
    perror("Calloc returned NULL");
    exit(2);
  }


  do {
    delta = 0.0;
    for (int state = 0; state < num_states; state++){
      if (p_mdp->terminal[state] == true) { //if terminal util = R
        utilities_prime[state] = p_mdp->rewards[state];
      }
      else {
        double eu;
        //if actions available then calculate eu 
        if (!(p_mdp->numAvailableActions[state] == 0)) {
          eu = calc_eu(p_mdp, state, utilities_prime, policy[state]);
        utilities_prime[state] = p_mdp->rewards[state] +
          gamma * eu;
      }
      double util_diff = fabs(utilities_prime[state] - utilities[state]);
      if (util_diff > delta) {
        delta = util_diff;
      }
      }
    }
    //update utils
    memmove(utilities, utilities_prime, num_states * (sizeof (double)));
  } while  (!(delta <= epsilon));

  free(utilities_prime);
}
