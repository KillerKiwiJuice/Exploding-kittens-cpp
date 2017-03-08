/*****************************************
** File:    Game.cpp
** Project: CMSC 202 Project 2, Spring 2017
** Author:  Eoin Fitzpatrick
** Date:    3/4/17
** Section: 19
** E-mail:  efitz1@umbc.edu
**
** Source code for game file
**
***********************************************/
#include "Game.h"
#include <ctime>

const int MAX_ITERATION = 10;

Game::Game() {
	string welcomeMsg = "MEOWMEOWMEOWMEOWMEOWMEOWMEOWMEOWMEOW\n" +
		string("Welcome to Exploding Kittens\n") +
		string("A Game By Elan Lee, Shane Small, and Matthew Inman\n") +
		string("MEOWMEOWMEOWMEOWMEOWMEOWMEOWMEOWMEOW");
	string playerMsg = "How many players?\nChoose 2-4 players (Inclusive): ";
	int numPlayers;
	cout << welcomeMsg << endl;
	cout << playerMsg;
	cin >> numPlayers;
	while (numPlayers < 2 || numPlayers > 4) {
		cout << "Invalid input!\n" << playerMsg;
		cin >> numPlayers;
	}
	SetNumPlayers(numPlayers);
	for (int i = 0; i < numPlayers; i++) {
		m_players[i] = Player("Player");
	}
	PlayGame();
}

// Redundant function
void Game::SetNumPlayers(int numPlayers) {
	m_numPlayers = numPlayers;
}

void Game::Shuffle() {
	cout << "Shuffling cards..." << endl;
	int shuffleInt;
	for (int i = 0; i < MAX_ITERATION; i++) {
		// seed
		srand(time(NULL));
		for (size_t j = 0; j < m_deck.size(); j++) {
			shuffleInt = rand() % m_deck.size();
			Card temp1 = m_deck[j];
			Card temp2 = m_deck[shuffleInt];
			m_deck[j] = temp2;
			m_deck[shuffleInt] = temp1;
		}
	}
}

//see-the-future
void Game::Peek() {
	cout << "\n*.-..*~. SHIFTING SPACE-TIME... *.-..*~." << endl;
	for (size_t i = 0; i < PEEK_NUM; i++) {
		cout << i << ". " << m_deck[i].ToString() << endl;
	}
	cout << endl;
}

Card Game::DrawCard() {
	// I don't think the deck can EVER be empty, but just in case... refill?
	if (m_deck.size() == 0) {
		cout << "Refilling deck..." << endl;
		for (size_t i = 0; i < m_discard.size(); i++) {
			m_deck.push_back(m_discard[i]);
		}
		Shuffle();
	}
	cout << "Card drawn." << endl;
	Card topCard = m_deck[0];
	m_deck.erase(m_deck.begin());
	return topCard;
}

// I have no idea why this function would be used
int Game::HandleCard(int currentPlayer) {
	cout << "Not used?" << endl;
	return 0;
}

