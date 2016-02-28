// $Id: rgbcolor.h,v 1.1 2015-07-16 16:47:51-07 - - $

#ifndef __RGBCOLOR_H__
#define __RGBCOLOR_H__

#include <string>
#include <unordered_map>
using namespace std;

#include <OpenGL/freeglut.h>

// POD (Plain Old Data), so no ADT needed.
struct rgbcolor {
   union {
      GLubyte ubvec[3];
      struct {
         GLubyte red;
         GLubyte green;
         GLubyte blue;
      };
   };
   explicit rgbcolor(): red(0), green(0), blue(0) {}
    // Specify colors by 0-255, or name itself
   explicit rgbcolor (GLubyte red, GLubyte green, GLubyte blue):
               red(red), green(green), blue(blue) {}
   explicit rgbcolor (const string&);
   const GLubyte* ubvec3() { return ubvec; }
    // Provides an implicit
   operator string() const;
};

ostream& operator<< (ostream&, const rgbcolor&);

extern const std::unordered_map<std::string,rgbcolor> color_names;

#endif

