#pragma once
#include <typeindex>
#include <unordered_map>
#include <memory>
#include <cassert>

#include "Storage.h"
#include "Headers/Object.h"

class World {
public:
    World()
    : entityCounter(0) {
    }

    Entity CreateEntity() {
        return entityCounter++;
    }

    template<typename T>
    void AddComponent(Entity e, T t) {
        GetStorage<T>().Add(e, t);
    }

    template<typename T>
    T &GetComponent(Entity e) {
        Storage<T> &s = GetStorage<T>();
        assert(s.Has(e));

        return s.Get(e);
    }

    void DestroyEntity(Entity e) {
        for (auto& [key, storage] : TypeStorage) {
            if (storage->Has(e)) {
                storage->Remove(e);
            }
        }
    }

    template<typename T>
    Storage<T> &GetStorage() {
        auto i = get_iter<T>();

        if (i == TypeStorage.end()) {
            std::unique_ptr<Storage<T>> s = std::make_unique<Storage<T>>();

            i = TypeStorage.insert({std::type_index(typeid(T)), std::move(s)}).first;
        }

        return *static_cast<Storage<T>*>(i->second.get());
    }


private:
    template<typename T>
    std::unordered_map<std::type_index, std::unique_ptr<IStorage>>::iterator get_iter() {
        std::type_index idx = std::type_index(typeid(T));
        auto i = TypeStorage.find(idx);
        return i;
    }

    Entity entityCounter;

    std::unordered_map<std::type_index, std::unique_ptr<IStorage>> TypeStorage;
};