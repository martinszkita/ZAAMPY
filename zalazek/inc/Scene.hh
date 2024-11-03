#include "AbstractScene.hh"
#include "MobileObj.hh"
#include <string>
#include <map>
#include <memory>  // std::shared_ptr

class Scene : public AbstractScene {
public:
    Scene() = default; // uzyc domyslnej implementacji
    ~Scene() = default;

    // Implementacja metody do znajdowania obiektu mobilnego
    AbstractMobileObj* FindMobileObj(const char *sName) override {
        auto it = _objects.find(sName);
        if (it != _objects.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    // Implementacja metody do dodawania obiektu mobilnego do sceny
    void AddMobileObj(AbstractMobileObj *pMobObj) override {
        _objects[pMobObj->GetName()] = std::shared_ptr<AbstractMobileObj>(pMobObj);
    }

    // Getter dla _objects - zwraca stałą referencję
    const std::map<std::string, std::shared_ptr<AbstractMobileObj>>& GetObjects() const {
        return _objects;
    }

    // Setter dla _objects - przypisuje nową mapę obiektów
    void SetObjects(const std::map<std::string, std::shared_ptr<AbstractMobileObj>>& objects) {
        _objects = objects;
    }

private:
    std::map<std::string, std::shared_ptr<AbstractMobileObj>> _objects;  // Kolekcja obiektów
};
