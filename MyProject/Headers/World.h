#pragma once
#include <typeindex>
#include <unordered_map>
#include <memory>
#include <cassert>
#include <type_traits>
#include <vector>

#include "Storage.h"
#include "InterfaceView.h"
#include "Headers/Object.h"
#include "Headers/Interfaces/ICollide.h"
#include "Headers/Interfaces/IScript.h"

class World {
public:
    World()
    : entityCounter(0) {
        TypeDependency[get_typeIndex<Renderable>()] = {get_typeIndex<Transform>()};
    }

    Entity CreateEntity() {
        //reuse array
        if (!entity_freed.empty()) {
            uint16_t id = entity_freed.back();
            entity_freed.pop_back();
            return Entity{id, entity_generation[id]};
        }
        //extend array
        else {
            entity_generation.push_back(0);
            return Entity{entityCounter, entity_generation[entityCounter++]};
        }
    }

    void DestroyEntity(Entity e) {
        if (!checkGeneration(e)) { return; }

        entity_generation[e.id]++;
        for (auto& [key, storage] : TypeStorage) {
            if (storage->Has(e)) {
                storage->Remove(e);
            }
        }
        entity_freed.push_back(e.id);
    }

    template<typename T>
    void AddComponent(Entity e, T t) {
        if (!checkGeneration(e)) { return; }

        assert(CheckDependency<T>(e));

        GetStorage<T>()->Add(e, t);
    }

    template<typename T>
    T *GetComponent(Entity e) {
        if (!checkGeneration(e)) { return nullptr; }   // 죽은 ID

        Storage<T> *s = GetStorage<T>();
        return s->Has(e) ? s->Get(e) : nullptr;
    }

    template<typename T>
    Storage<T> *GetStorage() {
        auto i = get_iter<T>();

        if (i == TypeStorage.end()) {
            std::unique_ptr<Storage<T>> s = std::make_unique<Storage<T>>();
            Storage<T> &ref = *s;           // move 전에 참조 확보 (힙 객체라 주소 안 바뀜)

            i = TypeStorage.insert({get_typeIndex<T>(), std::move(s)}).first;

            // 저장소가 만들어지는 순간 딱 한 번, 인터페이스별 명단에 등록
            Register<IScript>(ref);
            Register<ICollide>(ref);
        }

        return static_cast<Storage<T>*>(i->second.get());
    }

    // 인터페이스 I 를 구현한 저장소들의 명단
    template<typename I>
    std::vector<std::unique_ptr<IView<I>>> &Implementers() {
        if constexpr (std::is_same_v<I, IScript>)  return scriptViews;
        else                                        return collideViews;
    }

    // 엔티티 e 가 가진 I 전부
    template<typename I>
    std::vector<I*> FindAll(Entity e) {
        std::vector<I*> result;
        for (auto& view : Implementers<I>()) {
            if (I* found = view->Find(e)) {
                result.push_back(found);
            }
        }
        return result;
    }

private:
    bool checkGeneration(Entity e) {
        return (e.id < entity_generation.size() && e.gen == entity_generation[e.id]);
    }

    template<typename I, typename T>
    void Register(Storage<T> &storage) {
        if constexpr (std::is_base_of_v<I, T>) {
            Implementers<I>().push_back(std::make_unique<View<I, T>>(storage));
        }
    }

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

    uint16_t entityCounter;

    std::vector<uint16_t> entity_freed;
    std::vector<uint8_t> entity_generation;

    std::unordered_map<std::type_index, std::unique_ptr<IStorage>> TypeStorage;

    std::unordered_map<std::type_index, std::vector<std::type_index>> TypeDependency;

    std::vector<std::unique_ptr<IView<IScript>>> scriptViews;
    std::vector<std::unique_ptr<IView<ICollide>>> collideViews;
};