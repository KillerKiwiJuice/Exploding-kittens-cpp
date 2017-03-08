/*****************************************
** File:    Game.cpp
** Project: CMSC 202 Project 2, Spring 2017
** Author:  Eoin Fitzpatrick
** Date:    3/4/17
** Section: 19
** E-mail:  efitz1@umbc.edu
**
** Source code for player file
**
***********************************************/
#include "Player.h"

Player::Player() {}

Player::Player(string playerName) {
	m_lost = false;
	m_name = playerName;
	m_extraTurn = false;
}

Card Player::PlayCard() {
	if (!HasCards()) {
		// I'd like to use throw/catch here, but we're not allowed
		cout << "If you are seeing this, something is not set up correctly." << endl;
		return Card(0, "temp");
	}
	else {
		Card temp = m_hand[0];
		m_hand.erase(m_hand.begin());
		return temp;
	}
}
// steal card
Card Player::LoseCard(int cardIndex) {
	if (!HasCards()) {
		// I'd like to use throw/catch here, but we're not allowed
		cout << "If you are seeing this, something is not set up correctly." << endl;
		return Card(0, "temp");
	}
	else {
		Card choice = m_hand[cardIndex];
		m_hand.erase(m_hand.begin() + cardIndex);
		return choice;
	}
}

bool Player::HasCards() {
	if (m_hand.size() > 0) {
		return true;
	}
	return false;
}

bool Player::HasLost() {
	return m_lost;
}

bool Player::HasExtraTurn() {
	return m_extraTurn;
}

int Player::AddToHand(Card drawnCard) {
	// return zero means the player is OK, they do not die
	// return one means the player does not have a diffuse card, they die
	if (drawnCard.GetType() == 0) {
		for (size_t i = 0; i < m_hand.size(); i++) {
			if (m_hand[i].GetType() == 1) {
				return 0;
			}
		}
		return 1;
	}
	else {
		m_hand.push_back(drawnCard);
		return 0;
	}
}

string Player::GetName() {
	return m_name;
}

void Player::DisplayHand() {
	if (!HasCards()) {
		cout << endl << "You have no cards!" << endl << endl;
	}
	else {
		cout << endl << "Deck: " << endl;
		// compensate for extra characters before card name
		string horizontalRule = "---";
		size_t maxStringLength = 0;
		// this is an extra loop that determines max card name length
		for (size_t i = 0; i < m_hand.size(); i++) {
			if (maxStringLength < m_hand[i].ToString().size()) {
				maxStringLength = m_hand[i].ToString().size();
			}
		}
		// this makes the displayed deck look a bit cleaner
		for (size_t i = 0; i < maxStringLength; i++) {
			horizontalRule += string("-");
		}
		for (size_t i = 0; i < m_hand.size(); i++) {
			cout << i << ". " << m_hand[i].ToString() << endl;
			if (i != m_hand.size() - 1) {
				cout << horizontalRule << endl;
			}
		}
		cout << endl;
	}
}

int Player::GetNumberOfCards() {
	return m_hand.size();
}

Card Player::ReturnBomb() {
	if (!HasCards()) {
		// I'd like to use throw/catch here, but we're not allowed
		cout << "If you are seeing this, something is not set up correctly." << endl;
		return Card(0, "temp");
	}
	else {
		Card bomb = m_hand[0];
		m_hand.erase(m_hand.begin());
		return bomb;
	}
}

void Player::SetLost(bool input) {
	m_lost = input;
}

void Player::SetExtraTurn(bool input) {
	m_extraTurn = input;
}