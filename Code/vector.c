/*
 * A type definition of a three dimensional vector, as well as a collection of
 * functions for use with the Vector3 type.
 *
 * For http://github.com/Zalodu/Quadcontrol-2
 * Author: Jesper Larsson (mrlarssonjr)
 * Date: 15/12/16
 */

#include "vector.h"

Vector3 vector_add(Vector3 a, Vector3 b) {
	Vector3 result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
}

Vector3 vector_subtract(Vector3 a, Vector3 b) {
	Vector3 result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;

	return result;
}

Vector3 vector_scalarProduct(double scalar, Vector3 v) {
	Vector3 result;

	result.x = v.x * scalar;
	result.y = v.y * scalar;
	result.z = v.z * scalar;

	return result;
}

double vector_dotProduct(Vector3 a, Vector3 b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vector3 vector_crossProduct(Vector3 a, Vector3 b) {
	Vector3 result;

	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;

	return result;
}
