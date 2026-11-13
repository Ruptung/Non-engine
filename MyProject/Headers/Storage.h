#pragma once

#include <assert.h>
#include <cstdint>
#include <vector>

struct Entity {
    uint16_t id;
    uint8_t gen;
};

constexpr Entity NO_ENTITY = {UINT16_MAX, 0};   // "엔티티 없음"

constexpr uint16_t NONE = UINT16_MAX;

class IStorage {
public:
    virtual ~IStorage() = default;

    virtual bool Has(Entity e) const = 0;

    virtual void Remove(Entity e) = 0;
};

template <typename T>
class Storage : public IStorage{
private:
    std::vector<uint16_t> sparse;  // 엔티티 ID → dense 위치 (빈칸은 NONE)
    std::vector<T> dense;          // 실제 데이터, 빈칸 없음
    std::vector<Entity> owners;    // dense[i]의 주인
public:
    bool Has(Entity e) const override {
        return (e.id < sparse.size()) && (sparse[e.id] != NONE) && (owners[sparse[e.id]].gen == e.gen);
    }

    virtual bool Add(Entity e, T value) {
        if (e.id >= sparse.size()) sparse.resize(e.id + 1, NONE);

        if (sparse[e.id] != NONE) return false;

        sparse[e.id] = dense.size();
        owners.push_back(e);
        dense.push_back(value);
        return true;
    }

    T* Get(Entity e) {
        assert(Has(e));

        return &dense[sparse[e.id]];
    }

    T* TryGet(Entity e) {
        return Has(e) ? &dense[sparse[e.id]] : nullptr;
    }

    void Remove(Entity e) override {            // swap-remove: 빈칸 없이 O(1)
        if (!Has(e)) return;

        uint32_t i = sparse[e.id];
        dense[i] = dense.back();
        owners[i] = owners.back();
        sparse[owners[i].id] = i;
        dense.pop_back(); owners.pop_back();
        sparse[e.id] = NONE;
    }

    // 시스템은 dense만 순회하고, 필요하면 owners[i]로 엔티티 ID를 얻음
    std::vector<T>& All() { return dense; }

    const std::vector<Entity>& Owners() const { return owners; }
};
