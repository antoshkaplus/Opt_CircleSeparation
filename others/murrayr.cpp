#ifndef MURRAYRS_SOLUTION
#define MURRAYRS_SOLUTION

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <deque>
#include <fstream>
#include <queue>
#include <iostream>
#include <map>
#include <set>
#include <list>
#include <complex>
#include <iomanip>
#include <iterator>

using namespace std;

#ifdef TESTING_AT_HOME
#ifdef _DEBUG
#define TIME_LIMIT 10000
#define MIN_STEPS 0
#else
#define TIME_LIMIT 5000
#define MIN_STEPS 0
#endif
#else
#define TIME_LIMIT 9500
#define MIN_STEPS 0
#endif
#define TIME_LIMIT2 TIME_LIMIT+1000

#ifdef TESTING_AT_HOME
#include <time.h>
int Milliseconds(bool reset = false)
	{
	static clock_t start = clock();
	if (reset)
		start = clock();
	clock_t now = clock();
	int elapsed = now - start;
	elapsed = (elapsed*1000)/(CLOCKS_PER_SEC);
	return elapsed;
	}
#else
#include <sys/time.h>
int getTime()
{
    timeval t;
    gettimeofday(&t, NULL);
    int tm = t.tv_sec;
    tm *= 1000;
    tm += (t.tv_usec/1000);
    return tm;
}
int Milliseconds(bool /*reset*/ = false)
{
    static int start = getTime();
    int now = getTime();
    if (now < start)
        now += 60000;	// to account for minute rollover
    int elapsed = now - start;
    return elapsed;
}
#endif

#define ALL(cont) (cont).begin(), (cont).end()

template<bool Condition> struct STATIC_ASSERTION_FAILURE {enum {ASize=-1};};
template <> struct STATIC_ASSERTION_FAILURE<true> {enum {ASize=1};};
#define STATIC_ASSERT(cond, name) extern int name[STATIC_ASSERTION_FAILURE<cond>::ASize];

bool KeepGoing(int aStart, int aLimit, int aSteps, int aMaxSteps=2000000000, int aMinSteps=MIN_STEPS)
{
    // max takes precedence over min
    if (aSteps >= aMaxSteps)
        return false;
    if (aSteps < aMinSteps)
        return true;
    int now = Milliseconds();
    if (now > aLimit)
        return false;
    int perStep = (now - aStart) / max(aSteps, 1);
//	cerr << "perstep = " << perStep << " est = " << (aStart + perStep * (aSteps + 1)) << endl;
    return (aStart + perStep * (aSteps + 1)) < aLimit;
}

typedef unsigned int uint;
typedef long long ll;

class MTRand
{
    // Mersenne Twistor adapted from wikipedia with improved seed
private:
    enum
    {
        KSize = 624,
    };

    uint iNums[KSize];
    uint iRand;
    uint iSeq;

    uint Next()
    {
//		return rand() ^ (rand() << 11)  ^ (rand() << 22) ;
        iSeq++;
        if (iSeq >= KSize)
        {
            for (int i=0; i<KSize; i++)
            {
                unsigned& num = iNums[i];
                unsigned next = iNums[(i+1)%KSize];
                unsigned next397x = iNums[(i + 397) % KSize];
                iRand =
                    (num & 0x80000000) +
                    (next & 0x7fffffff);
                next397x = next397x ^ ((iRand >> 1) & 0x7fffffff);
                if (iRand&1)
                {
                    num =
                        next397x ^
                        2567483615u;
                }
                else
                {
                    num = next397x;
                }
            }
            iSeq = 0;
        }

        iRand = iNums[iSeq];
        iRand = iRand ^ ((iRand >> 11) & 0x001fffff);
        iRand = iRand ^ ((iRand << 7) & 2636928640u);
        iRand = iRand ^ ((iRand << 15) & 4022730752u);
        iRand = iRand ^ ((iRand >> 18) & 0x00003fff);
        return iRand;
    }

public:
    MTRand()
    { Seed(5489); }

    MTRand(uint aSeed)
    { Seed(aSeed); /*srand(aSeed);*/}

    void Seed(uint aSeed)
    {
        iNums[0] = aSeed;
        for (int i=1; i<KSize; i++)
            iNums[i] = 1812433253 * (iNums[i-1] ^ ((iNums[i-1] >> 30) & 0x3 )) + i;
        iRand = 0;
        iSeq = KSize-1;
    }

    bool Bool()
    {
        return Next()&1;
    }

    uint Int()
    {
        return Next();
    }

    int Int(int aFrom, int aTo)
    {
        if (aFrom < aTo)
            return aFrom + (Next() % (1+aTo-aFrom));
        else
            return aTo + (Next() % (1+aFrom-aTo));
    }

    int Int(int below)
    {
        return Int(0, below-1);
    }

    double Real()
    {
        const static double p32 = pow(0.5, 32);
        const static double p64 = p32 * p32;
        double r = p32 * Next();
        r += p64 * Next();
        return r;
    }

    double Real(double aFrom, double aTo)
    {
        if (aFrom < aTo)
            return aFrom + (Real() * (aTo-aFrom));
        else
            return aTo + (Real() * (aFrom-aTo));
    }

    double Normal(double aMean, double aDev)
    {
        double x,y,r2;
        do  {
            x = Real(-1,1);
            y = Real(-1,1);
            r2 = x*x + y*y;
        } while (r2 > 1.0);
        double u = sqrt(-2 * log(r2) / r2) * x;
        return aMean + u * aDev;
    }

public:	// algorithms
    template<typename It>
    void Shuffle(It aBegin, It aEnd)
    {
        if (aBegin == aEnd)
            return;
        int count = int(aEnd - aBegin);
        for (It p = aBegin; p != aEnd; ++p)
        {
            It other = (aBegin + (Int() % count));
            if (p != other)
                swap(*p, *other);
        }
    }

    // Pick from [aBegin..aEnd) unless empty, then aEnd
    template<typename It>
    It Pick(It aBegin, It aEnd)
    {
        int d = aEnd - aBegin;
        if (d == 0)
            return aEnd;
        return aBegin + Int(d);
    }
};

