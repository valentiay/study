#include <list>
#include <vector>
#include <unordered_map>

/******************************************************************************/

typedef unsigned char uc;
typedef unsigned long long ull;

using std::vector;
using std::list;
using std::unordered_map;
using std::pair;
using std::make_pair;

const ull GOAL = 1147797409030816545;

/******************************************************************************/

struct State {
    inline State();

    inline State(const State &state);

    inline State &operator=(const State &state);

    // Return achievable states
    // Also update h, and depth
    inline State up() const;

    inline State down() const;

    inline State left() const;

    inline State right() const;

    // Returns heuristic of current node ("modified" manhattan distance)
    int heuristic() const;

    // Returns true if goal may be achieved from the current node
    bool isSolvable() const;

    // Sets plate
    inline void setAt(int place, uc value);

    // Gets plate
    inline uc getAt(int place) const;

    friend bool operator<(const State &L, const State &R) { return true; }

    // Stores info about state. 4 bits per plate
    ull data;
    // Stores position of empty plate
    int nullPos;
    // Stores length of the way to current state
    long depth;
    // Stores heuristic of current node
    int h;
};

// Type for pair in queue
typedef pair<int, State> q_t;

/******************************************************************************/

const ull AntiMasks[] = {
        0xFFFFFFFFFFFFFFF0, 0xFFFFFFFFFFFFFF0F, 0xFFFFFFFFFFFFF0FF,
        0xFFFFFFFFFFFF0FFF, 0xFFFFFFFFFFF0FFFF, 0xFFFFFFFFFF0FFFFF,
        0xFFFFFFFFF0FFFFFF, 0xFFFFFFFF0FFFFFFF, 0xFFFFFFF0FFFFFFFF,
        0xFFFFFF0FFFFFFFFF, 0xFFFFF0FFFFFFFFFF, 0xFFFF0FFFFFFFFFFF,
        0xFFF0FFFFFFFFFFFF, 0xFF0FFFFFFFFFFFFF, 0xF0FFFFFFFFFFFFFF,
        0x0FFFFFFFFFFFFFFF
};

const ull Masks[] = {
        0x000000000000000F, 0x00000000000000F0, 0x0000000000000F00,
        0x000000000000F000, 0x00000000000F0000, 0x0000000000F00000,
        0x000000000F000000, 0x00000000F0000000, 0x0000000F00000000,
        0x000000F000000000, 0x00000F0000000000, 0x0000F00000000000,
        0x000F000000000000, 0x00F0000000000000, 0x0F00000000000000,
        0xF000000000000000
};

/******************************************************************************/

class priority_queue {
public:
    priority_queue();

    ~priority_queue();

    q_t pop();

    void emplace(q_t val);

private:
    void siftUp(int index);

    void siftDown(int index);

    q_t *buffer_;
    int defaultSize_;
    int bufferSize_;
    int size_;
};

/******************************************************************************/

inline void AStar(const State &from, list<char> &ans) {
    // Priority queue for A*, stores pair
    // <f(state) = path(state) + heuristic(state), state>
    // States in queue are not being updated!
    // This algorithm doesn't find shortest solution with admissible heuristic!
    priority_queue q;
    q.emplace(make_pair(0, from));

    // Stores State.data as key and pair
    // <move(char), parent state(State.data)> as value
    unordered_map<ull, pair<char, ull>> parents;
    parents.emplace(from.data, make_pair(0, 0));

    // State to be visited
    State v;
    State state;
    while (true) {
        // Taking state from queue
        v = q.pop().second;
        // Visiting every possible state
        if (v.nullPos >= 4) {
            state = v.up();
            // Checking is state was not visited
            if (parents.find(state.data) == parents.end()) {
                // Inserting it into parents set
                parents.emplace(state.data, make_pair('D', v.data));
                // Stop search if goal is achieved
                if (state.data == GOAL)
                    break;
                // Inserting into queue
                q.emplace(make_pair(state.depth + state.h, state));
            }
        }
        // Same actions
        if (v.nullPos <= 11) {
            state = v.down();
            if (parents.find(state.data) == parents.end()) {
                parents.emplace(state.data, make_pair('U', v.data));
                if (state.data == GOAL)
                    break;
                q.emplace(make_pair(state.depth + state.h, state));
            }
        }
        if (v.nullPos % 4 != 3) {
            state = v.right();
            if (parents.find(state.data) == parents.end()) {
                parents.emplace(state.data, make_pair('L', v.data));
                if (state.data == GOAL)
                    break;
                q.emplace(make_pair(state.depth + state.h, state));
            }
        }
        if (v.nullPos % 4 != 0) {
            state = v.left();
            if (parents.find(state.data) == parents.end()) {
                parents.emplace(state.data, make_pair('R', v.data));
                if (state.data == GOAL)
                    break;
                q.emplace(make_pair(state.depth + state.h, state));
            }
        }
    }
    // Recovering solution
    ull u = GOAL;
    pair<char, ull> curMove;
    while (u != from.data) {
        curMove = parents[u];
        ans.push_front(curMove.first);
        u = curMove.second;
    }
}

/******************************************************************************/

int main() {
    // Init state
    State from;
    for (int i = 0; i < 16; i++) {
        int x;
        scanf("%d", &x);
        if (x == 0)
            from.nullPos = i;
        from.setAt(i, static_cast<uc>(x));
    }
    // Setting depth and heuristic
    from.depth = 0;
    from.h = from.heuristic();
    list<char> ans;
    if (from.data == GOAL) {
        printf("%d", 0);
    } else if (from.isSolvable()) {
        AStar(from, ans);
        printf("%lu\n", ans.size());
        for (list<char>::iterator i = ans.begin(); i != ans.end(); i++)
            printf("%c", *i);
    } else {
        printf("%d", -1);
    }
    return 0;
}

