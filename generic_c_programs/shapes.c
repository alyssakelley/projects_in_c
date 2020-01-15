#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    double radius, originX, originY;
} Circle;

typedef struct
{
    double pt1X, pt2X, minY, maxY;
} Triangle;

typedef struct
{
    double minX, minY, maxX, maxY;
} Rectangle;

typedef struct
{
    void *self;
    void (*GetBoundingBox)(void *self, double *bbox); // ERROR with Shape -> void, so this needs to be void *self.
    double (*GetArea)(void *self);
} Shape;

double GetCircleArea(void *circ)
{
    //printf("This is the GetCircleArea function!\n");
    Circle *c = (Circle *) circ; // casting void to be a circle type.
    double CircleArea = 3.14159 * c -> radius * c -> radius;
    //printf("This is radius: %f, X: %f, Y: %f\n", c -> radius, c -> originX, c -> originY);
    //printf("This is the circle area in GetCircleArea function: %f", CircleArea);
    return CircleArea;
};

double GetRectangleArea(void *rect)
{
    //printf("This is the GetRectangleArea function!\n");   
    Rectangle *r = (Rectangle *) rect;
    double RectArea = (r -> maxX - r -> minX) * (r -> maxY - r -> minY);
    return RectArea;
};

double GetTriangleArea(void *tri)
{
    //printf("This is the GetTriangleArea function!\n");
    Triangle * t = (Triangle *) tri;
    double TriangleArea = (t -> pt2X - t -> pt1X) * (t -> maxY - t -> minY) / 2;
    return TriangleArea;
};


void GetCircleBoundingBox(void *circ, double *bbox)
{
    //printf("This is the GetCircleBoundingBox function!\n");
    //(x-radius) to (x+radius) in X, and (y-radius) to (y+radius) in Y.
    Circle * c = (Circle *) circ;
    bbox[0] = (c -> originX) - (c -> radius);
    bbox[1] = (c -> originX) + (c -> radius);
    bbox[2] = (c -> originY) - (c -> radius);
    bbox[3] = (c -> originY) + (c -> radius);

};

void GetRectangleBoundingBox(void *rect, double * bbox)
{
    //printf("This is the GetRectangleBoundingBox function!\n");
    Rectangle *r = (Rectangle *) rect;
    bbox[0] = r -> minX;
    bbox[1] = r -> maxX;
    bbox[2] = r -> minY;
    bbox[3] = r -> maxY;
};

void GetTriangleBoundingBox(void *tri, double *bbox)
{
    //printf("This is the GetTriangleBoundingBox function!\n");
    //pt1X to pt2X in X, and from minY to maxY in Y.
    Triangle *t = (Triangle *) tri;
    bbox[0] = t -> pt1X;
    bbox[1] = t -> pt2X;
    bbox[2] = t -> minY;
    bbox[3] = t -> maxY;

};

/* Define these 9 functions */
Shape *CreateCircle(double radius, double originX, double originY)
{
    //printf("This is the CreateCircle function!\n");
    /* 
    Creating the correct shape in the function definition (with a malloc). 
    Connecting the shape’s dispatch table to circle/rectangle/triangle. 
    */
    Circle *c;
    c = malloc(sizeof(Circle));

    c -> radius = radius;
    c -> originX = originX;
    c -> originY = originY;

    Shape *circ_shape;
    circ_shape = malloc(sizeof(Shape));

    circ_shape -> self = c; // this is void

    circ_shape -> GetArea = GetCircleArea; // These are that functions parameters: (circ_shape -> self) but you do not use them here.
    circ_shape -> GetBoundingBox = GetCircleBoundingBox; // (circ_shape -> self, bbox);

    return circ_shape;
};

Shape *CreateRectangle(double minX, double maxX, double minY, double maxY)
{
    //printf("This is the CreateRectangle function!\n");

    Rectangle *r;
    r = malloc(sizeof(Rectangle));

    r -> minX = minX;
    r -> minY = minY;
    r -> maxX = maxX;
    r -> maxY = maxY;

    Shape *rect_shape;

    rect_shape = malloc(sizeof(Shape));

    rect_shape -> self = r;

    rect_shape -> GetArea = GetRectangleArea; // (rect_shape -> self); //(void *r);
    rect_shape -> GetBoundingBox = GetRectangleBoundingBox; // (rect_shape -> self, bbox);

    return rect_shape;
};

Shape *CreateTriangle(double pt1X, double pt2X, double minY, double maxY)
{
    //printf("This is the CreateTriangle function!\n");

    Triangle *t;
    t = malloc(sizeof(Triangle));

    t -> pt1X = pt1X;
    t -> pt2X = pt2X;
    t -> minY = minY;
    t -> maxY = maxY;

    Shape *tri_shape;
    tri_shape = malloc(sizeof(Shape));

    tri_shape -> self = t;

    tri_shape -> GetArea = GetTriangleArea; // (tri_shape -> self);
    tri_shape -> GetBoundingBox = GetTriangleBoundingBox; // (tri_shape -> self, bbox);

    return tri_shape;
};

double GetArea(Shape *s)
{   
    //printf("This is the GetArea function!\n");
    double area = s -> GetArea(s->self);
    return area;
};

void GetBoundingBox(Shape *s, double *bbox)
{
    //printf("This is the GetBoundingBox function!\n");
    s -> GetBoundingBox(s->self, bbox);
};

int main()
{
    //printf("I am in main!\n");
    Shape *shapes[9];
    int    i;
    shapes[0] = CreateCircle(1, 0, 0);
    shapes[1] = CreateCircle(1.5, 6, 8);
    shapes[2] = CreateCircle(0.5, -3, 4);

    shapes[3] = CreateRectangle(0, 1, 0, 1);
    shapes[4] = CreateRectangle(1, 1.1, 10, 20);
    shapes[5] = CreateRectangle(1.5, 3.5, 10, 12);

    shapes[6] = CreateTriangle(0, 1, 0, 1);
    shapes[7] = CreateTriangle(0, 1, 0, 0.1);
    shapes[8] = CreateTriangle(0, 10, 0, 50);

    for (i = 0 ; i < 9 ; i++)
    {
        double bbox[4];
        printf("Shape %d\n", i);
        printf("\tArea: %f\n", GetArea(shapes[i]));
        GetBoundingBox(shapes[i], bbox);
        printf("\tBbox: %f-%f, %f-%f\n", bbox[0], bbox[1], bbox[2], bbox[3]);
        free(shapes[i]); // freeing the memory we malloc in the Create functions.
    }

}