struct Error : public std::exception
{
    string iErr;
    Error(const string& e)
        : iErr(e)
    {}
    ~Error() throw()
    {}
    const char* what() const throw()
    { return iErr.c_str(); }
};

void Report(const string& msg)
{
#ifdef TESTING_AT_HOME
    throw Error(msg);
#else
    cerr << msg << endl;
#endif
}

template <typename T>
bool Incr(const T& a, const T& b, const T& c)
{ return a <= b && b <= c; }
template <typename T>
bool IncrStrict(const T& a, const T& b, const T& c)
{ return a < b && b < c; }
template <typename T>
bool Between(const T& a, const T& b, const T& c)
{ return min(a,c) <= b && b <= max(a,c); }
template <typename T>
bool BetweenStrict(const T& a, const T& b, const T& c)
{ return min(a,c) < b && b < max(a,c); }

const int KUp = -1;
enum TDir {EUp = 0, ERight, EDown, ELeft, ENumDirs, ENorth = EUp, EEast = ERight, ESouth = EDown, EWest = ELeft, ENoDir = 8};
bool Horizontal(TDir d) { return d == ERight || d == ELeft; }

template <typename T>
struct Point2D
{
    T x;
    T y;
    Point2D()
        : x(), y()
    {}
    template <typename U1, typename U2>
    Point2D(const U1& aX, const U2& aY)
        : x(aX), y(aY)
    {}
    template <typename U>
    Point2D(const Point2D<U> aPoint2D)
        : x(T(aPoint2D.x)), y(T(aPoint2D.y))
    {}
    template <typename U>
    const Point2D& operator=(const Point2D<U> aPoint2D)
    {
        x = T(aPoint2D.x);
        y = T(aPoint2D.y);
        return *this;
    }
    template <typename U>
    Point2D operator+(const Point2D<U>& aPoint2D) const
    {
        return Point2D(x+aPoint2D.x, y+aPoint2D.y);
    }
    template <typename U>
    const Point2D& operator+=(const Point2D<U>& aPoint2D)
    {
        x += aPoint2D.x;
        y += aPoint2D.y;
        return *this;
    }
    Point2D operator-(const Point2D& aPoint2D) const
    {
        return Point2D(x-aPoint2D.x, y-aPoint2D.y);
    }
    const Point2D& operator-=(const Point2D& aPoint2D)
    {
        x -= aPoint2D.x;
        y -= aPoint2D.y;
        return *this;
    }
    bool operator==(const Point2D& aPoint2D) const
    {
        return x == aPoint2D.x && y == aPoint2D.y;
    }
    bool operator!=(const Point2D& aPoint2D) const
    {
        return x != aPoint2D.x || y != aPoint2D.y;
    }
    Point2D operator*(T aFactor) const
    {
        return Point2D(x*aFactor, y*aFactor);
    }
    const Point2D& operator*=(T aFactor)
    {
        x = x*aFactor;
        y = y*aFactor;
        return *this;
    }
    Point2D operator/(T aFactor) const
    {
        return Point2D(x/aFactor, y/aFactor);
    }
    const Point2D& operator/=(T aFactor)
    {
        x = x/aFactor;
        y = y/aFactor;
        return *this;
    }
    Point2D operator-() const
    {
        return Point2D(-x, -y);
    }
    bool operator<(const Point2D& aOther) const
    {
        return (y < aOther.y) || ((y == aOther.y) && (x < aOther.x));
    }
    T Cross(const Point2D& aP) const
    {
        return x * aP.y - y * aP.x;
    }
    T operator*(const Point2D& aP) const
    {
        return Cross(aP);
    }
    T Dot(const Point2D& aP) const
    {
        return x * aP.x + y * aP.y;
    }
    T operator()(const Point2D& aP) const
    {
        return Dot(aP);
    }
    T BoxDist(const Point2D& aPoint2D) const
    {
        return abs(x - aPoint2D.x) + abs(y - aPoint2D.y);
    }
    T SquaredMagnitude() const
    {
        return x*x + y*y;
    }
    double Magnitude() const
    {
        return sqrt(double(SquaredMagnitude()));
    }
    double Dist(const Point2D& aPoint2D) const
    {
        T dx = x - aPoint2D.x;
        T dy = y - aPoint2D.y;
        return sqrt(double(dx*dx + dy*dy));
    }
    double Angle()
    {
        return atan2(double(y), double(x));
    }
    Point2D<double> Unit() const
    {
        double mag = Magnitude();
        if (mag != 0.0)
            return Point2D(x/mag, y/mag);
        else
            return Point2D(0.0,0.0);
    }
    Point2D Rotate(double aAngle) const
    {
        double c = cos(aAngle);
        double s = sin(aAngle);
        return Point2D(T(x*c - y*s), T(x*s + y*c));
    }
    Point2D RotateCW() const
    {
        return Point2D(-y, x);
    }
    Point2D RotateCCW() const
    {
        return Point2D(y, -x);
    }
    T operator[](TDir dir) const
    {
        switch (dir)
        {
            case EUp:
            case EDown:
                return y;
            case ELeft:
            case ERight:
                return x;
            default:
                return T();
        }
    }
    T& operator[](TDir dir)
    {
        switch (dir)
        {
            case EUp:
            case EDown:
                return y;
            case ELeft:
            case ERight:
            default:
                return x;
        }
    }
};

template <typename T>
ostream& operator<<(ostream& aStream, const Point2D<T>& aPoint2D)
{
    aStream << aPoint2D.x << "," << aPoint2D.y;
    return aStream;
}

typedef Point2D<int> Pos;
typedef Point2D<ll> PosLL;
typedef Point2D<double> Point;
typedef Point2D<float> PointF;
template <typename T>
Pos ToPos(const Point2D<T>& p)
{
    return Pos(int(p.x + 0.5), int(p.y + 0.5));
}

