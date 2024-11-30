#ifndef ABSGTRACTMOBILEOBJ_HH
#define ABSGTRACTMOBILEOBJ_HH


/*!
 * \file 
 * \brief Zawiera definicję klasy abstrakcyjnej AbstractMobileObj
 *
 *  Zawiera definicję klasy abstrakcyjnej AbstractMobileObj.
 *  Wyznacza ona niezbędny interfejs klas pochodnych.
 */

#include "Vector3D.hh"

   /*!
    * \brief Definiuje interfejs dla obiektów mobilnych.
    *
    * Definiuje interfejs dla obiektów mobilnych.
    * Zakładamy, że przód obiektu jest wskazywany przez strzałkę osi OX.
    * Nazwy metod są obowiązujące.
    */
    class AbstractMobileObj {
     public:

       virtual ~AbstractMobileObj() {}
       
        /*!
        * \brief Zmienia współrzędne aktualnej rotacji obiektu.
        *
        * Zmienia współrzędne aktualnej rotacji obiektu.
        * \param[in] rRot - nowa orientacja obiektu. Przyjmuje się,
        *         że współrzędne wyrażone są w stopniach.
        */
       virtual void SetRotXYZ_deg(const Vector3D &rPos) = 0;


      virtual   const Vector3D & GetRotXYZ_deg() const = 0;

       virtual const Vector3D & GetPosition_m() const = 0;
       /*!
        * \brief Zmienia współrzędne aktualnej pozycji obiektu.
        *
        * Zmienia współrzędne aktualnej pozycji obiektu.
        * \param[in] rPos - nowe współrzędne obiektu. Przyjmuje się,
        *         że współrzędne wyrażone są w metrach.
        */
       virtual void SetPosition_m(const Vector3D &rPos) = 0;

       /*!
        * \brief Zmienia nazwę obiektu.
        *
        *  Zmienia nazwę obiektu, która go identyfikuje.
        *  \param[in]  sName - nowa nazwa obiektu.
        */
        virtual void SetName(const char* sName) = 0;
       /*!
        * \brief Udostępnia nazwę obiektu.
        *  Udostępnia nazwę identyfikującą obiekt.
        *  \return Nazwa obiektu.
        */
        virtual const std::string & GetName() const = 0;
        virtual std::string GetStateDesc() const = 0;
    };



#endif
