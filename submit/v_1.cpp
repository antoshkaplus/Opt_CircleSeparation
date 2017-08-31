#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <map>
#include <functional>

using namespace std;

//ofstream out("/Users/antoshkaplus/Documents/Programming/Contests/TopCoder/CirclesSeparation/Scripts/out.txt");
double step;

const double eps = 1.e-8;

struct Pair {
    double x, y;
    double distance(const Pair& c) const {
        return sqrt((x-c.x)*(x-c.x) + (y-c.y)*(y-c.y));
    }

    double distanceToOrigin() const {
        return sqrt(x*x + y*y);
    }

    Pair normed() {
        double d = distanceToOrigin();
        return x < eps && y < eps ? Pair(x, y) : Pair(x/d, y/d);
    }

    Pair() : x(0), y(0) {}
    Pair(double x, double y) : x(x), y(y) {}

    Pair& operator+=(const Pair& p) {
        x += p.x;
        y += p.y;
        return *this;
    }
};

Pair operator*(double d, const Pair& p) {
    return Pair(d*p.x, d*p.y);
}

Pair operator-(const Pair& p1, const Pair& p2) {
    return Pair(p1.x-p2.x, p1.y-p2.y);
}

Pair operator+(const Pair& p1, const Pair& p2) {
    return Pair(p1.x+p2.x, p1.y+p2.y);
}

bool operator<(const Pair& p1, const Pair& p2) {
    return p1.distanceToOrigin() < p2.distanceToOrigin();
}

bool operator==(const Pair& p1, const Pair& p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

struct Circle {
    double mass, radius;
    Pair curCenter;
    Pair mainCenter;
    Circle(){}
    Circle(double x, double y, double radius, double mass)
        : mass(mass), radius(radius), curCenter(Pair(x, y)), mainCenter(Pair(x, y)) {}
};

void separate(vector<Circle>& circles) {
    vector<Circle>& c = circles;
    int N = (int)circles.size();
    bool noIntersection;
    int k = 0;
    while(true) {
        noIntersection = true;
        vector<vector<int> > intersection(N);
        for (int i = 0; i < N-1; i++) {
            for (int j = i+1; j < N; j++) {
                if (c[i].curCenter.distance(c[j].curCenter) < c[i].radius+c[j].radius) {
                    noIntersection = false;
                    intersection[i].push_back(j);
                    intersection[j].push_back(i);
                    //out << c[i].curCenter.distance(c[j].curCenter) << " , " << c[i].radius+c[j].radius << endl;
                }
            }
        }
        if (noIntersection) break;
        //int minI = -1;
        //Pair minF = Pair(MAXFLOAT, MAXFLOAT);
        vector<Pair> Fs(N);
        for (int i = 0; i < N; i++) {
            vector<int>& v = intersection[i];
            Pair F_out;
            double distToCenter = c[i].curCenter.distance(c[i].mainCenter);
            for (int j = 0; j < v.size(); j++) {
                F_out += c[v[j]].mass /** (c[i].radius + c[v[j]].radius - c[i].curCenter.distance(c[v[j]].curCenter))*/
                         * (c[i].curCenter-c[v[j]].curCenter).normed();
            }
            //Fs[i] = F_out + c[i].mass * distToCenter * (c[i].mainCenter-c[i].curCenter).normed();
            Fs[i] = F_out;
        }
        for (int i = 0; i < N; i++) {
            if (intersection[i].size()) {
                circles[i].curCenter += step * Fs[i];
                /*
                if (!isfinite(circles[i].curCenter.x) || !isfinite(circles[i].curCenter.y)) {
                  out << k << " " << i << "\n";
                  out.flush();
                  throw "omg";
                }
                */
            }
        }
        k++;
    }
}

class CirclesSeparation {
public:
    vector<double> minimumWork(vector<double> x, vector<double> y, vector<double> r, vector<double> m);
};

vector<double> CirclesSeparation::minimumWork(vector<double> x, vector<double> y, vector<double> r, vector<double> m) {
    int N = (int)m.size();
    vector<Circle> circles(N);
    vector<double> result(2*N);
    step = 0.05;
    for (int i = 0; i < N; i++) {
        r[i] += 1.e-6;
        circles[i] = Circle(x[i], y[i], r[i], m[i]);
    }
    separate(circles);
    for (int i = 0; i < N; i++) {
        Pair &cur = circles[i].curCenter;
        result[2*i] = cur.x;
        result[2*i+1] = cur.y;
    }
    return result;
}