const double triEps = 1e-12;
template <typename T>
bool pointInTriangle(const Point2D<T>& p, const Point2D<T>& a, const Point2D<T>& b, const Point2D<T>& c)
{
    double x=(a-b)*(a-p), y=(b-c)*(b-p), z=(c-a)*(c-p);
    return ((x<=triEps && y<=triEps && z<=triEps) || (x>=-triEps && y>=-triEps && z>=-triEps));
}

const Pos KDir[9] = {Pos(0,KUp), Pos(1,0), Pos(0,-KUp), Pos(-1,0), Pos(1,-KUp), Pos(1,KUp), Pos(-1,-KUp), Pos(-1,KUp), Pos(0,0)};
const string KDirName = "URDL";
const string KCompassName = "NESW";
const TDir KOppositeDir[4] = {EDown, ELeft, EUp, ERight};

TDir DirTo(const Pos& aFrom, const Pos& aTo)
{
    int dx = aTo.x - aFrom.x;
    int dy = aTo.y - aFrom.y;
//	if (dx && dy)
//		throw Error("No Direction!");
    if (abs(dy) > abs(dx))
    {
        bool b = dy < 0;
#ifdef TESTING_AT_HOME
#pragma warning(disable:4127)
#endif
        if (KUp < 0)
            b = !b;
#ifdef TESTING_AT_HOME
#pragma warning(default:4127)
#endif
        return b ? EDown : EUp;
    }
    else
        return dx > 0 ? ERight : (dx < 0 ? ELeft : ENoDir);
}

struct TDirs {TDir x, y;};
TDirs DirsTo(const Pos& aFrom, const Pos& aTo)
{
    TDirs dirs = {ENoDir, ENoDir};
    if (aTo.x < aFrom.x)
        dirs.x = ELeft;
    if (aTo.x > aFrom.x)
        dirs.x = ERight;
    if (aTo.y < aFrom.y)
        dirs.y = EUp;
    if (aTo.y > aFrom.y)
        dirs.y = EDown;
#if	KUp == 1
    dirs.second = TDir(EDown-dirs.second);
#endif
    return dirs;
}

template <typename T>
struct Grid
{
    typedef vector<T> Coll;
    typedef typename Coll::iterator iterator;
    typedef typename Coll::const_iterator const_iterator;
    typedef typename Coll::reference Ref;
    typedef typename Coll::const_reference ConstRef;

    Grid()
        : width(0), height(0), bWidth(0), bHeight(0), border(0)
    {}

    Grid(int aWidth, int aHeight, const T& aVal)
    {
        Init(aWidth, aHeight, aVal, 0);
    }

    Grid(int aWidth, int aHeight, int aBorder, const T& aVal)
    {
        Init(aWidth, aHeight, aVal, aBorder);
    }

    Grid(const Grid& aGrid)
    {
        *this = aGrid;
    }

    const Grid& operator=(const Grid& aGrid)
    {
        if (&aGrid == this)
            return *this;
        width = aGrid.width;
        height = aGrid.height;
        bWidth = aGrid.bWidth;
        bHeight = aGrid.bHeight;
        border = aGrid.border;
        iGrid = aGrid.iGrid;
        iBegin = iGrid.begin();
        iBegin = Iter(Pos(border, border));
        return *this;
    }

    void Init(int aWidth, int aHeight, const T& aVal, int aBorder = 0)
    {
        bWidth = aWidth + aBorder*2;
        bHeight = aHeight + aBorder*2;
        width = aWidth;
        height = aHeight;
        border = aBorder;
        iGrid.clear();
        iGrid.resize(bWidth*bHeight, aVal);
        iBegin = iGrid.begin();
        iBegin = Iter(Pos(aBorder, aBorder));
    }

    inline int Index(const Pos& aPos) const
    {
        int base = border * (bWidth + 1);
        int idx = aPos.x + aPos.y*bWidth + base;
#ifdef _DEBUG
        if (idx < 0 || idx > Size(iGrid))	// allow for "end" positions
			Report("Grid bad pos");
#endif
        return idx-base;
    }

    Ref operator[](const Pos& aPos)
    {
        return iBegin[Index(aPos)];
    }

    T operator[](const Pos& aPos) const
    {
        return iBegin[Index(aPos)];
    }

    ConstRef Get(const Pos& aPos) const
    {
        return iBegin[Index(aPos)];
    }

    Ref Get(const Pos& aPos)
    {
        return iBegin[Index(aPos)];
    }

    void Set(const Pos& aPos, const T& v)
    {
        iBegin[Index(aPos)] = v;
    }

    iterator begin()
    {
        return iGrid.begin();
    }

    iterator end()
    {
        return iGrid.end();
    }

    const_iterator begin() const
    {
        return iGrid.begin();
    }

    const_iterator end() const
    {
        return iGrid.end();
    }

    iterator Iter(const Pos& aPos)
    {
        return iBegin+Index(aPos);
    }

    const_iterator Iter(const Pos& aPos) const
    {
        return iBegin+Index(aPos);
    }

    bool IsValid(const Pos& aPos) const
    {
        return 0 <= aPos.x && aPos.x < width && 0 <= aPos.y && aPos.y < height;
    }

    bool IsBorderValid(const Pos& aPos) const
    {
        return -border <= aPos.x && aPos.x < width+border && -border <= aPos.y && aPos.y < height+border;
    }

    void Limit(Pos& aPos) const
    {
        aPos.x = min(max(0, aPos.x), width-1);
        aPos.y = min(max(0, aPos.y), height-1);
    }

    template<typename T2>
    void Swap(Grid<T2>& other)
    {
        iGrid.swap(other.iGrid);
        swap(width, other.width);
        swap(height, other.height);
        swap(bWidth, other.bWidth);
        swap(bHeight, other.bHeight);
        swap(border, other.border);
        swap(iBegin, other.iBegin);
    }

    Pos rndPos(MTRand& rnd) const
    {
        return Pos(rnd.Int(width), rnd.Int(height));
    }

    Ref rndElement(MTRand& rnd)
    {
        return Get(rndPos(rnd));
    }

