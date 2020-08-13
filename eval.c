#include "eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>

//ERROR
void print_cards(card_t *cards[5]){
  for(int i = 0;i<5;i++){
    print_card(*cards[i]);
    printf(" ");
  }
  printf("\n%p\n",cards);
}

deck_t ** error_hand;

void hands_ERROR(deck_t * hand1, deck_t * hand2){
    //ERROR
    print_hand(hand1);
    printf(" <%p\n",hand1);
    print_hand(hand2);
    printf(" <%p\n",hand2);
    //ERROR
}
//ERROR



int card_ptr_comp(const void * vp1, const void * vp2) {
  const card_t * const * cp1 = vp1;
  const card_t * const * cp2 = vp2;
  if (cp1 == cp2) return 0;
  int value_diff = (*cp2)->value - (*cp1)->value;
  if (value_diff!=0) return value_diff;
  int suit_diff = (*cp2)->suit - (*cp1)->suit;
  return suit_diff;
}

suit_t flush_suit(deck_t * hand) {
  //printf("<<flush_suit>>\n");
  //print_hand(hand);printf("<<\n");
  int suits_card_no [NUM_SUITS+1] = {0};
  for (size_t i=0; i<hand->n_cards; i++){
    suits_card_no[hand->cards[i]->suit] += 1;
  };
  for (size_t i=SPADES; i<NUM_SUITS; i++){
    if(suits_card_no[i]>=5) {
      //printf("siut:%zu\n",i);
      return (suit_t) i;
    }
  };
  //printf("siut:N_S\n");
  return NUM_SUITS;
}

unsigned get_largest_element(unsigned * arr, size_t n) {
  //printf("<<gle>>\n");
  unsigned max = 0;
  for (size_t i = 0; i<n; i++){
    //printf("%lu:%u ",i,arr[i]);
    if (arr[i]>max) max = arr[i];
  };
  //printf("gle:%u\n",max);
  return max;
}

size_t get_match_index(unsigned * match_counts, size_t n,unsigned n_of_akind){
  //printf("<<gmi:%lu,%u>>\n",n,n_of_akind);
  for (size_t i=0; i<n; i++){
    if (match_counts[i] == n_of_akind) {
      //printf("idx:%lu\n",i);
      return i;
    }
  }
  assert(!"n_of_akind not in match_counts");
  return 0;
}

ssize_t  find_secondary_pair(deck_t * hand,
                             unsigned * match_counts,
                             size_t match_idx) {
  //printf("<<fsp/match_idx:%lu>>\n",match_idx);
  //print_hand(hand);printf("<<\n");   

  for (size_t i=0; i<hand->n_cards; i++){
    //printf("%lu:%u ",i,match_counts[i]);
  }
  //printf("\n");

  card_t ** cards_ar = hand->cards;
   for (size_t i=0;i<hand->n_cards;i++){
    if ((match_counts[i]>1) && (cards_ar[i]->value != cards_ar[match_idx]->value)) {
      //printf("<<fsp/ret:%lu>>\n",i);
      return i;
    }
  }
  //printf("<<fsp/ret:-1>>\n");
  return -1;
}

int no_cards_in_row(deck_t * hand, size_t index, suit_t fs){
  card_t ** cards_ar = hand->cards;
  unsigned cards_in_row = 1;

  if ((cards_ar[index]->suit != fs) && (fs != NUM_SUITS)) return 0;

  unsigned diff = 0;
  unsigned last_cart_val = cards_ar[index]->value;

  for(size_t i=index; i<hand->n_cards;i++){
    if ((cards_ar[i]->suit != fs) && (fs != NUM_SUITS))  continue;

    diff = last_cart_val - cards_ar[i]->value;
    last_cart_val = cards_ar[i]->value;
    if (diff == 0) continue;
    if (diff > 1 ) break;

    cards_in_row += 1;
  };
  return cards_in_row;
}

int is_straight_at(deck_t * hand, size_t index, suit_t fs) {
  //printf("<<isa/i:%lu/s:%d>>\n",index,fs);
  //print_hand(hand);
    if (no_cards_in_row(hand,index,fs) >=5 ) {
    //printf("ias/ret:1\n");
    return 1; //stright
  }
  // if less than 5 cards check if card is A
  if ((hand->cards[index])->value == VALUE_ACE) {
    if((hand->cards[hand->n_cards-1])->value != 2) return 0;
    if(((hand->cards[index])->suit != fs) && (fs != NUM_SUITS) ) return 0;
    size_t new_index = hand->n_cards;
    new_index -= 4; //rest of ace low
    if (no_cards_in_row(hand,new_index,fs) == 4 ){
      //printf("ias/ret:-1\n");   
      return -1; //stright
    }
  }
  //printf("ias/ret:0\n");
  return 0;
}

