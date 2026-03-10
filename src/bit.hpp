#pragma once

class Bit {
public:
    Bit() : value{false} {}
    Bit(bool value) : value{value} {}
    Bit(int value) : value{value != 0} {}

    bool get() const {
        return value;
    }

    // AND
    Bit operator&(const Bit& other) const {
        return Bit(value && other.value);
    }

    // OR
    Bit operator|(const Bit& other) const {
        return Bit(value || other.value);
    }

    // XOR
    Bit operator^(const Bit& other) const {
        return Bit(value != other.value);
    }

    // NOT
    Bit operator~() const {
        return Bit(!value);
    }

    bool operator==(const Bit& other) const {
        return value == other.value;
    }

    bool operator!=(const Bit& other) const {
        return value != other.value;
    }

private:
    bool value;
};