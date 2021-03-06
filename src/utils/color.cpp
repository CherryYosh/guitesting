/* 
 * File:   color.cpp
 * Author: brandon
 * 
 * Created on May 29, 2009, 4:44 PM
 */

#include <stdio.h>

#include "color.h"

using namespace util;

static const float MAX_VALUE = 0xFF;

Color::Color() : r(0), b(0), g(0), a(0) { }

Color::Color(const Color& o) : r(o.r), g(o.g), b(o.b), a(o.a) { }

Color::Color(const char* sc) {
	Init(sc);
}

Color::Color(std::string sc) {
	Init(sc.c_str());
}

Color::Color(float a, float b, float c, float d) : r(a), g(b), b(c), a(d) { }

Color::~Color() { }

/**
 * The private Init function used for string based init
 */
void Color::Init(const char* sc) {
	if (IsHexStr(sc)) {
		unsigned int v1, v2, v3, v4;

		if (HasAlpha(sc)) {
			sscanf(sc, "%*c%2x%2x%2x%2x", &v1, &v2, &v3, &v4);
		} else {
			v4 = MAX_VALUE; //fully alpha if not specifyed
			sscanf(sc, "%*c%2x%2x%2x", &v1, &v2, &v3);
		}

		r = float(v1 / MAX_VALUE);
		g = float(v2 / MAX_VALUE);
		b = float(v3 / MAX_VALUE);
		a = float(v4 / MAX_VALUE);
	} else {
		printf("%s is not a valid color!\n", sc);
	}
}


/**
 * Returns a pointer to the color's array
 */
float* Color::array() {
	return _array;
}

/**
 * Returns true if the string is a hex string. Starting with # and consisting of atleast 6 hex values
 */
bool Color::IsHexStr(std::string s) {
	return(s.find_first_not_of("#0123456789abcdefABCDEF") == std::string::npos && (s.size() == 7 || s.size() == 9));
}

/**
 * Returns true if the string has alpha data, ie is 9 characters wide
 */
bool Color::HasAlpha(std::string s) {
	return(s.size() == 9);
}