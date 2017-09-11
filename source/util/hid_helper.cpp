#include "hid_helper.hpp"

KeyRepeatItem& KeyRepeatStore::get(u32 key)
{
    if (key_repeat_store_map.find(key) == key_repeat_store_map.end())
        key_repeat_store_map[key].key = key;

    return key_repeat_store_map[key];
}

u64& KeyRepeatStore::get_frame(u32 key)
{
    return get(key).frame;
}

int& KeyRepeatStore::get_step(u32 key)
{
    return get(key).step;
}

bool hidKeyRepeat(KeyRepeatItem& kri, u32 repeat_ms, u32 fake_held)
{
    return hidKeyRepeat(kri.key, kri.frame, repeat_ms, fake_held);
}

bool hidKeyRepeatQuick(KeyRepeatItem& kri, u32 repeat_ms, int triggers_until_quick, u32 repeat_quick_ms, u32 fake_held)
{
    return hidKeyRepeatQuick(kri.key, kri.frame, kri.step, repeat_ms, triggers_until_quick, repeat_quick_ms, fake_held);
}

bool hidKeyRepeat(u32 key, u64& old_time, u32 repeat_ms, u32 fake_held)
{
    u32 held = (fake_held == KEY_SENTINEL) ? hidKeysHeld() : fake_held;
    if ((held & key) == 0)
    {
        old_time = 0;
        return false;
    }

    if (osGetTime() - old_time > repeat_ms)
    {
        old_time = osGetTime();
        return held & key;
    }

    return false;
}

bool hidKeyRepeatQuick(u32 key, u64& old_time, int& step, u32 repeat_ms, int triggers_until_quick, u32 repeat_quick_ms, u32 fake_held)
{
    u32 held = (fake_held == KEY_SENTINEL) ? hidKeysHeld() : fake_held;
    if ((held & key) == 0)
    {
        step = 0;
        old_time = 0;
        return false;
    }

    if (step < triggers_until_quick + 1)
    {
        if (hidKeyRepeat(key, old_time, repeat_ms, held))
        {
            step += 1;
            return true;
        }
    }
    else
    {
        return hidKeyRepeat(key, old_time, repeat_quick_ms, held);
    }

    return false;
}
