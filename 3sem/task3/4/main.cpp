#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <set>


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

    friend bool operator<(const Point& p1, const Point& p2) {
        return p1.x < p2.x;
    }

    friend Point operator-(const Point& p1, const Point& p2) {
        return Point(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
    }

    friend double vec(const Point &p1, const Point &p2) {
        return p1.x * p2.y - p1.y * p2.x;
    }
};



struct Face {
    int points[3];

    Face(int first, int second, int third)
            : points({first, second, third}) {}

    friend bool operator<(const Face& f1, const Face& f2) {
        if (f1.points[0] < f2.points[0]) {
            return true;
        } else if (f1.points[0] > f2.points[0]) {
            return false;
        } else {
            if (f1.points[1] < f2.points[1]) {
                return true;
            } else if (f1.points[1] > f2.points[1]) {
                return false;
            } else {
                return f1.points[2] < f2.points[2];
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
    for (Event* event : events) {
        Face current(event->prev->id, event->id, event->next->id);
        if (!event->performEvent()) {
            std::swap(current.points[0], current.points[1]);
        }
        result.push_back(current);
    }
    for (Point& p : points) {
        p.next = nullptr;
        p.prev = nullptr;
        p.z = -p.z;
    }
    events = buildDownHull(points, 0, n);
    for (Event* event : events) {
        Face current(event->prev->id, event->id, event->next->id);
        if (event->performEvent()) {
            std::swap(current.points[0], current.points[1]);
        }
        result.push_back(current);
    }
    return result;
}



bool isAcceptable(const Point &a, const Point &b, const Point &c) {
    return vec(b - a, c - b) > 0;
}



typedef std::pair<int, int> Edge;


double countAvgVoronoiEdgeNum(std::vector<Point> & sites) {
    std::vector<Face> dimentional_hull = buildConvexHull(sites);
    std::set<Edge> edges;
    std::vector<int> edges_number(sites.size());
    std::vector<bool> is_in_planar_hull(sites.size(), false);

    for (Face& face : dimentional_hull) {
        for (int j = 0; j < 3; j++) {
            Edge edge(face.points[j], face.points[(j + 1) % 3]);
            if (edge.first > edge.second) {
                std::swap(edge.first, edge.second);
            }
            edges.insert(edge);
        }
    }
    for (const Edge& edge : edges) {
        edges_number[edge.first]++;
        edges_number[edge.second]++;
    }

    std::sort(sites.begin(), sites.end());
    std::vector<Point> planar_hull;
    for (const Point& site : sites) {
        while (planar_hull.size() >= 2) {
            if (isAcceptable(planar_hull[planar_hull.size() - 2], planar_hull.back(), site)) {
                break;
            }
            planar_hull.pop_back();
        }
        planar_hull.push_back(site);
    }
    for (int i = sites.size() - 2, bottom = planar_hull.size(); i >= 0; --i) {
        while (static_cast<int>(planar_hull.size()) > bottom) {
            if (isAcceptable(planar_hull[planar_hull.size() - 2], planar_hull.back(), sites[i])) {
                break;
            }
            planar_hull.pop_back();
        }
        planar_hull.push_back(sites[i]);
    }
    for (Point& i : planar_hull) {
        is_in_planar_hull[i.id] = true;
    }

    int delaunay_inner_pts = 0;
    int total_degree = 0;
    for (int i = 0; i < sites.size(); i++) {
        if (!is_in_planar_hull[i]) {
            total_degree += edges_number[i];
            delaunay_inner_pts++;
        }
    }

    if (delaunay_inner_pts == 0) {
        return 0.0;
    } else {
        return static_cast<double>(total_degree) / delaunay_inner_pts;
    }
}



int main() {
    freopen("input.txt", "r", stdin);
    std::vector<Point> points;
    double x, y;

    for (int i = 0; std::cin >> y >> x; i++) {
        Point p(x, y, x * x + y * y, i);
        points.push_back(p);
    }

    std::cout << countAvgVoronoiEdgeNum(points) << "\n";

    return 0;
}