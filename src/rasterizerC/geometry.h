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
	Vector(double x_in = 0.0, double y_in = 0.0);
	// TODO: operator overload
	Vector operator+(const Vector &o) const;
	Vector operator-(const Vector &o) const;
	Vector operator*(double k) const;

	double dot(const Vector &o) const;
	Vector min(const Vector &o) const;
	Vector max(const Vector &o) const;
	double length();
	void repr(); // TODO: what's the meaning?
};

class AABox
{
public:
	Vector low, high; // use Vector to represent points
public:
 	AABox(const Vector &p1 = Vector(), const Vector &p2 = Vector());
 	Vector midpoint() const;
 	Vector size() const;
 	bool contains(const Vector &p) const;
 	bool overlaps(const AABox &r) const;
 	AABox intersection(const AABox &other) const;
 	static AABox from_vectors(const Vector *vcts, const int vnum);
};

class HalfPlane
{
	Vector v;
	double c;
 public:
 	HalfPlane(const Vector &p1, const Vector &p2);
 	double signed_distance(const Vector p) const;
};

// Transform represents an affine transformation
// of a 2D vector ("affine" because it's a linear transformation
// combined with a translation)
class Transform
{
public:
	double m[3][3];
public:
 	Transform(double m11 = 1.0, double m12 = 0.0, double tx = 0.0, 
 				double m21 = 0.0, double m22 = 1.0, double ty = 0.0);
 	const Transform operator*(const Transform &other) const;
 	const Vector operator*(const Vector &other) const;
 	double det() const;
 	Transform inverse() const;
};

const Transform identity();
const Transform rotate(double theta);
const Transform translate(double tx, double ty);
const Transform scale(double x, double y);
const Transform around(const Vector &v, const Transform &t);

#endif //__GEOMETRY_H__