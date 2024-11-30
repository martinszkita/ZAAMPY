/*!
 * \file MobileObj.hh
 * \brief Definicja klasy MobileObj
 *
 * Plik zawiera definicję klasy \link MobileObj MobileObj \endlink,
 * która modeluje prostopadłościan reprezentujący części robota.
 * Klasa dziedziczy po \link AbstractMobileObj AbstractMobileObj \endlink
 * i implementuje jej interfejs.
 */

#ifndef MOBILEOBJ_HH
#define MOBILEOBJ_HH

#include "AbstractMobileObj.hh"
#include "Vector3D.hh"

/*!
 * \class MobileObj
 * \brief Modeluje prostopadłościan reprezentujący części robota.
 *
 * Klasa \link MobileObj MobileObj \endlink przechowuje informacje
 * o pozycji, orientacji oraz nazwie bryły w przestrzeni 3D. Służy
 * do reprezentacji części robota i pozwala na odczyt i modyfikację
 * tych właściwości.
 */
class MobileObj : public AbstractMobileObj {
  private:

    /*!
     * \brief Nazwa bryły.
     *
     * Unikalna nazwa identyfikująca obiekt.
     */
    std::string _name;

    /*!
     * \brief Wektor pozycji bryły w przestrzeni 3D.
     *
     * Przechowuje współrzędne pozycji środka bryły w metrach.
     */
    Vector3D _position;

    /*!
     * \brief Kąt obrotu wokół osi X (roll).
     *
     * Wartość kąta w stopniach.
     */
    double _roll;

    /*!
     * \brief Kąt obrotu wokół osi Y (pitch).
     *
     * Wartość kąta w stopniach.
     */
    double _pitch;

    /*!
     * \brief Kąt obrotu wokół osi Z (yaw).
     *
     * Wartość kąta w stopniach.
     */
    double _yaw;


  public:

    /*!
     * \brief Konstruktor parametryczny.
     *
     * Inicjalizuje obiekt mobilny z nazwą oraz opcjonalnymi wartościami pozycji i orientacji.
     * 
     * \param name - Nazwa obiektu (wymagana).
     * \param position - Pozycja obiektu (domyślnie {0, 0, 0}).
     * \param roll - Kąt obrotu wokół osi X (domyślnie 0).
     * \param pitch - Kąt obrotu wokół osi Y (domyślnie 0).
     * \param yaw - Kąt obrotu wokół osi Z (domyślnie 0).
     */
    MobileObj(const std::string& name, 
              const Vector3D& position = {0.0, 0.0, 0.0}, 
              double roll = 0.0, 
              double pitch = 0.0, 
              double yaw = 0.0)
        : _name(name), _position(position), _roll(roll), _pitch(pitch), _yaw(yaw) {}

    /*!
     * \brief Pobiera pozycję obiektu w metrach.
     * \return Wektor 3D zawierający współrzędne pozycji.
     */
    const Vector3D & GetPosition_m() const override;

    /*!
     * \brief Ustawia pozycję obiektu w metrach.
     * \param[in] rPos - Wektor pozycji w przestrzeni 3D.
     */
    void SetPosition_m(const Vector3D &rPos) override;

    /*!
     * \brief Pobiera kąt obrotu wokół osi X (roll).
     * \return Kąt w stopniach.
     */
    double GetAng_Roll_deg() const override;

    /*!
     * \brief Ustawia kąt obrotu wokół osi X (roll).
     * \param[in] Ang_Roll_deg - Kąt w stopniach.
     */
    void SetAng_Roll_deg(double Ang_Roll_deg) override;

    /*!
     * \brief Pobiera kąt obrotu wokół osi Y (pitch).
     * \return Kąt w stopniach.
     */
    double GetAng_Pitch_deg() const override;

    /*!
     * \brief Ustawia kąt obrotu wokół osi Y (pitch).
     * \param[in] Ang_Pitch_deg - Kąt w stopniach.
     */
    void SetAng_Pitch_deg(double Ang_Pitch_deg) override;

    /*!
     * \brief Pobiera kąt obrotu wokół osi Z (yaw).
     * \return Kąt w stopniach.
     */
    double GetAng_Yaw_deg() const override;

    /*!
     * \brief Ustawia kąt obrotu wokół osi Z (yaw).
     * \param[in] Ang_Yaw_deg - Kąt w stopniach.
     */
    void SetAng_Yaw_deg(double Ang_Yaw_deg) override;

    /*!
     * \brief Pobiera nazwę obiektu.
     * \return Ciąg znaków reprezentujący nazwę obiektu.
     */
    const std::string & GetName() const override;

    /*!
     * \brief Ustawia nazwę obiektu.
     * \param[in] sName - Wskaźnik na ciąg znaków reprezentujący nazwę.
     */
    void SetName(const char* sName) override;
};

#endif
