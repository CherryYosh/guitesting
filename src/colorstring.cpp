/* 
 * File:   colorstring.cpp
 * Author: brandon
 * 
 * Created on June 7, 2009, 1:14 PM
 */

#include "colorstring.h"

#include <cassert>
#include <stdexcept>

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

/*
colorstring& colorstring::erase(size_t pos) {
    std::vector<colorchar>::iterator it = str.begin();
    std::advance(it, pos);
    str.erase(it);

    return *this;
}
 */

colorstring& colorstring::erase(size_t pos, size_t n) {
	if (str.size() < pos) throw std::out_of_range("colostring::erase::out_of_range");
	size_t size = (n < (str.size() - pos)) ? n : (str.size() - pos);

	std::vector<colorchar>::iterator start = str.begin();
	std::vector<colorchar>::iterator end = str.begin();

	std::advance(start, pos);
	std::advance(end, pos + size);

	str.erase(start, end);

	return *this;
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

std::string colorstring::toString() const {
	std::string ret;

	for (unsigned int i = 0; i < str.size(); i++)
		ret.push_back(str[i].character);

	return ret;
}

const char* colorstring::c_str() const {
	//lazy right now
	return toString().c_str();
}

/**
 * splits the string into two, returning the second half.
 * same as calling substr(pos); erase(pos);
 */
colorstring colorstring::split(size_t pos) {
	colorstring ret = substr(pos);
	erase(pos);
	return ret;
}

/**
 * Generates and returns a substring of the orignal string
 */
colorstring colorstring::substr(size_t pos, size_t n) {
	if (str.size() < pos) throw std::out_of_range("colostring::substr::out_of_range");

	colorstring ret;

	size_t end = (n < (str.size() - pos)) ? pos + n : str.size();
	for (size_t i = pos; i < end; i++) {
		ret.push_back(str[i]);
	}

	return ret;
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
