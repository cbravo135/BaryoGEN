#include "../include/LHEWriter.h"
#include <iostream>
#include <math.h>
using namespace std;

LHEWriter::LHEWriter(string fName)
{
    oF.open((fName+".lhe").c_str());
    oF << std::scientific;
    oF << "<LesHouchesEvents version=\"1.0\">" << endl;
    oF << "<header>" << endl;
    oF << "</header>" << endl;
    oF << "<init>" << endl;
    oF << "\t2212 2212 0.65000000000e+04 0.65000000000e+04 292200 292200 292200 292200 3 1" << endl;//beam conditions information
    oF << "\t7.3 1.0 1.0 7000" << endl;//sphaleron process information
    oF << "</init>" << endl;
}

LHEWriter::~LHEWriter()
{
}

int LHEWriter::writeEvent(vector<particle> outParts, double Q)
{
    //cout << "size check: " << decayK.size() << "\t" << decayPIDs.size() << "\t" << decayColz.size() << endl;
    oF << "<event>" << endl;
    oF << "\t" << outParts.size() << " 7000 1 "<< Q << " 7.3e-03 0.118" << endl;
    for(int i = 0; i < int(outParts.size()); i++)
    {
        oF << "\t" << outParts[i].pid;

        if(i < 2) oF << "\t-1\t";
        else if(fabs(outParts[i].pid) > 1000 || ( i < outParts.size() - 2 && (outParts[i].color == 501 || outParts[i].color == 502) ) ) oF << "\t2\t";
        else oF << "\t1\t";
        oF << outParts[i].m1 << "\t" << outParts[i].m2 << "\t";

        if(fabs(outParts[i].pid) < 1000)
        {
            if(outParts[i].pid > 0) oF << outParts[i].color << "\t0\t";
            else oF << "0\t"  << outParts[i].color << "\t";
        }
        else
        {
            if(outParts[i].ic > 0) 
            {
                oF << outParts[i].color << "\t0\t";
            }
            else 
            {
                oF << "0\t"  << outParts[i].color << "\t";
            }
        }

        oF << outParts[i].p4v.Px() << "\t";
        oF << outParts[i].p4v.Py() << "\t";
        oF << outParts[i].p4v.Pz() << "\t";
        oF << outParts[i].p4v.E() << "\t";
        oF << outParts[i].p4v.M() << "\t";
        oF << "0\t9" << endl;
    }
    oF << "</event>" << endl;
    return 1;
}

int LHEWriter::close()
{
    oF << "</LesHouchesEvents>" << endl;
    oF.close();
    return 1;
}
