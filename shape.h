// $Id: shape.h,v 1.1 2015-07-16 16:47:51-07 - - $

#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <iomanip>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
using namespace std;

#include "rgbcolor.h"

//
// Shapes constitute a single-inheritance hierarchy, summarized
// here, with the superclass listed first, and subclasses indented
// under their immediate superclass.
//
// shape
//    text
//    ellipse
//       circle
//    polygon
//       rectangle
//          square
//       diamond
//       triangle
//          right_triangle
//          isosceles
//          equilateral
//
// No ellipse in OpenGL, so we need to write a function to draw
// it. Basically, it's a whole bunch of vertices connecting
// together to deceive human eyes.

class shape;
struct vertex {GLfloat xpos; GLfloat ypos; };
using vertex_list = vector<vertex>;
using shape_ptr = shared_ptr<shape>; // a vector of vertices

//
// Abstract base class for all shapes in this system.
//

class shape {
   friend ostream& operator<< (ostream& out, const shape&);
   private:
    // The following 4 can all be under "public".
      shape (const shape&) = delete; // Prevent copying.
      shape& operator= (const shape&) = delete; // Prevent copying.
      shape (shape&&) = delete; // Prevent moving.
      shape& operator= (shape&&) = delete; // Prevent moving.
   protected:
    // It's OK to be in "public" b/c u can't instantiate it unless
      inline shape(); // Only subclass may instantiate.
   public:
      virtual ~shape() {}
      virtual void draw (const vertex&, const rgbcolor&) const = 0;
    // show() for debugging purposes,  x.show (cout) meaning
    // cout << x.
      virtual void show (ostream&) const;
};


//
// Class for printing text.
//
// Figure out and override each of the functions.
class text: public shape {
   protected:
      void* glut_bitmap_font = nullptr; // Default: nullptr.
      // GLUT_BITMAP_8_BY_13
      // GLUT_BITMAP_9_BY_15
      // GLUT_BITMAP_HELVETICA_10
      // GLUT_BITMAP_HELVETICA_12
      // GLUT_BITMAP_HELVETICA_18 
      // GLUT_BITMAP_TIMES_ROMAN_10
      // GLUT_BITMAP_TIMES_ROMAN_24
      string textdata;
   public:
    // Default constructor
    // We don't need destructor herre, b/c void* is not pointing to
    // data, it's pointing to static data, and never delete static
    // data.
      text (void* glut_bitmap_font, const string& textdata);
      virtual void draw (const vertex&, const rgbcolor&) const override;
      virtual void show (ostream&) const override;
};

//
// Classes for ellipse and circle.
//

class ellipse: public shape {
   protected:
      vertex dimension;
   public:
      ellipse (GLfloat width, GLfloat height);
      virtual void draw (const vertex&, const rgbcolor&) const override;
      virtual void show (ostream&) const override;
};

class circle: public ellipse {
   public:
      circle (GLfloat diameter);
    // We don't override draw or show, b/c the way u draw a circle is
    // the way u draw an ellipse.
};

//
// Class polygon.
//

class polygon: public shape {
   protected:
      const vertex_list vertices;
   public:
      polygon();
      polygon (const vertex_list& vertices);
      virtual void draw (const vertex&, const rgbcolor&) const override;
      virtual void show (ostream&) const override;
};


//
// Classes rectangle, square, etc.
// Add triangle, etc.
//

class rectangle: public polygon {
   public:
      rectangle (GLfloat width, GLfloat height);
};

class square: public rectangle {
   public:
      square (GLfloat width);
};

class diamond: public polygon {
   public:
    // height == top vertex to bottom vertex.
      diamond (GLfloat width, GLfloat height);
};

class triangle : public polygon {
   public:
      triangle (const vertex_list& vertices);
};

ostream& operator<< (ostream& out, const shape&);

#endif

