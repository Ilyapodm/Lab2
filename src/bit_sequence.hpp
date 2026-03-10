#pragma once

#include "sequence.hpp"
#include "bit.hpp"

class BitSequence {
public:
    // TODO конструктор

    BitSequence* operator&(const BitSequence& other) const;
    BitSequence* operator|(const BitSequence& other) const;

private:
    Sequence<Bit> *sequence;
};