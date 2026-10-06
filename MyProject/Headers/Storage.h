#pragma once

#include <cstdint>
#include <vector>
#include <assert.h>


using Entity = uint32_t;

constexpr uint32_t NONE = UINT32_MAX;

class IStorage {
public:
    virtual ~IStorage() = default;

    virtual bool Has(Entity e) const = 0;

    virtual void Remove(Entity e) = 0;
};

template <typename T>
class Storage : public IStorage{
public:
    bool Has(Entity e) const override {
        return e < sparse.size() && sparse[e] != NONE;
    }

    virtual bool Add(Entity e, T value) {
        if (e >= sparse.size()) sparse.resize(e + 1, NONE);

        if (sparse[e] != NONE) return false;

        sparse[e] = dense.size();
        owners.push_back(e);
        dense.push_back(value);
        return true;
    }

    T& Get(Entity e) {
        assert(Has(e));

        return dense[sparse[e]];
    }

    void Remove(Entity e) override {            // swap-remove: 빈칸 없이 O(1)
        assert(Has(e));

        uint32_t i = sparse[e];
        dense[i] = dense.back();
        owners[i] = owners.back();
        sparse[owners[i]] = i;
        dense.pop_back(); owners.pop_back();
        sparse[e] = NONE;
    }

    // 시스템은 dense만 순회하고, 필요하면 owners[i]로 엔티티 ID를 얻음
    std::vector<T>& All() { return dense; }

    const std::vector<Entity>& Owners() const { return owners; }

private:
    std::vector<uint32_t> sparse;  // 엔티티 ID → dense 위치 (빈칸은 NONE)
    std::vector<T> dense;          // 실제 데이터, 빈칸 없음
    std::vector<Entity> owners;    // dense[i]의 주인
};
