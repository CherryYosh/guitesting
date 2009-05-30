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
		virtual ~Color();

		Color(std::string);
		Color(float, float, float, float);

		float RFromStr(std::string);
		float GFromStr(std::string);
		float BFromStr(std::string);
		float AFromStr(std::string);

		bool HasAlpha(std::string);
		bool IsHexStr(std::string);

		float operator[](int i) {
			return _array[i];
		};

		Color& operator=(const std::string& s) {
			*this = Color(s.c_str());
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
	};
};

#endif	/* _COLOR_H */

