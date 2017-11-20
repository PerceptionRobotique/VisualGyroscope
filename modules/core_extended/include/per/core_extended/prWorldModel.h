/*!
 \file prWorldModel.h
 \brief Header file for the base prWorldModel class
 \author Guillaume CARON
 \version 0.1
 \date february 2017
 */

#include <per/prPhysicalElement.h>
#include <vector>

#if !defined(_PRWORLDMODEL_H)
#define _PRWORLDMODEL_H

/*!
 \class prWorldModel prWorldModel.h <per/prWorldModel.h>
 \brief Base class defining the world elements
 */
class prWorldModel
{
protected:
    std::vector<prPhysicalElement **> vec_elements; /*!< an array of pointers to arrays of various physical elements */
    std::vector<unsigned long> vec_nbElementTypes; /*!< the number of physical elements types */
    
    /*!
     * \fn prWorldModel()
     * \brief Default constructor of a prWorldModel object
     */
    prWorldModel(unsigned int nbElementTypes = 0, unsigned long nbElementsPerType = 0);
    
    /*!
     * \fn int setElement(unsigned int elementTypeIndex, unsigned int elementIndex, prPhysicalElement *elem)
     * \brief Sets a physical element at the correct place
     * \param elementTypeIndex the index of element type (depends on how tab_element has been created)
     * \param elementIndex the index of the element to set
     * \param elem a pointer to the element to set
     * \return  0 if the set successes
     *         -1 if the elementTypeIndex is out of range
     *         -2 if the elementIndex is out of range
     */
    int setElement(unsigned int elementTypeIndex, unsigned long elementIndex, prPhysicalElement *elem);
    
    /*!
     * \fn int getElement(unsigned int elementTypeIndex, unsigned int elementIndex, prPhysicalElement * &elem)
     * \brief Sets a physical element at the correct place
     * \param elementTypeIndex the index of element type (depends on how tab_element has been created)
     * \param elementIndex the index of the element to get
     * \param elem a pointer to the got element (output)
     * \return  0 if the set successes
     *         -1 if the elementTypeIndex is out of range
     *         -2 if the elementIndex is out of range
     *         -3 if no element is stored at indexes [elementTypeIndex, elementIndex] (~warning)
     */
    int getElement(unsigned int elementTypeIndex, unsigned long elementIndex, prPhysicalElement * &elem);

    /*!
     * \fn void addElementType(unsigned int nbElementsForTheType)
     * \brief Adds a physical element type
     * \param nbElementsForTheType the amount of elments of that type that the world model should contain
     * \return  Nothing
     */
    void addElementType(unsigned long nbElementsForTheType);
    
public:
    /*!
     * \fn ~prWorldModel()
     * \brief Default destructor of a prWorldModel object
     *        Frees physical element containers AND the elements themselves
     */
    ~prWorldModel();
    
    /*!
     * \fn unsigned long getNumberOfElementTypes()
     * \brief Gets the number of different physical element types in the world model
     * \return The amount of element types that the world model contains
     */
    /*inline*/ unsigned long getNumberOfElementTypes() {return vec_nbElementTypes.size();}
    
    /*!
     * \fn unsigned long getNumberOfElements(unsigned int elementTypeIndex)
     * \brief Gets the number of physical elements for the type elementTypeIndex in the world model
     * \return The amount of elements that the world model contains
     */
    /*inline*/ unsigned long getNumberOfElements(unsigned int elementTypeIndex = 0) {return (elementTypeIndex <vec_nbElementTypes.size())?vec_nbElementTypes[elementTypeIndex]:0;}
    
private:
    /*!
     * \fn void addTabElements(unsigned int nbElementsPerType)
     * \brief Internal method that allocates and initializes to NULL an array of pointers to physical elements
     * \param nbElementsPerType the amount of elements of that type that the world model should contain
     * \return  Nothing
     */
    void addTabElements(unsigned int nbElementsPerType);
};

#endif  //_PRWORLDMODEL_H
