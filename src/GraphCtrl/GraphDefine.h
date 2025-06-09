#ifndef CGRAPH_GRAPHDEFINE_H
#define CGRAPH_GRAPHDEFINE_H

#include "../UtilsCtrl/UtilsInclude.h"

const static int INIT_THREAD_NUM = 4;
const static int MAX_THREAD_NUM =16;

using TaskFunc = std::function<void()>;

#endif //CGRAPH_GRAPHDEFINE_H