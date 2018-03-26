#include "../include/configBuilder.h"
#include <iostream>

using namespace std;

configBuilder::configBuilder()
{
    //Seed random number generator
    rand.SetSeed(0);

    //Build Fermion Doublets, the order of them is SUPER important
    particle partBuf;
    partBuf.mass = ELE_MASS;
    partBuf.pid = ELE_PID;
    partBuf.color = 0;
    partBuf.q3 = ELE_Q3;
    parts.push_back(partBuf);  // 0

    partBuf.mass = 0;
    partBuf.pid = ENU_PID;
    partBuf.color = 0;
    partBuf.q3 = ENU_Q3;
    parts.push_back(partBuf);  // 1

    partBuf.mass = DQ_MASS;
    partBuf.pid = DQ_PID;
    partBuf.color = 1;
    partBuf.q3 = DQ_Q3;
    parts.push_back(partBuf);  // 2

    partBuf.mass = UQ_MASS;
    partBuf.pid = UQ_PID;
    partBuf.color = 1;
    partBuf.q3 = UQ_Q3;
    parts.push_back(partBuf);  // 3

    partBuf.mass = MU_MASS;
    partBuf.pid = MU_PID;
    partBuf.color = 0;
    partBuf.q3 = MU_Q3;
    parts.push_back(partBuf);  // 4

    partBuf.mass = 0;
    partBuf.pid = MNU_PID;
    partBuf.color = 0;
    partBuf.q3 = MNU_Q3;
    parts.push_back(partBuf);  // 5

    partBuf.mass = SQ_MASS;
    partBuf.pid = SQ_PID;
    partBuf.color = 1;
    partBuf.q3 = SQ_Q3;
    parts.push_back(partBuf);  // 6

    partBuf.mass = CQ_MASS;
    partBuf.pid = CQ_PID;
    partBuf.color = 1;
    partBuf.q3 = CQ_Q3;
    parts.push_back(partBuf);  // 7

    partBuf.mass = TAU_MASS;
    partBuf.pid = TAU_PID;
    partBuf.color = 0;
    partBuf.q3 = TAU_Q3;
    parts.push_back(partBuf);  // 8

    partBuf.mass = 0;
    partBuf.pid = TNU_PID;
    partBuf.color = 0;
    partBuf.q3 = TNU_Q3;
    parts.push_back(partBuf);  // 9

    partBuf.mass = BQ_MASS;
    partBuf.pid = BQ_PID;
    partBuf.color = 1;
    partBuf.q3 = BQ_Q3;
    parts.push_back(partBuf);  // 10

    partBuf.mass = TQ_MASS;
    partBuf.pid = TQ_PID;
    partBuf.color = 1;
    partBuf.q3 = TQ_Q3;
    parts.push_back(partBuf);  // 11
}

configBuilder::~configBuilder()
{
}

vector<particle> configBuilder::build(int iQ1, int iQ2, float pNCS, bool bCancel)
{
    vector<particle> conf;
    vector<int> doublets;
    int dCS = 2*int(rand.Uniform() < pNCS) - 1;//Choose matter (1) or antimatter (-1)
    for(int i = 0; i < 12; i++)
    {
        doublets.push_back(i);
    }
    while(doublets.size() > 0)
    {
        int j = rand.Integer(doublets.size()); //Choose a random doublet from the list of the remaining doublets
        int doub = doublets[j];
        //Calculate index of particle in partBuf, this part conserves charge while also giving the correct number of each doublet
        int partI = (doub/4)*4 + 2*int(bool(doub%4)) + (12-doublets.size())%2;//This is the magic part of this whole thing
        particle part = parts[partI];
        part.pid = dCS*part.pid;
        part.q3 = dCS*part.q3;
        conf.push_back(part);
        doublets.erase(doublets.begin()+j);
    }

    bool spec1 = true;
    bool spec2 = true;

    if(bCancel)
    {
        for(int i = 0; i < 12; i++)
        {
            if(iQ1 + conf[i].pid == 0 && spec1)
            {
                conf.erase(conf.begin()+i);
                spec1 = false;
            }
            else if(iQ2 + conf[i].pid == 0 && spec2)
            {
                conf.erase(conf.begin()+i);
                spec2 = false;
            }
        }
    }
    if(spec1)
    {
        //Look up spectator quark particle info and push on configuration
        particle specBuf;
        for(int i = 0; i < parts.size(); i++)
        {
            if(fabs(iQ1) == parts[i].pid)
            {
                specBuf = parts[i];
                specBuf.pid = iQ1;
                specBuf.color = 501;
            }
        }
        conf.push_back(specBuf);
    }

    if(spec2)
    {
        //Look up spectator quark particle info and push on configuration
        particle specBuf;
        for(int i = 0; i < parts.size(); i++)
        {
            if(fabs(iQ2) == parts[i].pid)
            {
                specBuf = parts[i];
                specBuf.pid = iQ2;
                specBuf.color = 502;
            }
        }
        conf.push_back(specBuf);
    }

    /*conf.clear();
      for(int i = 0; i < parts.size(); i++)
      {
      if(i==1 || i==5 || i==9) continue;
      particle partBuf;
      partBuf.mass = parts[i].mass;
      partBuf.pid = dCS*parts[i].pid;
      partBuf.color = parts[i].color;
      conf.push_back(partBuf);
      if(i == 2 || i == 6 || i == 10) conf.push_back(partBuf);
      }*/
    return conf;
}






