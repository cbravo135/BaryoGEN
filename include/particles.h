#ifndef _PARTICLEH_
#define _PARTICLEH_

#include <vector>
#include "TLorentzVector.h"

#define ELE_MASS 0.000511
#define ENU_MASS 0.0
#define MU_MASS 0.10566
#define MNU_MASS 0.0
#define TAU_MASS 1.7768
#define TNU_MASS 0.0
#define TQ_MASS 173.34
#define BQ_MASS 4.18
#define CQ_MASS 1.275
#define SQ_MASS 0.095
#define UQ_MASS 0.0023
#define DQ_MASS 0.0048

#define ELE_PID 11
#define ENU_PID 12
#define MU_PID 13
#define MNU_PID 14
#define TAU_PID 15
#define TNU_PID 16
#define TQ_PID 6
#define BQ_PID 5
#define CQ_PID 4
#define SQ_PID 3
#define UQ_PID 2
#define DQ_PID 1

#define ELE_Q3 -3
#define ENU_Q3 0
#define MU_Q3 -3
#define MNU_Q3 0
#define TAU_Q3 -3
#define TNU_Q3 0
#define TQ_Q3 2
#define BQ_Q3 -1
#define CQ_Q3 2
#define SQ_Q3 -1
#define UQ_Q3 2
#define DQ_Q3 -1

struct particle
{
    int pid;
    float mass;
    int color;
    int q3;
    TLorentzVector p4v;
    int m1;
    int m2;
    int ic;
};

using namespace std;

class particleBase
{
    private:
        vector<particle> partLUT;

    public:
        particleBase();
        ~particleBase();

        particle getParticle(int pid);

};

#endif
