#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm> 
#include <time.h>

#define NB_ITERATIONS 10000

using namespace std;

struct Card
{
    int id;
    int value;
    vector<int> synergy;
    int value_with_synergy; 
};

bool compareCard(Card* first, Card* second)
{
	return first->value_with_synergy > second->value_with_synergy;
}

struct Deck
{
    vector<Card*> cards;
    int value;

    void setValue()
    {
		value = 0;

		for (int i = 0; i < cards.size(); ++i)
		{
			value += cards[i]->value;
			cards[i]->value_with_synergy = cards[i]->value;

			for (int j = 0; j < i ; ++j)
			{
				value += cards[j]->synergy[cards[i]->id];

				cards[j]->value_with_synergy += cards[j]->synergy[cards[i]->id]/2;
				cards[i]->value_with_synergy += cards[i]->synergy[cards[j]->id]/2;
			}
		}

    }
};

int getMinValue(vector<Deck>& decks)
{
	int  min = 99999999;
	for (Deck deck : decks)
	{
		if (deck.value < min)
			min = deck.value;
	}

	return min;
}

bool compareDeck(Deck& first, Deck& second)
{
	return first.value > second.value;
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

bool improve(vector<Deck>& decks_curr)
{
    Deck* worst_deck = &decks_curr.back();
	int deck_size = worst_deck->cards.size();

    for(int i = deck_size - 1; i > deck_size /2; --i)
    {
		sort(worst_deck->cards.begin(), worst_deck->cards.end(), compareCard);

  		for(int j = decks_curr.size() - 2; j > decks_curr.size()/2; --j)
  		{
			sort(decks_curr[j].cards.begin(), decks_curr[j].cards.end(), compareCard);
			for (int k = deck_size - 1; k > 0  ; --k)
			{
				Card* card_temp = decks_curr[j].cards[k];
				int max_value = worst_deck->value + decks_curr[j].value;

				decks_curr[j].cards.at(k) = worst_deck->cards[i];
				worst_deck->cards.at(i) = card_temp;

				worst_deck->setValue();
				decks_curr[j].setValue();

				if (worst_deck->value + decks_curr[j].value > max_value)
					return true;

				worst_deck->cards.at(i) = decks_curr[j].cards[k];
				decks_curr[j].cards.at(k) = card_temp;
				worst_deck->setValue();
				decks_curr[j].setValue();
			}
        }
    }
    return false;
}

vector<Deck> getBestMinDeck(vector<Deck> decks1, vector<Deck> decks2)
{
    sort(decks1.begin(), decks1.end(),compareDeck);
    sort(decks2.begin(), decks2.end(),compareDeck);

    if(decks1.back().value >= decks2.back().value)
        return decks1;

    return decks2;
}

vector<Deck> createDecksGreedy(vector<Card*> cards, int nbDecks)
{
	int nbCardsPerDeck = cards.size() / nbDecks;
	vector<Deck> decks(nbDecks);

	for (int i = 0; i < nbDecks; ++i)
	{
		int index = rand() % cards.size();
		decks[i].cards.push_back(cards[index]);
		decks[i].value += cards[index]->value;
		cards.erase(cards.begin() + index);
	}

	while (!cards.empty())
	{
		int index = rand() % cards.size();
		Card* currentCard = cards[index];
		cards.erase(cards.begin() + index);

		int bestBump = -9999;
		int bestValue = -9999;
		int deckIndex = -1;

		for (int i = 0; i < nbDecks; ++i)
		{
			if (decks[i].cards.size() < nbCardsPerDeck)
			{
				int deckValue = decks[i].value + currentCard->value;

				for (int j = 0; j < decks[i].cards.size(); ++j)
				{
					deckValue += decks[i].cards[j]->synergy[currentCard->id];
				}

				int currentBump = deckValue - decks[i].value;

				if (currentBump > bestBump)
				{
					bestBump = currentBump;
					bestValue = deckValue;
					deckIndex = i;
				}
			}
		}

		decks[deckIndex].cards.push_back(currentCard);
		decks[deckIndex].value = bestValue;
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

void printBestSolution(vector<Deck>& decks, int value, bool showBestSolution)
{
	if (!showBestSolution)
	{
		cout << value << endl;
	}
	else
	{
		for (int i = 0; i < decks.size(); ++i)
		{
			for (int j = 0; j < decks[i].cards.size(); ++j)
			{
				cout << decks[i].cards[j]->id << " ";
			}
			cout << endl;
		}
	}
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
	path = "C:/Users/Elisabeth/Documents/Algoo/INF8775/TP3/exemplaires/MTG_10_10";
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

	int currentMin = -9999;
    vector<Deck> best_decks;
    vector<Card*> cards = createCards(cardValues, synergies);
    srand(time(NULL));

	int k = 0;
    while(1)
    {
		++k;
		if (k == NB_ITERATIONS)
		{
			//cout << "Switching to improving" << endl;
			sort(best_decks.begin(), best_decks.end(), compareDeck);
		}
		if (k < NB_ITERATIONS)
		{
			vector<Deck> decks_curr = createDecksGreedy(cards, nbDecks);
			int newMin = getMinValue(decks_curr);
			// bool IsBetter = improve(decks_curr);

			if (currentMin < newMin)
			{
				// cout << "CURRENT ITERATION: " << k << endl << "CURRENT MIN :" << newMin << endl;
				currentMin = newMin;
				best_decks = decks_curr;

				printBestSolution(best_decks, currentMin, showNewBest);
			}
		}
		else
		{
			bool IsBetter = improve(best_decks);

			if (IsBetter)
			{
				int newMin = getMinValue(best_decks);
				if (newMin > currentMin)
				{
					currentMin = newMin;
					printBestSolution(best_decks, newMin, showNewBest);
				}
			}
		}
    }

    for (Card* card : cards)
	{
		delete card;
	}

   
	return 0;
}