#include "hid_helper.hpp"

KeyRepeatItem& KeyRepeatStore::get(int key)
{
    if (key_repeat_store_map.find(key) == key_repeat_store_map.end())
    {
        key_repeat_store_map[key].key = key;
    }

    return key_repeat_store_map[key];
}

u64* KeyRepeatStore::get_frame(int key)
{
    return &get(key).frame;
}

int* KeyRepeatStore::get_step(int key)
{
    return &get(key).step;
}

bool hidKeyRepeat(KeyRepeatItem& kri, unsigned int repeat_ms)
{
    return hidKeyRepeat(kri.key, &kri.frame, repeat_ms);
}

bool hidKeyRepeatQuick(KeyRepeatItem& kri, unsigned int repeat_ms, int triggers_until_quick, unsigned int repeat_quick_ms)
{
    return hidKeyRepeatQuick(kri.key, &kri.frame, &kri.step, repeat_ms, triggers_until_quick, repeat_quick_ms);
}

bool hidKeyRepeat(int key, u64* old_time, unsigned int repeat_ms)
{
    u32 held = hidKeysHeld();
    if ((held & key) == 0)
    {
        *old_time = 0;
        return false;
    }

    if (osGetTime() - *old_time > repeat_ms)
    {
        *old_time = osGetTime();
        return held & key;
    }

    return false;
}

bool hidKeyRepeatQuick(int key, u64* old_time, int* step, unsigned int repeat_ms, int triggers_until_quick, unsigned int repeat_quick_ms)
{
    u32 held = hidKeysHeld();
    if ((held & key) == 0)
    {
        *step = 0;
        *old_time = 0;
        return false;
    }

    if (*step < triggers_until_quick + 1)
    {
        if (hidKeyRepeat(key, old_time, repeat_ms))
        {
            *step += 1;
            return true;
        }
    }
    else
    {
        return hidKeyRepeat(key, old_time, repeat_quick_ms);
    }

    return false;
}
