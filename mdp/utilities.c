/*
;; Lab: Markov Decision Processes
;; CSC 261 - S18
;;
;; File
;;   utilities.c
;;
;; Summary
;;   Our implementation of calc_eu and calc_meu procedures 
;;   for Lab:11 extra credit
;;  
;;
;; Provides
;;  calc_eu
    (const mdp *  p_mdp, unsigned int state, const double * utilities,
          const unsigned int action)
;;  calc_meu 
     (const mdp * p_mdp, unsigned int state, const double * utilities,
           double * meu, unsigned int * action)
*/

#include "mdp.h"
#include "utilities.h"
#include "math.h"

#include <stdio.h> 

double
calc_eu ( const mdp *  p_mdp, unsigned int state, const double * utilities,
          const unsigned int action)
{
  double eu = 0.0;
  //loop through subsequent states
  for (int state_prime = 0; state_prime < p_mdp->numStates; state_prime++) {
    eu = eu +
      (p_mdp->transitionProb[state_prime][state][action] //add EU
       * utilities[state_prime]);
  }
  return eu;
}

void
calc_meu ( const mdp * p_mdp, unsigned int state, const double * utilities,
           double * meu, unsigned int * action )
{
  double max = -INFINITY;
  //loop through actions
  for (int possible_action = 0; possible_action < p_mdp->numAvailableActions[state]; possible_action++){
    double eu;
    eu = calc_eu(p_mdp, state, utilities, possible_action);
    if (eu > max) {
      *action = possible_action;
      *meu = eu;
      max = eu;      
    }    
  }
}
