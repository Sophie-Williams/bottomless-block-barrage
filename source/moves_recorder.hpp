#ifndef MOVE_RECORDER_HPP
#define MOVE_RECORDER_HPP

#include <vector>

class MovesRecorder
{
public:
    struct Move
    {
        Move(unsigned int f, unsigned int t, unsigned int h) : frame(f), trigger(t), held(h) {}
        unsigned int frame;
        unsigned int trigger;
        unsigned int held;
    };
    void add(unsigned int frame, unsigned int trigger, unsigned int held);
    void clear();
    void save();
    unsigned int seed;
private:
    std::vector<Move> moves;
};

#endif
