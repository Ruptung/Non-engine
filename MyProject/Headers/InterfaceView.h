#pragma once

#include <cstddef>

#include "Storage.h"

// Storage<T> 를 인터페이스 I 의 눈으로 보는 창문.
// World 가 저장소를 처음 만들 때, T 가 I 를 상속했으면 하나 만들어서 명단에 등록한다.
template <typename I>
class IView {
public:
    virtual ~IView() = default;

    virtual size_t Size() const = 0;
    virtual I &At(size_t i) = 0;                // dense 순회용
    virtual Entity OwnerAt(size_t i) const = 0;
    virtual I *Find(Entity e) = 0;              // 엔티티 하나 조회용 (없으면 nullptr)
};

template <typename I, typename T>
class View : public IView<I> {
public:
    View(Storage<T> &storage) : storage(storage) {}

    size_t Size() const override { return storage.Owners().size(); }
    I &At(size_t i) override { return storage.All()[i]; }      // T& -> I& 업캐스트
    Entity OwnerAt(size_t i) const override { return storage.Owners()[i]; }
    I *Find(Entity e) override { return storage.Has(e) ? storage.Get(e) : nullptr; }

private:
    Storage<T> &storage;
};
