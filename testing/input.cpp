#include "input.hpp"

void InputManager::update(const Input& input)
{
    last = current;
    current = input;
}

bool InputManager::trigger(unsigned int key)
{
    return current.on(key) && last.off(key);
}

bool InputManager::held(unsigned int key)
{
    return current.on(key) && last.on(key);
}

bool InputManager::released(unsigned int key)
{
    return current.off(key) && last.on(key);
}

bool InputManager::triggered_only(unsigned int key)
{
    return current.only(key) && last.off(key);
}

bool InputManager::held_only(unsigned int key)
{
    return current.only(key) && last.only(key);
}

bool InputManager::released_only(unsigned int key)
{
    return current.off(key) && last.only(key);
}

bool InputManager::repeat(unsigned int key, unsigned int repeat_frames)
{
    if (current.off(key))
    {
        repeat_map[key] = 0;
        return false;
    }

    return repeat_map[key]++ % repeat_frames == 0;
}

bool InputManager::repeat_quick(unsigned int key, unsigned int repeat_frames, unsigned int quick_delay, unsigned int quick_repeat)
{
    if (current.off(key))
    {
        repeat_map[key] = 0;
        return false;
    }

    if (repeat_map[key] < quick_delay + 1)
        return repeat(key, repeat_frames);

    return repeat(key, quick_repeat);
}
