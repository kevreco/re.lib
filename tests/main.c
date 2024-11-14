#include "strv_test.h"
#include "dstr_test.h"
#include "darr_test.h"
#include "darr_map_test.h"
#include "ht_test.h"

int main(void)
{
    if (!strv_test())
        return -1;
    
    if (!dstr_test())
        return -1;
    
    if (!darr_test())
         return -1;
     
    if (!darr_map_test())
         return -1;
     
    if (!ht_test())
         return -1;
     
    return 0;
}

#include "strv_test.c"
#include "dstr_test.c"
#include "darr_test.c"
#include "darr_map_test.c"
#include "ht_test.c"
