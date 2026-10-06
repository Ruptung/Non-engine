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
        TypeDependency[get_typeIndex<Renderable>()] = {get_typeIndex<Transform>()};
    }

    Entity CreateEntity() {
        return entityCounter++;
    }

    template<typename T>
    void AddComponent(Entity e, T t) {
        assert(CheckDependency<T>(e));

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

            i = TypeStorage.insert({get_typeIndex<T>(), std::move(s)}).first;
        }

        return *static_cast<Storage<T>*>(i->second.get());
    }


private:
    template<typename T>
    std::unordered_map<std::type_index, std::unique_ptr<IStorage>>::iterator get_iter() {
        std::type_index idx = get_typeIndex<T>();
        auto i = TypeStorage.find(idx);
        return i;
    }

    template<typename T>
    std::type_index get_typeIndex() {
        return std::type_index(typeid(T));
    }

    template<typename T>
    bool CheckDependency(Entity e) {
        auto dependency_list = TypeDependency.find(get_typeIndex<T>());

        if (dependency_list == TypeDependency.end()) return true;

        for (std::type_index dependency : dependency_list->second) {
            auto pair = TypeStorage.find(dependency);

            // Has Type List.
            if (pair == TypeStorage.end()) { return false; }

            // Has Type in Entity.
            if (!pair->second->Has(e)) { return false; }
        }

        return true;
    }

    Entity entityCounter;

    std::unordered_map<std::type_index, std::unique_ptr<IStorage>> TypeStorage;

    std::unordered_map<std::type_index, std::vector<std::type_index>> TypeDependency;
};