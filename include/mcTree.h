//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jul 31 14:53:13 2017 by ROOT version 6.06/01
// from TTree mcTree/mcTree
// found on file: test/testRun.root
//////////////////////////////////////////////////////////

#ifndef mcTree_h
#define mcTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

class mcTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   vector<double>  *daughtersPt;
   vector<double>  *daughtersEta;
   vector<double>  *daughtersPhi;
   vector<double>  *daughtersE;
   vector<double>  *daughtersID;
   Double_t        weight;
   Double_t        pz;
   Double_t        x1;
   Double_t        x2;
   Int_t           iq1;
   Int_t           iq2;
   Double_t        Q2;
   Double_t        momM;

   // List of branches
   TBranch        *b_daughtersPt;   //!
   TBranch        *b_daughtersEta;   //!
   TBranch        *b_daughtersPhi;   //!
   TBranch        *b_daughtersE;   //!
   TBranch        *b_daughtersID;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_pz;   //!
   TBranch        *b_x1;   //!
   TBranch        *b_x2;   //!
   TBranch        *b_iq1;   //!
   TBranch        *b_iq2;   //!
   TBranch        *b_Q2;   //!
   TBranch        *b_momM;   //!

   mcTree(TTree *tree=0);
   virtual ~mcTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef mcTree_cxx
mcTree::mcTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("test/testRun.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("test/testRun.root");
      }
      f->GetObject("mcTree",tree);

   }
   Init(tree);
}

mcTree::~mcTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t mcTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t mcTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void mcTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   daughtersPt = 0;
   daughtersEta = 0;
   daughtersPhi = 0;
   daughtersE = 0;
   daughtersID = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("daughtersPt", &daughtersPt, &b_daughtersPt);
   fChain->SetBranchAddress("daughtersEta", &daughtersEta, &b_daughtersEta);
   fChain->SetBranchAddress("daughtersPhi", &daughtersPhi, &b_daughtersPhi);
   fChain->SetBranchAddress("daughtersE", &daughtersE, &b_daughtersE);
   fChain->SetBranchAddress("daughtersID", &daughtersID, &b_daughtersID);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("pz", &pz, &b_pz);
   fChain->SetBranchAddress("x1", &x1, &b_x1);
   fChain->SetBranchAddress("x2", &x2, &b_x2);
   fChain->SetBranchAddress("iq1", &iq1, &b_iq1);
   fChain->SetBranchAddress("iq2", &iq2, &b_iq2);
   fChain->SetBranchAddress("Q2", &Q2, &b_Q2);
   fChain->SetBranchAddress("momM", &momM, &b_momM);
   Notify();
}

Bool_t mcTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void mcTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t mcTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef mcTree_cxx
