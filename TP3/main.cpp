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

    bool operator < (const Card& second) const
    {
        return value_with_synergy < second.value_with_synergy;
    }
};

struct Deck
{
    vector<Card*> cards;
    int value = 0;

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

vector<Deck*> createDecksGreedy(vector<Card*> cards, int nbDecks)
{
	int nbCardsPerDeck = cards.size() / nbDecks;
	vector<Deck*> decks(nbDecks);
	vector<int> availableCards(cards.size());

	for (int i = 0; i < cards.size(); ++i)
	{
		availableCards[i] = i;
	}

	for (int i = 0; i < nbDecks; ++i)
	{
		decks[i] = new Deck();
		int index = rand() % availableCards.size();
		decks[i]->cards.push_back(cards[availableCards[index]]);
        decks[i]->value += cards[availableCards[index]]->value;
		availableCards.erase(availableCards.begin() + index);
	}

	bool done = false;
	int tooManyLoops = 0;
	while (!availableCards.empty())
	{
        Card* currentCard = cards[availableCards.back()];
        availableCards.pop_back();

        int bestValue = -9999;
        int deckIndex = -1;

		for (int i = 0; i < nbDecks; ++i)
		{
            if (decks[i]->cards.size() < nbCardsPerDeck)
            {
                int deckValue = decks[i]->value + currentCard->value;

                for (int j = 0; j < decks[i]->cards.size(); ++j)
                {
                    deckValue += decks[i]->cards[j]->synergy[currentCard->id];
                }

                if (deckValue > bestValue)
                {
                    bestValue = deckValue;
                    deckIndex = i;
                }
            }
		}

        decks[deckIndex]->cards.push_back(currentCard);
        decks[deckIndex]->value = bestValue;
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

int getMinValue(vector<Deck*> decks)
{
	int  min = 99999999;
	for (Deck* deck : decks)
	{
		if (deck->value < min)
			min = deck->value;
	}

	return min;
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

	// TODO: Remove path

    ifstream infile(path);

	if (!infile.is_open())
	{
		cout << "Failed to open file";
		return 0;
	}

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

	int current_min = -9999;

    while(1)
    {
		decks_curr = createDecksGreedy(cards, nbDecks);

		int newMin = getMinValue(decks_curr);

		if (current_min < newMin)
		{ 
			cout << "New min: " << newMin << endl;
            for (int i = 0; i < decks_curr.size(); ++i)
            {
                for (int j = 0; j < decks_curr[i]->cards.size(); ++j)
                {
                    cout << decks_curr[i]->cards[j]->value << " ";
                }

                cout << "Deck Value: "<< decks_curr[i]->value << endl;
            }
			current_min = newMin;
			best_deck = decks_curr;
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