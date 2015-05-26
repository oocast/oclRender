#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

// *x stores both roots of the quadratic equation
// x: an array of double with size 2
void quadratic(double a, double b, double c,
				double *x);

class Vector
{
public:
	double x, y;
	Vector(double x_in, double y_in);
	// TODO: operator overload
	const Vector operator+(const Vector &o);
	const Vector operator-(const Vector &o);
	const Vector operator*(double k);

	double dot(const Vector &o);
	Vector min(const Vector &o);
	Vector max(const Vector &o);
	double length();
	void repr(); // TODO: what's the meaning?
};

class AABox
{
	Vector low, high; // use Vector to represent points
 public:
 	AABox(const Vector &p1, const Vector &p2);
 	Vector midpoint();
 	Vector size();
 	bool contains(const Vector &p);
 	bool overlaps(const AABox &r);
 	AABox intersection(const AABox &other);
 	// TODO: from_vectors
 	static AABox from_vectors(const Vector *vcts, const int vnum);
};

class HalfPlane
{
	Vector v;
	double c;
 public:
 	HalfPlane(const Vector &p1, const Vector &p2);
 	double signed_distance(const Vector p);
};

// Transform represents an affine transformation
// of a 2D vector ("affine" because it's a linear transformation
// combined with a translation)
class Transform
{
public:
	double m[3][3];
public:
 	Transform(double m11, double m12, double tx, 
 				double m21, double m22, double ty);
 	const Transform operator*(const Transform &other);
 	const Vector operator*(const Vector &other);
 	double det();
 	Transform inverse();
};

Transform identity();
Transform rotate(double theta);
Transform translate(double tx, double ty);
Transform scale(double x, double y);
Transform around(const Vector &v, const Transform &t);

#endif //__GEOMETRY_H__