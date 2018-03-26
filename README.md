# BaryoGEN
Compiles with gcc version >= 5.3.0
Requires ROOT version >= 6.06/01
Requires LHAPDF == 6.1.6

mstwpdf is from the mstw2008 package, available publically at https://mstwpdf.hepforge.org/
    A. D. Martin, W. J. Stirling, R. S. Thorne and G. Watt,
    "Parton distributions for the LHC",
    Eur. Phys. J. C63 (2009) 189-285
    [arXiv:0901.0002 [hep-ph]].

The LHAPDF code which can be found at:
    "LHAPDF6: parton density access in the LHC precision era"
    Eur.Phys.J. C75 (2015) 3, 132
    http://arxiv.org/abs/1412.7420

The pdf grid string is hard coded, and can be changed before compiling.
End user must make sure they have downloaded the desired pdf grid and
installed it into their installation of LHAPDF.
