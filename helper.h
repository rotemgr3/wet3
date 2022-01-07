#ifndef HELPER_H_
#define HELPER_H_

#include <memory>
#include "level.h"

typedef struct {
    int * key;
    std::shared_ptr<Level> * data;
} Helper;

Helper* HelperCreate(int * keyArr, std::shared_ptr<Level> * dataArr);
void HelperDestroy(Helper* helper);

#endif /* HELPER_H_ */