hand_eval_t build_hand_from_match(deck_t * hand,
                                  unsigned n,
                                  hand_ranking_t what,
                                  size_t idx) {
  hand_eval_t temp;
  temp.ranking = what;

  size_t i_eval = 0;
  if((n+idx)>0){
    for (size_t i=idx;i<(idx+n);i++){
      temp.cards[i_eval] = hand->cards[i]; //i
      i_eval += 1;
    }
  }

  //fill the reminder
  for (size_t i=0;i<hand->n_cards;i++){
    if ((i>=idx) && (i<(idx+n))) continue;
       temp.cards[i_eval] = hand->cards[i]; //i
    i_eval += 1;
    if (i_eval>4) break;
  }
  return temp;
}

/*
return :
  tie -> 0
  h1 win r > 0
  h2 win r < 0
*/
int compare_hands(deck_t * hand1, deck_t * hand2) {
  //printf("<<*compare_hands>>\n"); //ERROR
  qsort(hand1->cards,hand1->n_cards, sizeof(card_t *), card_ptr_comp);
  qsort(hand2->cards,hand2->n_cards, sizeof(card_t *), card_ptr_comp);

  //printf("----------------------eval 1\n"); //ERROR
  //print_hand(hand1);
  //printf("----------------------eval 1\n"); //ERROR
  hand_eval_t hand_e1 = evaluate_hand(hand1);
    //printf("-eval 1\n"); //ERROR
  //printf("    eval_r: %d\n",hand_e1.ranking); //ERROR

  //printf("-----------------------eval 2\n"); //ERROR 
  //print_hand(hand2);
  //printf("-----------------------eval 2\n"); //ERROR 
  hand_eval_t hand_e2 = evaluate_hand(hand2);
    //printf("-eval 2\n"); //ERROR 
  //printf("    eval_r: %d\n",hand_e2.ranking); //ERROR

  int diff = hand_e2.ranking - hand_e1.ranking;
  if (diff!=0) return diff;
  for (size_t i=0;i<5;i++){
    diff = (hand_e1.cards[i])->value - (hand_e2.cards[i])->value;
    if (diff!=0) return diff;
  };
  return 0;
}

//You will write this function in Course 4.
//For now, we leave a prototype (and provide our
//implementation in eval-c4.o) so that the
//other functions we have provided can make
//use of get_match_counts.
struct _one_count_t {
  unsigned key;
  size_t no;
};
typedef struct _one_count_t one_count_t;

struct _counts_t {
  one_count_t * arry;
  size_t arry_len;
  size_t unknown_no;
};
typedef struct _counts_t counts_t;

//OPTYMALIZACJA
unsigned * get_match_counts(deck_t * hand){
  char count[15] = {0};
  unsigned * match_counts = malloc(hand->n_cards * sizeof(*match_counts));
  for(size_t i=0;i<hand->n_cards;i++){
    count[hand->cards[i]->value] ++;
  }
  for(size_t i=0;i<hand->n_cards;i++){
    match_counts[i] = count[hand->cards[i]->value];
  }
  return match_counts;
}



// We provide the below functions.  You do NOT need to modify them
// In fact, you should not modify them!


//This function copies a straight starting at index "ind" from deck "from".
//This copies "count" cards (typically 5).
//into the card array "to"
//if "fs" is NUM_SUITS, then suits are ignored.
//if "fs" is any other value, a straight flush (of that suit) is copied.
void copy_straight(card_t ** to, deck_t *from, size_t ind, suit_t fs, size_t count) {
  //printf("<<*copy_stright>>\n");
  assert(fs == NUM_SUITS || from->cards[ind]->suit == fs);
  unsigned nextv = from->cards[ind]->value;
  size_t to_ind = 0;
  while (count > 0) {
    assert(ind < from->n_cards);
    assert(nextv >= 2);
    assert(to_ind <5);
    if (from->cards[ind]->value == nextv &&
        (fs == NUM_SUITS || from->cards[ind]->suit == fs)){
      to[to_ind] = from->cards[ind];
      to_ind++;
      count--;
      nextv--;
    }
    ind++;
  }
}


