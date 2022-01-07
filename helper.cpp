#include "helper.h"

Helper* HelperCreate(int * keyArr, std::shared_ptr<Level> * dataArr)
{
    Helper *helper = new Helper();
    helper->key = keyArr;
    helper->data = dataArr;
    return helper;
}

void HelperDestroy(Helper* helper)
{
    delete helper;
}