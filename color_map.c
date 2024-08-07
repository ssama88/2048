#include <stdio.h>
#include <math.h>

int color_map[20][3] = {{238, 228, 218}, {119, 110, 101}, {238, 225, 201}, {119, 110, 101}, {243, 178, 122}, {249, 246, 242}};

int getKey(int num)
{
    return 2 * (log(num) / log(2) - 1);
}