//This looks for a straight (or straight flush if "fs" is not NUM_SUITS)
// in "hand".  It calls the student's is_straight_at for each possible
// index to do the work of detecting the straight.
// If one is found, copy_straight is used to copy the cards into
// "ans".
int find_straight(deck_t * hand, suit_t fs, hand_eval_t * ans) {
  //printf("<<****find_stringht>>\n");
  if (hand->n_cards < 5){
    return 0;
  }
  for(size_t i = 0; i <= hand->n_cards -5; i++) {
    int x = is_straight_at(hand, i, fs);
    if (x != 0){
            if (x < 0) { //ace low straight
        //printf("Suit %u\n",fs);
        //printf("Val[%lu]=%u\n",i,hand->cards[i]->value);
        //printf("Su[%lu]=%u\n",i,hand->cards[i]->suit);
        //printf("1. %d\n",hand->cards[i]->value == VALUE_ACE);
        //printf("2. %d\n",fs == NUM_SUITS);
        //printf("3. %d\n",hand->cards[i]->suit == fs);
        //printf("4. %d\n",(fs == NUM_SUITS || hand->cards[i]->suit == fs));
        //printf("5. %d\n",(hand->cards[i]->value == VALUE_ACE &&
        //       (fs == NUM_SUITS || hand->cards[i]->suit == fs)));

        assert(hand->cards[i]->value == VALUE_ACE &&
               (fs == NUM_SUITS || hand->cards[i]->suit == fs));

        ///printf("xxxx"); 

        ans->cards[4] = hand->cards[i];
        size_t cpind = i+1;
        while(hand->cards[cpind]->value != 5 ||
              !(fs==NUM_SUITS || hand->cards[cpind]->suit ==fs)){
          cpind++;
          assert(cpind < hand->n_cards);
        }
        copy_straight(ans->cards, hand, cpind, fs,4) ;
      }
      else {
        copy_straight(ans->cards, hand, i, fs,5);
      }
      return 1;
    }
  }
  return 0;
}

//This function puts all the hand evaluation logic together.
//This function is longer than we generally like to make functions,
//and is thus not so great for readability :(
hand_eval_t evaluate_hand(deck_t * hand) {

  suit_t fs = flush_suit(hand);

  hand_eval_t ans;
  if (fs != NUM_SUITS) {
    if(find_straight(hand, fs, &ans)) {
      ans.ranking = STRAIGHT_FLUSH;
      return ans;
    }
  }
 
  unsigned * match_counts = get_match_counts(hand);
  
  unsigned n_of_a_kind = get_largest_element(match_counts, hand->n_cards);
  assert(n_of_a_kind <= 4);
  size_t match_idx = get_match_index(match_counts, hand->n_cards, n_of_a_kind);
  ssize_t other_pair_idx = find_secondary_pair(hand, match_counts, match_idx);
  free(match_counts);


  if (n_of_a_kind == 4) { //4 of a kind
    return build_hand_from_match(hand, 4, FOUR_OF_A_KIND, match_idx);
  }
  else if (n_of_a_kind == 3 && other_pair_idx >= 0) {     //full house
    ans = build_hand_from_match(hand, 3, FULL_HOUSE, match_idx);
    ans.cards[3] = hand->cards[other_pair_idx];
    ans.cards[4] = hand->cards[other_pair_idx+1];
    return ans;
  }
  else if(fs != NUM_SUITS) { //flush
    ans.ranking = FLUSH;
    size_t copy_idx = 0;
    for(size_t i = 0; i < hand->n_cards;i++) {
      if (hand->cards[i]->suit == fs){
        ans.cards[copy_idx] = hand->cards[i];
        copy_idx++;
        if (copy_idx >=5){
          break;
        }
     }
    }
    return ans;
  }
  else if(find_straight(hand,NUM_SUITS, &ans)) {     //straight
    ans.ranking = STRAIGHT;
    return ans;
  }
  else if (n_of_a_kind == 3) { //3 of a kind
    return build_hand_from_match(hand, 3, THREE_OF_A_KIND, match_idx);
  }
  else if (other_pair_idx >=0) {     //two pair
    assert(n_of_a_kind ==2);
    ans = build_hand_from_match(hand, 2, TWO_PAIR, match_idx);
    ans.cards[2] = hand->cards[other_pair_idx];
    ans.cards[3] = hand->cards[other_pair_idx + 1];
    if (match_idx > 0) {
      ans.cards[4] = hand->cards[0];
    }
    else if (other_pair_idx > 2) {  //if match_idx is 0, first pair is in 01
      //if other_pair_idx > 2, then, e.g. A A K Q Q
      ans.cards[4] = hand->cards[2];
    }
    else {       //e.g., A A K K Q
      ans.cards[4] = hand->cards[4];
    }
    return ans;
  }
  else if (n_of_a_kind == 2) {
    return build_hand_from_match(hand, 2, PAIR, match_idx);
  }
  return build_hand_from_match(hand, 0, NOTHING, 0);
}        