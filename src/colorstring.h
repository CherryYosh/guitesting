/* 
 * File:   colorstring.h
 * Author: brandon
 *
 * Created on June 7, 2009, 1:14 PM
 */

#ifndef _COLORSTRING_H
#define	_COLORSTRING_H

#include "utils/color.h"

#include <vector>
#include <string>

class colorchar {
public:

    colorchar() : character(0), color() {
    };

    colorchar(const colorchar& orig) : character(orig.c), color(orig.color) {
    };

    colorchar(char _c, util::Color _color = util::Color("#FFFFFF")) : character(_c), color(_color) {
    };

    virtual ~colorchar() {
    };

    bool operator==(const char& _c) {
	return character == _c;
    };

    bool operator==(const colorchar& _c) {
	return character == _c.character;
    };

    union {
	char c;
	char character;
    };
    util::Color color;
};

class colorstring {
public:
    colorstring();
    colorstring(const char*, util::Color = util::Color("#FFFFFF"));
    colorstring(std::string, util::Color = util::Color("#FFFFFF"));
    colorstring(const colorstring& orig);
    virtual ~colorstring();

    //modifyers
    void append(char, util::Color = util::Color("#FFFFFF"));
    void append(colorchar);
    void append(colorstring);
    void insert(size_t, char);
    colorstring& erase(size_t, size_t = std::string::npos);

    void push_back(const colorchar&);
    void pop_back();

    //capacity
    size_t size();
    size_t length();
    void clear();
    bool empty();

    //accessors
    colorchar at(int);


    std::string toString() const;
    const char* c_str() const;

    colorstring split(size_t);
    colorstring substr(size_t, size_t = std::string::npos);

    //overloads
    colorstring & operator=(const colorstring& s);
    colorstring & operator=(const std::string&);
    colorchar & operator[](int);

private:
    void Init(std::string, util::Color);

    std::vector<colorchar> str;
};

#endif	/* _COLORSTRING_H */