    ConstRef rndElement(MTRand& rnd) const
    {
        return Get(rndPos(rnd));
    }

    int width;
    int height;
    int bWidth;
    int bHeight;
    int border;
    Coll iGrid;
    typename Coll::iterator iBegin;
};

#define FOR_GRID(var, grid)    for (Pos var(0,0); var.y<grid.height; var.y++) for (var.x=0; var.x<grid.width; var.x++)
#define FOR_SIZE(var, w, h)    for (Pos var(0,0); var.y<(h); var.y++) for (var.x=0; var.x<(w); var.x++)
#define FOR_RECT(var, l, t, r, b)    for (Pos var((l),(t)); var.y<(b); var.y++) for (var.x=(l); var.x<(r); var.x++)

template<typename T>
ostream& operator<<(ostream& aStream, const Grid<T>& aGrid)
{
    for (Pos var(0,0); var.y<aGrid.height; var.y++)
    {
        for (var.x=0; var.x<aGrid.width; var.x++)
        {
            aStream << aGrid[var] << ",";
        }
        aStream << endl;
    }
    return aStream;
}

template<typename T>
ostream& operator<<(ostream& aStream, const vector<T>& aVect)
{
    int s = Size(aVect);
    aStream << "{";
    for (int i=0; i<s; i++)
    {
        if (i>0)
            aStream << ",";
        aStream << aVect[i];
    }
    aStream << "}" << endl;
    return aStream;
}

const double PI = acos(-1.0);
const double PI2 = PI * 2;
const double SQRT2 = sqrt(2.0);

inline double AngleNorm(double a)
{
    if (a < -PI)
    {
        int times = int((a-PI)/PI2);
        a-=times*PI2;
    }
    else if (a > PI)
    {
        int times = int((a+PI)/PI2);
        a-=times*PI2;
    }
    return a;
}
inline double AngleAdd(double a, double b)
{
    return AngleNorm(a+b);
}
inline double AngleSub(double a, double b)
{
    return AngleNorm(a-b);
}

struct OrderPointByAngleToRef
{
    Point iRef;
    OrderPointByAngleToRef(const Point& aRef) : iRef(aRef)
    {}
    bool operator()(const Point& aL, const Point& aR) const
    {
        return (aL - iRef).Angle() > (aR - iRef).Angle();
    }
};

struct OrderPointByDistToRef
{
    Point iRef;
    OrderPointByDistToRef(const Point& aRef) : iRef(aRef)
    {}
    bool operator()(const Point& aL, const Point& aR) const
    {
        return (aL - iRef).SquaredMagnitude() < (aR - iRef).SquaredMagnitude();
    }
};

template <class C>
int Size(const C& c)
{
    return int(c.size());
}

uint Hash(const string& str)
{
    uint hash = 5381;
    for (int i=0; i<Size(str); i++)
        hash = hash * 33 ^ str[i];
    return hash;
}

int bitCount[256] =
    {
        0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
    };

inline int CountBits(int bits)
{
    unsigned char* bytes = (unsigned char*)&bits;
    return bitCount[bytes[0]] + bitCount[bytes[1]] + bitCount[bytes[2]] + bitCount[bytes[3]];
}

template<typename TDATA, typename TSCORE>
class TrackMin
{
public:
    TDATA data;
    TSCORE val;
    TrackMin(const TDATA& startData, const TSCORE& startVal)
        : data(startData), val(startVal)
    {}

    bool update(const TDATA& newData, const TSCORE& newVal)
    {
        if (newVal < val)
        {
            data = newData;
            val = newVal;
            return true;
        }
        return false;
    }
};

template<typename TDATA, typename TSCORE>
class TrackMax
{
public:
    TDATA data;
    TSCORE val;
    TrackMax(const TDATA& startData, const TSCORE& startVal)
        : data(startData), val(startVal)
    {}

    bool update(const TDATA& newData, const TSCORE& newVal)
    {
        if (newVal > val)
        {
            data = newData;
            val = newVal;
            return true;
        }
        return false;
    }
};

template<typename TDATA, typename TSCORE>
class TrackMaxEq
{
public:
    TDATA data;
    TSCORE val;
    TrackMaxEq(const TDATA& startData, const TSCORE& startVal)
        : data(startData), val(startVal)
    {}

    bool update(const TDATA& newData, const TSCORE& newVal)
    {
        if (newVal >= val)
        {
            data = newData;
            val = newVal;
            return true;
        }
        return false;
    }
};

typedef long long LL;

template <typename T, typename U>
U& other(const T& from, U& a, U& b)
{
    if (from == a)
        return b;
    else if (from == b)
        return a;
    else
        Report("neither");
    return a;
}

#ifdef TESTING_AT_HOME
inline bool isnan(double x)
	{
	return x != x;
	}
#endif

////////////////////////////////////////////////////////
////////////////  Solution starts here  ////////////////
////////////////////////////////////////////////////////

struct Circle
{
    Point p, o;
    double r, m;
    vector<int> lines;
    int id;

    Circle() : r(0), m(0) {}
    Circle(double x, double y, double aR, double aM, int aId) : p(x,y), o(x,y), r(aR), m(aM), id(aId) {}

    void addLine(int l)
    {
        lines.push_back(l);
    }

    void removeLine(int l)
    {
        int nl = Size(lines);
        for (int i=0; i<nl; i++)
        {
            int& x = lines[i];
            if (x == l)
            {
                swap(x, lines.back());
                lines.pop_back();
                break;
            }
        }
    }
};

const int None = -1;

struct Line
{
    int id;
    int points[2];
    int triangles[2];
    double angle;
    double length;
    double gap;
    bool free;
    double maxCircles[2];
    int lastBad[2];

    bool isEdge() const
    {
        return triangles[0] == None || triangles[1] == None;
    }

    void setPoints(int a, int b)
    {
        points[0] = a;
        points[1] = b;
    }

