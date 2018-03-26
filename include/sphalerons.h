#include "particles.h"

double sphal[2][12];

int fillSphal()
{

    sphal[0][0] = ELE_MASS;
    sphal[1][0] = ELE_PID;

    sphal[0][1] = MU_MASS;
    sphal[1][1] = MU_PID;

    sphal[0][2] = TAU_MASS;
    sphal[1][2] = TAU_PID;

    sphal[0][3] = TQ_MASS;
    sphal[1][3] = TQ_PID;

    sphal[0][4] = TQ_MASS;
    sphal[1][4] = TQ_PID;

    sphal[0][5] = BQ_MASS;
    sphal[1][5] = BQ_PID;

    sphal[0][6] = CQ_MASS;
    sphal[1][6] = CQ_PID;

    sphal[0][7] = CQ_MASS;
    sphal[1][7] = CQ_PID;

    sphal[0][8] = SQ_MASS;
    sphal[1][8] = SQ_PID;

    sphal[0][9] = UQ_MASS;
    sphal[1][9] = UQ_PID;

    sphal[0][10] = UQ_MASS;
    sphal[1][10] = UQ_PID;

    sphal[0][11] = DQ_MASS;
    sphal[1][11] = DQ_PID;

    return 0;
}

