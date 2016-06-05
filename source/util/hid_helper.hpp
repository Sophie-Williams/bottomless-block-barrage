#ifndef HID_HELPER_HPP
#define HID_HELPER_HPP

#include <3ds.h>
#include <map>

struct KeyRepeatItem
{
    int key;
    u64 frame = 0;
    int step = 0;
};

struct KeyRepeatStore
{
    KeyRepeatItem& get(int key);
    u64* get_frame(int key);
    int* get_step(int key);
    std::map<int, KeyRepeatItem> key_repeat_store_map;
};

bool hidKeyRepeat(int key, u64* old_time, unsigned int repeat_ms);
bool hidKeyRepeat(KeyRepeatItem& kri, unsigned int repeat_ms);
bool hidKeyRepeatQuick(int key, u64* old_time, int* step, unsigned int repeat_ms, int triggers_until_quick, unsigned int repeat_quick_ms);
bool hidKeyRepeatQuick(KeyRepeatItem& kri, unsigned int repeat_ms, int triggers_until_quick, unsigned int repeat_quick_ms);

#endif
