#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_CASE 100000 // limit for the points

typedef struct{
  double x, y;
  double r;
} circle;

typedef struct{
  double a, b, c;
}line;

//tries evry possible circle and find the smallest
//starts trying circle passing by two points and then tries with circle passing
//by three of them
double calculateDistance(double x1, double y1, double x2, double y2){
  double distance;
  distance = sqrt(pow(x1-x2, 2) + pow(y1 -y2, 2));
  return distance;
}

double distancePointLine(double points[][2], int p1, line l){
  double distance;
  distance = ((l.a * points[p1][0])+(l.b*points[p1][1]) + l.c)/sqrt(pow(l.a, 2)+pow(l.b, 2));
  distance = fabs(distance);

  return distance;
}

line lineFromPoints(double points[][2], int p1, int p2) {
    line l;
    double a = points[p2][1] - points[p1][1];
    double b = points[p1][0] - points[p2][0];
    double c = a*(points[p1][0]) + b*(points[p1][1]);

    l.a = a;
    l.b = b;
    l.c = c;

    return l;
}

// Function to find the circle on
// which the given three points lies
circle findCircle(double x1, double y1, double x2, double y2, double x3, double y3) {
    double x12 = x1 - x2;
    double x13 = x1 - x3;

    double y12 = y1 - y2;
    double y13 = y1 - y3;

    double y31 = y3 - y1;
    double y21 = y2 - y1;

    double x31 = x3 - x1;
    double x21 = x2 - x1;

    circle aFuckingCircle;

    // x1^2 - x3^2
    double sx13 = pow(x1, 2) - pow(x3, 2);

    // y1^2 - y3^2
    double sy13 = pow(y1, 2) - pow(y3, 2);

    double sx21 = pow(x2, 2) - pow(x1, 2);
    double sy21 = pow(y2, 2) - pow(y1, 2);

    double f = ((sx13) * (x12)
             + (sy13) * (x12)
             + (sx21) * (x13)
             + (sy21) * (x13))
            / (2 * ((y31) * (x12) - (y21) * (x13)));
    double g = ((sx13) * (y12)
             + (sy13) * (y12)
             + (sx21) * (y13)
             + (sy21) * (y13))
            / (2 * ((x31) * (y12) - (x21) * (y13)));

    double c = -pow(x1, 2) - pow(y1, 2) - 2 * g * x1 - 2 * f * y1;

    // eqn of circle be x^2 + y^2 + 2*g*x + 2*f*y + c = 0
    // where centre is (h = -g, k = -f) and radius r
    // as r^2 = h^2 + k^2 - c
    double h = -g;
    double k = -f;
    double sqr_of_r = h * h + k * k - c;

    // r is the radius
    double r = sqrt(sqr_of_r);

    aFuckingCircle.x = h;
    aFuckingCircle.y = k;
    aFuckingCircle.r = r;
    return aFuckingCircle;
}

//algorithm complexity around O(n^4)
circle bruteForceMethod(double points[][2], int n){
  int i=0, j=0, k;
  double xc, yx, distance;
  circle best;
  circle temp;
  int findAPointOut = 0;
  best.r = INFINITY;


  for(i = 0; i<n; i++){
    for(j=i+1; j<n; j++){

      /*Finding the distance from two point, calculate the radius and the center
      of the hypotetical circle then checking if contains all the points. If that
      happends i will chek if is radius is better than the best one by far
      */
      distance = calculateDistance(points[i][0], points[i][1], points[j][0], points[j][0]);
      temp.r = distance/2;
      temp.x = (points[i][0] + points[j][0])/2;
      temp.y = (points[i][1] + points[j][1])/2;
      //checking if all points are in the new circle
      findAPointOut = 0;
      for(k=0; k<n; k++){
        if((pow(points[k][0] - temp.x, 2) + pow(points[k][1] - temp.y, 2) > pow(temp.r, 2)) && k!= i && k!=j){
          findAPointOut = 1;
        }
      }

      if(!findAPointOut){
        if(temp.r < best.r){
          best = temp;
        }
      }
    }
  }



  for(i=0; i<n; i++){
    for(j =i+1; j<n; j++){
      for(k=j+1; k<n; k++){
        temp = findCircle(points[i][0], points[i][1], points[j][0], points[j][1], points[k][0], points[k][1]);

        findAPointOut = 0;
        for(int z=0; z<n; z++){
          if((pow(points[z][0] - temp.x, 2) + pow(points[z][1] - temp.y, 2) > pow(temp.r, 2)) && z!= i && z!=j && z!=k){
            findAPointOut = 1;
          }
        }

        if(!findAPointOut){
          if(temp.r < best.r){
          best = temp;
          }
        }
      }
    }
  }

  return best;
}

