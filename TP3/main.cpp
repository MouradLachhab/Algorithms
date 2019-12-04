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

bool compareDeck(Deck& first, Deck& second)
{
	return first.value > second.value;
}

int getMinValue(vector<Deck>& decks)
{
	int  min = 99999;
	for (Deck deck : decks)
	{
		if (deck.value < min)
			min = deck.value;
	}

	return min;
}

bool improve(vector<Deck>& decks_curr)
{
    Deck* worst_deck = &decks_curr.back();
	int max_value_of_worst = worst_deck->value;
	int deck_size = worst_deck->cards.size();
	sort(worst_deck->cards.begin(), worst_deck->cards.end(), compareCard);

    for(int i = deck_size - 1; i > 0; --i)
	{
  		for(int j = 1; j < decks_curr.size(); ++j)
  		{
			sort(decks_curr[j].cards.begin(), decks_curr[j].cards.end(), compareCard);
			for (int k = deck_size -1 ; k > 0 ; --k)
			{
				Card* card_temp = decks_curr[j].cards[k];

				decks_curr[j].cards.at(k) = worst_deck->cards[i];
				worst_deck->cards.at(i) = card_temp;

				worst_deck->setValue();
				decks_curr[j].setValue();

				if (worst_deck->value > max_value_of_worst && decks_curr[j].value > max_value_of_worst)
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
    srand(time(NULL));

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

    vector<Card*> cards = createCards(cardValues, synergies);

    vector<Deck> best_decks;
	int allTimeMin = -9999;
	int currentMin = -9999;
	int staleLoop = 0;

	bool generatingNew = true;

    while(1)
    {
		if (generatingNew)
		{
			vector<Deck> decks_curr = createDecksGreedy(cards, nbDecks);
			int newMin = getMinValue(decks_curr);

			if (currentMin < newMin)
			{
				currentMin = newMin;
				best_decks = decks_curr;

				if (currentMin > allTimeMin)
				{
					allTimeMin = currentMin;
					printBestSolution(best_decks, currentMin, showNewBest);
				}
			}
			else
			{
				staleLoop++;

				if (staleLoop > 10000)
				{
					generatingNew = false;
					staleLoop = 0;
				}
			}
		}
		else
		{
			sort(best_decks.begin(), best_decks.end(), compareDeck);
			bool IsBetter = improve(best_decks);

			if (IsBetter)
			{
				int newMin = getMinValue(best_decks);
				if (newMin > currentMin)
				{
					currentMin = newMin;

					if (currentMin > allTimeMin)
					{
						allTimeMin = currentMin;
						printBestSolution(best_decks, newMin, showNewBest);
					}
				}
			}	
			else
			{
				staleLoop++;

				if (staleLoop > nbCards)
				{
					currentMin = -9999;
					generatingNew = true; 
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