#include "point2d.h"

Point2D::Point2D(){}

Point2D::Point2D(double x_, double y_){
    x = x_;
    y = y_;
}

Point2D::Point2D(const Point2D &p){
    x = p.x;
    y = p.y;
}

Point2D::Point2D(const Point2D *p){
    x = p->x;
    y = p->y;
}

Point2D::~Point2D(){
}

