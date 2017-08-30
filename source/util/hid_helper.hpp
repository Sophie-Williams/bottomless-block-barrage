#ifndef HID_HELPER_HPP
#define HID_HELPER_HPP

#include <3ds.h>
#include <map>

#define KEY_SENTINEL 0xFFFFFFFFU

struct KeyRepeatItem
{
    int key;
    u64 frame = 0;
    int step = 0;
};

struct KeyRepeatStore
{
    KeyRepeatItem& get(int key);
    u64& get_frame(int key);
    int& get_step(int key);
    std::map<int, KeyRepeatItem> key_repeat_store_map;
};

bool hidKeyRepeat(int key, u64& old_time, unsigned int repeat_ms, u32 fake_held = KEY_SENTINEL);
bool hidKeyRepeat(KeyRepeatItem& kri, unsigned int repeat_ms, u32 fake_held = KEY_SENTINEL);
bool hidKeyRepeatQuick(int key, u64& old_time, int& step, unsigned int repeat_ms, int triggers_until_quick, unsigned int repeat_quick_ms, u32 fake_held = KEY_SENTINEL);
bool hidKeyRepeatQuick(KeyRepeatItem& kri, unsigned int repeat_ms, int triggers_until_quick, unsigned int repeat_quick_ms, u32 fake_held = KEY_SENTINEL);

class InputSource
{
public:
    InputSource() {}
    virtual ~InputSource() {}
    virtual u32 trigger() const;
    virtual u32 held() const;
    virtual bool trigger(u32 key) const;
    virtual bool held(u32 key) const;
    virtual bool repeat(u32 key, u32 repeat_ms);
    virtual bool repeat(u32 key, u32 repeat_ms, u32 fake_held);
    virtual bool repeat_quick(u32 key, u32 repeat_ms, u32 triggers_until_quick, u32 repeat_quick_ms);
    virtual bool repeat_quick(u32 key, u32 repeat_ms, u32 triggers_until_quick, u32 repeat_quick_ms, u32 fake_held);
    virtual void update();
private:
    KeyRepeatStore repeat_store;
    u32 held_;
    u32 trigger_;
};

#endif
