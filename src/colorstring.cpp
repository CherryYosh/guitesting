/* 
 * File:   colorstring.cpp
 * Author: brandon
 * 
 * Created on June 7, 2009, 1:14 PM
 */

#include "colorstring.h"

#include <cassert>

colorstring::colorstring() { }

colorstring::colorstring(const char* s, util::Color color) {
	Init(s, color);
}

colorstring::colorstring(std::string s, util::Color color) {
	Init(s, color);
}

colorstring::colorstring(const colorstring& orig) : str(orig.str) { }

colorstring::~colorstring() { }

void colorstring::Init(std::string s, util::Color color) {
	colorchar c;
	size_t size = s.size();
	for (unsigned int i = 0; i < size; i++) {
		c.c = s[i];
		c.color = color;

		str.push_back(c);
	}
}

//modifyers

void colorstring::append(char c, util::Color color) {
	str.push_back(colorchar(c, color));
}

void colorstring::append(colorchar c) {
	str.push_back(c);
}

void colorstring::append(colorstring s) {
	size_t size = s.size();
	for (unsigned int i = 0; i < size; i++) {
		append(s.at(i));
	}
}

void colorstring::insert(size_t pos, char c) {
	assert(pos <= str.size());

	if (pos == str.size()) {
		append(c);
	} else {
		std::vector<colorchar>::iterator it = str.begin();
		std::advance(it, pos);
		str.insert(it, c);
	}
}

void colorstring::erase(size_t pos) {
	std::vector<colorchar>::iterator it = str.begin();
	std::advance(it, pos);
	str.erase(it);
}

void colorstring::push_back(const colorchar& c) {
	str.push_back(c);
}

void colorstring::pop_back() {
	str.pop_back();
}

size_t colorstring::size() {
	return str.size();
}

size_t colorstring::length() {
	return str.size();
}

void colorstring::clear() {
	str.clear();
}

bool colorstring::empty() {
	return str.empty();
}

colorchar colorstring::at(int i) {
	return str.at(i);
}

colorstring& colorstring::operator=(const colorstring& s) {
	str = s.str;
	return *this;
}

colorstring& colorstring::operator=(const std::string& s) {
	return *this = colorstring(s);
}

colorchar& colorstring::operator[](int i) {
	return str[i];
}