void Game::PlayGame() {
	LoadDeck(DECK_NAME);
	Shuffle();
	cout << "Dealing cards..." << endl;
	for (int i = 0; i < m_numPlayers; i++) {
		for (int c = 0; c < INIT_CARDS; c++) {
			m_players[i].AddToHand(m_deck[0]);
			m_deck.erase(m_deck.begin());
		}
	}
	AddBombCards(BOMB_NAME);
	Shuffle();
	cout << "Cards remaining in deck: " << m_deck.size() << endl << endl;
	bool playerHasWon = false;
	bool playerHasTurn = true;
	int numTurnsPerPlayer = 1;
	// ***********
	// GIANT ENORMOUS LOOP
	// ***********
	while (!playerHasWon) {
		for (int currentPlayerIndex = 0; currentPlayerIndex < m_numPlayers; currentPlayerIndex++) {
			if (!m_players[currentPlayerIndex].HasLost()) {
				playerHasTurn = true;
				bool comboCard = false;
				while (playerHasTurn && !playerHasWon) {
					// MAIN MENU
					for (int j = 0; j < numTurnsPerPlayer; j++) {
						// CHECK IF FOLLOWING 'FOR' LOOP SHOULD ACCOUNT FOR EXTRA TURNS
						if (m_players[currentPlayerIndex].HasExtraTurn()) {
							numTurnsPerPlayer = 2;
							cout << m_players[currentPlayerIndex].GetName() << " " << currentPlayerIndex + 1 << " has an extra turn!" << endl;
						}
						else {
							m_players[currentPlayerIndex].SetExtraTurn(false);
							numTurnsPerPlayer = 1;
						}
						cout << "It is player " << m_players[currentPlayerIndex].GetName() << " " << currentPlayerIndex + 1 << "'s turn."
							<< "\nWhat would you like to do?:\n1. View cards\n2. Play a card\n"
							<< "3. Draw a card" << endl;
						int choice1;
						cin >> choice1;
						bool invalidTwo = false;
						if (choice1 == 2 && !m_players[currentPlayerIndex].HasCards()) {
							invalidTwo = true;
						}
						if ((choice1 < 1 || choice1 > 3) || invalidTwo) {
							while (choice1 == 2) {
								cout << "You have no cards to play! Choose another option." << endl;
								cin >> choice1;
							}
							invalidTwo = false;
							while (choice1 < 1 || choice1 > 3) {
								cout << "Invalid input! Enter a number between 1 and 3 (Inclusive)." << endl;
								cin >> choice1;
							}
						}
						// CHOICE 1: DISPLAY HAND
						if (choice1 == 1) {
							m_players[currentPlayerIndex].DisplayHand();
						}
						// CHOICE 2: PLAY A CARD
						else if (choice1 == 2) {
							if (!m_players[currentPlayerIndex].HasCards()) {
								cout << "You have no cards to play! You must draw a card." << endl;
							}
							else {
								m_players[currentPlayerIndex].DisplayHand();
								cout << "Select a card: " << endl;
								int choice2;
								cin >> choice2;
								while (choice2 < 0 || choice2 > m_players[currentPlayerIndex].GetNumberOfCards() - 1) {
									cout << "Invalid input! Enter a number between 0 and " << m_players[currentPlayerIndex].GetNumberOfCards() - 1 << "(inclusive)." << endl;
									cin >> choice2;
								}
								Card choiceCard = m_players[currentPlayerIndex].LoseCard(choice2);
								// ATTACK CARDS
								if (choiceCard.GetType() == 2) {
									if (numTurnsPerPlayer != 2) {
										playerHasTurn = false;
									}
									// find the attack target
									int nextTarget;
									int a = currentPlayerIndex;
									bool targetFound = false;
									while (a < m_numPlayers && !targetFound) {
										if (!m_players[a].HasLost() && a != currentPlayerIndex) {
											nextTarget = a;
											targetFound = true;
										}
										else if (a == (m_numPlayers - 1)) {
											// go to beginning of player list
											a = 0;
										}
										else {
											a++;
										}

									}
									cout << "TEST" << endl;
									m_players[nextTarget].SetExtraTurn(true);
									cout << "You attacked " + m_players[nextTarget].GetName() << " " << nextTarget + 1 << "!" << endl;
									m_players[currentPlayerIndex].SetExtraTurn(false);
									m_discard.push_back(choiceCard);
								}
								// SKIP CARDS
								else if (choiceCard.GetType() == 3) {
									cout << "Turn skipped." << endl;
									m_discard.push_back(choiceCard);
									if (numTurnsPerPlayer != 2) {
										playerHasTurn = false;
									}
									m_players[currentPlayerIndex].SetExtraTurn(false);
								}
								// SEE-THE-FUTURE CARDS
								else if (choiceCard.GetType() == 4) {
									m_discard.push_back(choiceCard);
									Peek();
								}
								// SHUFFLE CARDS
								else if (choiceCard.GetType() == 5) {
									m_discard.push_back(choiceCard);
									Shuffle();
								}
								// REGULAR CARDS
								else if (choiceCard.GetType() != 1) {
									m_discard.push_back(choiceCard);
									if (m_discard.size() > 1 && choiceCard.GetType() == m_discard[m_discard.size() - 2].GetType()) {
										comboCard = true;
									}
									if (!comboCard) {
										cout << "Card placed." << endl;
									}
									else {
										// check if any players actually have cards
										bool noOtherPlayerHasCards = false;
										for (int s = 0; s < m_numPlayers; s++) {
											if (s != currentPlayerIndex) {
												if (!m_players[s].HasCards()) {
													cout << m_players[s].GetNumberOfCards() << endl;
													noOtherPlayerHasCards = true;
												}
												else {
													noOtherPlayerHasCards = false;
												}
											}
										}
										// if at least one player has cards, do this (this entire system was very hard to think about logically)
										if (!noOtherPlayerHasCards) {
											comboCard = false;
											cout << "You played a pair! You can steal a card from another player.\n"
												+ string("Select a player:") << endl;
											for (int a = 0; a < m_numPlayers; a++) {
												if (a != currentPlayerIndex) {
													cout << a << ". " << m_players[a].GetName() << " " << currentPlayerIndex + 1 << endl;
												}
											}
											int choice3;
											cin >> choice3;
											while ((choice3 < 0 || choice3 > m_numPlayers - 1) || !m_players[choice3].HasCards() || choice3 == currentPlayerIndex) {
												if (choice3 == currentPlayerIndex) {
													cout << "Invalid input! You cannt steal a card from yourself." << endl;
												}
												else if (choice3 < 0 || choice3 > m_numPlayers - 1) {
													cout << "Invalid input! Enter a number between 0 and " << m_numPlayers - 1 << "(Inclusive)." << endl;
												}
												else {
													cout << "This player has no cards! Retry." << endl;
												}
												cin >> choice3;
											}
											for (int a = 0; a < m_players[choice3].GetNumberOfCards(); a++) {
												cout << "Card " << a << endl;
											}
											cout << "Select a card:" << endl;
											int choice4;
											cin >> choice4;
											while (choice4 < 0 || choice4 > m_players[choice3].GetNumberOfCards() - 1) {
												cout << "Invalid input! Enter a number between 0 and " << m_players[choice3].GetNumberOfCards() - 1 << "(Inclusive)." << endl;
												cin >> choice4;
											}
											// steal card
											m_players[currentPlayerIndex].AddToHand(m_players[choice3].LoseCard(choice4));
										}
										// else, reset
										else {
											cout << "No players have cards to steal! Resetting turn..." << endl;
											m_players[currentPlayerIndex].AddToHand(m_discard[m_discard.size() - 1]);
											m_discard.erase(m_discard.begin() + m_discard.size() - 1);
										}
									}
								}
							}
						}
						// CHOICE 3: DRAW A CARD
						else {
							if (numTurnsPerPlayer != 2) {
								playerHasTurn = false;
							}
							m_players[currentPlayerIndex].SetExtraTurn(false);
							Card drawnCard = DrawCard();
							// using string literals for raw input
							// bomb ascii
							string bomb = "  /\\___/\\" + string("\n") +
								string(" ( o   o )") + string("\n") +
								string(" (  =^=  )") + string("\n") +
								string(" (        )") + string("\n") +
								string(" (         )") + string("\n") +
								string(" (          )))))))))))") + string("\n") +
								string(" ____________________   . , ; .") + string("\n") +
								string("(_____T__N__T_______|~~~~~X.;' .") + string("\n") +
								string("                      ' `" " '") + string("\n\n");
							// explosion ascii
							string explosion = "     _.-^^---....,,--     " + string("\n") +
								string(" _--                  --_ ") + string("\n") +
								string("<                        >)") + string("\n") +
								string("|                         |") + string("\n") +
								string(" \\._                   _./") + string("\n") +
								string("    ```--. . , ; .--''' ") + string("\n") +
								string("          | |   |  ") + string("\n") +
								string("       .-=||  | |=-. ") + string("\n") +
								string("       `-=#$%&%$#=-'  ") + string("\n") +
								string("          | ;  :|    ") + string("\n") +
								string(" _____.,-#%&$@%#&#~,._____") + string("\n\n");
							// if drawn card was a bomb
							if (drawnCard.GetType() == 0) {
								cout << "Oh no! You drew an Exploding Kitten: " + drawnCard.ToString() << endl;
								if (m_players[currentPlayerIndex].AddToHand(drawnCard) == 0) {
									cout << "But you have a defuse card! Play it or LOSE!" << endl;
									cout << bomb;
									m_players[currentPlayerIndex].DisplayHand();
									cout << "Select a card: " << endl;
									int choice2;
									cin >> choice2;
									while (choice2 < 0 || choice2 > m_players[currentPlayerIndex].GetNumberOfCards() - 1) {
										cout << "Invalid input! Enter a number between 0 and " << m_players[currentPlayerIndex].GetNumberOfCards() - 1 << "(inclusive)." << endl;
										cin >> choice2;
									}
									Card choiceCard = m_players[currentPlayerIndex].LoseCard(choice2);
									if (choiceCard.GetType() != 1) {
										cout << "Sorry, you're out of the game!" << endl << endl;
										cout << explosion;
										m_players[currentPlayerIndex].SetLost(true);
										// discard all cards 
										int finalNumCards = m_players[currentPlayerIndex].GetNumberOfCards();
										for (int a = 0; a < finalNumCards; a++) {
											m_discard.push_back(m_players[currentPlayerIndex].LoseCard(0));
										}
									}
									else {
										cout << "Bomb diffused!" << endl;
										m_discard.push_back(choiceCard);
										srand(time(NULL));
										int randomIndex = rand() % m_deck.size();
										m_deck.insert(m_deck.begin() + randomIndex, drawnCard);
									}
								}
								else {
									cout << "Sorry, you're out of the game!" << endl << endl;
									cout << explosion;
									m_players[currentPlayerIndex].SetLost(true);
									// discard all cards
									for (int a = 0; a < m_players[currentPlayerIndex].GetNumberOfCards(); a++) {
										m_discard.push_back(m_players[a].LoseCard(0));
									}
								}
								int counter = 0;
								// check for presence of winner by exhaustion part 1
								Player winner;
								for (int i = 0; i < m_numPlayers; i++) {
									if (m_players[i].HasLost()) {
										counter++;
									}
								}
								// check for presence of winner by exhaustion part 2
								if (counter == m_numPlayers - 1) {
									// check for exact winner by exhaustion
									for (int i = 0; i < m_numPlayers; i++) {
										if (!m_players[i].HasLost()) {
											winner = m_players[i];
										}
									}
									cout << "The winner is " << winner.GetName() << " " << currentPlayerIndex + 1 << "! gg." << endl;
									playerHasWon = true;
								}
							}
							else {
								m_players[currentPlayerIndex].AddToHand(drawnCard);
							}
						}
					}
				}
			}
		}
	}
}

// filename should be deck.txt (or file w different name)
int Game::LoadDeck(string fileName) {
	vector<string> cardData;
	ifstream file(fileName.c_str());
	for (string line; getline(file, line);) {
		cardData.push_back(line);
	}
	for (size_t i = 0; i < cardData.size(); i += 2) {
		m_deck.push_back(Card(atoi(cardData[i].c_str()), cardData[i + 1]));
	}
	file.close();
	return 0;
}
// filename should be bomb.txt (or file w different name)
int Game::AddBombCards(string fileName) {
	cout << "Inserting the exploding kittens!" << endl;
	LoadDeck(fileName);
	return 0;
}
