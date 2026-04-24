#include "eg.h"

int main(void)
{
    fnresult_t result = eg_text_box();
    if (result != FN_SUCCESS)
    {
        puts("Debug: Example failed.");
        return 1;
    }  

    puts("Debug: Example succeeded");

    return 0;
}