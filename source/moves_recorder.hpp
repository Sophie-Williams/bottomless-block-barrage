#ifndef MOVE_RECORDER_HPP
#define MOVE_RECORDER_HPP

#include <3ds.h>
#include <string>
#include <vector>

class MovesRecorder
{
public:
    struct Move
    {
        Move(u32 f = 0, u32 t = 0, u32 h = 0) : frame(f), trigger(t), held(h), frames(0) {}
        u32 frame;
        u32 trigger;
        u32 held;
        u32 frames;
    };
    void clear();

    void add(u32 frame, u32 trigger, u32 held);
    bool save(const std::string& specific_filename = "");

    bool load(const std::string& filename);
    void keys(u32& trigger, u32& held);
    void update();


    u32 seed;
private:
    std::vector<Move> moves;
    u32 frame = 0;
    u32 index = 0;
};

#endif
