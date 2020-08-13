#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "deck.h"

/*
Add the particular card to the given deck 
(which will involve reallocing the array of cards in that deck).
*/
void add_card_to(deck_t * deck, card_t c){
  if((c.value ==0) && (c.suit==0)) return; //Shall it be ?
  deck->cards = realloc(deck->cards, 
                       (deck->n_cards + 1) * sizeof(*(deck->cards)));
  deck->cards[deck->n_cards] = malloc(sizeof(c));
  deck->cards[deck->n_cards]->value = c.value;
  deck->cards[deck->n_cards]->suit  = c.suit;  
  deck->n_cards++;
}

/*
Add a card whose value and suit are both 0, and return a pointer to it in the deck.
This will add an invalid card to use as a placeholder for an unknown card.
*/
card_t * add_empty_card(deck_t * deck){
  deck->cards = realloc(deck->cards, 
                       (deck->n_cards + 1) * sizeof(*(deck->cards)));
  deck->cards[deck->n_cards] = malloc(sizeof(card_t));
  card_t * empty_p = deck->cards[deck->n_cards];
  empty_p->value = 0;
  empty_p->suit  = 0;  
  deck->n_cards++;
  return empty_p;
}
/*
  	Create a deck that is full EXCEPT for all the cards that appear in excluded_cards. 

	 For example,
   		if excluded_cards has Kh and Qs, you would create
   			a deck with 50 cards---all of them except Kh and Qs.
   
		You will need to use malloc to allocate this deck (You will want this for the next function).
   		
		Don't forget you wrote:
			- card_t card_from_num(unsigned c) in Course 2 and 
			- int deck_contains(deck_t * d, card_t c) in Course 3!  
	
	They might be useful here.
*/
deck_t * make_deck_exclude(deck_t * excluded_cards){
  deck_t * new_deck = malloc(sizeof(deck_t));
  new_deck->cards = NULL;
  new_deck->n_cards = 0;
  card_t c; 
  for(int i=0;i<52;i++){
    c = card_from_num(i);
    if (deck_contains(excluded_cards,c)) continue;
    add_card_to(new_deck,c);
  }
  return new_deck;
}

/*
   	This function takes an array of hands 
		(remember that we use deck_t to represent a hand).  
	
	It then builds the deck of cards that remain after those cards have been removed from a full deck.  
	For example, if we have two hands:
      		Kh Qs ?0 ?1 ?2 ?3 ?4
      		As Ac ?0 ?1 ?2 ?3 ?4
   
	then this function should build a deck with 48 cards (all but As Ac Kh Qs).  
	You can just build one deck with all the cards from all the hands
   		(remember you just wrote add_card_to), and then pass it to make_deck_exclude.
*/
deck_t * build_remaining_deck(deck_t ** hands, size_t n_hands){
  deck_t * excluded_cards = malloc(sizeof(deck_t));
  excluded_cards->cards = NULL;
  excluded_cards->n_cards = 0;
  for (size_t h=0;h<n_hands;h++){
    for(size_t c=0;c<hands[h]->n_cards;c++){
      add_card_to(excluded_cards, *(hands[h]->cards[c]));
    }
  }
  deck_t * deck_exclude = make_deck_exclude(excluded_cards);
  free_deck(excluded_cards);
  return deck_exclude;
}

/*
   	Free the memory allocated to a deck of cards.
   	For example, if you do
     		deck_t * d = make_excluded_deck(something);
     		
		free_deck(d);
   			it should free all the memory allocated by:
				make_excluded_deck.
   			Once you have written it, add calls to free_deck anywhere you need to to avoid memory leaks.
*/
void free_deck(deck_t * deck){
  for(size_t i=0;i<deck->n_cards;i++){
    free(deck->cards[i]);
  }
  free(deck->cards);
  free(deck);
}

void print_hand(deck_t * hand){
  for (size_t i=0; i<hand->n_cards; i++){
    print_card(*(hand->cards[i]));
    printf(" ");
  };
}

int deck_contains(deck_t * d, card_t c) {
  //card_t *c_p = NULL;
  for (size_t i=0; i<d->n_cards; i++){
    //c_p = d->cards[i];
    if((d->cards[i]->value == c.value) &&
       (d->cards[i]->suit  == c.suit)) {
        return 1;
      };
  };
  return 0;
}

void shuffle(deck_t * d){
  card_t * c_p_temp = 0;
  int counter = 0;
  for (int i = d->n_cards-1; i>-1; i--){ //time safe
  //for (int i = 0; i<d->n_cards; i++){ //time safe
    int id_card_to_swap = rand() % (i+1);
    c_p_temp = d->cards[counter];
    d->cards[counter] = d->cards[id_card_to_swap];
    d->cards[id_card_to_swap] = c_p_temp;
    counter++;
    if (counter>10) break;
  }
}

void assert_full_deck(deck_t * d) {
  assert(d->n_cards == 52);
  for (size_t i=0;i<52;i++){
    assert(deck_contains(d, card_from_num(i)));
  };
}
