#include <iostream>
#include <cmath>

#include "TRandom3.h"
#include "TLorentzVector.h"
#include "TVector3.h"

using namespace std;

struct biP 
{
    TLorentzVector p1;
    TLorentzVector p2;
};

struct triP 
{
    TLorentzVector p1;
    TLorentzVector p2;
    TLorentzVector p3;
};

biP decayTwo(TLorentzVector mother,double m1, double m2)
{
    TRandom3 rand;
    rand.SetSeed();

    //Get boost vector
    TVector3 boost;
    boost = mother.BoostVector();
    double mass = mother.M();

    if(m1 + m2 > mass) 
    {
        cout << "m1: " << m1 << endl;
        cout << "m2: " << m2 << endl;
        cout << "mass: " << mass << endl;
        throw std::invalid_argument( "Daughter mass sum must be less than Mother mass" );
    }

    //Decay in random direction
    double th1 = acos(2.0*rand.Uniform()-1.0);
    double phi1 = 2.0*M_PI*rand.Uniform() - M_PI;

    //Solve for everything else
    double E1 = (mass*mass - m2*m2 + m1*m1)/(2.0*mass);
    double E2 = mass - E1;
    double mp1 = sqrt(E1*E1 - m1*m1);

    TVector3 p1(mp1*cos(phi1)*sin(th1),mp1*sin(phi1)*sin(th1),mp1*cos(th1));
    TVector3 p2 = -1.0*p1;

    biP output;
    output.p1.SetPxPyPzE(p1.Px(),p1.Py(),p1.Pz(),E1);
    output.p2.SetPxPyPzE(p2.Px(),p2.Py(),p2.Pz(),E2);

    if(abs(output.p1.Eta()) > 100 )
    {
        cout << "mass: " << mass << endl;
        cout << "th1: " << th1 << endl;
        cout << "E1: " << E1 << endl;
        cout << "phi1: " << phi1 << endl;
        cout << "mp1: " << mp1 << endl;
    }

    //Boost into lab frame
    output.p1.Boost(boost);
    output.p2.Boost(boost);

    return output;


};


triP decayThree(TLorentzVector mother,double m1, double m2,double m3)
{
    TRandom3 rand;
    rand.SetSeed();

    //Get boost vector
    TVector3 boost;
    boost = mother.BoostVector();
    double mass = mother.M();

    if(m1 + m2 + m3 > mass) throw std::invalid_argument( "Daughter mass sum must be less than Mother mass" );

    //Decay in random direction
    double th1 = acos(2.0*rand.Uniform()-1.0);
    double phi1 = 2.0*M_PI*rand.Uniform() - M_PI;

    //Solve for everything else
    double E1 = (mass*mass - m2*m2 + m1*m1)/(2.0*mass);
    double E2 = mass - E1;
    double E3 = mass - E1;
    double mp1 = sqrt(E1*E1 - m1*m1);

    TVector3 p1(mp1*cos(phi1)*sin(th1),mp1*sin(phi1)*sin(th1),mp1*cos(th1));
    TVector3 p2 = -1.0*p1;
    TVector3 p3 = -2.0*p1;

    triP output;
    output.p1.SetPxPyPzE(p1.Px(),p1.Py(),p1.Pz(),E1);
    output.p2.SetPxPyPzE(p2.Px(),p2.Py(),p2.Pz(),E2);
    output.p3.SetPxPyPzE(p3.Px(),p3.Py(),p3.Pz(),E3);

    if(abs(output.p1.Eta()) > 100 )
    {
        cout << "mass: " << mass << endl;
        cout << "th1: " << th1 << endl;
        cout << "E1: " << E1 << endl;
        cout << "phi1: " << phi1 << endl;
        cout << "mp1: " << mp1 << endl;
    }

    //Boost into lab frame
    output.p1.Boost(boost);
    output.p2.Boost(boost);

    return output;


};
