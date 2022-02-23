#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/General_polygon_2.h>

#ifndef __DEFS_H__
#define __DEFS_H__

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Segment_2<Kernel> Segment;
typedef Kernel::Point_2 Point;
typedef Kernel::Line_2 Line;
typedef Kernel::Direction_2 Direction;
typedef CGAL::Arr_segment_traits_2<Kernel> Traits;
typedef CGAL::Arrangement_2<Traits> Arrangement;
typedef CGAL::General_polygon_2<Traits> Polygon;

typedef Arrangement::Vertex_const_handle Vertex;
typedef Arrangement::Halfedge_const_handle Halfedge;
typedef Arrangement::Face_const_handle Face;

#endif
