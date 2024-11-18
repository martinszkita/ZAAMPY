#ifndef  COMMAND4MOVE_HH
#define  COMMAND4MOVE_HH

#ifndef __GNUG__
# pragma interface
# pragma implementation
#endif

#include "AbstractInterp4Command.hh"

/*!
 * \file
 * \brief Definicja klasy Interp4Move
 *
 * Plik zawiera definicję klasy Interp4Move ...
 */

/*!
 * \brief Modeluje polecenie dla robota mobilnego, które wymusza jego ruch do przodu
 *
 *  Klasa modeluje ...
 */
class Interp4Move: public AbstractInterp4Command {
  /*
   *  Tu należy zdefiniować pola, które są niezbędne
   *  do przechowywania wartości parametrów danego polecenia.
   *  Ponieżej zdefiniowane jest tylko jedno pole jako przykład.
   */
  double  _Speed_mmS;
  double  _Distance_mm;
  const char * _robotName; //Nazwa robota dla ktorego wykonywane jest poleceni
 public:

  void setRobotName(const char * _name);
  const char * getRobotName() const;
  void setSpeed(const double _speed);
  void setDistance(const double distance);
  const double & getSpeed() const;
  const double & getDistance() const;
  /*!
   * \brief
   */
  Interp4Move();  
  /*!
   * \brief Wyświetla postać bieżącego polecenia (nazwę oraz wartości parametrów)
   */
  virtual void PrintCmd() const override;
  /*!
   * \brief Wyświetla składnię polecenia
   */
  virtual void PrintSyntax() const override;
  /*!
   * \brief Wyświetla wartości wczytanych parametrów
   */
  virtual void PrintParams() const override {} 

  /*!
   * \brief Wyświetla nazwę polecenia
   */
  virtual const char* GetCmdName() const override;

  /*!
   * \brief Wykonuje polecenie oraz wizualizuje jego realizację
   *
   * Wykonuje polecenie oraz wizualizuje jego realizację.
   * \param[in,out]  rScn - scena zawierającą obiekty mobilne,
   * \param[in]      sMobObjName - wskaźnik na nazwę lokalizującą i identyfikującą obiekt,
   * \param[in,out]  rComChann - kanał komunikacyjny z serwerem graficznym.
   * \retval true - operacja powiodła się,
   * \retval false - w przypadku przeciwnym.
   */
  virtual bool ExecCmd( AbstractScene      &rScn, 
                        const char         *sMobObjName,
                        AbstractComChannel &rComChann ) override;
  /*!
   * \brief Czyta wartości parametrów danego polecenia
   */
  virtual bool ReadParams(std::istream& Strm_CmdsList) override;

  
  /*!
   * \brief
   *
   *  Ta metoda nie musi być zdefiniowna w klasie bazowej.
   */
  static AbstractInterp4Command* CreateCmd();
 };

#endif
/////////////////

// #ifndef INTERP4MOVE_HH
// #define INTERP4MOVE_HH

// #include "AbstractInterp4Command.hh"
// #include "Vector3D.hh"
// #include "AbstractScene.hh"
// #include "AbstractComChannel.hh"

// class Interp4Move : public AbstractInterp4Command {
//     double _Speed_mmS;   // Prędkość ruchu
//     double _Distance_mm; // Długość trasy
//     const char * _robotName; //Nazwa robota dla ktorego wykonywane jest polecenie

//   public:
//     Interp4Move();
//     const char* GetCmdName() const override;
//     bool ExecCmd(AbstractScene &rScn, const char *sMobObjName, AbstractComChannel &rComChann) override;
//     bool ReadParams(std::istream &rStrm_CmdsList) override;
//     void PrintCmd() const override;
//     void PrintSyntax() const override;
//     void PrintParams() const override;
//     void setRobotName(const char * _name);
//     const char * getRobotName() const;
//     void setSpeed(const double _speed);
//     void setDistance(const double distance);
//     const double & getSpeed() const;
//     const double & getDistance() const;

//     static AbstractInterp4Command* CreateCmd();
    
// };

// std::istream & operator >> (std::istream & strWej, Interp4Move & interp);


// #endif

