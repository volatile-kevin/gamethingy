#include "shape.hpp"
// Kevin Hu - kwh2
// Joe Lopez -Joseal2
// Daniel Ao -dao3
//this program is composed of classes and demonstrates inheritance


//Base class
//Please implement Shape's member functions
//constructor, getName()

Shape::Shape(string name){
    name_ = name;
}
string Shape::getName(){
    return name_;
}


//Rectangle
//Please implement the member functions of Rectangle:
//getArea(), getVolume(), operator+, operator-
//copy(), clear()

void Rectangle::copy(const Rectangle& other){
    spec = new double[2];
    spec[0] = other.getWidth();
    spec[1] = other.getLength();
}
void Rectangle::clear(){
    free(spec);
}
Rectangle::Rectangle(double width, double length):Shape("Rectangle"){
    spec = new double[2];
    spec[0] = width;
    spec[1] = length;
}
Rectangle::Rectangle(const Rectangle& other):Shape("Rectangle"){
    copy(other);
}
Rectangle::~Rectangle(){
    clear();
}
const Rectangle& Rectangle::operator = (const Rectangle& other){
    clear();
    copy(other);
    return *this;
}
double Rectangle::getArea()const{
    //TODO
    return spec[0]*spec[1];
}
double Rectangle::getVolume()const{
    //TODO
    return 0;
}
Rectangle Rectangle::operator + (const Rectangle& rhs){
    double newWidth = rhs.getWidth() + this->spec[0];
    double newLength = rhs.getLength() + this->spec[1];
    return Rectangle(newWidth,newLength);
}

Rectangle Rectangle::operator - (const Rectangle& rhs){
  double newWidth = max(0.0,(-rhs.getWidth() + this->spec[0]));
  double newLength = max(0.0,(-rhs.getLength() + this->spec[1]));
  return Rectangle(newWidth,newLength);
}

// double * spec;
//spec[0] should be width
//spec[1] should be length
double Rectangle::getWidth()const{return spec? spec[0]:0;}
double Rectangle::getLength()const{return spec? spec[1]:0;}
void Rectangle::setWidth(double width){
    if (spec == NULL)
        return;

    spec[0] = width;
}
void Rectangle::setLength(double length){
    if (spec == NULL)
        return;

    spec[1] = length;
}


//Rectprism
//Please implement the member functions of RectPrism:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
void RectPrism::copy(const RectPrism& other){
  spec = new double[3];
  spec[1] = other.getWidth();
  spec[0] = other.getLength();
  spec[2] = other.getHeight();
}
void RectPrism::clear(){
    free(spec);
}
RectPrism::RectPrism(double width, double length, double height):Shape("RectPrism"){
    spec = new double[3];
    spec[0] = length;
    spec[1] = width;
    spec[2] = height;
}
RectPrism::RectPrism(const RectPrism& other):Shape("RectPrism"){
    copy(other);
}
RectPrism::~RectPrism(){
    clear();
}
const RectPrism& RectPrism::operator = (const RectPrism& other){
    clear();
    copy(other);
    return *this;
}
double RectPrism::getVolume()const{
    return (spec[2]*spec[1]*spec[0]);
}

double RectPrism::getArea()const{
    return (2 * (spec[1]*spec[0] + spec[1]*spec[2] + spec[0]*spec[2]));
}

RectPrism RectPrism::operator + (const RectPrism& rhs){
  double newLength = rhs.getLength() + spec[0];
  double newWidth = rhs.getWidth() + spec[1];
  double newHeight = rhs.getHeight() + spec[2];
  return RectPrism(newWidth,newLength,newHeight);
}

RectPrism RectPrism::operator - (const RectPrism& rhs){
    double newWidth = max(0.0,-rhs.getWidth() + spec[1]);
    double newLength = max(0.0,-rhs.getLength() + spec[0]);
    double newHeight = max(0.0,-rhs.getHeight() + spec[2]);
    return RectPrism(newWidth,newLength,newHeight);
}

// double * spec;
//spec[0] should be length
//spec[1] should be width
//spec[2] should be height
double RectPrism::getWidth()const{return spec? spec[1]:0;}
double RectPrism::getHeight()const{return spec? spec[2]:0;}
double RectPrism::getLength()const{return spec? spec[0]:0;}
void RectPrism::setWidth(double width){
    if (spec == NULL)
        return;

    spec[1] = width;
}
void RectPrism::setHeight(double height){
    if (spec == NULL)
        return;

    spec[2] = height;
}
void RectPrism::setLength(double length){
    if (spec == NULL)
        return;

    spec[0] = length;
}



// Read shapes from test.txt and initialize the objects
// Return a vector of pointers that points to the objects
vector<Shape*> CreateShapes(char* file_name){
    //@@Insert your code here

    ifstream ifs (file_name, std::ifstream::in);
    double num_shapes = 0;
    ifs >> num_shapes;
    vector<Shape*> shape_ptrs;
    //TODO
    for(int i =0; i<num_shapes; i++){
      string name;
      double w,l,h;
      Shape* temp;
      ifs>>name>>w>>l;
      if(name.compare("Rectangle")==0){
        temp = new Rectangle(w,l);
      } else {
        ifs>>h;
        temp = new RectPrism(w,l,h);
      }
      shape_ptrs.push_back(temp);
    }

    ifs.close();
    for (int i = 0; i<num_shapes; i++){
    }
    return shape_ptrs;
}

// call getArea() of each object
// return the max area
double MaxArea(vector<Shape*> shapes){
    double max_area = 0;
    int numberOfShapes = shapes.size();
    int i;

    for (i = 0;i<numberOfShapes;i++){
      if(shapes[i]->getArea() > max_area){
          max_area = shapes[i]->getArea();
      }
    }

    return max_area;
}


// call getVolume() of each object
// return the max volume
double MaxVolume(vector<Shape*> shapes){
    double max_volume = 0;
    //@@Insert your code here
    int numberOfShapes = shapes.size();
    int i;

    for (i = 0;i<numberOfShapes;i++){
      if(shapes[i]->getVolume() > max_volume){
        max_volume = shapes[i]->getVolume();
      }
    }


    return max_volume;
}
