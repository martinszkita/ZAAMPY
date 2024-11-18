#ifndef SCENE_HH
#define SCENE_HH

#include "AbstractScene.hh"
#include "AbstractMobileObj.hh"
#include <string>
#include <map>
#include <memory>

class Scene : public AbstractScene {
public:
    Scene();
    ~Scene();

    AbstractMobileObj* FindMobileObj(const char *sName) override;
    void AddMobileObj(AbstractMobileObj *pMobObj) override;

    // Getter i setter dla _objects
    const std::map<std::string, std::shared_ptr<AbstractMobileObj>>& GetObjects() const;
    void SetObjects(const std::map<std::string, std::shared_ptr<AbstractMobileObj>>& objects);

private:
    std::map<std::string, std::shared_ptr<AbstractMobileObj>> _objects;  // Kolekcja obiektów
};

#endif // SCENE_HH
