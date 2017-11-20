#include <per/prWorldModel.h>

prWorldModel::prWorldModel(unsigned int nbElementTypes, unsigned long nbElementsPerType)
{
    if(nbElementTypes != 0)
    {
        unsigned int i;

        vec_elements.reserve(nbElementTypes);
        vec_nbElementTypes.reserve(nbElementTypes);
        for(i = 0 ; i < nbElementTypes ; i++)
        {
            vec_nbElementTypes.push_back(nbElementsPerType);
    
            if(nbElementsPerType != 0)
            {
                addTabElements(nbElementsPerType);
            }
        }
    }
}

prWorldModel::~prWorldModel()
{
    unsigned int i, j;
    
    for (i = 0 ; i < vec_nbElementTypes.size() ; i++)
    {
        for(j = 0 ; j < vec_nbElementTypes[i] ; j++)
            if(vec_elements[i][j] != NULL)
                delete vec_elements[i][j];
        delete [] vec_elements[i];
    }
    
}

void prWorldModel::addTabElements(unsigned int nbElementsPerType)
{
    prPhysicalElement **tab_elements = new prPhysicalElement *[nbElementsPerType];
    memset(tab_elements, NULL, nbElementsPerType*sizeof(prPhysicalElement *));
    vec_elements.push_back(tab_elements);
}

void prWorldModel::addElementType(unsigned long nbElementsForTheType)
{
    if(nbElementsForTheType != 0)
    {
        vec_nbElementTypes.push_back(nbElementsForTheType);
        addTabElements(nbElementsForTheType);
    }
}

int prWorldModel::setElement(unsigned int elementTypeIndex, unsigned long elementIndex, prPhysicalElement *elem)
{
    if(elementTypeIndex >= vec_nbElementTypes.size())
        return -1;
    
    if(elementIndex >= vec_nbElementTypes[elementTypeIndex])
        return -2;
    
    vec_elements[elementTypeIndex][elementIndex] = elem;
    
    return 0;
}

int prWorldModel::getElement(unsigned int elementTypeIndex, unsigned long elementIndex, prPhysicalElement * &elem)
{
    if(elementTypeIndex >= vec_nbElementTypes.size())
        return -1;
    
    if(elementIndex >= vec_nbElementTypes[elementTypeIndex])
        return -2;
    
    elem = vec_elements[elementTypeIndex][elementIndex];
    
    if(elem == NULL)
        return -3;
    
    return 0;
}

