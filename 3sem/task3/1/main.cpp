#include <iostream>
#include <iomanip>
#include <cmath>

const double E = 0.00000001;

/*====== Vector3 ======*/

struct Vector3 {
    Vector3(double x, double y, double z);
    
    Vector3 operator-();

    friend Vector3 operator*(double c, const Vector3 & v);
    friend Vector3 operator+(const Vector3 & v1, const Vector3 & v2);
    friend Vector3 operator-(const Vector3 & v1, const Vector3 & v2);

    double norm();

    friend double dot(const Vector3 & v1, const Vector3 & v2);

    friend std::ostream & operator<<(std::ostream & os, const Vector3 & vector);

    double x;
    double y;
    double z;
};

/*====== Segment3 ======*/

struct Segment3 {
    Vector3 from;
    Vector3 to;

    Segment3(const Vector3 & from, const Vector3 & to);
};

/*============*/

double dabs(double n) {
    return (n > 0 ? n: -n);
}

double segmentToSegmentDist(Segment3 S1, Segment3 S2) {
    Vector3 u = S1.to - S1.from;
    Vector3 v = S2.to - S2.from;
    Vector3 w = S1.from - S2.from;
    double uu = dot(u, u);
    double uv = dot(u, v);
    double vv = dot(v, v);
    double uw = dot(u, w);
    double vw = dot(v, w);
    double D = uu*vv - uv*uv;
    double s, sN, sD = D;
    double t, tN, tD = D;

    if (D <= E) {
        sN = 0.0;
        sD = 1.0;
        tN = vw;
        tD = vv;
    } else {
        sN = (uv*vw - vv*uw);
        tN = (uu*vw - uv*uw);
        if (sN <= 0.0) {
            sN = 0.0;
            tN = vw;
            tD = vv;
        } else if (sN >= sD) {
            sN = sD;
            tN = vw + uv;
            tD = vv;
        }
    }

    if (tN <= 0.0) {
        tN = 0.0;
        if (-uw <= 0.0) {
            sN = 0.0;
        } else if (-uw >= uu) {
            sN = sD;
        } else {
            sN = -uw;
            sD = uu;
        }
    } else if (tN >= tD) {
        tN = tD;
        if ((-uw + uv) <= 0.0) {
            sN = 0;
        } else if ((-uw + uv) >= uu) {
            sN = sD;
        } else {
            sN = (-uw +  uv);
            sD = uu;
        }
    }
    s = (dabs(sN) <= E ? 0.0 : sN / sD);
    t = (dabs(tN) <= E ? 0.0 : tN / tD);

    Vector3   dP = w + (s * u) - (t * v);

    return dP.norm();
}

/*====== ======*/

int main() {
    double x1, y1, z1, x2, y2, z2;
    double x3, y3, z3, x4, y4, z4;
    std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
    std::cin >> x3 >> y3 >> z3 >> x4 >> y4 >> z4;

    printf("%.7f", segmentToSegmentDist(Segment3(Vector3(x1, y1, z1), Vector3(x2, y2, z2)),
                                        Segment3(Vector3(x3, y3, z3), Vector3(x4, y4, z4))));
}


/*====== Vector3 ======*/

Vector3::Vector3(double x, double y, double z)
        : x(x)
        , y(y)
        , z(z){}



Vector3 Vector3::operator-() {
    return Vector3(-x, -y, -z);
}



Vector3 operator+(const Vector3 & v1, const Vector3 & v2) {
    return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}



Vector3 operator-(const Vector3 & v1, const Vector3 & v2) {
    return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}



double Vector3::norm() {
    return sqrt(dot(*this, *this));
}



double dot(const Vector3 & v1, const Vector3 & v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}



std::ostream & operator<<(std::ostream & os, const Vector3 & vector) {
    os << std::setprecision(5) << "Vector3(" << vector.x << ", " << vector.y << ", " << vector.z << ") ";
    return os ;
}

Vector3 operator*(double c, const Vector3 & v) {
    return Vector3(c * v.x, c * v.y, c * v.z);
}

Vector3 operator/(double c, const Vector3 & v) {
    return Vector3(c * v.x, c * v.y, c * v.z);
}


/*====== Segment3 ======*/

Segment3::Segment3(const Vector3 & from, const Vector3 & to)
        : from(from)
        , to(to) {}
