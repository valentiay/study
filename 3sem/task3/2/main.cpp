#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>


double const INFTY = 1e9;



struct Point {
    double x;
    double y;
    double z;
    int id;
    Point *next, *prev;

    Point(double x = 0, double y = 0, double z = 0, int id = -1)
            : x(x), y(y), z(z)
            , id(id)
            , next(nullptr)
            , prev(nullptr) {}

    bool performEvent() {
        if (prev->next != this) {
            prev->next = this;
            next->prev = this;
            return true;
        } else {
            prev->next = next;
            next->prev = prev;
            return false;
        }
    }

    friend bool operator < (const Point& p1, const Point& p2) {
        return p1.x < p2.x;
    }
};



struct Face {
    int first;
    int second;
    int third;

    Face(int first, int second, int third)
            : first(first)
            , second(second)
            , third(third) {}

    void fixOrder() {
        if (second < first && second < third) {
            std::swap(first, second);
            std::swap(second, third);
        } else if (third < first && third < second) {
            std::swap(second, third);
            std::swap(first, second);
        }
    }

    friend bool operator<(const Face& f1, const Face& f2) {
        if (f1.first < f2.first) {
            return true;
        } else if (f1.first > f2.first) {
            return false;
        } else {
            if (f1.second < f2.second) {
                return true;
            } else if (f1.second > f2.second) {
                return false;
            } else {
                return f1.third < f2.third;
            }
        }
    }
};



double sign(const Point* a, const Point* b, const Point* c) {
    if (a == nullptr || b == nullptr || c == nullptr) {
        return INFTY;
    }
    return (b->x - a->x) * (c->y - b->y) - (b->y - a->y) * (c->x - b->x);
}



double time(const Point* a, const Point* b, const Point* c) {
    if (a == nullptr || b == nullptr || c == nullptr) {
        return INFTY;
    }
    return ((b->x - a->x) * (c->z - b->z) - (b->z - a->z) * (c->x - b->x)) / sign(a, b, c);
}



typedef Point Event;



std::vector<Event*> buildDownHull(std::vector<Point>& points, size_t l, size_t r) {
    if (r - l == 1) {
        return std::vector<Event*>();
    }
    size_t m = (l + r) / 2;
    std::vector<Event*> evs[2] = {
            buildDownHull(points, l, m),
            buildDownHull(points, m, r)
    };
    std::vector<Event*> result;

    Point* u = &points[m - 1];
    Point* v = &points[m];
    for (;;) {
        if (sign(u, v, v->next) < 0) {
            v = v->next;
        } else if (sign(u->prev, u, v) < 0) {
            u = u->prev;
        } else {
            break;
        }
    }

    size_t p1 = 0, p2 = 0;
    for (double current_t = -INFTY;;) {
        Point* left = nullptr;
        Point* right = nullptr;
        std::vector<double> next_t(6, INFTY);

        if (p1 < evs[0].size()) {
            left = evs[0][p1];
            next_t[0] = time(left->prev, left, left->next);
        }
        if (p2 < evs[1].size()) {
            right = evs[1][p2];
            next_t[1] = time(right->prev, right, right->next);
        }
        next_t[2] = time(u, v, v->next);
        next_t[3] = time(u, v->prev, v);
        next_t[4] = time(u->prev, u, v);
        next_t[5] = time(u, u->next, v);

        int min_i = -1;
        double min_t = INFTY;
        for (int i = 0; i < 6; i++) {
            if (next_t[i] > current_t && next_t[i] < min_t) {
                min_t = next_t[i];
                min_i = i;
            }
        }
        if (min_i == -1 || min_t >= INFTY) {
            break;
        }

        switch (min_i) {
            case 0:
                if (left->x < u->x) {
                    result.push_back(left);
                }
                left->performEvent();
                p1++;
                break;
            case 1:
                if (right->x > v->x) {
                    result.push_back(right);
                }
                right->performEvent();
                p2++;
                break;
            case 2:
                result.push_back(v);
                v = v->next;
                break;
            case 3:
                v = v->prev;
                result.push_back(v);
                break;
            case 4:
                result.push_back(u);
                u = u->prev;
                break;
            case 5:
                u = u->next;
                result.push_back(u);
                break;
            default:
                break;
        }
        current_t = min_t;
    }
    u->next = v;
    v->prev = u;
    for (int i = static_cast<int>(result.size() - 1); i >= 0; i--) {
        Point* current = result[i];
        if (current->x > u->x && current->x < v->x) {
            u->next = v->prev = current;
            current->prev = u;
            current->next = v;
            if (current->x <= points[m - 1].x) {
                u = current;
            } else {
                v = current;
            }
        } else {
            current->performEvent();
            if (current == u) {
                u = u->prev;
            }
            if (current == v) {
                v = v->next;
            }
        }
    }
    return result;
}



std::vector<Face> buildConvexHull(std::vector<Point> points) {
    size_t n = points.size();
    std::vector<Face> result;
    std::sort(points.begin(), points.end());
    std::vector<Event*> events = buildDownHull(points, 0, n);
    for (Event* e : events) {
        Face current(e->prev->id, e->id, e->next->id);
        if (!e->performEvent()) {
            std::swap(current.first, current.second);
        }
        result.push_back(current);
    }
    for (Point& p : points) {
        p.next = nullptr;
        p.prev = nullptr;
        p.z = -p.z;
    }
    events = buildDownHull(points, 0, n);
    for (Event* e : events) {
        Face current(e->prev->id, e->id, e->next->id);
        if (e->performEvent()) {
            std::swap(current.first, current.second);
        }
        result.push_back(current);
    }
    return result;
}



void rotate(Point& p, double angle) {
    // Y
    double new_x = p.x * cos(angle) + p.z * sin(angle);
    double new_z = -p.x * sin(angle) + p.z * cos(angle);
    p.x = new_x;
    p.z = new_z;

    // X
    new_z = p.z * cos(angle) + p.y * sin(angle);
    double new_y = -p.z * sin(angle) + p.y * cos(angle);
    p.z = new_z;
    p.y = new_y;

    // Z
    new_x = p.x * cos(angle) + p.y * sin(angle);
    new_y = -p.x * sin(angle) + p.y * cos(angle);
    p.x = new_x;
    p.y = new_y;
}


int main() {
    int n;
    std::cin >> n;
    for(size_t i = 0; i < n; i++) {
        int m;
        std::vector<Point> points;

        std::cin >> m;
        for (int j = 0; j < m; j++) {
            int x, y, z;
            std::cin >> x >> y >> z;
            Point p(x, y, z, j);
            rotate(p, 0.01);
            points.push_back(p);
        }

        std::vector<Face> hull = buildConvexHull(points);
        for (Face& f : hull) {
            f.fixOrder();
        }

        std::sort(hull.begin(), hull.end());
        std::cout << hull.size() << "\n";
        for (Face& f : hull) {
            std::cout << 3 << " " << f.first << " " << f.second << " " << f.third << "\n";
        }
    }
    return 0;
}