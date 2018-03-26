#include <iostream>
#include <stdlib.h> 
#include <cmath>
#include <vector>
#include <string>

#include "LHAPDF/LHAPDF.h"

#include "TLorentzVector.h"
#include "TGenPhaseSpace.h"
#include "TRandom3.h"
#include "TTree.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TString.h"

#include "../include/configBuilder.h"
#include "../include/BaryoGEN.h"
#include "../include/LHEWriter.h"

#define ARGS 7
#define MPW 5.0e-7

using namespace LHAPDF;
using namespace std;

int main(int argc, char* argv[])
{

    //read arguements and do conversions and checks on them
    if(argc != ARGS+1)
    {
        cout << "genTree sqrt(s) threshold maxweight Nevents pNCS bCancel Filename" << endl;
        return -99;
    }

    float SQRTS = atof(argv[1]);
    float thr = atof(argv[2]);
    float MCW = atof(argv[3]);
    if(MCW < 0.0)
    {
        cout << "maxweight must be greater than zero" << endl;
        return -99;
    }
    int Nevt = atoi(argv[4]);
    float pNCS = atof(argv[5]);
    if(pNCS > 1 || pNCS < 0)
    {
        cout << "pNCS must be in [0,1]" << endl;
        return -99;
    }
    int bCancel = atoi(argv[6]);
    if(bCancel < 0 || bCancel > 1)
    {
        cout << "bCancel must be 0 or 1" << endl;
        return -99;
    }
    bool bCan = bCancel;
    string ofName = string(argv[7]);

    //Init structures needed during generatation
    double maxwt = 0;
    double maxMCtot = 0.0;

    int NF = 0;
    TRandom3 rand;
    rand.SetSeed(0);
    particleBase *partBase = new particleBase();
    configBuilder confBuild;
    vector<double> daughtersPt;
    vector<double> daughtersEta;
    vector<double> daughtersPhi;
    vector<double> daughtersE;
    vector<double> daughtersID;
    TLorentzVector mom(0.0,0.0,0.0,thr);
    TLorentzVector u1(0.0,0.0,0.0,0.0);
    TLorentzVector u2(0.0,0.0,0.0,0.0);
    double masses[15];

    double weight = 0.0;

    TGenPhaseSpace gen;
    double x1 = 0.0;
    double x2 = 0.0;
    int iq1 = 0;
    int iq2 = 0;
    double Q2 = 0.0;
    double momM = 0.0;
    double pz = 0.0;

    //Init output files
    TFile *myF = new TFile((ofName+".root").c_str(),"RECREATE","Holds daughters from sphaleron decay");
    LHEWriter lheF(ofName);

    double minx = thr*thr/(SQRTS*SQRTS);

    //Init parton distribution functions
    const PDF* LHApdf = mkPDF("NNPDF30_lo_as_0118",0);
    cout << LHApdf->xfxQ2(2, 0.5, SQRTS*SQRTS) << endl;

    //Initialize histograms for debugging
    TH1D *x1_h = new TH1D("x1_h","x1 inclusive",1000,0.0,1.0);
    TH1D *mcTot_h = new TH1D("mcTot_h","Monte Carlo Probabilities",100,0.0,MCW);
    TH1D *sumInterQ3_h = new TH1D("sumInterQ3_h","Intermediate particle charges",21,-10.5,10.5);
    TH1D *sphM_h = new TH1D("sphM_h","Sphaleron Transition Energy;Invariant Mass [TeV];Events / 100 GeV",int(SQRTS/100.0),0.0,SQRTS/1000.0);
    TH1D *st_h = new TH1D("st_h","S_{T};S_{T} [TeV];Events / 100 GeV", int(SQRTS/100.0), 0.0, SQRTS/1000.0);
    TH1D *sphPz_h = new TH1D("sphPz_h","Sphaleron p_{z};p_{z} [GeV];Events / 100 GeV",80,-4000.0,4000.0);
    TH1D *outID_h = new TH1D("outID_h","Outgoing PDG IDs;PDG ID;Entries",33,-16.5,16.5);
    TH1D *p1x_h = new TH1D("p1x_h","Parton 1 Momentum Fraction;x_{1};Events / 0.01",60,0.4,1.0);
    TH1D *p1id_h = new TH1D("p1id_h","Parton 1 PDG ID;PDG ID;Events",13,-6.5,6.5);

    TH2D *inQid_h = new TH2D("inQid_h","Colliding Parton Species;Parton 2 PDG ID;Parton 1 PDG ID",11,-5.5,5.5,11,-5.5,5.5);
    TH2D *frac2D_h = new TH2D("frac2D_h","Parton Momentum Fractions;x_{2};x_{1}",60,0.4,1.0,60,0.4,1.0);

    vector<TH1D> pt_hv;
    vector<TH1D> eta_hv;
    vector<TH1D> phi_hv;
    for(int iq = -6; iq < 7; iq++)
    {
        string nameBuf;
        string titBuf;
        titBuf = Form("Quark %i p_{T};p_{T} [GeV];Entries / 20 GeV",iq);
        if(iq < 0) nameBuf = Form("pt_iqm%i_h",int(fabs(iq)));
        else nameBuf = Form("pt_iqp%i_h",int(fabs(iq)));
        TH1D hBufpt(nameBuf.c_str(),titBuf.c_str(),250,0.0,5000.0);
        pt_hv.push_back(hBufpt);

        titBuf = Form("Quark %i #eta;#eta;Entries / 0.1",iq);
        if(iq < 0) nameBuf = Form("eta_iqm%i_h",int(fabs(iq)));
        else nameBuf = Form("eta_iqp%i_h",int(fabs(iq)));
        TH1D hBufeta(nameBuf.c_str(),titBuf.c_str(),100,-5,5);
        eta_hv.push_back(hBufeta);

        titBuf = Form("Quark %i #phi;#phi;Entries / 0.1 #pi",iq);
        if(iq < 0) nameBuf = Form("phi_iqm%i_h",int(fabs(iq)));
        else nameBuf = Form("phi_iqp%i_h",int(fabs(iq)));
        TH1D hBufphi(nameBuf.c_str(),titBuf.c_str(),20,-1.0*TMath::Pi(),TMath::Pi());
        phi_hv.push_back(hBufphi);
    }

    //Initialize TTree to save event info
    TTree *myT = new TTree("mcTree","mcTree");
    myT->Branch("daughtersPt",&daughtersPt); 
    myT->Branch("daughtersEta",&daughtersEta); 
    myT->Branch("daughtersPhi",&daughtersPhi); 
    myT->Branch("daughtersE",&daughtersE); 
    myT->Branch("daughtersID",&daughtersID); 
    myT->Branch("weight",&weight); 
    myT->Branch("pz",&pz); 
    myT->Branch("x1",&x1); 
    myT->Branch("x2",&x2); 
    myT->Branch("iq1",&iq1); 
    myT->Branch("iq2",&iq2); 
    myT->Branch("Q2",&Q2); 
    myT->Branch("momM",&momM); 

    int pdfN = 0;

    while(NF < Nevt)//Main event generation loop
    {
        //Make sure everything is reset from the last event
        daughtersPt.clear();
        daughtersEta.clear();
        daughtersPhi.clear();
        daughtersE.clear();
        daughtersID.clear();

        Q2 = 0.0;
        double mcP = 1.1;
        bool mcPass = false;
        //Collide protons
        while(!mcPass)
        {
            //Choose x1 and x2 in proper range
            Q2 = 0.0;
            while(Q2 < thr*thr)
            {
                x1 = (1.0-minx)*rand.Uniform()+minx;
                x2 = (1.0-minx)*rand.Uniform()+minx;
                Q2 = x1*x2*SQRTS*SQRTS;
            }
            mcP = MCW*rand.Uniform();
            double mcTot = 0.0;
            for(int i1 = -5; i1 < 6; i1++)
            {
                if(i1 == 0) continue;
                iq1 = i1;
                double x1p = LHApdf->xfxQ2(iq1, x1, Q2)/x1;
                for(int i2 = -5; i2 < 6; i2++)
                {
                    if(i2 == 0) continue;
                    iq2 = i2;
                    //double x2p = pdf->parton(iq2,x2,SQRTS);
                    double x2p = LHApdf->xfxQ2(iq2, x2, Q2)/x2;
                    mcTot += x1p*x2p;
                    if(mcTot > mcP) {mcPass = true; break;}
                }
                if(mcPass) break;
            }
            pdfN++;
            x1_h->Fill(x1);
            mcTot_h->Fill(mcTot);
            if(maxMCtot < mcTot) {maxMCtot = mcTot; cout << "Max MC Total: " << maxMCtot << endl;}
        }

        //Build incoming particles, sphaleron, and prepare to decay
        vector<particle> inParts;
        particle partBuf = partBase->getParticle(iq1);
        if(iq1 != partBuf.pid) cout << "iq1 = " << iq1 << " != partBuf.pid = " << partBuf.pid << endl;
        partBuf.color = 501;
        partBuf.p4v.SetXYZM(0.0,0.0,x1*6500,partBuf.mass);
        inParts.push_back(partBuf); //Push first incoming quark

        partBuf = partBase->getParticle(iq2);
        partBuf.color = 502;
        partBuf.p4v.SetXYZM(0.0,0.0,x2*(-6500),partBuf.mass);
        inParts.push_back(partBuf); //Push second incoming quark

        int colNow = 503;//This is used to keep track of the color line numbers already used

        //Build the mother particle from incoming partons
        particle mother;
        mother.p4v = inParts[0].p4v + inParts[1].p4v;
        mother.mass = mother.p4v.M();

        //Generate vector of outgoing fermionic configuration
        vector<particle> confBuf = confBuild.build(iq1,iq2,pNCS,bCan);
        int Nline = 0;
        for(int i = 0; i < confBuf.size(); i++)
        {
            masses[i] = confBuf[i].mass;
        }

        u1.SetXYZM(0.0,0.0,x1*6500,UQ_MASS);
        u2.SetXYZM(0.0,0.0,x2*(-6500),UQ_MASS);
        mom = u1 + u2;
        momM = mom.M();
        pz = mom.Pz();

        //"Decay" mother to the generated configuration
        weight = 1.0;
        while(weight > MPW*rand.Uniform())
        {
            gen.SetDecay(mom, confBuf.size(), masses);

            weight = gen.Generate();
            if(weight > MPW) cout << "The code needs to recompiled with a higher MPW" << endl;
        }

        //Extract Decay 4-vectors and assign colors to non-spectator quarks
        for(int ii = 0; ii < confBuf.size(); ii++)
        {
            TLorentzVector prod = *gen.GetDecay(ii);
            confBuf[ii].p4v = prod;
            confBuf[ii].m1 = 0;
            confBuf[ii].m2 = 0;
            if(confBuf[ii].color != 501 && confBuf[ii].color != 502 && fabs(confBuf[ii].pid) < 7) confBuf[ii].color = colNow++;
            if(fabs(confBuf[ii].pid) > 7) confBuf[ii].color = 0;
            int kinI = confBuf[ii].pid + 6;
            if(confBuf[ii].color != 501 && confBuf[ii].color != 502 && fabs(confBuf[ii].pid) < 7) 
            {
                pt_hv[kinI].Fill(confBuf[ii].p4v.Pt());
                eta_hv[kinI].Fill(confBuf[ii].p4v.Eta());
                phi_hv[kinI].Fill(confBuf[ii].p4v.Phi());
            }
            outID_h->Fill(confBuf[ii].pid);
        }

        vector<particle> g1q;
        vector<particle> g2q;
        vector<particle> g3q;
        vector<particle> leps;
        vector<particle> specParts;

        vector<int> qIg1;
        vector<int> qIg2;
        vector<int> qIg3;
        int Nlep = 0;
        vector<int> lepI;
        //Seperate leptons and quarks
        for(int i = 0; i < confBuf.size(); i++)
        {
            if(confBuf[i].color == 501 || confBuf[i].color == 502) 
            { 
                confBuf[i].m1 = 1;
                confBuf[i].m2 = 2;
                specParts.push_back(confBuf[i]); 
                continue; 
            }
            if(fabs(confBuf[i].pid) > 7) //lepton
            {
                leps.push_back(confBuf[i]);
                lepI.push_back(i);
                Nlep++;
            }
            else //quark
            {
                if(fabs(confBuf[i].pid) > 4) { qIg3.push_back(i); g3q.push_back(confBuf[i]); }
                else if(fabs(confBuf[i].pid) > 2) { qIg2.push_back(i); g2q.push_back(confBuf[i]); }
                else { qIg1.push_back(i); g1q.push_back(confBuf[i]); }
            }
        }
        int interCol = 0;
        int icolS = 0;
        if(specParts.size() == 1)
        {
            if(specParts[0].color == 501) 
            {
                interCol = 502;
                if(iq2 > 0) icolS = 1;
                else icolS = -1;
            }
            if(specParts[0].color == 502) 
            {
                interCol = 501;
                if(iq1 > 0) icolS = 1;
                else icolS = -1;
            }
        }

        vector<particle> fileParts;
        fileParts.push_back(inParts[0]);
        fileParts.push_back(inParts[1]);

        //Calculate color flow case
        int specN = specParts.size();
        vector<particle> anParts;
        for(int i = 0; i < inParts.size(); i++)
        {
            bool mF = false;
            for(int ii = 0; ii < specParts.size(); ii++) if(inParts[i].color == specParts[ii].color) mF = true;
            if(!mF) anParts.push_back(inParts[i]);
        }
        bool dcg3 = (specParts.size() == 0 && g3q.size() == 2);
        bool dcg2 = (specParts.size() == 0 && g2q.size() == 2);
        bool dcg1 = (specParts.size() == 0 && g1q.size() == 2);

        //Push third Gen quarks and fake susy mediator onto output stack
        TLorentzVector interP(0.0,0.0,0.0,0.0);
        int interQ3 = 0;
        for(int ii = 0; ii < g3q.size(); ii++)
        {
            if(g3q[ii].color == 501 || g3q[ii].color == 502) continue;
            interP = interP + g3q[ii].p4v;
            interQ3 += g3q[ii].q3;
        }
        sumInterQ3_h->Fill(interQ3);
        particle interBuf;
        interBuf.mass = interP.M();
        interBuf.color = 0;
        if(g3q.size() == 2) 
        {
            interBuf.color = interCol;
            interBuf.ic = icolS;
        }
        interBuf.p4v = interP;
        interBuf.q3 = interQ3;
        interBuf.m1 = 1;
        interBuf.m2 = 2;
        if(interQ3 == 0) interBuf.pid = 1000022; 
        if(fabs(interQ3) == 3) interBuf.pid = interQ3*1006213/fabs(interQ3); 
        if(fabs(interQ3) == 6) interBuf.pid = interQ3*1006223/fabs(interQ3); 
        if(g3q.size() == 2) for(int i = 0; i < anParts.size(); i++) if(fabs(anParts[i].pid) == 5 || fabs(anParts[i].pid) == 6)
        {
            interBuf.pid = anParts[i].pid;
            interBuf.color = anParts[i].color;
        }
        bool useInter = (g3q.size() > 1);
        if(useInter) fileParts.push_back(interBuf);
        int interI = fileParts.size();
        for(int ii = 0; ii < g3q.size(); ii++)
        {
            g3q[ii].m1 = interI;
            g3q[ii].m2 = interI;
            if(!useInter) 
            {
                g3q[ii].m1 = 1;
                g3q[ii].m2 = 2;
                g3q[ii].ic = icolS;
            }
            fileParts.push_back(g3q[ii]);
            daughtersPt.push_back(g3q[ii].p4v.Pt());
            daughtersEta.push_back(g3q[ii].p4v.Eta());
            daughtersPhi.push_back(g3q[ii].p4v.Phi());
            daughtersE.push_back(g3q[ii].p4v.E());
            daughtersID.push_back(g3q[ii].pid);
        }

        //Push second Gen quarks onto output stack
        interP.SetPxPyPzE(0.0,0.0,0.0,0.0);
        interQ3 = 0;
        for(int ii = 0; ii < g2q.size(); ii++)
        {
            if(g2q[ii].color == 501 || g2q[ii].color == 502) continue;
            interP = interP + g2q[ii].p4v;
            interQ3 += g2q[ii].q3;
        }
        sumInterQ3_h->Fill(interQ3);
        interBuf.mass = interP.M();
        interBuf.color = 0;
        if(g2q.size() == 2) 
        {
            interBuf.color = interCol;
            interBuf.ic = icolS;
        }
        interBuf.p4v = interP;
        interBuf.q3 = interQ3;
        if(interQ3 == 0) interBuf.pid = 1000022; 
        if(fabs(interQ3) == 3) interBuf.pid = interQ3*1006213/fabs(interQ3);
        if(fabs(interQ3) == 6) interBuf.pid = interQ3*1006223/fabs(interQ3);
        if(dcg2 && dcg1) for(int i = 0; i < anParts.size(); i++) if(fabs(anParts[i].pid) == 3 || fabs(anParts[i].pid) == 4)
        {
            interBuf.pid = anParts[i].pid;
            interBuf.color = anParts[i].color;
        }
        useInter = (g2q.size() == 3 || (dcg2 && dcg1));
        if(useInter) fileParts.push_back(interBuf);
        interI = fileParts.size();
        for(int ii = 0; ii < g2q.size(); ii++)
        {
            g2q[ii].m1 = interI;
            g2q[ii].m2 = interI;
            if(!useInter) 
            {
                g2q[ii].m1 = 1;
                g2q[ii].m2 = 2;
                g2q[ii].ic = icolS;
            }
            fileParts.push_back(g2q[ii]);
            daughtersPt.push_back(g2q[ii].p4v.Pt());
            daughtersEta.push_back(g2q[ii].p4v.Eta());
            daughtersPhi.push_back(g2q[ii].p4v.Phi());
            daughtersE.push_back(g2q[ii].p4v.E());
            daughtersID.push_back(g2q[ii].pid);
        }

        //Push first Gen quarks onto output stack
        interP.SetPxPyPzE(0.0,0.0,0.0,0.0);
        interQ3 = 0;
        for(int ii = 0; ii < g1q.size(); ii++)
        {
            if(g1q[ii].color == 501 || g1q[ii].color == 502) continue;
            interP = interP + g1q[ii].p4v;
            interQ3 += g1q[ii].q3;
        }
        sumInterQ3_h->Fill(interQ3);
        interBuf.mass = interP.M();
        interBuf.color = 0;
        if(g1q.size() == 2) 
        {
            interBuf.color = interCol;
            interBuf.ic = icolS;
        }
        interBuf.p4v = interP;
        interBuf.q3 = interQ3;
        if(interQ3 == 0) interBuf.pid = 1000022; 
        if(fabs(interQ3) == 3) interBuf.pid = interQ3*1006213/fabs(interQ3);
        if(fabs(interQ3) == 6) interBuf.pid = interQ3*1006223/fabs(interQ3);
        useInter = (g1q.size() == 3);
        if(useInter) fileParts.push_back(interBuf);
        interI = fileParts.size();
        for(int ii = 0; ii < g1q.size(); ii++)
        {
            g1q[ii].m1 = interI;
            g1q[ii].m2 = interI;
            if(!useInter) 
            {
                g1q[ii].m1 = 1;
                g1q[ii].m2 = 2;
            }
            fileParts.push_back(g1q[ii]);
            daughtersPt.push_back(g1q[ii].p4v.Pt());
            daughtersEta.push_back(g1q[ii].p4v.Eta());
            daughtersPhi.push_back(g1q[ii].p4v.Phi());
            daughtersE.push_back(g1q[ii].p4v.E());
            daughtersID.push_back(g1q[ii].pid);
        }

        //Push leptons onto output stack
        for(int i = 0; i < leps.size(); i++)
        {
            leps[i].m1 = 1;
            leps[i].m2 = 2;
            fileParts.push_back(leps[i]);
            daughtersPt.push_back(leps[i].p4v.Pt());
            daughtersEta.push_back(leps[i].p4v.Eta());
            daughtersPhi.push_back(leps[i].p4v.Phi());
            daughtersE.push_back(leps[i].p4v.E());
            daughtersID.push_back(leps[i].pid);
        }

        //Push spectator Quarks if there are any
        for(int i = 0; i < specParts.size(); i++) 
        {
            fileParts.push_back(specParts[i]);
            daughtersPt.push_back(specParts[i].p4v.Pt());
            daughtersEta.push_back(specParts[i].p4v.Eta());
            daughtersPhi.push_back(specParts[i].p4v.Phi());
            daughtersE.push_back(specParts[i].p4v.E());
            daughtersID.push_back(specParts[i].pid);
        }

        if(weight > maxwt) 
        {
            maxwt = weight;
            cout << "MaxWt: " << maxwt << endl;
        }

        inQid_h->Fill(iq2,iq1);
        frac2D_h->Fill(x2,x1);
        sphM_h->Fill(momM/1000.0);
        sphPz_h->Fill(pz);
        p1x_h->Fill(x1);
        p1id_h->Fill(iq1);

        lheF.writeEvent(fileParts,sqrt(Q2));
        myT->Fill();
        NF++;
        if(NF%(Nevt/10) == 0) cout << "Produced Event " << NF << "  pdfN : " << pdfN << endl;
        //cout << "Produced Event " << NF << "  pdfN : " << pdfN << endl;
    }

    cout << "Max Weight: " << maxwt << endl;

    lheF.close();
    myF->cd();
    x1_h->Write();
    mcTot_h->Write();
    inQid_h->Write();
    frac2D_h->Write();
    p1x_h->Write();
    p1id_h->Write();
    outID_h->Write();
    sphM_h->Write();
    st_h->Write();
    sphPz_h->Write();
    for(int i = 0; i < pt_hv.size(); i++)
    {
        pt_hv[i].Write();
        eta_hv[i].Write();
        phi_hv[i].Write();
    }
    myT->Write();
    myF->Close();


    return 0;
};






