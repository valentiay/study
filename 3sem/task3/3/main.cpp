#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <algorithm>

const double E = 0.00001;

/*====== Vector2 ======*/

class Vector2 {
public:
    Vector2(double x, double y);


    Vector2(const Vector2 & other)             = default;
    Vector2 & operator=(const Vector2 & other) = default;
    Vector2(Vector2 && other)                  = default;
    Vector2 & operator=(Vector2 && other)      = default;
    ~Vector2()                                 = default;

    Vector2 operator-();

    friend Vector2 operator*(double c, const Vector2 & v);

    friend Vector2 operator+(const Vector2 & v1, const Vector2 & v2);
    friend Vector2 operator-(const Vector2 & v1, const Vector2 & v2);

    double norm();

    friend double dot(const Vector2 & v1, const Vector2 & v2);

    friend std::ostream & operator<<(std::ostream & os, const Vector2 & vector);

    double x;
    double y;
};

/*============*/

bool lessPolarAngle(Vector2 v1, Vector2 v2) {
    if (v1.x > 0 && v2.x < 0) {
        return true;
    }
    if (v1.x < 0 && v2.x > 0) {
        return false;
    }

    double tg1 = v1.y / v1.x;
    double tg2 = v2.y / v2.x;

    if (fabs(v1.x) < E) {
        if (v1.y > 0) {
            return v2.x < 0;
        } else {
            return false;
        }
    }
    if (fabs(v2.x) < E) {
        if (v2.y > 0) {
            return v1.x > 0;
        } else {
            return fabs(tg1 - tg2) > E;
        }
    }

    if (fabs(tg1 - tg2) < E) {
        return false;
    }
    return tg1 < tg2;
}

std::vector<Vector2> minkowskiSum(std::vector<Vector2> & polygon1, std::vector<Vector2> & polygon2) {
    polygon1.push_back(polygon1[0]);
    polygon2.push_back(polygon2[0]);

    std::vector<Vector2> result;
    size_t i = 0, j = 0;
    while (i < polygon1.size() - 1 && j < polygon2.size() - 1) {
        result.push_back(polygon1[i] + polygon2[j]);
        if (lessPolarAngle(polygon1[i + 1] - polygon1[i], polygon2[j + 1] - polygon2[j])) {
            i++;
        } else if (lessPolarAngle(polygon2[j + 1] - polygon2[j], polygon1[i + 1] - polygon1[i])) {
            j++;
        } else {
            i++;
            j++;
        }
    }
    return result;
}

/*============*/

bool differentSigns(double a, double b) {
    return a < 0 && b > 0 || a > 0 && b < 0;
}

bool containsZero(const std::vector<Vector2> & polygon) {
    std::vector<double> x0;

    if (differentSigns(polygon[0].y, polygon[polygon.size() - 1].y)) {
        x0.push_back((polygon[0].x + polygon[polygon.size() - 1].x) / 2);
    }

    for (size_t i = 0; i < polygon.size() - 1; i++) {
        if (differentSigns(polygon[i].y, polygon[i + 1].y)) {
            x0.push_back((polygon[i].x + polygon[i + 1].x) / 2);
        }
    }

    for (auto i : polygon) {
        if (fabs(i.y) < E) {
            x0.push_back(i.x);
        }
    }

    if (x0.empty()) {
        return false;
    }

    double min = x0[0];
    double max = x0[0];
    for (double x: x0) {
        if (x < min) {
            min = x;
        }
        if (x > max) {
            max = x;
        }
    }

    return fabs(max) < E || fabs(min) < E || differentSigns(max, min);
}

/*============*/

std::vector<Vector2> normalize(const std::vector<Vector2> & v) {
    std::vector<Vector2> r;

    size_t min = 0;
    for (size_t i = 1; i < v.size(); i++) {
        if (v[i].x < v[min].x || fabs(v[i].x - v[min].x) < E && v[i].y < v[min].y) {
            min = i;
        }
    }

    for (size_t i = 0; i < v.size(); i++) {
        r.push_back(v[(i + min + 1) % v.size()]);
    }

    std::reverse(r.begin(), r.end());
    return r;
}

/*============*/

int main() {
    size_t n, m;
    std::vector<Vector2> polygon1;
    std::vector<Vector2> polygon2;
    std::cin >> n;
    for (size_t i = 0; i < n; i++) {
        double x, y;
        std::cin >> x >> y;
        polygon1.emplace_back(x, y);
    }
    std::cin >> m;
    for (size_t i = 0; i < m; i++) {
        double x, y;
        std::cin >> x >> y;
        polygon2.emplace_back(-x, -y);
    }

    polygon1 = normalize(polygon1);
    polygon2 = normalize(polygon2);

    std::vector<Vector2> msum = minkowskiSum(polygon1, polygon2);
    if (containsZero(msum)) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }

    return 0;
}


/*====== Vector2 ======*/

Vector2::Vector2(double x, double y)
        : x(x)
        , y(y) {}



Vector2 Vector2::operator-() {
    return Vector2(-x, -y);
}



Vector2 operator+(const Vector2 & v1, const Vector2 & v2) {
    return Vector2(v1.x + v2.x, v1.y + v2.y);
}



Vector2 operator-(const Vector2 & v1, const Vector2 & v2) {
    return Vector2(v1.x - v2.x, v1.y - v2.y);
}


double Vector2::norm() {
    return sqrt(dot(*this, *this));
}



double dot(const Vector2 & v1, const Vector2 & v2) {
    return v1.x * v2.x + v1.y * v2.y;
}



std::ostream & operator<<(std::ostream & os, const Vector2 & vector) {
    os << std::setprecision(5) << "Vector2(" << vector.x << ", " << vector.y << ") ";
    return os ;
}

Vector2 operator*(double c, const Vector2 & v) {
    return Vector2(c * v.x, c * v.y);
}