/*!
 * \file Scene.hh
 * \brief Definicja klasy Scene
 *
 * Plik zawiera definicję klasy \link Scene Scene \endlink,
 * która implementuje zarządzanie obiektami mobilnymi na scenie.
 */

#ifndef SCENE_HH
#define SCENE_HH

#include "AbstractScene.hh"
#include "AbstractMobileObj.hh"
#include <string>
#include <map>
#include <memory>

/*!
 * \class Scene
 * \brief Implementacja klasy zarządzającej obiektami mobilnymi na scenie.
 *
 * Klasa \link Scene Scene \endlink przechowuje obiekty mobilne (MobileObj)
 * i umożliwia ich wyszukiwanie oraz zarządzanie. Klasa implementuje interfejs
 * \link AbstractScene AbstractScene \endlink.
 */
class Scene : public AbstractScene {
public:
    /*!
     * \brief Konstruktor domyślny klasy Scene.
     *
     * Inicjalizuje pustą scenę.
     */
    Scene();

    /*!
     * \brief Destruktor klasy Scene.
     *
     * Zwalnia wszystkie zasoby związane z przechowywanymi obiektami.
     */
    ~Scene();

    /*!
     * \brief Wyszukuje obiekt mobilny po nazwie.
     *
     * Wyszukuje obiekt mobilny o podanej nazwie na scenie.
     * \param[in] sName - Nazwa obiektu mobilnego do znalezienia.
     * \return Wskaźnik na obiekt mobilny, jeśli istnieje; nullptr w przeciwnym razie.
     */
    AbstractMobileObj* FindMobileObj(const char *sName) override;

    /*!
     * \brief Dodaje obiekt mobilny do sceny.
     *
     * Dodaje nowy obiekt mobilny do kolekcji obiektów na scenie.
     * \param[in] pMobObj - Wskaźnik na obiekt mobilny do dodania.
     */
    void AddMobileObj(AbstractMobileObj *pMobObj) override;

    /*!
     * \brief Pobiera mapę wszystkich obiektów mobilnych na scenie.
     *
     * Zwraca stałą referencję do mapy przechowującej obiekty mobilne
     * identyfikowane przez ich nazwy.
     * \return Stała referencja do mapy obiektów mobilnych.
     */
    const std::map<std::string, std::shared_ptr<AbstractMobileObj>>& GetObjects() const;

    /*!
     * \brief Ustawia nową mapę obiektów mobilnych na scenie.
     *
     * Ustawia kolekcję obiektów mobilnych na scenie, zastępując poprzednie dane.
     * \param[in] objects - Mapa obiektów mobilnych identyfikowanych przez ich nazwy.
     */
    void SetObjects(const std::map<std::string, std::shared_ptr<AbstractMobileObj>>& objects);

private:
    /*!
     * \brief Mapa przechowująca obiekty mobilne na scenie.
     *
     * Klucz mapy to nazwa obiektu mobilnego, a wartość to wskaźnik (smart pointer)
     * na obiekt mobilny.
     */
    std::map<std::string, std::shared_ptr<AbstractMobileObj>> _objects;
};

#endif