/****************************STATE*********************************************/

State::State() :
        data(0) {}


State::State(const State &state) :
        data(state.data),
        nullPos(state.nullPos),
        depth(state.depth),
        h(state.h) {}


void State::setAt(int place, uc value)
{
    data = (data & AntiMasks[place]) |
           (static_cast<ull>(value) << (place << 2));
}


uc State::getAt(int place) const
{
    return static_cast<uc>((data & Masks[place]) >> (place << 2));
}


State State::up() const
{
    State result(*this);

    int trg = nullPos - 4;
    uc c = getAt(trg);
    // Update heuristic
    result.h -= 10 * (abs((trg) % 4 - (c - 1) % 4)
                      + abs(trg / 4 - (c - 1) / 4));
    result.setAt(trg, 0);
    result.setAt(nullPos, c);
    // Update heuristic
    result.h += 10 * (abs((nullPos) % 4 - (c - 1) % 4)
                      + abs(nullPos / 4 - (c - 1) / 4));
    result.nullPos = trg;
    result.depth++;

    return result;
}


State State::down() const
{
    State result(*this);

    int trg = nullPos + 4;
    uc c = getAt(trg);
    result.h -= 10 * (abs((trg) % 4 - (c - 1) % 4)
                      + abs(trg / 4 - (c - 1) / 4));
    result.setAt(trg, 0);
    result.setAt(nullPos, c);
    result.h += 10 * (abs((nullPos) % 4 - (c - 1) % 4)
                      + abs(nullPos / 4 - (c - 1) / 4));
    result.nullPos = trg;
    result.depth++;

    return result;
}


State State::left() const
{
    State result(*this);

    int trg = nullPos - 1;
    uc c = getAt(trg);
    result.h -= 10 * (abs((trg) % 4 - (c - 1) % 4)
                      + abs(trg / 4 - (c - 1) / 4));
    result.setAt(trg, 0);
    result.setAt(nullPos, c);
    result.h += 10 * (abs((nullPos) % 4 - (c - 1) % 4)
                      + abs(nullPos / 4 - (c - 1) / 4));
    result.nullPos = trg;
    result.depth++;

    return result;
}


State State::right() const
{
    State result(*this);

    int trg = nullPos + 1;
    uc c = getAt(trg);
    result.h -= 10 * (abs((trg) % 4 - (c - 1) % 4)
                      + abs(trg / 4 - (c - 1) / 4));
    result.setAt(trg, 0);
    result.setAt(nullPos, c);
    result.h += 10 * (abs((nullPos) % 4 - (c - 1) % 4)
                      + abs(nullPos / 4 - (c - 1) / 4));
    result.nullPos = trg;
    result.depth++;

    return result;
}


int State::heuristic() const
{
    int heuristic = 0;
    for (int i = 0; i < 16; i++) {
        if (getAt(i) == 0)
            continue;
        heuristic += 10 * (abs(i % 4 - (getAt(i) - 1) % 4)
                           + abs(i / 4 - (getAt(i) - 1) / 4));
    }
    return heuristic;
}


bool State::isSolvable() const
{
    int inversions = 0;
    for (int i = 0; i < 16; i++)
        for (int j = i + 1; j < 16; j++)
            if (getAt(i) != 0 && getAt(j) != 0 && getAt(i) > getAt(j))
                inversions++;
    return (inversions % 2 == 0) == (nullPos / 4 % 2 == 1);
}


State &State::operator=(const State &state)
{
    data = state.data;
    nullPos = state.nullPos;
    depth = state.depth;
    h = state.h;
    return *this;
}

/****************************PRIORITY_QUEUE************************************/

priority_queue::priority_queue() :
        defaultSize_(16),
        bufferSize_(defaultSize_),
        size_(0)
{
    buffer_ = new q_t[bufferSize_];
}


priority_queue::~priority_queue()
{
    delete[] buffer_;
}


void priority_queue::emplace(q_t val)
{
    if (size_ + 1 > bufferSize_) {
        q_t *tmp = new q_t[bufferSize_ * 2];
        for (int i = 0; i < bufferSize_; i++)
            tmp[i] = buffer_[i];
        delete[] buffer_;
        buffer_ = tmp;
        bufferSize_ *= 2;
    }
    buffer_[size_] = val;
    size_++;
    siftUp(size_ - 1);
}


q_t priority_queue::pop()
{
    q_t max = buffer_[0];
    buffer_[0] = buffer_[size_ - 1];
    size_--;
    if (size_)
        siftDown(0);
    return max;
}


void priority_queue::siftUp(int index)
{
    while (index && buffer_[index] < buffer_[(index - 1) / 2]) {
        q_t tmp = buffer_[index];
        buffer_[index] = buffer_[(index - 1) / 2];
        buffer_[(index - 1) / 2] = tmp;
        index = (index - 1) / 2;
    }
}


void priority_queue::siftDown(int index)
{
    int max = index;
    if (index * 2 + 1 < size_
        && buffer_[index * 2 + 1] < buffer_[max])
        max = index * 2 + 1;
    if (index * 2 + 2 < size_
        && buffer_[index * 2 + 2] < buffer_[max])
        max = index * 2 + 2;
    if (max != index) {
        q_t tmp = buffer_[index];
        buffer_[index] = buffer_[max];
        buffer_[max] = tmp;
        siftDown(max);
    }
}