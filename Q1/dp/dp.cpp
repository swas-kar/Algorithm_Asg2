#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <limits.h>
using namespace std;

// Maximum Vertices the Polygon can have
#define MAX 16

// Maximum value of abscissa and ordinates
#define MAX_VAL 200

// Total number of polygons to be generated
#define TOTAL 50
#define MAX_LINE_LENGTH 100

double dp[MAX][MAX];

typedef struct
{
    int x;
    int y;
} Vertex;

typedef struct
{
    int total_vertices;
    Vertex vertices[MAX];
} Polygon;

double min(double a, double b)
{
    return (a < b) ? a : b;
}

// Generating the vertices randomly
Vertex generateVertex()
{
    Vertex v;
    v.x = rand() % MAX_VAL;
    v.y = rand() % MAX_VAL;
    return v;
}

double dist(Vertex v1, Vertex v2)
{
    return sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}

int crossProduct(Vertex v1, Vertex v2) // vertex direction vector
{
    return v1.x * v2.y - v1.y * v2.x;
}

// Function to determine if a polygon is convex
int isConvex(Polygon poly)
{
    int n = poly.total_vertices;
    int crossproduct = 0;
    for (int i = 0; i < n; i++)
    {
        Vertex v1 = poly.vertices[i];
        Vertex v2 = poly.vertices[(i + 1) % n];
        Vertex v3 = poly.vertices[(i + 2) % n];
        int cp = crossProduct((Vertex){v2.x - v1.x, v2.y - v1.y}, (Vertex){v3.x - v2.x, v3.y - v2.y});
        if (i == 0)
        {
            crossproduct = cp;
        }
        else if (crossproduct * cp < 0)
        {
            return 0;
        }
    }
    return 1;
}

Polygon generatePolygon(int n)
{
    Polygon P;
    P.total_vertices = n;
    Vertex* arr = new Vertex[n];
    arr[0] = generateVertex();
    for (int i = 1; i < n; i++)
    {
        Vertex v;
        do
        {
            v = generateVertex();
        } while (!isConvex((Polygon){i, *arr}));
        arr[i] = v;
    }
    for (int i = 0; i < n; i++)
    {
        P.vertices[i] = arr[i];
    }
    return P;
}
double cost(Vertex *points, int i, int j, int k)
{
    Vertex v1 = points[i];
    Vertex v2 = points[j];
    Vertex v3 = points[k];
    return dist(v1, v2) + dist(v2, v3) + dist(v3, v1);
}

vector<double> computeSideLengths(vector<Vertex> points) {
    vector<double> sideLengths;
    int n = points.size();
    for (int i = 0; i < n; i++) {
        Vertex v1 = points[i];
        Vertex v2 = points[(i+1)%n]; // next vertex
        double d = dist(v1, v2);
        sideLengths.push_back(d); // round to nearest integer
    }
    return sideLengths;
}

double minTriangulationCost(Vertex *points, int i, int j)
{
    // Base Case
    if (j <= i + 1)
    {
        return 0;
    }
    double ans = 1e7;
    for (int k = i + 1; k < j; k++)
    {
        ans = min(ans, (minTriangulationCost(points, i, k) + minTriangulationCost(points, k, j) + cost(points, i, k, j)));
    }
    return ans;
}

double minTriangulationCostDP(Vertex *points, int n)
{
  double Max = 1e7;
  if (n <= 2)
  {
    return 0;
  }
  /* table[i][j] will store the min triangulation cost for vertices from points
    i to j. Final ans will table[0][n-1]
  */
  for (int m = 0; m < n; m++)
  {
    for (int i = 0, j = m; i < n; i++, j++)
    {
      if (j <= i + 1)
      {
        dp[i][j] = 0.0;
      }
      else
      {
        dp[i][j] = Max;
        for (int k = i + 1; k < j; k++)
        {
          double ans = dp[i][k] + dp[k][j] + cost(points, i, j, k);
          dp[i][j] = min(dp[i][j], ans);
        }
      }
    }
  }
  return dp[0][n - 1];
}

int main()
{
  srand(time(0));
  double minCostDP;
  double minCost;
  FILE *fout = fopen("ObservationDPcpp.csv", "w");
  FILE *fp = fopen("Results_dp.txt", "w");
  
  fprintf(fout, "Vertices,Avg.Time Taken,Avg.Time Taken DP\n");
  for (int i = 3; i <= MAX; i++)
  {
    float time_sum = 0;
    float time_sumDP = 0;
    printf("\nNumber of Vertices:%d\n", i);
    for (int j = 0; j < TOTAL; j++)
    {
      Polygon poly;
      do
      {
        poly = generatePolygon(i);
      } while (!isConvex(poly));
      Vertex points[i];
      fprintf(fp, "Number of Vertices: %d\n", poly.total_vertices);
      for (int k = 0; k < poly.total_vertices; k++)
      {
        points[k].x = poly.vertices[k].x;
        points[k].y = poly.vertices[k].y;
        fprintf(fp, "%d,%d\n", poly.vertices[k].x, poly.vertices[k].y);
      }

      float start = clock();
      minCostDP = minTriangulationCostDP(points, i);
      float end = clock();
      float time_reqDP = (end - start) * 1000 / CLOCKS_PER_SEC;

      start = clock();
      minCost = minTriangulationCost(points, 0, i - 1);
      end = clock();

      fprintf(fp, "Cost of Triangulation from DP is %f\n", minCostDP);
      fprintf(fp, "Cost of Triangulation is %f\n", minCost);
      
      float time_req = (end - start) * 1000 / CLOCKS_PER_SEC;

      printf("Time taken for round %d is %0.4fms for BruteForce and %0.4fms for DP\n", j + 1, time_req, time_reqDP);
      time_sum += time_req;
      time_sumDP += time_reqDP;
      fprintf(fp, "\n");
    }
    fprintf(fout, "%d,%0.4f,%0.4f\n", i, time_sum / TOTAL, time_sumDP / TOTAL);
  }
  fclose(fp);
  fclose(fout);
  return 0;
}