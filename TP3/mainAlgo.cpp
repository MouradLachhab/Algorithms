int n = ...;
int m = ...;

typedef Card{
    int id;
    int value;
    int synergy[n];
    int value_with_synergy; 
}
typedef Deck{
    Card cards[n];
    int value;
}
typedef Permutation{
    Card cards[2];
    int age;
}

bool Deck::compare(Deck* first, Deck* second) {
    if (first->value < second->value)
        return true;
    else
        return false;
}

bool Card::compare(Card* first, Card* second) {
    if (first->value_with_synergy< second->value_with_synergy)
        return true;
    else
        return false;
}

list<Permutation*> tabu_permutation();
list<Deck*> decks_curr();
list<Deck*> best_deck();

int main(){
    decks_curr = randomize();
    for(Deck deck : decks_curr)
        deck.setValue();

    while(!stop){
        bool IsBetter = decks_curr.improve();
        if(!IsBetter)
            {
            best_deck = getBestMinDeck();
            decks_curr = randomize();
            }
    }

}

bool improve(list<Deck*> decks_curr){
    list<Deck*> decks_temp = decks_curr.copy();

    for(int i = m ; i >= 1; i--){
        Card card1 = decks_temp[i].cards.last;
        Card card2 = decks_temp[i-1].cards.last;
        Permutation perm = new Perm();
        perm.cards = [card1,card2];
        // if(perm is in tabo_perm )
        // get another worst
        swap(card1, card2); 
        decks_temp[i].setValue();
        decks_temp[i-1].setValue();

        getBestMinDeck
        
        if (decks_temp.value > decks_curr.value){
            decks_curr = decks_temp;
            return true;
        }
    }
    return false;
}

Deck getBestMinDeck(list<Deck*> decks1, list<Deck*> decks2){
    decks1.sort(compare);
    deck2.sort(compare);
    if(decks1.back()->value > decks2.back()->value)
        return decks1;
    return decks2;
}

void setValue(Deck deck){
    int value_deck = 0;
    int value_card = 0;
    std::list<Card>::iterator<Card> it;
    for(Card& card : deck.cards){
        value_deck += (*it)->value;
        value_card = (*it)->value;
        std::list<Card>::iterator<Card> it2;
         for(Card& card2 : deck.cards){
            value_deck += card->synergy[card2->id]/2;
            value_card += card->synergy[card2->id]/2;
        }
    card->value_with_synergy; = value_card;
    }
    Deck.value = value;
    Deck.cards.sort();
}