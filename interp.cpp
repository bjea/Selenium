// $Id: interp.cpp,v 1.2 2015-07-16 16:57:30-07 - - $

#include <memory>
#include <string>
#include <vector>
using namespace std;

#include <OpenGL/freeglut.h>

#include "debug.h"
#include "interp.h"
#include "shape.h"
#include "shape.cpp"
#include "util.h"

// Whenever u declare sth. static in header files, u need to
// re-declare it in .cpp, like here, interp_map, factory_map,
// objmap. Here, objmap need not be initialized.
unordered_map<string,interpreter::interpreterfn>
interpreter::interp_map {
   {"define" , &interpreter::do_define },
   {"draw"   , &interpreter::do_draw   },
};

unordered_map<string,interpreter::factoryfn>
interpreter::factory_map {
   {"text"     , &interpreter::make_text     },
   {"ellipse"  , &interpreter::make_ellipse  },
   {"circle"   , &interpreter::make_circle   },
   {"polygon"  , &interpreter::make_polygon  },
   {"rectangle", &interpreter::make_rectangle},
   {"square"   , &interpreter::make_square   },
   {"diamond"  , &interpreter::make_diamond  },
   {"triangle" , &interpreter::make_triangle },
   //{"equilateral"}
};

interpreter::shape_map interpreter::objmap;

// Debugger statement.
interpreter::~interpreter() {
   for (const auto& itor: objmap) {
      cout << "objmap[" << itor.first << "] = "
           << *itor.second << endl;
   }
}

void interpreter::interpret (const parameters& params) {
   DEBUGF ('i', params);
   param begin = params.cbegin();
   string command = *begin;
   auto itor = interp_map.find (command);
   if (itor == interp_map.end()) throw runtime_error ("syntax error");
   interpreterfn func = itor->second;
   func (++begin, params.cend());
}

void interpreter::do_define (param begin, param end) {
   DEBUGF ('f', range (begin, end)); // range is to generate a pair
   string name = *begin;
   // emplace is a lot more efficient than insert when creating a
   // temporary obj. B/c emplace does not really create an obj.,
   // just construct it inside the data structure.
   objmap.emplace (name, make_shape (++begin, end));
}


void interpreter::do_draw (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin != 4) throw runtime_error ("syntax error");
   string name = begin[1];
   shape_map::const_iterator itor = objmap.find (name);
   if (itor == objmap.end()) {
      throw runtime_error (name + ": no such shape");
   }
   rgbcolor color {begin[0]};
   vertex where {from_string<GLfloat> (begin[2]),
                 from_string<GLfloat> (begin[3])};
   // 2nd argument is the obj. itself, and we tell it to go draw
   // itself w/ where and color specified.
   itor->second->draw (where, color);
}

shape_ptr interpreter::make_shape (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string type = *begin++; // postfix ++ more expensive b/c it needs
   // to create a copy of the obj., while prefix ++ does it
   // in-place. So here we can change it to "= *begin", then
   // return func (++begin, end).
   auto itor = factory_map.find(type);
   if (itor == factory_map.end()) {
      // runtime_error is defined in parsefile().
      throw runtime_error (type + ": no such shape");
   }
   factoryfn func = itor->second;
   return func (begin, end);
}

// Need to changed.
shape_ptr interpreter::make_text (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   auto font = *begin++;
   auto itor = fontcode.find(font);
   if (itor == fontcode.end()) {
      // runtime_error is defined in parsefile().
      throw runtime_error (font + ": no such font");
   }
   string textdata = "";
   bool space = false;
   for (; begin != end; ++begin)
   {
      if (space) textdata += " ";
      textdata += *begin;
      space = true;
   }
   //text words (void* glut_bitmap_font, const string& textdata);
   //return make_shared<text> (nullptr, string());
   return make_shared<text> (itor->second, textdata);
}

// Need to changed.
shape_ptr interpreter::make_ellipse (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   GLfloat width = *begin++;
   GLfloat height = *begin;
   return make_shared<ellipse> (GLfloat(), GLfloat());
}

// Need to changed.
shape_ptr interpreter::make_circle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   GLfloat diameter = *begin;
   return make_shared<circle> (GLfloat());
}

// Need to changed.
// throw a range error if passed in arguments are not 2x.
shape_ptr interpreter::make_polygon (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (distance(begin, end) % 2) {
      throw runtime_error ("Number of vertices is odd.");
   }
   vertex_list vertexList;
   for (; begin != end; ++begin)
   {
      vertex v;
      v.xpos = *begin++;
      v.ypos = *begin++;
      vertexList.push_back(v);

   }
   return make_shared<polygon> (vertex_list());
}

// Need to changed.
shape_ptr interpreter::make_rectangle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   GLfloat width = *begin++;
   GLfloat height = *begin;
   return make_shared<rectangle> (GLfloat(), GLfloat());
}

// Need to changed.
shape_ptr interpreter::make_square (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   GLfloat width = *begin;
   return make_shared<square> (GLfloat());
}

// make_triangle:
shape_ptr interpreter::make_triangle () {
   DEBUGF ('f', range (begin, end));
   if (distance(begin, end) != 6) {
      throw runtime_error ("Number of vertices is not 6.");
   }
   vertex_list vertexList;
   for (; begin != end; ++begin)
   {
      vertex v;
      v.xpos = *begin++;
      v.ypos = *begin++;
      vertexList.push_back(v);

   }
   return make_shared<polygon> (vertex_list());
}
// throw a range error if passed in arguments are not 6.
