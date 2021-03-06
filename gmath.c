#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  normalize(light[LOCATION]);
  normalize(normal);
  normalize(view);
  color ambient = calculate_ambient(alight, areflect);
  color diffuse = calculate_diffuse(light, dreflect, normal);
  color specular = calculate_specular(light, sreflect, view, normal);
  i.red = ambient.red + diffuse.red + specular.red;
  i.green = ambient.green + diffuse.green + specular.green;
  i.blue = ambient.blue + diffuse.blue + specular.blue;
  limit_color(&i);
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red = alight.red * areflect[0];
  a.green = alight.green * areflect[1];
  a.blue = alight.blue * areflect[2];
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  double dotProduct = dot_product(normal, light[LOCATION]);
  d.red = light[COLOR][RED] * dotProduct * dreflect[0];
  d.green = light[COLOR][GREEN] * dotProduct * dreflect[1];
  d.blue = light[COLOR][BLUE] * dotProduct * dreflect[2];
  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {

  color s;
  double temp[3];
  double dot = 2 * dot_product(normal, light[LOCATION]);
  temp[0] = (normal[0] * dot) - light[LOCATION][0];
  temp[1] = (normal[1] * dot) - light[LOCATION][1];
  temp[2] = (normal[2] * dot) - light[LOCATION][2];
  double dotProduct = dot_product(view, dot);
  dotProduct = dotProduct > 0 ? pow(dotProduct, SPECULAR_EXP) : 0;
  s.red = light[COLOR][RED] * dotProduct * sreflect[0];
  s.green = light[COLOR][GREEN] * dotProduct * sreflect[1];
  s.blue = light[COLOR][BLUE] * dotProduct * sreflect[2];
  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  c -> red = c -> red > 255 ? 255 : c -> red;
  c -> green = c -> green > 255 ? 255 : c -> green;
  c -> blue = c -> blue > 255 ? 255 : c -> blue;
  c -> red = c -> red < 0 ? 0 : c -> red;
  c -> green = c -> green < 0 ? 0 : c -> green;
  c -> blue = c -> blue < 0 ? 0 : c -> blue;
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double length = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
  vector[0] = vector[0]/length;
  vector[1] = vector[1]/length;
  vector[2] = vector[2]/length;
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}