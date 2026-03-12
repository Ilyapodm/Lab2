#pragma once

#include "array_sequence.hpp"
#include "bit.hpp"

// abstract class for mutable/immutable bit sequence
class BitSequence : public ArraySequence<Bit> {
public:
    BitSequence() : ArraySequence<Bit>() {}
    BitSequence(bool *items, int size) : ArraySequence<Bit>(size){
        for (int i = 0; i < size; i++)
            this->array->set(i, Bit(items[i]));
    }
    BitSequence(const BitSequence &other) : ArraySequence<Bit>(other) {}

    // redefinition with more clear return type
    virtual BitSequence* instance() = 0;              //  pure virtual
    virtual BitSequence* empty_sequence() const = 0;

    // 1. the length of result bit sequence is the minimum of "this" and "other" lengths for all operations 
    // we can't write "operator&", because it means, that we return Bit&, but if we use instance()(Mutable/Immutable), 
    // we can get clone (new object in the heap) in the method, so need to use ptr
    // 2. could use enumerate, but it's not so quick (new, delete...)
    // 3. don't use try-catch, because set can't throw, because = for "Bit" do not work with memory
    BitSequence* bit_and(const BitSequence &other) {
        int result_size = (this->get_size() > other.get_size()) ? other.get_size() : this->get_size();

        BitSequence* inst = this->instance();

        for (int i = 0; i < result_size; i++) 
            inst->array->set(i, inst->get(i) & other.get(i));

        inst->array->resize(result_size);  // change size (minimum of "this" and "other")
        return inst;
    }

    BitSequence* bit_or(const BitSequence &other) {
        int result_size = (this->get_size() > other.get_size()) ? other.get_size() : this->get_size();

        BitSequence* inst = this->instance();

        
        for (int i = 0; i < result_size; i++) 
            inst->array->set(i, inst->get(i) | other.get(i));

        inst->array->resize(result_size);  // change size (minimum of "this" and "other")
        return inst;
    }

    BitSequence* bit_xor(const BitSequence &other) {
        int result_size = (this->get_size() > other.get_size()) ? other.get_size() : this->get_size();

        BitSequence* inst = this->instance();

        for (int i = 0; i < result_size; i++) 
            inst->array->set(i, inst->get(i) ^ other.get(i));

        inst->array->resize(result_size);  // change size (minimum of "this" and "other")
        return inst;
    }

    BitSequence* bit_not() {
        BitSequence* inst = this->instance();

        for (int i = 0; i < inst->get_size(); i++) 
            inst->array->set(i, ~inst->get(i));

        return inst;
    }
};

// MutableArraySequence can use Sequence's interface, so we use it
class MutableBitSequence : public BitSequence {
public:
    MutableBitSequence() : BitSequence() {}
    MutableBitSequence(bool* items, int size) : BitSequence(items, size) {}; 
    MutableBitSequence(const BitSequence &other) : BitSequence(other) {}

protected:
    BitSequence* instance() override {
        return this;
    } 

    BitSequence* empty_sequence() const override {
        return new MutableBitSequence();
    }
};

class ImmutableBitSequence : public BitSequence {
public:
    ImmutableBitSequence() : BitSequence() {}
    ImmutableBitSequence(bool* items, int size) : BitSequence(items, size) {}
    ImmutableBitSequence(const BitSequence &other) : BitSequence(other) {}

protected:
    BitSequence* instance() override {
        return new ImmutableBitSequence(*this);
    } 

    BitSequence* empty_sequence() const override {
        return new ImmutableBitSequence();
    }
};