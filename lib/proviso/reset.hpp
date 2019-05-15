// Added by Pablo Edronkin.

#ifndef _QLIB_QUANTUM_GATES_RESET_H
#define _QLIB_QUANTUM_GATES_RESET_H

#include "onequbitgate.hpp"
//#include "controlledgate.hpp"

namespace qlib {
namespace quantum {
namespace gates {

onequbitgate reset = onequbitgate(
    "Reset", 
    {
        complex(1), complex(0),
        complex(0), complex(1)
    });


}
}
}

#endif
