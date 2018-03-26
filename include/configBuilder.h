#include "particles.h"

#include "TRandom3.h"

#include <vector>

using namespace std;

class configBuilder
{
    private:
        vector<particle> parts;
        TRandom3 rand;

    public:
        configBuilder();
        ~configBuilder();

        vector<particle> build(int iQ1, int iQ2, float pNCS, bool bCancel);

};

