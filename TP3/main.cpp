#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm> 
#include <time.h>

using namespace std;

struct Card
{
    int id;
    int value;
    vector<int> synergy;
    int value_with_synergy; 
};


bool compareCard(Card* first, Card* second) {
	if (first->value_with_synergy > second->value_with_synergy)
		return true;
	else
		return false;
}
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
        sort(cards.begin(), cards.end(),compareCard);
    }
};

bool compareDeck(Deck* first, Deck* second) {
	if (first->value > second->value)
		return true;
	else
		return false;
}
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
    Deck* worst_deck = decks_curr.back();
	int deck_size = worst_deck->cards.size();

    for(int i = deck_size - 1; i > deck_size /2; --i)
    {
    	Card* card1 = worst_deck->cards[i];

  		for(int j = decks_curr.size() - 2; j > decks_curr.size()/2; --j)
  		{
			for (int k = 0; k <deck_size ; ++k)
			{
				Card* card2 = decks_curr[j]->cards[k];

				int max_value = worst_deck->value + decks_curr[j]->value;
				swap(card1, card2);

				worst_deck->setValue();
				decks_curr[j]->setValue();

				if (worst_deck->value + decks_curr[j]->value > max_value)
					return true;

				swap(card2, card1);
				worst_deck->setValue();
				decks_curr[j]->setValue();
			}
       
        }
    }
    return false;
}

vector<Deck*> getBestMinDeck(vector<Deck*> decks1, vector<Deck*> decks2)
{
    sort(decks1.begin(), decks1.end(),compareDeck);
    sort(decks2.begin(), decks2.end(),compareDeck);

    if(decks1.back()->value >= decks2.back()->value)
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
	path = "C:/Users/Elisabeth/Documents/Algoo/INF8775/TP3/exemplaires/MTG_10_10";
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

		if (!IsBetter)
		{
			best_deck = getBestMinDeck(best_deck, decks_curr);
			
			cout << "Found Better Deck" << endl;
			cout << best_deck.front()->value << endl;
			cout << best_deck.back()->value << endl;

			decks_curr = randomize(cards, nbDecks);
		}
		else
			cout << "improved";
        
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