/*!
 \file prRGBColor.h
 \brief Header file for the base prRGBColor class
 \author Guillaume CARON
 \version 0.1
 \date february 2017
 */

#if !defined(_PRRGBCOLOR_H)
#define _PRRGBCOLOR_H

#include <per/prColorLight.h>

/*!
 \class prRGBColor prRGBColor.h <per/prRGBColor.h>
 \brief Class defining the color of light coded with three 8 bits Red Green Blue values
 */
class prRGBColor : public prColorLight
{
public:
    /*!
     * \fn prRGBColor(const unsigned char R = 0, const unsigned char G = 0, const unsigned char B = 0)
     * \brief Constructor setting the 3 channels color coded on 8 bits
     * \param R red channel value
     * \param G green channel value
     * \param B blue channel value
     */
    prRGBColor(const unsigned char R = 0, const unsigned char G = 0, const unsigned char B = 0);

    /*!
     * \fn ~prRGBColor()
     * \brief Destructor
     */
    ~prRGBColor();
    
    /*!
     * \fn void set_R()
     * \brief Sets the red channel value
     * \return Nothing
     */
    /*inline*/ void set_R(const unsigned char R) { p[0] = R ; }
    
    /*!
     * \fn void set_G()
     * \brief Sets the green channel value
     * \return Nothing
     */
    /*inline*/ void set_G(const unsigned char G) { p[1] = G ; }
    
    /*!
     * \fn void set_B()
     * \brief Sets the blue channel value
     * \return Nothing
     */
    /*inline*/ void set_B(const unsigned char B) { p[2] = B ; }
    
  
    /*!
     * \fn unsigned char get_R()
     * \brief Accessor to the red channel value
     * \return R
     */
    unsigned char get_R()  const { return p[0] ; }
    
    /*!
     * \fn unsigned char get_G()
     * \brief Accessor to the green channel value
     * \return G
     */
    unsigned char get_G()  const { return p[1] ; }

    /*!
     * \fn unsigned char get_B()
     * \brief Accessor to the blue channel value
     * \return B
     */
    unsigned char get_B()  const { return p[2] ; }

    /*!
     * \fn void set(const unsigned char & R, const unsigned char & G, const unsigned char & B)
     * \brief Sets the RGB color 3 channel values
     * \param R the color red channel
     * \param G the color green channel
     * \param B the color blue channel
     * \return Nothing
     */
    void set(const unsigned char & R, const unsigned char & G, const unsigned char & B);
    
};

#endif  //_PRRGBCOLOR_H
