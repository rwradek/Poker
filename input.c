#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include "input.h"


deck_t ** read_input(FILE * f, size_t * n_hands, future_cards_t * fc){
  deck_t ** answer = malloc(sizeof(** answer));
  deck_t *  hand = NULL;
  *n_hands = 0;

  char * line = NULL;
  size_t sz = 0;
  while(getline(&line,&sz,f) >= 0){
    //printf("h:%lu -> %s \n",*n_hands,line);
    answer = realloc(answer,(*n_hands + 1) * sizeof(*answer));
    hand = hand_from_string(line,fc);
    //if HAND < 5 perror !!
    if (hand->n_cards < 5){
      fprintf(stderr, "Not enought cards in hand (less then 5)\n");
      free_deck(hand);
    } else {
      answer[*n_hands] = hand;
      (*n_hands)++;
    }
  }
  free(line);

  return answer;
}

deck_t * hand_from_string(const char * str, future_cards_t * fc){
  int i = 0;
  deck_t * new_deck = malloc(sizeof(*new_deck));
  new_deck->cards = NULL;
  new_deck->n_cards = 0;
  while(str[i] != 0){
    if(iscntrl(str[i]) || isspace(str[i])) {
      i++;
      continue;
    }
    //fprintf(stderr,"%c<%d>",str[i],(str[i] == '?'));
    if(str[i] == '?') {
      i++;
      card_t * empty_c = add_empty_card(new_deck);
      add_future_card(fc, atoi(&str[i]), empty_c);
      if (isdigit(str[i+1])) i++;
      
    } else if(isalpha(str[i]) || isdigit(str[i])){
      add_card_to(new_deck, card_from_letters(str[i], str[i+1]));
      i++;
    }
    i++;
  }
  //printf("\n");
  return new_deck;
}