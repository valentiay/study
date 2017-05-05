#include <fstream>
#include <vector>
#include <assert.h>
#include <list>
#include <set>
#include <map>
#include <queue>

/******************************************************************************/

using std::vector;
using std::list;
using std::queue;
using std::map;
using std::ifstream;
using std::ofstream;

typedef unsigned long ul;
typedef unsigned char uc;

const ul GOAL = 2271560481;

/******************************************************************************/

struct State {
public:
    State() {}
    State(const State &state);
    State(const vector<uc> &positions);

    State &operator=(const State &state);

    State up() const;
    State down() const;
    State left() const;
    State right() const;

    // Returns true if goal may be achieved from current state
    bool isSolvable() const;

    void setAt(int place, uc value);
    uc getAt(int place) const;

    // Stores info about plates position. 4 bits per plate
    ul data;
    // Stores position of empty plate
    int nullPos;
    // Stores father of current state
    ul father;
    // Stores move made from father to reach current state
    char move;
};

/******************************************************************************/

const ul Masks[] = {
        0x00000000F, 0x0000000F0, 0x000000F00, 0x00000F000, 0x0000F0000,
        0x000F00000, 0x00F000000, 0x0F0000000, 0xF00000000
};

const ul AntiMasks[] = {
        0xFFFFFFFF0, 0xFFFFFFF0F, 0xFFFFFF0FF, 0xFFFFF0FFF, 0xFFFF0FFFF,
        0xFFF0FFFFF, 0xFF0FFFFFF, 0xF0FFFFFFF, 0x0FFFFFFFF
};

/******************************************************************************/

// Function object. Returns true if puzzle is solved and false otherwise
class BFS {
public:
    bool operator()(const State &start, list<char> &ans) {
        ans.clear();

        // If puzzle can't be solved return false
        if (!start.isSolvable())
            return false;

        // Initialising queue and map of discovered states
        q_.emplace(start.data);
        discovered_[start.data] = start;

        ul v;
        while (true) {
            // Popping state out of queue
            v = q_.front();
            q_.pop();
            if (v == GOAL) {
                // Stop searching if goal was achieved
                break;
            }
            State st = discovered_[v];

            // Expanding any possible state
            if (st.nullPos >= 3)
                visit(st.up());
            if (st.nullPos <= 5)
                visit(st.down());
            if (st.nullPos % 3 != 0)
                visit(st.left());
            if (st.nullPos % 3 != 2)
                visit(st.right());
        }
        // Recovering answer
        while (v != start.data) {
            ans.push_front(discovered_[v].move);
            v = discovered_[v].father;
        }
        return true;
    }

private:
    // Expands node
    void visit(const State &state) {
        if (discovered_.find(state.data) == discovered_.end()) {
            discovered_[state.data] = state;
            q_.emplace(state.data);
        }
    }

    // Queue for BFS
    queue<ul> q_;
    // Stores objects of discovered states
    map<ul, State> discovered_;
};

/******************************************************************************/

int main() {
    ifstream fin("puzzle.in");
    ofstream fout("puzzle.out");
    // Initialising start state
    vector<uc> v(9);
    for (int i = 0; i < 9; i++) {
        int x;
        fin >> x;
        v[i] = (uc) x;
    }
    State start(v);

    // Starting BFS
    list<char> ans;
    BFS solve;
    if(solve(v, ans)) {
        fout << ans.size() << '\n';
        for (list<char>::iterator i = ans.begin(); i != ans.end(); i++)
            fout << *i;
    }
    else{
        fout << -1;
    }

    fin.close();
    fout.close();
}

/****************************STATE*********************************************/

State::State(const State &state) :
        data(state.data),
        nullPos(state.nullPos) {}


State::State(const vector<uc> &positions)
{
    data = 0;
    for (int i = 0; i < 9; i++) {
        setAt(i, positions[i]);
        if (positions[i] == 0)
            nullPos = i;
    }
}


void State::setAt(int place, uc value)
{
    data = (data & AntiMasks[place]) | (static_cast<ul>(value) << (place << 2));
}


uc State::getAt(int place) const
{
    return static_cast<uc>((data & Masks[place]) >> (place << 2));
}


State State::up() const
{
    assert(nullPos >= 3);

    State result(*this);

    uc c = getAt(nullPos - 3);
    result.setAt(nullPos - 3, 0);
    result.setAt(nullPos, c);
    result.nullPos -= 3;
    result.move = 'U';
    result.father = data;

    return result;
}


State State::down() const
{
    assert(nullPos <= 5);

    State result(*this);

    uc c = getAt(nullPos + 3);
    result.setAt(nullPos + 3, 0);
    result.setAt(nullPos, c);
    result.nullPos += 3;
    result.move = 'D';
    result.father = data;

    return result;
}


State State::left() const
{
    assert(nullPos % 3 != 0);

    State result(*this);

    uc c = getAt(nullPos - 1);
    result.setAt(nullPos - 1, 0);
    result.setAt(nullPos, c);
    result.nullPos -= 1;
    result.move = 'L';
    result.father = data;

    return result;
}


State State::right() const
{
    assert(nullPos % 3 != 2);

    State result(*this);

    uc c = getAt(nullPos + 1);
    result.setAt(nullPos + 1, 0);
    result.setAt(nullPos, c);
    result.nullPos += 1;
    result.move = 'R';
    result.father = data;

    return result;
}


bool State::isSolvable() const
{
    int inversions = 0;
    for (int i = 0; i < 9; i++)
        for (int j = i + 1; j < 9; j++)
            if (getAt(i) != 0 && getAt(j) != 0 && getAt(i) > getAt(j))
                inversions++;
    return inversions % 2 == 0;
}


State &State::operator=(const State &state)
{
    data = state.data;
    nullPos = state.nullPos;
    father = state.father;
    move = state.move;
    return *this;
}