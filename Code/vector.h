#pragma once

typedef struct {
	double x;
	double y;
	double z;
} Vector3;

Vector3 vector_add(Vector3 a, Vector3 b);

Vector3 vector_subtract(Vector3 a, Vector3 b);

Vector3 vector_scalarProduct(double scalar, Vector3 v);

double vector_dotProduct(Vector3 a, Vector3 b);

Vector3 vector_crossProduct(Vector3 a, Vector3 b);
