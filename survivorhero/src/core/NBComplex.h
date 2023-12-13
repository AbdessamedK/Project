#ifndef _NBCOMPLEX_H
#define _NBCOMPLEX_H
#include<iostream>

/// @class NBComplex
class NBComplex
{
    public :
    /// @brief float représentant la partie réelle du nombre complexe
    float re ;

    /// @brief float représentant la partie imaginaire du nombre complexe
    float im ;

    /// @brief Constructeur par défaut de la class NBComplex
    NBComplex ();

    /// @brief Construteur de la class NBComplex
    /// @param _re : float
    /// @param _im : float
    NBComplex (float _re, float _im);

    /// @brief Destructeur de la class NBComplex
    ~NBComplex();

    /// @brief Surchage de l'opérateur << pour NBComplex
    /// @param flux : ostream
    /// @param c : NBComplex
    /// @return ostream
    friend std::ostream& operator<< (std::ostream& flux, const NBComplex& c);

    /// @brief Surchage de l'opérateur >> pour NBComplex
    /// @param flux : istream
    /// @param c : NBComplex
    /// @return istream
    friend std::istream& operator>> (std::istream& flux, NBComplex &c); 

    /// @brief Surchage de l'opérateur + pour NBComplex
    /// @param c : NBComplex
    /// @return NBComplex
    NBComplex operator+ (const NBComplex& c)const ;

    /// @brief Surchage de l'opérateur - pour NBComplex
    /// @param c : NBComplex
    /// @return NBComplex
    NBComplex operator- (const NBComplex& c)const ;

    /// @brief Surchage de l'opérateur * pour NBComplex
    /// @param c : NBComplex
    /// @return NBComplex
    NBComplex operator* (const NBComplex& c)const ;

    /// @brief Test de régression de la class Corps pour s'assurer que tout marche correctement
    void testRegression () const ;

    /// @brief Multiplication entre un NBComplex est une constante
    /// @param lambda Réelle avec lequel on multiplie notre NBComplex
    /// @return NBComplex
    NBComplex operator* (const float lambda) const;

    /// @brief Forme exponentielle d'un nombre complex
    /// @param r Norme
    /// @param theta angle
    /// @return NBComplex
    NBComplex complexExp (const float r, const float theta) const;

    /// @brief Effectue une rotation autour d'un point donné.
    /// @param c Le point de rotation
    /// @param theta_deg L'angle de rotation
    /// @return NBComplex le résultat de la rotation
    NBComplex rotate (const NBComplex & c, float theta_deg) const;


    /// @brief Renvoie un float représentant la distance entre 2 points 
    /// @param nb NBComplex
    /// @return float : distance entre les 2 points 
    float distance (const NBComplex & nb) const;


};

#endif