    void addTriangle(int t)
    {
        if (triangles[0] == None)
            triangles[0] = t;
        else if (triangles[1] == None)
            triangles[1] = t;
        else
            Report("line has too many triangles");
    }

    void replaceTriangle(int f, int t)
    {
        if (triangles[0] == f)
            triangles[0] = t;
        else if (triangles[1] == f)
            triangles[1] = t;
        else
            Report("line doesn't have that triangle to replace");
    }
};

struct Triangle
{
    int id;
    bool free;
    int points[3];
    int lines[3];

    void setPoints(int a, int b, int c)
    {
        points[0] = a;
        points[1] = b;
        points[2] = c;
    }

    void setLines(int a, int b, int c)
    {
        lines[0] = a;
        lines[1] = b;
        lines[2] = c;
    }
};

struct ByMass
{
    const vector<Circle>& circles;
    ByMass(const vector<Circle>& circles) : circles(circles) {}
    bool operator()(int a, int b) const
    {
        return circles[a].m > circles[b].m;
    }
};

struct ByMassOverRad
{
    const vector<Circle>& circles;
    ByMassOverRad(const vector<Circle>& circles) : circles(circles) {}
    bool operator()(int a, int b) const
    {
        return circles[a].m/circles[a].r > circles[b].m/circles[b].r;
    }
};

struct ByMassNegRad
{
    const vector<Circle>& circles;
    double f;
    ByMassNegRad(const vector<Circle>& circles, double f) : circles(circles), f(f) {}
    bool operator()(int a, int b) const
    {
        return circles[a].m-f*circles[a].r > circles[b].m-f*circles[b].r;
    }
};

double eps = 1e-9;
double eps2 = 2e-9;
double eps4 = 4e-9;

class CirclesSeparation2;
void showTriangles(CirclesSeparation2& soln, int id = -1, int highlight = -1);

int iterCount = 0;
const int gridMag=3;
const int gridLim=6;

class CirclesSeparation2
{
public:
    int N;
    vector<Circle> circles;
    vector<Line> lines;
    vector<Triangle> triangles;
    list<int> freeLines;
    list<int> freeTriangles;
    MTRand rnd;
    vector<int> placed;
    deque<int> delaunayQueue;
    int lastCollision;
    vector<double> scores;
    Grid<vector<int> > circlePoss;
    int saIter;

    CirclesSeparation2()
        : lastCollision(0)
    {
        Milliseconds(true);
    }

    vector<double> minimumWork(const vector<double>& x, const vector<double>& y, const vector<double>& r, const vector<double>& m)
    {
        N = Size(x);
        saIter = 0;
        scores.resize(N);
        circles.reserve(N);
        for (int i=0; i<N; i++)
            circles.push_back(Circle(x[i], y[i], r[i], m[i], i));
        initPlaceGrids();
        moveCircles();
        return formatPoss();
    }

    void moveCircles()
    {
        vector<int> order(N);
        for (int i=0; i<N; i++)
            order[i] = i;
        sort(ALL(order), ByMass(circles));
        vector<int> bestOrder = order;
        placeOrdered(order);
        double sc = score();
        TrackMin<vector<Circle>, double> best(circles, sc);
        for (double f = 0.25; f <= 4; f *= 2)
        {
            sort(ALL(order), ByMassNegRad(circles, f));
            placeOrdered(order);
            sc = score();
            if (best.update(circles, sc))
                bestOrder = order;
        }
        sort(ALL(order), ByMassOverRad(circles));
        placeOrdered(order);
        sc = score();
        if (best.update(circles, sc))
            bestOrder = order;
        order = bestOrder;
        set<uint> tabu;
        while (Milliseconds() < TIME_LIMIT)
        {
            mutateOrder(order);
            uint h = hashOrder(order);
            if (tabu.find(h) != tabu.end())
            {
                if (rnd.Int(2))
                    order = bestOrder;
                continue;
            }
            tabu.insert(h);
            placeOrdered(order);
            sc = score();
            if (best.update(circles, sc))
            {
                bestOrder = order;
//				cerr << "+";
            }
            else
            {
                if (rnd.Int(2))
                    order = bestOrder;
//				cerr << "-";
            }
            iterCount++;
            saIter++;
        }
        circles = best.data;
//		squash();
    }

/*	void squash()
		{
		while (Milliseconds() < TIME_LIMIT2)
			{
			ResolveForces();
			TryMove();
			}
		}

	void ResolveForces()
		{
		// set up gravity
		for (int i=0; i<N; i++)
			{
			Circle& circle = circles[i];
			circle.pressure = Point();
			circle.gravity = circle.o - circle.p;
			double l = g.Magnitude();
			if (l < eps)
				continue;
			circle.gravity *= circle.m / l;
			}

		// propagate pressure till balanced (somewhat)
		for (int p=0; p<10; p++)
			{
			for (int i=0; i<N; i++)
				{
				Circle& circle = circles[i];
				Point net = circle.gravity + circle.pressure;
				int nl = Size(circle.lines);
				for (int l=0; l<nl; l++)
					{
					const Line& line = lines[circle.lines[l]];
					if (line.gap > 0.01)
						continue;
					int o = other(circle.id, line.points[0], line.points[1]);
					}
				}
			}
		}

	void TryMove()
		{
		}
		*/
    uint hashOrder(const vector<int>& order)
    {
        uint h = 0;
        for (int i=0; i<N; i++)
        {
            h = h * 726162741 ^ order[i];
        }
        return h;
    }

    double score()
    {
        double tot = 0;
        for (int i=0; i<N; i++)
        {
            const Circle& c = circles[i];
            double sc = (c.p - c.o).Magnitude() * c.m;
            scores[i] = sc;
            tot += sc;
        }
        return tot;
    }

    void mutateOrder(vector<int>& order)
    {
        switch (rnd.Int(saIter < 100 ? 1 : 2))
        {
            case 0:
                mutateOrderBest(order);
                break;
            case 1:
                mutateOrderSwap(order);
                break;
        }
    }

    void mutateOrderSwap(vector<int>& order)
    {
        swap(order[rnd.Int(N)], order[rnd.Int(N)]);
    }

