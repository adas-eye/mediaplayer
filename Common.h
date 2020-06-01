#ifndef COMMON_H
#define COMMON_H

#include <assert.h>

namespace media{
//删除指针
#define SAFE_DELETE_PTR(pData) {\
    try {\
        if(pData != nullptr){\
            delete pData;\
        }\
        else{\
            return ;\
        }\
    }\
    catch (...) {\
        assert(FALSE);\
    }\
    pData=NULL;\
}

//删除数组
#define SAFE_DELETE_PTR_ARR(pData) { \
    try { \
        delete [] pData; \
    } \
    catch (...) { \
     assert(FALSE);\
    } \
    pData=NULL;\
}
}
#endif // COMMON_H
