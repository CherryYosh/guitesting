/* 
 * File:   color.h
 * Author: brandon
 *
 * Created on May 29, 2009, 4:44 PM
 */

#ifndef _COLOR_H
#define	_COLOR_H

#include <string>

namespace util {

    class Color {
    public:
	Color();
	Color(const Color& orig);
	Color(const char*);
	Color(std::string);
	Color(float, float, float, float);
	virtual ~Color();

	float* array();

	bool HasAlpha(std::string);
	bool IsHexStr(std::string);

	float operator[](int i) {
	    return _array[i];
	};
/*
	Color & operator=(const Color& c){
	    if(this != &c){
		//this->~Color();
		delete this;
		new (this) Color(c);
	    }
	    return *this;
	}
*/
	Color & operator=(const char* s) {
	    this->~Color();
	    new (this) Color(s);
	    return *this;
	}

	Color & operator+=(const Color& rhs) {
	    r += rhs.r;
	    g += rhs.g;
	    b += rhs.b;
	    a += rhs.a;

	    return *this;
	}

	union {

	    struct {
		float r, g, b, a;
	    };

	    struct {
		float red, green, blue, alpha;
	    };
	    float _array[4];
	};

    private:
	void Init(const char*);
    };
};

#endif	/* _COLOR_H */

