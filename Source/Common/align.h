#pragma once

template <typename T>
T AlignUp(T Value, size_t Size)
{
    return (T)(Value + (Size - Value % Size) % Size);
}

template <typename T>
bool Is4KBAligned(T Value) 
{
    return (Value & 0xFFF) == 0;
}