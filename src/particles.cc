//This class serves as a LUT of the particles that can come from sphaleron transitions
#include "../include/particles.h"
#include <iostream>
#include <math.h>

particleBase::particleBase()
{
    partLUT.clear();
    particle partBuf;
    //Put in a blank first so the entry in partLUT is equal to fabs(PID)
    partBuf.pid = -1; partBuf.mass = -1.0; partBuf.color = -99; partBuf.q3 = -4; partBuf.p4v = TLorentzVector(0.0,0.0,0.0,0.0);
    partLUT.push_back(partBuf); //0
    //1 Down Quark, the p4v will always stay zero at this stage, as the color will also be established later
    //Also I only store the matter version of particles and use getParticle to make the correction if antimatter is requested (negative pid)
    partBuf.pid = DQ_PID; partBuf.mass = DQ_MASS; partBuf.q3 = DQ_Q3;
    partLUT.push_back(partBuf);
    //2 Up Quark
    partBuf.pid = UQ_PID; partBuf.mass = UQ_MASS; partBuf.q3 = UQ_Q3;
    partLUT.push_back(partBuf);
    //3 Strange Quark
    partBuf.pid = SQ_PID; partBuf.mass = SQ_MASS; partBuf.q3 = SQ_Q3;
    partLUT.push_back(partBuf);
    //4 Charm Quark
    partBuf.pid = CQ_PID; partBuf.mass = CQ_MASS; partBuf.q3 = CQ_Q3;
    partLUT.push_back(partBuf);
    //5 Bottom Quark
    partBuf.pid = BQ_PID; partBuf.mass = BQ_MASS; partBuf.q3 = BQ_Q3;
    partLUT.push_back(partBuf);
    //6 Top Quark
    partBuf.pid = TQ_PID; partBuf.mass = TQ_MASS; partBuf.q3 = TQ_Q3;
    partLUT.push_back(partBuf);
    //Put in some more blanks to get to ELE_PID
    partBuf.pid = -1; partBuf.mass = -1.0; partBuf.q3 = -4;
    partLUT.push_back(partBuf); //7
    partLUT.push_back(partBuf); //8
    partLUT.push_back(partBuf); //9
    partLUT.push_back(partBuf); //10
    //11 Electron
    partBuf.pid = ELE_PID; partBuf.mass = ELE_MASS; partBuf.q3 = ELE_Q3;
    partLUT.push_back(partBuf);
    //12 Electron Neutrino
    partBuf.pid = ENU_PID; partBuf.mass = ENU_MASS; partBuf.q3 = ENU_Q3;
    partLUT.push_back(partBuf);
    //13 Muon
    partBuf.pid = MU_PID; partBuf.mass = MU_MASS; partBuf.q3 = MU_Q3;
    partLUT.push_back(partBuf);
    //14 Muon Neutrino
    partBuf.pid = MNU_PID; partBuf.mass = MNU_MASS; partBuf.q3 = MNU_Q3;
    partLUT.push_back(partBuf);
    //15 Tau
    partBuf.pid = TAU_PID; partBuf.mass = TAU_MASS; partBuf.q3 = TAU_Q3;
    partLUT.push_back(partBuf);
    //16 Tau Neutrino
    partBuf.pid = TNU_PID; partBuf.mass = TNU_MASS; partBuf.q3 = TNU_Q3;
    partLUT.push_back(partBuf);
    

}

particleBase::~particleBase()
{
}

particle particleBase::getParticle(int pid)
{
    int ii = fabs(pid);
    int ss = pid/ii;
    if(!(ss == 1 || ss == -1)) cout << "Something odd happened in particleBase::getParticle(int pid)" << endl;
    particle partBuf = partLUT[ii];
    partBuf.pid = pid; partBuf.q3 = ss*partBuf.q3;
    partBuf.m1 = 0;
    partBuf.m2 = 0;
    partBuf.ic = 0;
    return partBuf;

}
