 /*
 * This file is part of the big-integer (https://github.com/tigertv/big-integer).
 * Copyright (c) 2020 Max Vetrov(tigertv).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <bits/stdc++.h>
#include "BigInteger.h"

void BigInteger::addBinary(const std::vector<uint32_t>&bin) {
	bool carry = false;

	if (number.size() < bin.size()) {
		int diff = bin.size() - number.size();
		for(int i = 0; i < diff; i++) {
			number.push_back(0);
		}
	}

	int j = 0;
	for(; j < bin.size(); j++) {
		number[j] += bin[j] + carry;
		carry = takeCarry(number[j]);
	}

	if (carry) {
		for(int i = j; i < number.size(); i++) {
			number[i] += carry;	
			carry = takeCarry(number[i]);
			if (!carry) break;
		}

		if (carry) {
			number.push_back(1);
		}
	}

}

void BigInteger::shiftLeftBinary(std::vector<uint32_t>&bin, int places) {
	bool carry = false;
	for (int j=0; j < places; j++) {
		for (int i=0; i < bin.size(); i++) {
			bin[i] <<= 1;
			bin[i] += carry;
			carry = takeCarry(bin[i]);
		}
		if (carry) {
			bin.push_back(1);
			carry = false;
		}
	}
	
}

void BigInteger::mult10() {
	std::vector<uint32_t> m = number;
	shiftLeftBinary(number, 3);
	shiftLeftBinary(m, 1);
	addBinary(m);
}

void BigInteger::addDigit(char c) {
	c = c & 0xf;
	uint32_t i = (int32_t)c;
	if (i > 9) i = 0;

	std::vector<uint32_t>v = {i};
	addBinary(v);
}

BigInteger::BigInteger() {
	number.push_back(0);
}

void BigInteger::setDecimal(const std::string& s) {

	number.clear();
	number.push_back(0);

	for(int i = 0;i < s.size(); i++) {
		this->mult10();
		this->addDigit(s[i]);
	}
}

std::string BigInteger::toString() {
	// binary number output, '0' and '1'
	std::string s = "";
	uint32_t current;

	int j = 0;
	for(; j < number.size()-1; j++) {
		current = number[j];
		for (int i = 0; i < 31; i++){
			s += (current & 1) + 0x30;
			current >>= 1;
		}
	}

	current = number[j];
	while(current){
		s += (current & 1) + 0x30;
		current >>= 1;
	}

	std::reverse(s.begin(), s.end());
	return s;
}

BigInteger& BigInteger::operator += (const BigInteger& a) {
	this->addBinary(a.number);
}

bool BigInteger::takeCarry(uint32_t& num) {
	if (num & 0x80000000) {
		num &= 0x7fffffff;
		return true;
	}
	return false;
}

BigInteger::operator std::string() {
	return this->toString();
}
