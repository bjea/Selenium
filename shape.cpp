// $Id: shape.cpp,v 1.1 2015-07-16 16:47:51-07 - - $

#include <typeinfo>
#include <unordered_map>
#include <OpenGL/OpenGL.h>

using namespace std;

#include "shape.h"
#include "util.h"

static unordered_map<void*,string> fontname {
   {GLUT_BITMAP_8_BY_13       , "Fixed-8x13"    },
   {GLUT_BITMAP_9_BY_15       , "Fixed-9x15"    },
   {GLUT_BITMAP_HELVETICA_10  , "Helvetica-10"  },
   {GLUT_BITMAP_HELVETICA_12  , "Helvetica-12"  },
   {GLUT_BITMAP_HELVETICA_18  , "Helvetica-18"  },
   {GLUT_BITMAP_TIMES_ROMAN_10, "Times-Roman-10"},
   {GLUT_BITMAP_TIMES_ROMAN_24, "Times-Roman-24"},
};

static unordered_map<string,void*> fontcode {
   {"Fixed-8x13"    , GLUT_BITMAP_8_BY_13       },
   {"Fixed-9x15"    , GLUT_BITMAP_9_BY_15       },
   {"Helvetica-10"  , GLUT_BITMAP_HELVETICA_10  },
   {"Helvetica-12"  , GLUT_BITMAP_HELVETICA_12  },
   {"Helvetica-18"  , GLUT_BITMAP_HELVETICA_18  },
   {"Times-Roman-10", GLUT_BITMAP_TIMES_ROMAN_10},
   {"Times-Roman-24", GLUT_BITMAP_TIMES_ROMAN_24},
};

ostream& operator<< (ostream& out, const vertex& where) {
   out << "(" << where.xpos << "," << where.ypos << ")";
   return out;
}

// shpae constructor: nothing needed to be done b/c
shape::shape() {
   DEBUGF ('c', this);
}

text::text (void* glut_bitmap_font, const string& textdata):
      glut_bitmap_font(glut_bitmap_font), textdata(textdata) {
   DEBUGF ('c', this); // call this program w/ -@c, then it
   // will print out some info.
}

ellipse::ellipse (GLfloat width, GLfloat height):
// here, dimension needs a vertex to initialize it, but here
// we use { }, which will cast it to vertex (?). vertex list.
dimension ({width, height}) {
   DEBUGF ('c', this);

}

circle::circle (GLfloat diameter): ellipse (diameter, diameter) {
   DEBUGF ('c', this);
}


polygon::polygon (const vertex_list& vertices): vertices(vertices) {
   DEBUGF ('c', this);
}


rectangle::rectangle (GLfloat width, GLfloat height) {//:
            //polygon({}) { // polygon({}) is wrong.
   // This one is wrong, u need a loop, and call some fcn. to
   // return the vertex list
   DEBUGF ('c', this << "(" << width << "," << height << ")");
   vertex_list vList;
   vertex v = {width, height};
   vList.push_back(v);
   v = {width-width, height};
   vList.push_back(v);
   v = {width-width, height-height};
   vList.push_back(v);
   v = {width, height-height};
   vList.push_back(v);
   polygon(vList);
}

square::square (GLfloat width): rectangle (width, width) {
   DEBUGF ('c', this);
}

// Wrong, presumptively convex polygon, not sure what OpenGL does
// when it's non-convex polygon.
void text::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");

   glColor3ubv(color.ubvec3());
}

// See ellipse.cpp, points u use should be power of 4, then
// draw_ (GL_POLYGON) as field, not LINE-LOOP.
void ellipse::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glBegin(GL_POLYGON);
   glEnable (GL_LINE_SMOOTH);
   glColor3ubv(color.ubvec3());
   const GLfloat delta = 2 * M_PI / 32;
   for (GLfloat theta = 0; theta < 2 * M_PI; theta += delta)
   {
      GLfloat x = center.xpos + dimension.xpos * cos (theta);
      GLfloat y = center.ypos + dimension.ypos * sin (theta);
      glVertex2f (x, y);
   }
   glEnd();

}

// center is defined in polygon constructor, (avg. of x, avg., of y)
void polygon::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
}

// DEBUG
void shape::show (ostream& out) const {
   out << this << "->" << demangle (*this) << ": ";
}

// DEBUG
void text::show (ostream& out) const {
   shape::show (out);
   out << glut_bitmap_font << "(" << fontname[glut_bitmap_font]
       << ") \"" << textdata << "\"";
}

void ellipse::show (ostream& out) const {
   shape::show (out);
   out << "{" << dimension << "}";
}

void polygon::show (ostream& out) const {
   shape::show (out);
   out << "{" << vertices << "}";
}

ostream& operator<< (ostream& out, const shape& obj) {
   obj.show (out);  // Reversing the thing.
   return out;
}