    void mutateOrderBest(vector<int>& order)
    {
        double sc = accumulate(ALL(scores), 0.0);
        double p = rnd.Real() * sc;
        int c;
        for (c=0; c<N; c++)
        {
            p -= scores[order[c]];
            if (p<0)
                break;
        }
        if (c > 0 && c < N)
        {
            int o = rnd.Int(c);
            int cc = order[c];
            for (int i = c; i>o; i--)
                order[i] = order[i-1];
            order[o] = cc;
        }
    }

    void placeOrdered(const vector<int>& order)
    {
        resetTriangulation();
        resetPlaceGrids();
        lastCollision = None;
        placed.clear();
        for (int i=0; i<N; i++)
        {
            int o = order[i];
            place(o);
            addToCircleGrid(circles[o]);
//			consistencyCheck();
            placed.push_back(o);
            addToTriangulation(o);
//			consistencyCheck();
        }
    }

    void place(int co)
    {
        Circle& c = circles[co];

        if (tryPlace(c, c.o))
            return;

        // try moving away from the collision
        if (lastCollision != None)
        {
            const Circle& b = circles[lastCollision];
            Point v = c.p - b.p;
            v = v * (c.r + b.r + eps2) / v.Magnitude();
            if (tryPlace(c, b.p + v))
                return;
        }

        // otherwise try between other circles
        int nl = Size(lines);
        TrackMin<Point, double> best(Point(), 10000);

        Point ps[2];
        for (int l=0; l<nl; l++)
        {
            Line& line = lines[l];
            if (line.free)
                continue;
            if (line.maxCircles[0] < c.r && line.maxCircles[1] < c.r)
                continue;
            touchingCirclePoss(l, co, ps[0], ps[1]);
            for (int i=0; i<2; i++)
            {
                if (line.maxCircles[i] < c.r)
                    continue;
                const Point& pt = ps[i];
                if (isnan(pt.x))
                    continue;
                c.p = pt;
                double d = (c.p - c.o).SquaredMagnitude();
                if (d < best.val)
                {
                    if (!tooCloseFromLine(line, c, 0))
                        best.update(pt, d);
                    else
                        line.maxCircles[i] = c.r;
                }
            }
        }

        c.p = best.data;
    }

    void initPlaceGrids()
    {
        int n = 2 * gridMag * gridLim;
        circlePoss.Init(n, n, vector<int>());
    }

    void resetPlaceGrids()
    {
        FOR_GRID(p, circlePoss)
                circlePoss[p].clear();
    }

    void gridLimits(const Point& p, int& x, int& y)
    {
        double dx = (p.x + gridLim) * gridMag;
        x = int(dx);
        double dy = (p.y + gridLim) * gridMag;
        y = int(dy);
    }

    void gridLimits(const Circle& circle, int& xl, int& xh, int& yl, int& yh)
    {
        Point r(circle.r, circle.r);
        Point l(circle.p - r);
        Point h(circle.p + r);
        gridLimits(l, xl, yl);
        gridLimits(h, xh, yh);
    }

    void addToCircleGrid(const Circle& circle)
    {
        int xl, xh, yl, yh;
        gridLimits(circle, xl, xh, yl, yh);
        Pos p;
        for (p.y=yl; p.y<=yh; p.y++)
        {
            for (p.x=xl; p.x<=xh; p.x++)
            {
                circlePoss[p].push_back(circle.id);
            }
        }
    }

    bool tooCloseFromLine(Line& line, const Circle& circle, int s)
    {
        if (line.lastBad[s] != None && tooClose(circle, circles[line.lastBad[s]]))
            return true;

        int xl, xh, yl, yh;
        gridLimits(circle, xl, xh, yl, yh);
        Pos p;
        for (p.y=yl; p.y<=yh; p.y++)
        {
            for (p.x=xl; p.x<=xh; p.x++)
            {
                const vector<int>& cs = circlePoss[p];
                int nc = Size(cs);
                for (int i=0; i<nc; i++)
                {
                    int c = cs[i];
                    if (tooClose(circle, circles[c]))
                        return true;
                }
            }
        }

        return false;
    }

    bool tryPlace(Circle& c, const Point& p)
    {
        c.p = p;
        int np = Size(placed);
        if (lastCollision != None && tooClose(c, circles[lastCollision]))
            return false;
        int xl, xh, yl, yh;
        gridLimits(c, xl, xh, yl, yh);
        Pos pos;
        for (pos.y=yl; pos.y<=yh; pos.y++)
        {
            for (pos.x=xl; pos.x<=xh; pos.x++)
            {
                const vector<int>& cs = circlePoss[pos];
                int nc = Size(cs);
                for (int i=0; i<nc; i++)
                {
                    int ci = cs[i];
                    if (tooClose(c, circles[ci]))
                    {
                        lastCollision = ci;
                        return false;
                    }
                }
            }
        }
        return true;
    }

    bool tooClose(const Circle& c1, const Circle& c2)
    {
        double r = c1.r + c2.r + eps;
        double d = (c1.p - c2.p).SquaredMagnitude();
        return (d < r*r);
    }

    vector<double> formatPoss()
    {
        vector<double> work;
        for (int i=0; i<N; i++)
        {
            work.push_back(circles[i].p.x);
            work.push_back(circles[i].p.y);
        }
        return work;
    }

    void resetTriangulation()
    {
        lines.clear();
        freeLines.clear();
        triangles.clear();
        freeTriangles.clear();
        for (int i=0; i<N; i++)
            circles[i].lines.clear();
    }

    void addToTriangulation(int circle)
    {
//		if (circle == 314)
//			showTriangles(*this, 314, 314);
        int t = findTriangle(circle);
        if (t == None)
            addOutsideTriangles(circle);
        else
            addToTriangle(circle, t);
//		consistencyCheck();
        processDelaunay();
    }

