#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "cards.h"


void assert_card_valid(card_t c) {
  assert((c.value>=2) && (c.value<=VALUE_ACE));
  assert((c.suit>=SPADES) && (c.suit<=CLUBS));
}

const char * ranking_to_string(hand_ranking_t r) {
  switch(r){
  case(STRAIGHT_FLUSH):return "STRAIGHT_FLUSH"; break;
  case(FOUR_OF_A_KIND):return "FOUR_OF_A_KIND"; break;
  case(FULL_HOUSE):return "FULL_HOUSE"; break;
  case(FLUSH):return "FLUSH"; break;
  case(STRAIGHT):return "STRAIGHT"; break;
  case(THREE_OF_A_KIND):return "THREE_OF_A_KIND"; break;
  case(TWO_PAIR):return "TWO_PAIR"; break;
  case(PAIR):return "PAIR"; break;
  case(NOTHING):return "NOTHING"; break;
  }
  return "";
}

char value_letter(card_t c) {
  switch(c.value){
  case(VALUE_ACE):return 'A';
  case(VALUE_KING):return 'K';
  case(VALUE_QUEEN):return 'Q';
  case(VALUE_JACK):return 'J';
  case(10):return '0';
  }
  return c.value + 48; //conversion to digits to chars 2-9
}


char suit_letter(card_t c) {
  //('s', 'h', 'd', or 'c' for SPADES,HEARTS, DIAMONDS, or CLUBS)
  switch(c.suit){
  case(SPADES): return 's';
  case(HEARTS): return 'h';
  case(DIAMONDS): return 'd';
  case(CLUBS): return 'c';
  case(NUM_SUITS): return 'x';
  }
  return 'x';
}

void print_card(card_t c) {
  printf("%c%c",value_letter(c),suit_letter(c));
}

card_t card_from_letters(char value_let, char suit_let) {
  card_t temp;
  temp.suit = NUM_SUITS + 1; //error value for assert to work
  switch(suit_let){
  case('s'): temp.suit = SPADES; break;
  case('h'): temp.suit = HEARTS; break;
  case('d'): temp.suit = DIAMONDS; break;
  case('c'): temp.suit = CLUBS; break;
  };
  temp.value = (unsigned)value_let - (unsigned)48;
  switch(value_let){
  case('A'): temp.value = VALUE_ACE; break;
  case('K'): temp.value = VALUE_KING; break;
  case('Q'): temp.value = VALUE_QUEEN; break;
  case('J'): temp.value = VALUE_JACK; break;
  case('0'): temp.value =10; break;
  }
  assert_card_valid(temp);
  return temp;
}

card_t card_from_num(unsigned c) {
  card_t temp;
  temp.value = (c % 13) + 2; //card values from 2 - 14 (mod 0-12)
  temp.suit  = c / 13;
  assert_card_valid(temp);
  return temp;
}
