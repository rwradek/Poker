#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "cards.h"
#include "deck.h"
#include "eval.h"
#include "future.h"
#include "input.h"
#include <time.h>



int main(int argc, char ** argv) {

  if ((argc!=2) && (argc!=3)) {
    fprintf(stderr,"Usage: [file_name] [number of triaial, default =10000]\n");
    return EXIT_FAILURE;
  }
  const char * file_name = argv[1];
  size_t num_trials = 10000;

  if (argc==3) {
    num_trials = atoi(argv[2]);
  }
  //obsluga parametrow
    //obsluga bledow

  future_cards_t * future_c = malloc(sizeof(*future_c));
  future_c->decks = NULL;
  future_c->n_decks = 0;

  FILE * f = fopen(file_name,"r");
  if(f == NULL) {
      fprintf(stderr,"Connot open file :%s",file_name);
      return EXIT_FAILURE;
  }
  
  size_t n_hands = 0;
  deck_t ** my_arr =  read_input(f, &n_hands, future_c);
  
  if(fclose(f)!=0) {
      fprintf(stderr,"Cannot close file");
      return EXIT_FAILURE;
  }

  deck_t * remaining_deck = build_remaining_deck(my_arr, n_hands);
  printf("remaing = %lu\n",remaining_deck->n_cards);
  //future cards - free
  size_t f_no = 0;
  size_t f_i = 0;
  for(size_t i=0;i<future_c->n_decks;i++){
    if(future_c->decks[i].cards != NULL)
      f_no = f_no + (future_c->decks[i].n_cards);
      f_i++;
  }
  printf("Future = %lu / %lu\n",f_no,f_i);

  size_t * each_hand_win = calloc(n_hands,sizeof(*each_hand_win));
  size_t   tie_result = 0;
  
  clock_t begin = clock();

  //Monte Carlo -- loop ---
  int best_hand = 0;
  int cmp_result = 0;
  int hands_tie = 1;
  for(size_t mc = 0;mc<num_trials;mc++){
    shuffle(remaining_deck);
    future_cards_from_deck(remaining_deck,future_c);

    //printf("-----------\n");
    best_hand = 0;
    hands_tie = 1;
    for(size_t h=1;h<n_hands;h++){
      cmp_result = compare_hands(my_arr[best_hand],my_arr[h]);
      //printf("bh %d|h %lu|cp %d\n",best_hand,h,cmp_result);
      if(cmp_result < 0) {
        hands_tie = 0;
        best_hand = h;
      }
      if(cmp_result > 0) {
        hands_tie = 0;
      }
    }
    if (hands_tie) {
      tie_result++;
    } else {
      each_hand_win[best_hand]++;
    }
  }

clock_t end = clock();
double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

printf("Time %f\n",time_spent);

  for (size_t i=0;i<n_hands;i++){
    float percent = 100.0 * (float)each_hand_win[i]/num_trials;
    printf("Hand %zu won %lu / %lu times (%.2f%%)\n",
                                    i,
                                    each_hand_win[i],
                                    num_trials,
                                    percent);
  }
  printf("And there were %lu ties\n",tie_result);

  //-----------------FREE----------------------
  //future cards - free
  for(size_t i=0;i<future_c->n_decks;i++){
    free(future_c->decks[i].cards);
  }
  free(future_c->decks);
  free(future_c);

  //free my_arr
  for (size_t i=0;i<n_hands;i++){
    free_deck(my_arr[i]);
  }
  free(my_arr);

  free(each_hand_win);
  free_deck(remaining_deck);

  return EXIT_SUCCESS;
}