circle randomizedIncrementalMethodGivenTwoPoints(double points[][2], int n, int firstPointPosition, int secondPointPosition){
  circle c;
  float isInTheCircle;

  c.x = (points[firstPointPosition][0] + points[secondPointPosition][0]/2);
  c.y = (points[firstPointPosition][1] + points[secondPointPosition][1]/2);
  c.r = (calculateDistance(points[firstPointPosition][0], points[firstPointPosition][1], points[secondPointPosition][0], points[secondPointPosition][1])/2);

  for(int i=0; i<n; i++){
    isInTheCircle = pow(points[i][0] - c.x, 2) + pow(points[i][1] - c.y, 2) - pow(c.r, 2);

    if(isInTheCircle > 0) {
      c = findCircle(points[firstPointPosition][0], points[firstPointPosition][1], points[secondPointPosition][0], points[secondPointPosition][1], points[i][0], points[i][1]);
    }
  }

  return c;
}


circle RandomizedIncrementalMethodGivenOnePoint(double points[][2], int pointPosition, int n){
  circle c;
  int i, j;
  double isInTheCircle;


  c.x = (points[pointPosition][0]+points[0][0])/2;
  c.y = (points[pointPosition][1]+points[0][1])/2;
  c.r = (calculateDistance(points[pointPosition][0], points[pointPosition][1], points[0][0], points[0][1])/2);


  for(j=1; j<pointPosition; j++){
    isInTheCircle = pow(points[j][0] - c.x, 2) + pow(points[j][1] - c.y, 2) - pow(c.r, 2);
    if(isInTheCircle > 0){

      c = randomizedIncrementalMethodGivenTwoPoints(points, j, j, pointPosition);
    }
  }

  return c;

}

//algorithm complexity aroud O(n)
circle randomizedIncrementalMethod(double points[][2], int n){
  circle c;
  float x;
  double isInTheCircle;

  c.x = (points[1][0]+points[0][0])/2;
  c.y = (points[1][1]+points[0][1])/2;
  c.r = (calculateDistance(points[0][0], points[0][1], points[1][0], points[1][1])/2);

  for(int i=2; i<n; i++){
    isInTheCircle = pow(points[i][0] - c.x, 2) + pow(points[i][1] - c.y, 2) - pow(c.r, 2);
    if(isInTheCircle > 0){
      c = RandomizedIncrementalMethodGivenOnePoint(points, i, n);
    }
  }

  return c;
}


void printCircle(circle c){
  printf("choosen circle: Center -> (%.2lf, %.2lf), radius -> %.2lf\n", c.x, c.y, c.r);
}

int main(int argc, char const *argv[]) {

  FILE *f;
  int n;
  double points[MAX_CASE][2]; //x, y
  int i, j;
  circle c;
  int check = 0;
  int scelta = 0;


  if(argc == 1){
    printf("no argumets was passed\n");
    printf("Usage: ./executable \"file.txt\"\n" );
    return 0;
  }

  if((f = fopen(argv[1], "r")) == NULL){
    printf("Error opening the file");
    exit(1);
  }

  //reads the number of test points
  fscanf(f, "%d", &n);

  for(i=0;i<n;i++){
    fscanf(f, "%lf %lf", &points[i][0], &points[i][1]);
  }

  printf("Choose algorithm (type number):\n1.Brute Force\n2.Incremental Method\n");
  scanf("%d", &scelta);

  if(scelta == 1){
    c = bruteForceMethod(points, n);
    printf("choosen circle by Brute Froce Method: Center -> (%.2lf, %.2lf), radius -> %.2lf\n", c.x, c.y, c.r);
  }

  if(scelta == 2){
    c = randomizedIncrementalMethod(points, n);
    printf("choosen circle by Incremental Method: Center -> (%.2lf, %.2lf), radius -> %.2lf\n", c.x, c.y, c.r);
  }

  if((scelta != 1) && (scelta != 2)){
    printf("wrong number!\n");
  }

  return 0;
}