    void processDelaunay()
    {
        while (!delaunayQueue.empty())
        {
            int t = delaunayQueue.front();
            delaunayQueue.pop_front();
            Triangle& tri = triangles[t];
            if (tri.free)
                continue;
            for (int l=0; l<3; l++)
            {
                if (delaunayCheckLine(tri.lines[l]))
                {
//					consistencyCheck();
                    break;
                }
            }
        }
    }

    void consistencyCheck()
    {
        for (int i=0; i<Size(triangles); i++)
        {
            const Triangle& tri = triangles[i];
            if (tri.free)
                continue;
            if (tri.lines[0] == tri.lines[1] || tri.lines[0] == tri.lines[2] || tri.lines[1] == tri.lines[2])
                Report("duplicate line");
            if (tri.points[0] == tri.points[1] || tri.points[0] == tri.points[2] || tri.points[1] == tri.points[2])
                Report("duplicate point");
            for (int j=0; j<3; j++)
            {
                const Line& line = lines[tri.lines[j]];
                if (line.points[0] == line.points[1])
                    Report("double ended line");
                int a = tri.points[j];
                int b = tri.points[(j+1)%3];
                if (other(a, line.points[0], line.points[1]) != b)
                    Report("line point mismatch");
                if (other(i, line.triangles[0], line.triangles[1]) == i)
                    Report("duplicate triangle");
            }
        }
    }

    bool delaunayCheckLine(int l)
    {
        Line& line = lines[l];
        int oldt1 = line.triangles[0];
        int oldt2 = line.triangles[1];
        if (oldt1 == None || oldt2 == None)
            return false;
        int a = line.points[0];
        int b = line.points[1];
        int c = otherPoint(line.triangles[0], a, b);
        int d = otherPoint(line.triangles[1], a, b);
        if (angle(c, a, b) + angle(d, a, b) > PI)
        {
            // swap the line
            // remove the old line and triangles
            deleteTriangle(oldt1);
            deleteTriangle(oldt2);
            deleteLine(l);
            // create the new lines and triangles
            Line& lineCD = newLine(c, d);
            int t1 = newTriangle();
            int t2 = newTriangle();
            int lac = findLineBetween(a, c);
            int lad = findLineBetween(a, d);
            int lbc = findLineBetween(b, c);
            int lbd = findLineBetween(b, d);
            Triangle& tri1 = triangles[t1];
            Triangle& tri2 = triangles[t2];
            tri1.setPoints(a, c, d);
            tri1.setLines(lac, lineCD.id, lad);
            lines[lac].replaceTriangle(oldt1, t1);
            lines[lad].replaceTriangle(oldt2, t1);
            lineCD.addTriangle(t1);
            tri2.setPoints(b, c, d);
            tri2.setLines(lbc, lineCD.id, lbd);
            lines[lbc].replaceTriangle(oldt1, t2);
            lines[lbd].replaceTriangle(oldt2, t2);
            lineCD.addTriangle(t2);
            delaunayQueue.push_back(t1);
            delaunayQueue.push_back(t2);
            return true;
        }
        return false;
    }

    double angle(int c, int a, int b)
    {
        const Point& pc = circles[c].p;
        const Point& pa = circles[a].p;
        const Point& pb = circles[b].p;
        double a1 = (pa-pc).Angle();
        double a2 = (pb-pc).Angle();
        return abs(AngleSub(a1, a2));
    }

    int otherPoint(int t, int a, int b)
    {
        const Triangle& tri = triangles[t];
        for (int i=0; i<3; i++)
        {
            int c = tri.points[i];
            if (c != a && c != b)
                return c;
        }
        return None;
    }

    void addOutsideTriangles(int circle)
    {
        int nc = Size(placed);
        if (nc == 1)
        {
            // do nothing
        }
        else if (nc == 2)
        {
            // create a line
            newLine(circle, placed[0]);
        }
        else
        {
            // make new outer triangles
            // get all the current outer points
            vector<pair<double, int> > outside;
            Point p = circles[circle].p;
            for (int c=0; c<nc; c++)
            {
                int place = placed[c];
                if (isOuterEdge(place))
                {
                    Point d = circles[place].p - p;
                    double angle = atan2(d.y, d.x);
                    outside.push_back(make_pair(angle, place));
                }
            }
            // sort them, and find the big gap (>= 180)
            sort(ALL(outside));
            int gp = 0;
            int no = Size(outside);
            double angle;
            for (gp = 0; gp < no-1; gp++)
            {
                angle = AngleSub(outside[gp+1].first, outside[gp].first);
                if (angle < 0 || angle > PI-eps)
                    break;
            }
            angle = AngleSub(outside[(gp+1)%no].first, outside[gp].first);
            if (angle >= 0 && angle < PI-eps)
                Report("not outside");
            int start = outside[(gp+1)%no].second;
            int end = outside[gp].second;
            // which way is the near outside?
            int a, b;
            getOutsideNeighbours(start, a, b);
            Point pa = circles[a].p;
            Point pb = circles[b].p;
            Point ps = circles[start].p;
            int next = b;
            if ((pa-ps) * (pb-ps) < 0)
                next = a;
            // add triangles
            bool fan = false;
            while (start != end)
            {
                getOutsideNeighbours(next, a, b);
                int nn = other(start, a, b);
                addTriangle(circle, start, next, fan);
                start = next;
                next = nn;
                fan = true;
            }
        }
    }

    bool isOuterEdge(int circle)
    {
        int a,b;
        getOutsideNeighbours(circle, a, b);
        return a != None;
    }

    void getOutsideNeighbours(int circle, int& n1, int& n2)
    {
        n1 = n2 = None;
        const Circle& c = circles[circle];
        int found = 0;
        int nl = Size(c.lines);
        for (int i=0; i<nl; i++)
        {
            int l = c.lines[i];
            const Line& line = lines[l];
            int n = other(circle, line.points[0], line.points[1]);
            if (lines[l].isEdge())
            {
                found++;
                (found == 1 ? n1 : n2) = n;
            }
        }
        if (found == 1)
        {
            if (nl == 1)
                n2 = n1;
            else
                Report("circle with single edge line");
        }
        else if (found > 2)
            Report("circle with too many edge lines");
    }

