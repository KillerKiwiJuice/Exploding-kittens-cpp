/*****************************************
** File:    Game.cpp
** Project: CMSC 202 Project 2, Spring 2017
** Author:  Eoin Fitzpatrick
** Date:    3/4/17
** Section: 19
** E-mail:  efitz1@umbc.edu
**
** Source code for card file
**
***********************************************/
#include "Card.h"

Card::Card(int cardType, string cardName) {
	m_type = cardType;
	m_desc = cardName;
}

int Card::GetType() {
	return m_type;
}

string Card::ToString() {
	string cardDesc;
	if (m_type == 1) {
		cardDesc = "Defuse: " + m_desc;
	}
	else if (m_type == 2) {
		cardDesc = "Attack: " + m_desc;
	}
	else if (m_type == 3) {
		cardDesc = "Skip: " + m_desc;
	}
	else if (m_type == 4) {
		cardDesc = "See-the-Future: " + m_desc;
	}
	else if (m_type == 5) {
		cardDesc = "Shuffle: " + m_desc;
	}
	else {
		cardDesc = m_desc;
	}
	return cardDesc;
}