//
// Created by pierre on 10/04/2020.
//

#include <cstdint>

class VMMemory;

class AggregatedTypeIterator {
private:
    virtual int elementType() = 0;
public:
    virtual ~AggregatedTypeIterator() = default;
    virtual int32_t operator* () = 0;
    virtual AggregatedTypeIterator& operator++ () = 0;
    virtual bool operator== (const AggregatedTypeIterator& other) = 0;
    virtual bool operator!= (const AggregatedTypeIterator& other) = 0;
    virtual bool hasNext() = 0;
    virtual bool isAggregatedType() = 0;
};

class ArrayIterator: public AggregatedTypeIterator {
private:
    unsigned arrayDSTOCEntry;
    VMMemory* memory;
    int i;

    int elementType() override;

public:
    ArrayIterator(VMMemory* mem, unsigned entry);

    int32_t operator* () override;
    AggregatedTypeIterator& operator++() override {i++; return *this; }

    bool operator== (const AggregatedTypeIterator& other) override { return false; }
    bool operator== (const ArrayIterator& other) { return other.arrayDSTOCEntry == arrayDSTOCEntry && other.i == i; }

    bool operator!= (const AggregatedTypeIterator& other) override { return false; }
    bool operator!= (const ArrayIterator& other) { return other.arrayDSTOCEntry != arrayDSTOCEntry || other.i != i; }

    bool hasNext() override;
    bool isAggregatedType() override;
};

class ClusterIterator: public AggregatedTypeIterator {
private:
    unsigned clusterDSTOCEntry;
    unsigned size;
    VMMemory* memory;
    int i;
    int real_i;

    int elementType() override;

public:
    ClusterIterator(VMMemory* mem, unsigned entry);

    int32_t operator* () override;
    AggregatedTypeIterator& operator++() override;

    bool operator== (const AggregatedTypeIterator& other) override { return false; }
    bool operator== (const ClusterIterator& other) { return other.clusterDSTOCEntry == clusterDSTOCEntry && other.i == i; }

    bool operator!= (const AggregatedTypeIterator& other) override { return false; }
    bool operator!= (const ClusterIterator& other) { return other.clusterDSTOCEntry != clusterDSTOCEntry || other.i != i; }

    bool hasNext() override;
    bool isAggregatedType() override;
};

AggregatedTypeIterator* getAggregatedIterator(VMMemory*mem, unsigned memLoc);