    void addTriangle(int a, int b, int c, int fan)
    {
        // add a new triangle, where a is a the new point
        int t = newTriangle();
        Triangle& tri = triangles[t];
        tri.setPoints(a, b, c);
        // add lines
        int lab;
        if (fan)
        {
            lab = findLineBetween(a, b);
            Line& lineAb = lines[lab];
            lineAb.addTriangle(t);
        }
        else
        {
            Line& lineAb = newLine(a, b);
            lineAb.addTriangle(t);
            lab = lineAb.id;
        }
        int lbc = findLineBetween(b, c);
        Line& lineBc = lines[lbc];
        lineBc.addTriangle(t);
        Line& lineCa = newLine(c, a);
        lineCa.addTriangle(t);
        tri.setLines(lab, lbc, lineCa.id);
        delaunayQueue.push_back(t);
    }

    int findLineBetween(int a, int b)
    {
        const vector<int>& ls = circles[a].lines;
        int nl = Size(ls);
        for (int i=0; i<nl; i++)
        {
            int li = ls[i];
            const Line& line = lines[li];
            if (other(a, line.points[0], line.points[1]) == b)
                return li;
        }
        Report("no line between circles");
        return 0;
    }

    void addToTriangle(int circle, int triangle)
    {
        Triangle& tri = triangles[triangle];
        int newLines[3];
        for (int i=0; i<3; i++)
        {
            newLines[i] = newLine(tri.points[i], circle).id;
        }
        int newTris[3];
        for (int i=0; i<3; i++)
        {
            newTris[i] = newTriangle();
            Triangle& tri = triangles[triangle];
            Triangle& newTri = triangles[newTris[i]];
            Line& edgeLine = lines[tri.lines[i]];
            int p0 = edgeLine.points[0];
            int p1 = edgeLine.points[1];
            if (p0 != lines[newLines[i]].points[0])
                swap(p0, p1);
            newTri.setPoints(circle, p0, p1);
            newTri.setLines(newLines[i], tri.lines[i], newLines[(i+1)%3]);
            edgeLine.replaceTriangle(triangle, newTris[i]);
            delaunayQueue.push_back(newTris[i]);
        }
        for (int i=0; i<3; i++)
        {
            Line& line = lines[newLines[i]];
            line.addTriangle(newTris[i]);
            line.addTriangle(newTris[(i+2)%3]);
        }
        deleteTriangle(triangle);
    }

    int findTriangle(int circle)
    {
        int nt = Size(triangles);
        const Point& p = circles[circle].p;
        for (int t=0; t<nt; t++)
        {
            const Triangle& tri = triangles[t];
            if (!tri.free && pointInTriangle(p, circles[tri.points[0]].p, circles[tri.points[1]].p, circles[tri.points[2]].p))
                return t;
        }
        return None;
    }

    void touchingCirclePoss(int line, int circle, Point& p1, Point& p2)
    {
        const Line& ln = lines[line];
        const Circle& ca = circles[ln.points[0]];
        const Circle& cb = circles[ln.points[1]];
        const Circle& cc = circles[circle];
        if (ln.gap > 2*(cc.r+eps2))
        {
            Point v(cb.p - ca.p);
            p1 = ca.p + v * (ca.r + cc.r + eps2) / ln.length;
            p2 = cb.p - v * (cb.r + cc.r + eps2) / ln.length;
        }
        else
        {
            double a = cb.r + cc.r + eps2;
            double b = ca.r + cc.r + eps2;
            double c = ln.length;
            double angle = acos((b*b + c*c - a*a) / (2 * b * c));
            double a1 = AngleAdd(ln.angle, angle);
            p1 = ca.p + Point(b * cos(a1), b * sin(a1));
            double a2 = AngleSub(ln.angle, angle);
            p2 = ca.p + Point(b * cos(a2), b * sin(a2));
        }
    }

    Line& newLine(int a, int b)
    {
        int id = 0;
        if (freeLines.empty())
        {
            lines.push_back(Line());
            Line& l = lines.back();
            l.id = Size(lines)-1;
            id = l.id;
        }
        else
        {
            id = freeLines.front();
            freeLines.pop_front();
        }
        Line& line = lines[id];
        line.setPoints(a, b);
        line.triangles[0] = line.triangles[1] = None;
        Circle& ca = circles[a];
        ca.addLine(id);
        Circle& cb = circles[b];
        cb.addLine(id);
        line.free = false;
        Point v = cb.p - ca.p;
        line.length = v.Magnitude();
        line.gap = line.length - ca.r - cb.r - eps;
        if (line.gap < 0)
            Report("-ve gap");
        line.angle = v.Angle();
        line.maxCircles[0] = 1000;
        line.maxCircles[1] = 1000;
        line.lastBad[0] = None;
        line.lastBad[1] = None;
        return line;
    }

    void deleteLine(int l)
    {
        Line& line = lines[l];
        line.free = true;
        circles[line.points[0]].removeLine(l);
        circles[line.points[1]].removeLine(l);
        freeLines.push_back(l);
    }

    int newTriangle()
    {
        if (freeTriangles.empty())
        {
            triangles.push_back(Triangle());
            Triangle& t = triangles.back();
            t.id = Size(triangles)-1;
            t.free = false;
            return t.id;
        }
        else
        {
            int id = freeTriangles.front();
            freeTriangles.pop_front();
            triangles[id].free = false;
            return id;
        }
    }

    void deleteTriangle(int t)
    {
        triangles[t].free = true;
        freeTriangles.push_back(t);
    }

};

//void xyz();
//void zxy() { xyz(); }

class CirclesSeparation {
public:
    vector<double> minimumWork(vector<double> x, vector<double> y, vector<double> r, vector<double> m);
};

vector<double> CirclesSeparation::minimumWork(vector<double> x, vector<double> y, vector<double> r, vector<double> m) {
    CirclesSeparation2 cs;
    return cs.minimumWork(x, y, r, m);
}

#endif

