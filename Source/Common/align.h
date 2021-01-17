#pragma once

class Align
{
public:
    template <typename T>
    static T Up(T Value, size_t Size)
    {
        return (T)(Value + (Size - Value % Size) % Size);
    }
private:
    Align(void);
    Align(const Align&);
    Align& operator=(const Align&);
};