#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "future.h"

//OPTYMALIZACJA !!! - tylko dla petli MC


void add_card_ptr_to(deck_t * deck, card_t * c_p){
  deck->cards = realloc(deck->cards, 
                       (deck->n_cards + 1) * sizeof(*(deck->cards)));
  deck->cards[deck->n_cards] = c_p;
  deck->n_cards++;
}

void add_future_card(future_cards_t * fc, size_t index, card_t * ptr) {
  if((int)index >((int)fc->n_decks - 1)){
      fc->decks = realloc(fc->decks, (index + 1) * sizeof(*(fc->decks)));
      for (size_t i=fc->n_decks;i<=index;i++){
        //printf("i>%lu:\n",i);
        fc->decks[i].cards = NULL;
        fc->decks[i].n_cards = 0;
      }
      fc->n_decks = index + 1;
  }
  add_card_ptr_to(&(fc->decks[index]), ptr);
}

void future_cards_from_deck(deck_t * deck, future_cards_t * fc) {
  size_t shuf_dec_i = 0;

  /*
  OPTYMALIZACJA  
  if(deck->n_cards == 0){
    fprintf(stderr, "Deck has zero cards, no cards to draw from\n");
    return;
  }
  */

  for(size_t i=0;i<fc->n_decks;i++){
    if(fc->decks[i].cards == NULL) continue;

    /*
    OPTYMALIACJA  
    if(((int)deck->n_cards - 1)<shuf_dec_i){
      fprintf(stderr, "Deck has not enought cards, no cards to draw from\n");
      return;
    }
    */

    for(size_t j=0;j<fc->decks[i].n_cards;j++){
      fc->decks[i].cards[j]->value = deck->cards[shuf_dec_i]->value;
      fc->decks[i].cards[j]->suit  = deck->cards[shuf_dec_i]->suit;      
    }
    shuf_dec_i++;
  }
}