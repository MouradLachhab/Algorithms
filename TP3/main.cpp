#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <bits/stdc++.h> 
#include <time.h>

using namespace std;

struct Card
{
    int id;
    int value;
    vector<int> synergy;
    int value_with_synergy; 

    bool operator < (const Card& second) const
    {
        return value_with_synergy < second.value_with_synergy;
    }
};

struct Deck
{
    vector<Card*> cards;
    int value;

    void setValue()
    {
        int value_deck = 0;
        int value_card = 0;

        for(Card* card : cards)
        {
            value_deck += card->value;
            value_card = card->value;
            
            for(Card* card2 : cards)
            {
                value_deck += card->synergy[card2->id]/2;
                value_card += card->synergy[card2->id]/2;
            }

            card->value_with_synergy = value_card;
        }
        
        value = value_deck;
        sort(cards.begin(), cards.end());
    }

    bool operator < (const Deck& second) const
    {
        return value < second.value;
    }
};

struct Permutation
{
    Card* cards[2];
    int age;

    Permutation(Card* card1, Card* card2)
    {
        cards[0] = card1;
        cards[1] = card2;
    }
};

bool improve(vector<Deck*> decks_curr)
{
    // Copy ???
    vector<Deck*> decks_temp = decks_curr;

    for(int i = decks_curr.size() - 1; i > 0; --i)
    {
        Card* card1 = decks_temp[i]->cards.back();
        Card* card2 = decks_temp[i-1]->cards.back();

        Permutation perm(card1, card2);

        // if(perm is in tabo_perm )
        // get another worst

        swap(card1, card2);

        decks_temp[i]->setValue();
        decks_temp[i-1]->setValue();

        if (decks_temp[i]->value + decks_temp[i-1]->value >
            decks_curr[i]->value + decks_curr[i-1]->value )
        {
            decks_curr = decks_temp;
            return true;
        }
    }

    return false;
}

vector<Deck*> getBestMinDeck(vector<Deck*> decks1, vector<Deck*> decks2)
{
    sort(decks1.begin(), decks1.end());
    sort(decks2.begin(), decks2.end());

    if(decks1.back()->value > decks2.back()->value)
        return decks1;

    return decks2;
}

vector<Deck*> randomize(vector<Card*> cards, int nbDecks)
{
    int nbCardsPerDeck = cards.size() / nbDecks;
    vector<Deck*> decks(nbDecks);
    vector<int> availableDecks(nbDecks);

    for (int i = 0; i < nbDecks; ++i)
    {
        decks[i] = new Deck();
        availableDecks[i] = i;
    }

    for (int i = 0; i < cards.size(); ++i)
    {
        int index = rand() %  availableDecks.size();
        int deck = availableDecks[index];
        decks[deck]->cards.push_back(cards[i]);

        if (decks[deck]->cards.size() == nbCardsPerDeck)
        {
            availableDecks.erase(availableDecks.begin() + index);
        }
    }

    return decks;
}

vector<Card*> createCards(vector<int> cardValues, vector<vector<int>> synergies)
{
    vector<Card*> cards(cardValues.size());

    for (int i = 0; i < cardValues.size(); ++i)
    {
        cards[i] = new Card();
        cards[i]->id = i;
        cards[i]->value = cardValues[i];
        cards[i]->synergy = synergies[i];
    }

    return cards;
}


int main(int argc, char **argv)
{
    string path;
    bool showNewBest = false;

    // Parse Parameters
    for (int i = 1; i < argc; ++i)
    {      
        if (!strcmp(argv[i], "-e"))
        {
            path = argv[++i];
        }
        else if (!strcmp(argv[i], "-p"))
            showNewBest = true;
        else
        {
            cout << "Error: Syntax Invalid: ./tp.sh -e 'path' [-p]" << endl;
        }
        
    }

    int nbCards, nbDecks;
    ifstream infile(path);
    string line;

    // Read the number of cards per deck
    getline(infile, line);
    istringstream iss(line);
    iss >> nbCards;

    // Read the number of decks
    getline(infile, line);
    iss = istringstream(line);
    iss >> nbDecks;

    // Read the cards
    getline(infile, line);
    iss = istringstream(line);
    vector<int> cardValues = vector<int>(istream_iterator<int>(iss), istream_iterator<int>());

    vector<vector<int>> synergies;

    for (int i = 0; i < nbCards*nbDecks; ++i)
    {
        getline(infile, line);
        iss = istringstream(line);
        synergies.push_back(vector<int>(istream_iterator<int>(iss), istream_iterator<int>()));
    }

    vector<Permutation*> tabu_permutation;
    vector<Deck*> decks_curr;
    vector<Deck*> best_deck;
    vector<Card*> cards = createCards(cardValues, synergies);
    srand(time(NULL));

    decks_curr = randomize(cards, nbDecks);
    best_deck = decks_curr;
    
    for(Deck* deck : decks_curr)
        deck->setValue();

    while(1)
    {
        bool IsBetter = improve(decks_curr);

        if(!IsBetter)
        {
            best_deck = getBestMinDeck(best_deck, decks_curr);
            decks_curr = randomize(cards, nbDecks);
        }
        else
        {
            cout << "Found Better Deck" << endl;
        }
        
    }

   for (auto p : best_deck)
   {
     delete p;
   }
    for (auto p : decks_curr)
   {
     delete p;
   }
    for (auto p : cards)
   {
     delete p;
   }

   
	return 0;
}