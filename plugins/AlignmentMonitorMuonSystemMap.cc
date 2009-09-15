// -*- C++ -*-
//
// Package:     CommonAlignmentProducer
// Class  :     AlignmentMonitorMuonSystemMap
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Jim Pivarski
//         Created:  Mon Nov 12 13:30:14 CST 2007
//

// system include files
#include "Alignment/CommonAlignmentMonitor/interface/AlignmentMonitorPluginFactory.h"
#include "Alignment/CommonAlignmentMonitor/interface/AlignmentMonitorBase.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"

#include "Alignment/MuonAlignmentAlgorithms/interface/MuonResidualsFromTrack.h"
#include "Alignment/MuonAlignmentAlgorithms/interface/MuonResidualsPositionFitter.h"
#include "Alignment/MuonAlignmentAlgorithms/interface/MuonResidualsAngleFitter.h"
#include "Alignment/MuonAlignmentAlgorithms/interface/MuonResidualsTwoBin.h"

#include <sstream>

// user include files

// 
// class definition
// 

class AlignmentMonitorMuonSystemMap: public AlignmentMonitorBase {
public:
  AlignmentMonitorMuonSystemMap(const edm::ParameterSet& cfg);
  ~AlignmentMonitorMuonSystemMap() {};

  void book();
  void event(const edm::Event &iEvent, const edm::EventSetup &iSetup, const ConstTrajTrackPairCollection& iTrajTracks);
  void afterAlignment(const edm::EventSetup &iSetup);

private:
  double m_minTrackPt;
  double m_maxTrackPt;
  int m_minTrackerHits;
  double m_maxTrackerRedChi2;
  bool m_allowTIDTEC;
  int m_minDT13Hits;
  int m_minDT2Hits;
  int m_minCSCHits;
  double m_maxDT13AngleError;
  double m_maxDT2AngleError;
  double m_maxCSCAngleError;
  std::string m_writeTemporaryFile;
  std::vector<std::string> m_readTemporaryFiles;
  bool m_doFits;
  bool m_twoBin;
  int m_minFitHits;
  int m_strategy;
  bool m_DT13fitScattering;
  bool m_DT13fitZpos;
  bool m_DT13fitPhiz;
  bool m_DT2fitScattering;
  bool m_DT2fitPhiz;
  bool m_CSCfitScattering;
  bool m_CSCfitZpos;
  bool m_CSCfitPhiz;
  int m_residualsModel;

  // the rphires vs z/r plots
  // last array is: 0 offset residual, a.k.a local x (mm)
  //                1 offset antisymmetric part (mm)
  //                2 zpos (mm) (rphires vs trackangle, nearly degenerate with phi)
  //                3 slope residual, a.k.a phiy (mrad)
  //                4 slope antisymmetric part (mrad)
  //                5 scattering correction (mm)
  TH1F *m_DTrphires_vsz_station1[12][6];
  TH1F *m_DTrphires_vsz_station2[12][6];
  TH1F *m_DTrphires_vsz_station3[12][6];
  TH1F *m_DTrphires_vsz_station4[14][6];
  TH1F *m_CSCrphires_vsr_me1[2][36][6];
  TH1F *m_CSCrphires_vsr_me2[2][36][6];
  TH1F *m_CSCrphires_vsr_me3[2][36][6];
  TH1F *m_CSCrphires_vsr_me4[2][18][6];

  // the zres vs z plots
  // last array is: 0 offset residual, a.k.a local y (mm)
  //                1 offset antisymmetric part (mm)
  //                2 phiz (mrad) (zres vs phi)
  //                3 slope residual, a.k.a phix (mrad)
  //                4 slope antisymmetric part (mrad)
  //                5 scattering correction (mm)
  TH1F *m_DTzres_vsz_station1[12][6];
  TH1F *m_DTzres_vsz_station2[12][6];
  TH1F *m_DTzres_vsz_station3[12][6];

  // the rphires vs phi plots
  // the last array is: 0 offset residual, a.k.a local x (mm)
  //                    1 offset antisymmetric part (mm)
  //                    2 phiz (mrad) (rphires vs z)
  //                    3 slope residual, a.k.a phiy (mrad)
  //                    4 slope antisymmetric part (mrad)
  //                    5 scattering correction (mm)
  TH1F *m_DTrphires_vsphi_station1[5][6];
  TH1F *m_DTrphires_vsphi_station2[5][6];
  TH1F *m_DTrphires_vsphi_station3[5][6];
  TH1F *m_DTrphires_vsphi_station4[5][6];
  TH1F *m_CSCrphires_vsphi_me11[2][6];
  TH1F *m_CSCrphires_vsphi_me12[2][6];
  TH1F *m_CSCrphires_vsphi_me13[2][6];
  TH1F *m_CSCrphires_vsphi_me14[2][6];
  TH1F *m_CSCrphires_vsphi_me21[2][6];
  TH1F *m_CSCrphires_vsphi_me22[2][6];
  TH1F *m_CSCrphires_vsphi_me31[2][6];
  TH1F *m_CSCrphires_vsphi_me32[2][6];
  TH1F *m_CSCrphires_vsphi_me41[2][6];

  // the zres vs phi plots
  // the last array is: 0 offset residual a.k.a. local y (mm)
  //                    1 offset antisymmetric part (mm)
  //                    2 zpos (mm) (zres vs trackangle, nearly degenerate with z)
  //                    3 slope residual, a.k.a phix (mrad)
  //                    4 slope antisymmetric part (mrad)
  //                    5 scattering correction (mm)
  TH1F *m_DTzres_vsphi_station1[5][6];
  TH1F *m_DTzres_vsphi_station2[5][6];
  TH1F *m_DTzres_vsphi_station3[5][6];

  std::vector<TH1F*> m_DT13hists, m_DT2hists, m_CSChists;

  // profiles for all the offsets and slopes
  std::map<MuonResidualsTwoBin*,std::pair<TProfile*,int> > m_offsetprofs;
  std::map<MuonResidualsTwoBin*,std::pair<TProfile*,int> > m_slopeprofs;

  // and 1-D histogram projections of those profiles
  std::map<MuonResidualsTwoBin*,TH1F*> m_offsethists;
  std::map<MuonResidualsTwoBin*,TH1F*> m_slopehists;

  void book_and_link_up(std::string namestart, std::string titlestart, std::string *lastarray_name, std::string *lastarray_title, TH1F **hist, bool phiz, bool vsphi,
			double maxOffset, double maxZpos, double maxPhiz, double maxSlope,
			int bins, double low, double high, std::string vsname, std::string vstitle);
  void book_vsz(std::string namestart, std::string titlestart, std::string *lastarray_name, std::string *lastarray_title, TH1F **hist, bool phiz, double maxOffset, double maxZpos, double maxPhiz, double maxSlope);
  void book_vsr(std::string namestart, std::string titlestart, std::string *lastarray_name, std::string *lastarray_title, TH1F **hist, bool phiz, double maxOffset, double maxZpos, double maxPhiz, double maxSlope);
  void book_vsphi(std::string namestart, std::string titlestart, std::string *lastarray_name, std::string *lastarray_title, TH1F **hist, bool phiz, double maxOffset, double maxZpos, double maxPhiz, double maxSlope);

  std::map<std::pair<TH1F*,int>,MuonResidualsTwoBin*> m_positionFitters;
  std::map<std::pair<TH1F*,int>,MuonResidualsTwoBin*> m_angleFitters;
  std::map<MuonResidualsTwoBin*,std::pair<TH1F*,int> > m_offsetBin, m_offsetantisymBin, m_zposBin, m_phizBin, m_slopeBin, m_slopeantisymBin, m_scatteringBin;
  std::vector<MuonResidualsTwoBin*> m_allFitters;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// member functions
//

AlignmentMonitorMuonSystemMap::AlignmentMonitorMuonSystemMap(const edm::ParameterSet& cfg)
   : AlignmentMonitorBase(cfg, "AlignmentMonitorMuonSystemMap")
   , m_minTrackPt(cfg.getParameter<double>("minTrackPt"))
   , m_maxTrackPt(cfg.getParameter<double>("maxTrackPt"))
   , m_minTrackerHits(cfg.getParameter<int>("minTrackerHits"))
   , m_maxTrackerRedChi2(cfg.getParameter<double>("maxTrackerRedChi2"))
   , m_allowTIDTEC(cfg.getParameter<bool>("allowTIDTEC"))
   , m_minDT13Hits(cfg.getParameter<int>("minDT13Hits"))
   , m_minDT2Hits(cfg.getParameter<int>("minDT2Hits"))
   , m_minCSCHits(cfg.getParameter<int>("minCSCHits"))
   , m_maxDT13AngleError(cfg.getParameter<double>("maxDT13AngleError"))
   , m_maxDT2AngleError(cfg.getParameter<double>("maxDT2AngleError"))
   , m_maxCSCAngleError(cfg.getParameter<double>("maxCSCAngleError"))
   , m_writeTemporaryFile(cfg.getParameter<std::string>("writeTemporaryFile"))
   , m_readTemporaryFiles(cfg.getParameter<std::vector<std::string> >("readTemporaryFiles"))
   , m_doFits(cfg.getParameter<bool>("doFits"))
   , m_twoBin(cfg.getParameter<bool>("twoBin"))
   , m_minFitHits(cfg.getParameter<int>("minFitHits"))
   , m_strategy(cfg.getParameter<int>("strategy"))
   , m_DT13fitScattering(cfg.getParameter<bool>("DT13fitScattering"))
   , m_DT13fitZpos(cfg.getParameter<bool>("DT13fitZpos"))
   , m_DT13fitPhiz(cfg.getParameter<bool>("DT13fitPhiz"))
   , m_DT2fitScattering(cfg.getParameter<bool>("DT2fitScattering"))
   , m_DT2fitPhiz(cfg.getParameter<bool>("DT2fitPhiz"))
   , m_CSCfitScattering(cfg.getParameter<bool>("CSCfitScattering"))
   , m_CSCfitZpos(cfg.getParameter<bool>("CSCfitZpos"))
   , m_CSCfitPhiz(cfg.getParameter<bool>("CSCfitPhiz"))
{
  std::string model = cfg.getParameter<std::string>("residualsModel");
  if (model == std::string("pureGaussian")) {
    m_residualsModel = MuonResidualsFitter::kPureGaussian;
  }
  else if (model == std::string("powerLawTails")) {
    m_residualsModel = MuonResidualsFitter::kPowerLawTails;
  }
  else if (model == std::string("ROOTVoigt")) {
    m_residualsModel = MuonResidualsFitter::kROOTVoigt;
  }
  else {
    throw cms::Exception("AlignmentMonitorMuonSystemMap") << "residualsModel must be one of \"pureGaussian\", \"powerLawTails\", \"ROOTVoigt\"" << std::endl;
  }
}

void AlignmentMonitorMuonSystemMap::book_and_link_up(std::string namestart, std::string titlestart, std::string *lastarray_name, std::string *lastarray_title, TH1F **hist, bool phiz, bool vsphi,
						     double maxOffset, double maxZpos, double maxPhiz, double maxSlope,
						     int bins, double low, double high, std::string vsname, std::string vstitle) {
  for (int lastarray = 0;  lastarray < 6;  lastarray++) {
    std::stringstream name, title;
    name << namestart << lastarray_name[lastarray] << vsname;
    title << titlestart << lastarray_title[lastarray] << vstitle;
    hist[lastarray] = book1D("/iterN/", name.str().c_str(), title.str().c_str(), bins, low, high);

    if (lastarray == 0) hist[lastarray]->SetAxisRange(-maxOffset, maxOffset, "Y");
    else if (lastarray == 1) hist[lastarray]->SetAxisRange(-maxOffset, maxOffset, "Y");
    else if (lastarray == 2  &&  phiz) hist[lastarray]->SetAxisRange(-maxPhiz, maxPhiz, "Y");
    else if (lastarray == 2  &&  !phiz) hist[lastarray]->SetAxisRange(-maxZpos, maxZpos, "Y");
    else if (lastarray == 3) hist[lastarray]->SetAxisRange(-maxSlope, maxSlope, "Y");
    else if (lastarray == 4) hist[lastarray]->SetAxisRange(-maxSlope, maxSlope, "Y");
    else if (lastarray == 5) hist[lastarray]->SetAxisRange(-3., 3., "Y");
  }

  TProfile *offsetprof = NULL;
  if (true) {
    std::stringstream name, title;
    name << namestart << lastarray_name[0] << vsname << "_prof";
    title << titlestart << lastarray_title[0] << vstitle << " (simple profile)";
    offsetprof = bookProfile("/iterN/", name.str().c_str(), title.str().c_str(), bins, low, high);
    offsetprof->SetAxisRange(-maxOffset, maxOffset, "Y");
  }

  TProfile *slopeprof = NULL;
  if (true) {
    std::stringstream name, title;
    name << namestart << lastarray_name[3] << vsname << "_prof";
    title << titlestart << lastarray_title[3] << vstitle << " (simple profile)";
    slopeprof = bookProfile("/iterN/", name.str().c_str(), title.str().c_str(), bins, low, high);
    slopeprof->SetAxisRange(-maxSlope, maxSlope, "Y");
  }

  TH1F *offsethist = NULL;
  if (true) {
    std::stringstream name, title;
    name << namestart << lastarray_name[0] << vsname << "_hist";
    title << titlestart << lastarray_title[0] << vstitle << " (simple histogram)";
    offsethist = book1D("/iterN/", name.str().c_str(), title.str().c_str(), 100, -maxOffset*5., maxOffset*5.);
  }

  TH1F *slopehist = NULL;
  if (true) {
    std::stringstream name, title;
    name << namestart << lastarray_name[3] << vsname << "_hist";
    title << titlestart << lastarray_title[3] << vstitle << " (simple histogram)";
    slopehist = book1D("/iterN/", name.str().c_str(), title.str().c_str(), 100, -maxSlope*5., maxSlope*5.);
  }

  for (int bin = 1;  bin <= bins;  bin++) {
    MuonResidualsTwoBin *positionFitter = new MuonResidualsTwoBin(m_twoBin, new MuonResidualsPositionFitter(m_residualsModel, m_minFitHits), new MuonResidualsPositionFitter(m_residualsModel, m_minFitHits));
    MuonResidualsTwoBin *angleFitter = new MuonResidualsTwoBin(m_twoBin, new MuonResidualsAngleFitter(m_residualsModel, m_minFitHits), new MuonResidualsAngleFitter(m_residualsModel, m_minFitHits));
    positionFitter->setPrintLevel(-1);
    angleFitter->setPrintLevel(-1);
    positionFitter->setStrategy(m_strategy);
    angleFitter->setStrategy(m_strategy);
    m_allFitters.push_back(positionFitter);
    m_allFitters.push_back(angleFitter);

    // can only allow phiz or zpos to float, which one depends on how the histogram is binned
    if (phiz) positionFitter->fix(MuonResidualsPositionFitter::kZpos);
    else positionFitter->fix(MuonResidualsPositionFitter::kPhiz);

    angleFitter->fix(MuonResidualsAngleFitter::kXControl);
    angleFitter->fix(MuonResidualsAngleFitter::kYControl);

    std::pair<TH1F*,int> index0(hist[0], bin);
    std::pair<TH1F*,int> index1(hist[1], bin);
    std::pair<TH1F*,int> index2(hist[2], bin);
    std::pair<TH1F*,int> index3(hist[3], bin);
    std::pair<TH1F*,int> index4(hist[4], bin);
    std::pair<TH1F*,int> index5(hist[5], bin);
    
    m_positionFitters[index0] = positionFitter;
    m_angleFitters[index0] = angleFitter;
  
    m_offsetBin[positionFitter] = index0;
    m_offsetantisymBin[positionFitter] = index1;
    if (phiz) m_phizBin[positionFitter] = index2;
    else m_zposBin[positionFitter] = index2;
    m_slopeBin[angleFitter] = index3;
    m_slopeantisymBin[angleFitter] = index4;
    m_scatteringBin[positionFitter] = index5;

    m_offsetprofs[positionFitter] = std::pair<TProfile*,int>(offsetprof, bin);
    m_slopeprofs[angleFitter] = std::pair<TProfile*,int>(slopeprof, bin);

    m_offsethists[positionFitter] = offsethist;
    m_slopehists[angleFitter] = slopehist;
  }
}

void AlignmentMonitorMuonSystemMap::book_vsz(std::string namestart, std::string titlestart, std::string *lastarray_name, std::string *lastarray_title, TH1F **hist, bool phiz, double maxOffset, double maxZpos, double maxPhiz, double maxSlope) {
  book_and_link_up(namestart, titlestart, lastarray_name, lastarray_title, hist, phiz, false, maxOffset, maxZpos, maxPhiz, maxSlope, 60, -660, 660, std::string("_vsz"), std::string(" vs global Z"));
}

void AlignmentMonitorMuonSystemMap::book_vsr(std::string namestart, std::string titlestart, std::string *lastarray_name, std::string *lastarray_title, TH1F **hist, bool phiz, double maxOffset, double maxZpos, double maxPhiz, double maxSlope) {
  book_and_link_up(namestart, titlestart, lastarray_name, lastarray_title, hist, phiz, false, maxOffset, maxZpos, maxPhiz, maxSlope, 60, 100, 700, std::string("_vsr"), std::string(" vs global R"));
}

void AlignmentMonitorMuonSystemMap::book_vsphi(std::string namestart, std::string titlestart, std::string *lastarray_name, std::string *lastarray_title, TH1F **hist, bool phiz, double maxOffset, double maxZpos, double maxPhiz, double maxSlope) {
  book_and_link_up(namestart, titlestart, lastarray_name, lastarray_title, hist, phiz, true, maxOffset, maxZpos, maxPhiz, maxSlope, 180, -M_PI, M_PI, std::string("_vsphi"), std::string(" vs global phi"));
}

void AlignmentMonitorMuonSystemMap::book() {
  // histogram vertical window range
  const double maxOffset = 10.;
  const double maxZpos = 10.;
  const double maxPhix = 50.;
  const double maxPhiy = 20.;
  const double maxPhiz = 5.;

  std::string o;
  std::string lastarray_name[6], lastarray_title[6];
  
  lastarray_name[0] = std::string("_rphi_offset");        lastarray_title[0] = std::string(" global rphi residual (mm)");
  lastarray_name[1] = std::string("_rphi_offsetantisym");  lastarray_title[1] = std::string(" charge-antisymmmetric rphi residual (mm)");
  lastarray_name[2] = std::string("_rphi_zpos");          lastarray_title[2] = std::string(" local z correction from local x residuals (mm)");
  lastarray_name[3] = std::string("_rphi_slope");         lastarray_title[3] = std::string(" phiy residual (mrad)");
  lastarray_name[4] = std::string("_rphi_slopeantisym");   lastarray_title[4] = std::string(" charge-antisymmetric phiy residual (mrad)");
  lastarray_name[5] = std::string("_rphi_scattering");    lastarray_title[5] = std::string(" distance to scattering center (m)");

  for (int sector = 1;  sector <= 14;  sector++) {
    if (sector < 10) o = std::string("0");
    else o = std::string("");

    if (sector <= 12) {
      std::stringstream name, title;
      name << "DTst1sec" << o << sector;
      title << "DT station 1 sector " << sector;
      book_vsz(name.str(), title.str(), lastarray_name, lastarray_title, m_DTrphires_vsz_station1[sector-1], false, maxOffset, maxZpos, maxPhiz, maxPhiy);
      m_DT13hists.push_back(m_DTrphires_vsz_station1[sector-1][0]);
    }

    if (sector <= 12) {
      std::stringstream name, title;
      name << "DTst2sec" << o << sector;
      title << "DT station 2 sector " << sector;
      book_vsz(name.str(), title.str(), lastarray_name, lastarray_title, m_DTrphires_vsz_station2[sector-1], false, maxOffset, maxZpos, maxPhiz, maxPhiy);
      m_DT13hists.push_back(m_DTrphires_vsz_station2[sector-1][0]);
    }

    if (sector <= 12) {
      std::stringstream name, title;
      name << "DTst3sec" << o << sector;
      title << "DT station 3 sector " << sector;
      book_vsz(name.str(), title.str(), lastarray_name, lastarray_title, m_DTrphires_vsz_station3[sector-1], false, maxOffset, maxZpos, maxPhiz, maxPhiy);
      m_DT13hists.push_back(m_DTrphires_vsz_station3[sector-1][0]);
    }

    if (true) {
      std::stringstream name, title;
      name << "DTst4sec" << o << sector;
      title << "DT station 4 sector " << sector;
      book_vsz(name.str(), title.str(), lastarray_name, lastarray_title, m_DTrphires_vsz_station4[sector-1], false, maxOffset, maxZpos, maxPhiz, maxPhiy);
      m_DT13hists.push_back(m_DTrphires_vsz_station4[sector-1][0]);
    }
  }

  for (int endcap = 1;  endcap <= 2;  endcap++) {
    for (int chamber = 1;  chamber <= 36;  chamber++) {
      if (chamber < 10) o = std::string("0");
      else o = std::string("");

      if (true) {
	std::stringstream name, title;
	name << "CSCme" << (endcap == 1 ? "p" : "m") << "1ch" << o << chamber;
	title << "CSC ME" << (endcap == 1 ? "+" : "-") << "1 chamber " << chamber;
	book_vsr(name.str(), title.str(), lastarray_name, lastarray_title, m_CSCrphires_vsr_me1[endcap-1][chamber-1], false, maxOffset, maxZpos, maxPhiz, maxPhiy);
	m_CSChists.push_back(m_CSCrphires_vsr_me1[endcap-1][chamber-1][0]);
      }

      if (true) {
	std::stringstream name, title;
	name << "CSCme" << (endcap == 1 ? "p" : "m") << "2ch" << o << chamber;
	title << "CSC ME" << (endcap == 1 ? "+" : "-") << "2 chamber " << chamber;
	book_vsr(name.str(), title.str(), lastarray_name, lastarray_title, m_CSCrphires_vsr_me2[endcap-1][chamber-1], false, maxOffset, maxZpos, maxPhiz, maxPhiy);
	m_CSChists.push_back(m_CSCrphires_vsr_me2[endcap-1][chamber-1][0]);
      }

      if (true) {
	std::stringstream name, title;
	name << "CSCme" << (endcap == 1 ? "p" : "m") << "3ch" << o << chamber;
	title << "CSC ME" << (endcap == 1 ? "+" : "-") << "3 chamber " << chamber;
	book_vsr(name.str(), title.str(), lastarray_name, lastarray_title, m_CSCrphires_vsr_me3[endcap-1][chamber-1], false, maxOffset, maxZpos, maxPhiz, maxPhiy);
	m_CSChists.push_back(m_CSCrphires_vsr_me3[endcap-1][chamber-1][0]);
      }

      if (chamber <= 18) {
	std::stringstream name, title;
	name << "CSCme" << (endcap == 1 ? "p" : "m") << "4ch" << o << chamber;
	title << "CSC ME" << (endcap == 1 ? "+" : "-") << "4 chamber " << chamber;
	book_vsr(name.str(), title.str(), lastarray_name, lastarray_title, m_CSCrphires_vsr_me4[endcap-1][chamber-1], false, maxOffset, maxZpos, maxPhiz, maxPhiy);
	m_CSChists.push_back(m_CSCrphires_vsr_me4[endcap-1][chamber-1][0]);
      }
    }
  }

  lastarray_name[0] = std::string("_z_offset");        lastarray_title[0] = std::string(" global z residual (mm)");
  lastarray_name[1] = std::string("_z_offsetantisym");  lastarray_title[1] = std::string(" charge-antisymmetric z residual (mm)");
  lastarray_name[2] = std::string("_z_phiz");          lastarray_title[2] = std::string(" phiz correction from local y residuals (mrad)");
  lastarray_name[3] = std::string("_z_slope");         lastarray_title[3] = std::string(" phix residual (mrad)");
  lastarray_name[4] = std::string("_z_slopeantisym");   lastarray_title[4] = std::string(" charge-antisymmetric phix residual (mrad)");
  lastarray_name[5] = std::string("_z_scattering");    lastarray_title[5] = std::string(" distance to scattering center (m)");

  for (int sector = 1;  sector <= 14;  sector++) {
    if (sector < 10) o = std::string("0");
    else o = std::string("");

    if (sector <= 12) {
      std::stringstream name, title;
      name << "DTst1sec" << o << sector;
      title << "DT station 1 sector " << sector;
      book_vsz(name.str(), title.str(), lastarray_name, lastarray_title, m_DTzres_vsz_station1[sector-1], true, maxOffset, maxZpos, maxPhiz, maxPhix);
      m_DT2hists.push_back(m_DTzres_vsz_station1[sector-1][0]);
    }

    if (sector <= 12) {
      std::stringstream name, title;
      name << "DTst2sec" << o << sector;
      title << "DT station 2 sector " << sector;
      book_vsz(name.str(), title.str(), lastarray_name, lastarray_title, m_DTzres_vsz_station2[sector-1], true, maxOffset, maxZpos, maxPhiz, maxPhix);
      m_DT2hists.push_back(m_DTzres_vsz_station2[sector-1][0]);
    }

    if (sector <= 12) {
      std::stringstream name, title;
      name << "DTst3sec" << o << sector;
      title << "DT station 3 sector " << sector;
      book_vsz(name.str(), title.str(), lastarray_name, lastarray_title, m_DTzres_vsz_station3[sector-1], true, maxOffset, maxZpos, maxPhiz, maxPhix);
      m_DT2hists.push_back(m_DTzres_vsz_station3[sector-1][0]);
    }
  }

  lastarray_name[0] = std::string("_rphi_offset");        lastarray_title[0] = std::string(" global rphi residual (mm)");
  lastarray_name[1] = std::string("_rphi_offsetantisym");  lastarray_title[1] = std::string(" charge-antisymmetric rphi residual (mm)");
  lastarray_name[2] = std::string("_rphi_phiz");          lastarray_title[2] = std::string(" phiz correction from local x residuals (mrad)");
  lastarray_name[3] = std::string("_rphi_slope");         lastarray_title[3] = std::string(" phiy residual (mrad)");
  lastarray_name[4] = std::string("_rphi_slopeantisym");   lastarray_title[4] = std::string(" charge-antisymmetric phiy residual (mrad)");
  lastarray_name[5] = std::string("_rphi_scattering");    lastarray_title[5] = std::string(" distance to scattering center (m)");

  for (int wheel = -2;  wheel <= 2;  wheel++) {
    std::string wheelname;
    if (wheel == -2) wheelname = std::string("A");
    else if (wheel == -1) wheelname = std::string("B");
    else if (wheel == 0) wheelname = std::string("C");
    else if (wheel == 1) wheelname = std::string("D");
    else if (wheel == 2) wheelname = std::string("E");

    if (true) {
      std::stringstream name, title;
      name << "DTst1wh" << wheelname;
      title << "DT station 1 wheel " << wheel;
      book_vsphi(name.str(), title.str(), lastarray_name, lastarray_title, m_DTrphires_vsphi_station1[wheel+2], true, maxOffset, maxZpos, maxPhiz, maxPhiy);
      m_DT13hists.push_back(m_DTrphires_vsphi_station1[wheel+2][0]);
    }

    if (true) {
      std::stringstream name, title;
      name << "DTst2wh" << wheelname;
      title << "DT station 2 wheel " << wheel;
      book_vsphi(name.str(), title.str(), lastarray_name, lastarray_title, m_DTrphires_vsphi_station2[wheel+2], true, maxOffset, maxZpos, maxPhiz, maxPhiy);
      m_DT13hists.push_back(m_DTrphires_vsphi_station2[wheel+2][0]);
    }

    if (true) {
      std::stringstream name, title;
      name << "DTst3wh" << wheelname;
      title << "DT station 3 wheel " << wheel;
      book_vsphi(name.str(), title.str(), lastarray_name, lastarray_title, m_DTrphires_vsphi_station3[wheel+2], true, maxOffset, maxZpos, maxPhiz, maxPhiy);
      m_DT13hists.push_back(m_DTrphires_vsphi_station3[wheel+2][0]);
    }

    if (true) {
      std::stringstream name, title;
      name << "DTst4wh" << wheelname;
      title << "DT station 4 wheel " << wheel;
      book_vsphi(name.str(), title.str(), lastarray_name, lastarray_title, m_DTrphires_vsphi_station4[wheel+2], true, maxOffset, maxZpos, maxPhiz, maxPhiy);
      m_DT13hists.push_back(m_DTrphires_vsphi_station4[wheel+2][0]);
    }
  }

  for (int endcap = 1;  endcap <= 2;  endcap++) {
    std::string p, plus;
    if (endcap == 1) {
      p = std::string("p");
      plus = std::string("+");
    }
    else {
      p = std::string("m");
      plus = std::string("-");
    }
    book_vsphi(std::string("CSCme") + p + std::string("11"), std::string("CSC ME") + plus + std::string("1/1b"), lastarray_name, lastarray_title, m_CSCrphires_vsphi_me11[endcap-1], true, maxOffset, maxZpos, maxPhiz, maxPhiy);
    book_vsphi(std::string("CSCme") + p + std::string("12"), std::string("CSC ME") + plus + std::string("1/2"), lastarray_name, lastarray_title, m_CSCrphires_vsphi_me12[endcap-1], true, maxOffset, maxZpos, maxPhiz, maxPhiy);
    book_vsphi(std::string("CSCme") + p + std::string("13"), std::string("CSC ME") + plus + std::string("1/3"), lastarray_name, lastarray_title, m_CSCrphires_vsphi_me13[endcap-1], true, maxOffset, maxZpos, maxPhiz, maxPhiy);
    book_vsphi(std::string("CSCme") + p + std::string("14"), std::string("CSC ME") + plus + std::string("1/1a"), lastarray_name, lastarray_title, m_CSCrphires_vsphi_me14[endcap-1], true, maxOffset, maxZpos, maxPhiz, maxPhiy);
    book_vsphi(std::string("CSCme") + p + std::string("21"), std::string("CSC ME") + plus + std::string("2/1"), lastarray_name, lastarray_title, m_CSCrphires_vsphi_me21[endcap-1], true, maxOffset, maxZpos, maxPhiz, maxPhiy);
    book_vsphi(std::string("CSCme") + p + std::string("22"), std::string("CSC ME") + plus + std::string("2/2"), lastarray_name, lastarray_title, m_CSCrphires_vsphi_me22[endcap-1], true, maxOffset, maxZpos, maxPhiz, maxPhiy);
    book_vsphi(std::string("CSCme") + p + std::string("31"), std::string("CSC ME") + plus + std::string("3/1"), lastarray_name, lastarray_title, m_CSCrphires_vsphi_me31[endcap-1], true, maxOffset, maxZpos, maxPhiz, maxPhiy);
    book_vsphi(std::string("CSCme") + p + std::string("32"), std::string("CSC ME") + plus + std::string("3/2"), lastarray_name, lastarray_title, m_CSCrphires_vsphi_me32[endcap-1], true, maxOffset, maxZpos, maxPhiz, maxPhiy);
    book_vsphi(std::string("CSCme") + p + std::string("41"), std::string("CSC ME") + plus + std::string("4/1"), lastarray_name, lastarray_title, m_CSCrphires_vsphi_me41[endcap-1], true, maxOffset, maxZpos, maxPhiz, maxPhiy);

    m_CSChists.push_back(m_CSCrphires_vsphi_me11[endcap-1][0]);
    m_CSChists.push_back(m_CSCrphires_vsphi_me12[endcap-1][0]);
    m_CSChists.push_back(m_CSCrphires_vsphi_me13[endcap-1][0]);
    m_CSChists.push_back(m_CSCrphires_vsphi_me14[endcap-1][0]);
    m_CSChists.push_back(m_CSCrphires_vsphi_me21[endcap-1][0]);
    m_CSChists.push_back(m_CSCrphires_vsphi_me22[endcap-1][0]);
    m_CSChists.push_back(m_CSCrphires_vsphi_me31[endcap-1][0]);
    m_CSChists.push_back(m_CSCrphires_vsphi_me32[endcap-1][0]);
    m_CSChists.push_back(m_CSCrphires_vsphi_me41[endcap-1][0]);
  }

  lastarray_name[0] = std::string("_z_offset");        lastarray_title[0] = std::string(" global z residual (mm)");
  lastarray_name[1] = std::string("_z_offsetantisym");  lastarray_title[1] = std::string(" charge-antisymmetric z residual (mm)");
  lastarray_name[2] = std::string("_z_zpos");          lastarray_title[2] = std::string(" local z correction from local y residuals (mm)");
  lastarray_name[3] = std::string("_z_slope");         lastarray_title[3] = std::string(" phix residual (mrad)");
  lastarray_name[4] = std::string("_z_slopeantisym");   lastarray_title[4] = std::string(" charge-antisymmetric phix residual (mrad)");
  lastarray_name[5] = std::string("_z_scattering");    lastarray_title[5] = std::string(" distance to scattering center (m)");

  for (int wheel = -2;  wheel <= 2;  wheel++) {
    std::string wheelname;
    if (wheel == -2) wheelname = std::string("A");
    else if (wheel == -1) wheelname = std::string("B");
    else if (wheel == 0) wheelname = std::string("C");
    else if (wheel == 1) wheelname = std::string("D");
    else if (wheel == 2) wheelname = std::string("E");

    if (true) {
      std::stringstream name, title;
      name << "DTst1wh" << wheelname;
      title << "DT station 1 wheel " << wheel;
      book_vsphi(name.str(), title.str(), lastarray_name, lastarray_title, m_DTzres_vsphi_station1[wheel+2], false, maxOffset, maxZpos, maxPhiz, maxPhix);
      m_DT2hists.push_back(m_DTzres_vsphi_station1[wheel+2][0]);
    }

    if (true) {
      std::stringstream name, title;
      name << "DTst2wh" << wheelname;
      title << "DT station 2 wheel " << wheel;
      book_vsphi(name.str(), title.str(), lastarray_name, lastarray_title, m_DTzres_vsphi_station2[wheel+2], false, maxOffset, maxZpos, maxPhiz, maxPhix);
      m_DT2hists.push_back(m_DTzres_vsphi_station2[wheel+2][0]);
    }

    if (true) {
      std::stringstream name, title;
      name << "DTst3wh" << wheelname;
      title << "DT station 3 wheel " << wheel;
      book_vsphi(name.str(), title.str(), lastarray_name, lastarray_title, m_DTzres_vsphi_station3[wheel+2], false, maxOffset, maxZpos, maxPhiz, maxPhix);
      m_DT2hists.push_back(m_DTzres_vsphi_station3[wheel+2][0]);
    }
  }

  for (std::vector<TH1F*>::const_iterator hist = m_DT13hists.begin();  hist != m_DT13hists.end();  ++hist) {
    for (int i = 1;  i <= (*hist)->GetNbinsX();  i++) {
      std::pair<TH1F*,int> index(*hist, i);
      MuonResidualsTwoBin *posfitter = m_positionFitters[index];
      // MuonResidualsTwoBin *angfitter = m_angleFitters[index];
      if (!m_DT13fitScattering) posfitter->fix(MuonResidualsPositionFitter::kScattering);
      if (!m_DT13fitZpos) posfitter->fix(MuonResidualsPositionFitter::kZpos);
      if (!m_DT13fitPhiz) posfitter->fix(MuonResidualsPositionFitter::kPhiz);
    }
  }

  for (std::vector<TH1F*>::const_iterator hist = m_DT2hists.begin();  hist != m_DT2hists.end();  ++hist) {
    for (int i = 1;  i <= (*hist)->GetNbinsX();  i++) {
      std::pair<TH1F*,int> index(*hist, i);
      MuonResidualsTwoBin *posfitter = m_positionFitters[index];
      // MuonResidualsTwoBin *angfitter = m_angleFitters[index];
      if (!m_DT2fitScattering) posfitter->fix(MuonResidualsPositionFitter::kScattering);
      posfitter->fix(MuonResidualsPositionFitter::kZpos);
      if (!m_DT2fitPhiz) posfitter->fix(MuonResidualsPositionFitter::kPhiz);
    }
  }

  for (std::vector<TH1F*>::const_iterator hist = m_CSChists.begin();  hist != m_CSChists.end();  ++hist) {
    for (int i = 1;  i <= (*hist)->GetNbinsX();  i++) {
      std::pair<TH1F*,int> index(*hist, i);
      MuonResidualsTwoBin *posfitter = m_positionFitters[index];
      // MuonResidualsTwoBin *angfitter = m_angleFitters[index];
      if (!m_CSCfitScattering) posfitter->fix(MuonResidualsPositionFitter::kScattering);
      if (!m_CSCfitZpos) posfitter->fix(MuonResidualsPositionFitter::kZpos);
      if (!m_CSCfitPhiz) posfitter->fix(MuonResidualsPositionFitter::kPhiz);
    }
  }

}

void AlignmentMonitorMuonSystemMap::event(const edm::Event &iEvent, const edm::EventSetup &iSetup, const ConstTrajTrackPairCollection& trajtracks) {
  edm::ESHandle<GlobalTrackingGeometry> globalGeometry;
  iSetup.get<GlobalTrackingGeometryRecord>().get(globalGeometry);

  for (ConstTrajTrackPairCollection::const_iterator trajtrack = trajtracks.begin();  trajtrack != trajtracks.end();  ++trajtrack) {
    const Trajectory* traj = (*trajtrack).first;
    const reco::Track* track = (*trajtrack).second;

    if (m_minTrackPt < track->pt()  &&  track->pt() < m_maxTrackPt) {
      char charge = (track->charge() > 0 ? 1 : -1);
      // double qoverpt = track->charge() / track->pt();
      // double qoverpz = track->charge() / track->pz();
      MuonResidualsFromTrack muonResidualsFromTrack(globalGeometry, traj, pNavigator(), 1000.);

      if (muonResidualsFromTrack.trackerNumHits() >= m_minTrackerHits  &&  muonResidualsFromTrack.trackerRedChi2() < m_maxTrackerRedChi2  &&  (m_allowTIDTEC  ||  !muonResidualsFromTrack.contains_TIDTEC())) {
	std::vector<DetId> chamberIds = muonResidualsFromTrack.chamberIds();

	for (std::vector<DetId>::const_iterator chamberId = chamberIds.begin();  chamberId != chamberIds.end();  ++chamberId) {
	  bool okay = false;
	  TH1F *hist_vszr = NULL;
	  TH1F *hist_vsphi = NULL;
	  double residual = 0.;
	  double resslope = 0.;
	  double trackangle = 0.;
	  double trackposition = 0.;
	  double trackdxdz = 0.;
	  double trackdydz = 0.;
	  double angleError = 0.;
	  double maxAngleError = 0.;
	  GlobalPoint trackpos;
	  double signConvention = 0.;

	  if (chamberId->det() == DetId::Muon  &&  chamberId->subdetId() == MuonSubdetId::DT) {
	    MuonChamberResidual *dt13 = muonResidualsFromTrack.chamberResidual(*chamberId, MuonChamberResidual::kDT13);
	    MuonChamberResidual *dt2 = muonResidualsFromTrack.chamberResidual(*chamberId, MuonChamberResidual::kDT2);

	    if (dt13 != NULL  &&  dt13->numHits() >= m_minDT13Hits) {
	      trackpos = dt13->global_trackpos();
	      signConvention = dt13->signConvention();

	      okay = true;
	      angleError = dt13->resslope();
	      maxAngleError = m_maxDT13AngleError;

	      DTChamberId chamberId(dt13->chamberId().rawId());
	      assert(1 <= chamberId.sector()  &&  chamberId.sector() <= 14  &&  (chamberId.station() == 4  ||  chamberId.sector() <= 12));
	      assert(-2 <= chamberId.wheel()  &&  chamberId.wheel() <= 2);
	      if (chamberId.station() == 1) {
		hist_vszr = m_DTrphires_vsz_station1[chamberId.sector()-1][0];
		hist_vsphi = m_DTrphires_vsphi_station1[chamberId.wheel()+2][0];
	      }
	      else if (chamberId.station() == 2) {
		hist_vszr = m_DTrphires_vsz_station2[chamberId.sector()-1][0];
		hist_vsphi = m_DTrphires_vsphi_station2[chamberId.wheel()+2][0];
	      }
	      else if (chamberId.station() == 3) {
		hist_vszr = m_DTrphires_vsz_station3[chamberId.sector()-1][0];
		hist_vsphi = m_DTrphires_vsphi_station3[chamberId.wheel()+2][0];
	      }
	      else if (chamberId.station() == 4) {
		hist_vszr = m_DTrphires_vsz_station4[chamberId.sector()-1][0];
		hist_vsphi = m_DTrphires_vsphi_station4[chamberId.wheel()+2][0];
	      }
	      else assert(false);
	      assert(hist_vszr != NULL  &&  hist_vsphi != NULL);

	      residual = dt13->residual();
	      resslope = dt13->resslope();
	      trackangle = dt13->trackdxdz();
	      trackposition = dt13->tracky();
	      trackdxdz = dt13->trackdxdz();
	      trackdydz = dt13->trackdydz();
	    } // end if DT13

	    if (dt2 != NULL  &&  dt2->numHits() >= m_minDT2Hits) {
	      trackpos = dt2->global_trackpos();
	      signConvention = dt2->signConvention();

	      okay = true;
	      angleError = dt2->resslope();
	      maxAngleError = m_maxDT2AngleError;

	      DTChamberId chamberId(dt2->chamberId().rawId());
	      assert(1 <= chamberId.sector()  &&  chamberId.sector() <= 14  &&  (chamberId.station() == 4  ||  chamberId.sector() <= 12));
	      assert(-2 <= chamberId.wheel()  &&  chamberId.wheel() <= 2);
	      if (chamberId.station() == 1) {
		hist_vszr = m_DTzres_vsz_station1[chamberId.sector()-1][0];
		hist_vsphi = m_DTzres_vsphi_station1[chamberId.wheel()+2][0];
	      }
	      else if (chamberId.station() == 2) {
		hist_vszr = m_DTzres_vsz_station2[chamberId.sector()-1][0];
		hist_vsphi = m_DTzres_vsphi_station2[chamberId.wheel()+2][0];
	      }
	      else if (chamberId.station() == 3) {
		hist_vszr = m_DTzres_vsz_station3[chamberId.sector()-1][0];
		hist_vsphi = m_DTzres_vsphi_station3[chamberId.wheel()+2][0];
	      }
	      else assert(false);
	      assert(hist_vszr != NULL  &&  hist_vsphi != NULL);

	      residual = dt2->residual();
	      resslope = dt2->resslope();
	      trackangle = dt2->trackdydz();
	      trackposition = dt2->trackx();
	      trackdxdz = dt2->trackdxdz();
	      trackdydz = dt2->trackdydz();
	    } // end if DT2
	  } // end if DT

	  else if (chamberId->det() == DetId::Muon  &&  chamberId->subdetId() == MuonSubdetId::CSC) {
	    MuonChamberResidual *csc = muonResidualsFromTrack.chamberResidual(*chamberId, MuonChamberResidual::kCSC);

	    if (csc->numHits() >= m_minCSCHits) {
	      okay = true;
	      angleError = csc->resslope();
	      maxAngleError = m_maxCSCAngleError;

	      trackpos = csc->global_trackpos();
	      signConvention = csc->signConvention();

	      CSCDetId chamberId(csc->chamberId().rawId());
	      assert(1 <= chamberId.endcap()  &&  chamberId.endcap() <= 2);
	      assert(1 <= chamberId.chamber()  &&  chamberId.chamber() <= 36  &&  (chamberId.station() == 1  ||  chamberId.ring() == 2  ||  chamberId.chamber() <= 18));
	      if (chamberId.station() == 1  &&  chamberId.ring() == 1) {
		hist_vszr = m_CSCrphires_vsr_me1[chamberId.endcap()-1][chamberId.chamber()-1][0];
		hist_vsphi = m_CSCrphires_vsphi_me11[chamberId.endcap()-1][0];
	      }
	      else if (chamberId.station() == 1  &&  chamberId.ring() == 2) {
		hist_vszr = m_CSCrphires_vsr_me1[chamberId.endcap()-1][chamberId.chamber()-1][0];
		hist_vsphi = m_CSCrphires_vsphi_me12[chamberId.endcap()-1][0];
	      }
	      else if (chamberId.station() == 1  &&  chamberId.ring() == 3) {
		hist_vszr = m_CSCrphires_vsr_me1[chamberId.endcap()-1][chamberId.chamber()-1][0];
		hist_vsphi = m_CSCrphires_vsphi_me13[chamberId.endcap()-1][0];
	      }
	      else if (chamberId.station() == 1  &&  chamberId.ring() == 4) {
		hist_vszr = m_CSCrphires_vsr_me1[chamberId.endcap()-1][chamberId.chamber()-1][0];
		hist_vsphi = m_CSCrphires_vsphi_me14[chamberId.endcap()-1][0];
	      }
	      else if (chamberId.station() == 2  &&  chamberId.ring() == 1) {
		hist_vszr = m_CSCrphires_vsr_me2[chamberId.endcap()-1][(chamberId.chamber()-1)*2][0];
		hist_vsphi = m_CSCrphires_vsphi_me21[chamberId.endcap()-1][0];
	      }
	      else if (chamberId.station() == 2  &&  chamberId.ring() == 2) {
		hist_vszr = m_CSCrphires_vsr_me2[chamberId.endcap()-1][chamberId.chamber()-1][0];
		hist_vsphi = m_CSCrphires_vsphi_me22[chamberId.endcap()-1][0];
	      }
	      else if (chamberId.station() == 3  &&  chamberId.ring() == 1) {
		hist_vszr = m_CSCrphires_vsr_me3[chamberId.endcap()-1][(chamberId.chamber()-1)*2][0];
		hist_vsphi = m_CSCrphires_vsphi_me31[chamberId.endcap()-1][0];
	      }
	      else if (chamberId.station() == 3  &&  chamberId.ring() == 2) {
		hist_vszr = m_CSCrphires_vsr_me3[chamberId.endcap()-1][chamberId.chamber()-1][0];
		hist_vsphi = m_CSCrphires_vsphi_me32[chamberId.endcap()-1][0];
	      }
	      else if (chamberId.station() == 4  &&  chamberId.ring() == 1) {
		hist_vszr = m_CSCrphires_vsr_me4[chamberId.endcap()-1][chamberId.chamber()-1][0];
		hist_vsphi = m_CSCrphires_vsphi_me41[chamberId.endcap()-1][0];
	      }
	      else assert(false);
	      assert(hist_vszr != NULL  &&  hist_vsphi != NULL);

	      residual = csc->residual();
	      resslope = csc->resslope();
	      trackangle = csc->trackdxdz();
	      trackposition = csc->tracky();
	      trackdxdz = csc->trackdxdz();
	      trackdydz = csc->trackdydz();
	    }
	  } // end if CSC

	  if (okay) { // we have enough information to fill the fitters
	    int bin_vszr;
	    if (chamberId->subdetId() == MuonSubdetId::DT) bin_vszr = hist_vszr->FindBin(trackpos.z());
	    else bin_vszr = hist_vszr->FindBin(sqrt(pow(trackpos.x(), 2) + pow(trackpos.y(), 2)));

	    int bin_vsphi = hist_vsphi->FindBin(trackpos.phi());

	    if (1 <= bin_vszr  &&  bin_vszr <= hist_vszr->GetNbinsX()) {
	      std::map<std::pair<TH1F*,int>,MuonResidualsTwoBin*>::const_iterator positionFitter_vsz = m_positionFitters.find(std::pair<TH1F*,int>(hist_vszr, bin_vszr));
	      std::map<std::pair<TH1F*,int>,MuonResidualsTwoBin*>::const_iterator angleFitter_vsz = m_angleFitters.find(std::pair<TH1F*,int>(hist_vszr, bin_vszr));

	      if (positionFitter_vsz != m_positionFitters.end()) {
		assert(maxAngleError != 0.);
		if (fabs(angleError) < maxAngleError) {
		  double *residdata = new double[MuonResidualsPositionFitter::kNData];
		  residdata[MuonResidualsPositionFitter::kResidual] = residual * signConvention;
		  residdata[MuonResidualsPositionFitter::kAngleError] = resslope;
		  residdata[MuonResidualsPositionFitter::kTrackAngle] = trackangle;
		  residdata[MuonResidualsPositionFitter::kTrackPosition] = trackposition;
		  positionFitter_vsz->second->fill(charge, residdata);
		  // the MuonResidualsFitter will delete the array when it is destroyed
		}
	      }
	      else assert(false);  // to catch programming errors

	      if (angleFitter_vsz != m_angleFitters.end()) {
		double *residdata = new double[MuonResidualsAngleFitter::kNData];
		residdata[MuonResidualsAngleFitter::kResidual] = resslope * signConvention;
		residdata[MuonResidualsAngleFitter::kXAngle] = trackdxdz;
		residdata[MuonResidualsAngleFitter::kYAngle] = trackdydz;
		angleFitter_vsz->second->fill(charge, residdata);
	      }
	      else assert(false);
	    }

	    if (1 <= bin_vsphi  &&  bin_vsphi <= hist_vsphi->GetNbinsX()) {
	      std::map<std::pair<TH1F*,int>,MuonResidualsTwoBin*>::const_iterator positionFitter_vsphi = m_positionFitters.find(std::pair<TH1F*,int>(hist_vsphi, bin_vsphi));
	      std::map<std::pair<TH1F*,int>,MuonResidualsTwoBin*>::const_iterator angleFitter_vsphi = m_angleFitters.find(std::pair<TH1F*,int>(hist_vsphi, bin_vsphi));

	      if (positionFitter_vsphi != m_positionFitters.end()) {
		assert(maxAngleError != 0.);
		if (fabs(angleError) < maxAngleError) {
		  double *residdata = new double[MuonResidualsPositionFitter::kNData];
		  residdata[MuonResidualsPositionFitter::kResidual] = residual * signConvention;
		  residdata[MuonResidualsPositionFitter::kAngleError] = resslope;
		  residdata[MuonResidualsPositionFitter::kTrackAngle] = trackangle;
		  residdata[MuonResidualsPositionFitter::kTrackPosition] = trackposition;
		  positionFitter_vsphi->second->fill(charge, residdata);
		  // this record must be separate because both MuonResidualsFitters will delete their contents
		}
	      }
	      else assert(false);

	      if (angleFitter_vsphi != m_angleFitters.end()) {
		double *residdata = new double[MuonResidualsAngleFitter::kNData];
		residdata[MuonResidualsAngleFitter::kResidual] = resslope * signConvention;
		residdata[MuonResidualsAngleFitter::kXAngle] = trackdxdz;
		residdata[MuonResidualsAngleFitter::kYAngle] = trackdydz;
		angleFitter_vsphi->second->fill(charge, residdata);
	      }
	      else assert(false);
	    }

	  } // end if okay

	} // end loop over chamberIds
      } // end if refit is okay
    } // end if track pT is within range
  } // end loop over tracks
}

void AlignmentMonitorMuonSystemMap::afterAlignment(const edm::EventSetup &iSetup) {
  // collect temporary files
  if (m_readTemporaryFiles.size() != 0) {
    for (std::vector<std::string>::const_iterator fileName = m_readTemporaryFiles.begin();  fileName != m_readTemporaryFiles.end();  ++fileName) {
      FILE *file;
      int size;
      file = fopen(fileName->c_str(), "r");
      fread(&size, sizeof(int), 1, file);
      if (int(m_allFitters.size()) != size) throw cms::Exception("AlignmentMonitorMuonSystemMap") << "file \"" << *fileName << "\" has " << size << " fitters, but this job has " << m_allFitters.size() << " fitters (probably corresponds to the wrong plotting job)" << std::endl;

      std::vector<MuonResidualsTwoBin*>::const_iterator fitter = m_allFitters.begin();
      for (int i = 0;  i < size;  ++i, ++fitter) {
	(*fitter)->read(file, i);
      }

      fclose(file);
    }
  }

  if (m_doFits) {
    // run all of the position fitters and put their results into the histograms
    for (std::map<std::pair<TH1F*,int>,MuonResidualsTwoBin*>::const_iterator fitter = m_positionFitters.begin();  fitter != m_positionFitters.end();  ++fitter) {
      std::map<MuonResidualsTwoBin*,std::pair<TH1F*,int> >::const_iterator offsetBin = m_offsetBin.find((*fitter).second);
      std::map<MuonResidualsTwoBin*,std::pair<TH1F*,int> >::const_iterator offsetantisymBin = m_offsetantisymBin.find((*fitter).second);
      std::map<MuonResidualsTwoBin*,std::pair<TH1F*,int> >::const_iterator scatteringBin = m_scatteringBin.find((*fitter).second);
      std::map<MuonResidualsTwoBin*,std::pair<TH1F*,int> >::const_iterator zposBin = m_zposBin.find((*fitter).second);
      std::map<MuonResidualsTwoBin*,std::pair<TH1F*,int> >::const_iterator phizBin = m_phizBin.find((*fitter).second);

      double offsetValue = 2e10;
      double offsetError = 1e10;
      double offsetAntisym = 2e10;
      double scatteringValue = 2e10;
      double scatteringError = 1e10;
      double zposValue = 2e10;
      double zposError = 1e10;
      double phizValue = 2e10;
      double phizError = 1e10;

//       // the fit is verbose in std::cout anyway
//       std::cout << "=====================================================================================================" << std::endl;
//       std::cout << "Fitting " << offsetBin->second.first->GetTitle() << " bin " << offsetBin->second.second << " (" << fitter->second->numResidualsPos() << " + " << fitter->second->numResidualsNeg() << " super-residuals)" << std::endl;
      if (fitter->second->fit(NULL)) {
	offsetValue = fitter->second->value(MuonResidualsPositionFitter::kPosition) * 10.;                // convert from cm to mm
	offsetError = fitter->second->error(MuonResidualsPositionFitter::kPosition) * 10.;
	offsetAntisym = fitter->second->antisym(MuonResidualsPositionFitter::kPosition) * 10.;
	scatteringValue = fitter->second->value(MuonResidualsPositionFitter::kScattering) / 100.;         // convert from cm to m
	scatteringError = fitter->second->error(MuonResidualsPositionFitter::kScattering) / 100.;
	zposValue = fitter->second->value(MuonResidualsPositionFitter::kZpos) * 10.;                      // convert from cm to mm
	zposError = fitter->second->error(MuonResidualsPositionFitter::kZpos) * 10.;
	phizValue = fitter->second->value(MuonResidualsPositionFitter::kPhiz) * 1000.;                    // convert from radians to mrad
	phizError = fitter->second->error(MuonResidualsPositionFitter::kPhiz) * 1000.;
      }

      if (offsetBin != m_offsetBin.end()) {
	offsetBin->second.first->SetBinContent(offsetBin->second.second, offsetValue);
	offsetBin->second.first->SetBinError(offsetBin->second.second, offsetError);
      }
      else assert(false);
    
      if (offsetantisymBin != m_offsetantisymBin.end()) {
	offsetantisymBin->second.first->SetBinContent(offsetantisymBin->second.second, offsetAntisym);
	offsetantisymBin->second.first->SetBinError(offsetantisymBin->second.second, offsetError);
      }
      else assert(false);
    
      if (scatteringBin != m_scatteringBin.end()) {
	scatteringBin->second.first->SetBinContent(scatteringBin->second.second, scatteringValue);
	scatteringBin->second.first->SetBinError(scatteringBin->second.second, scatteringError);
      }
      else assert(false);
    
      if (zposBin != m_zposBin.end()) {
	zposBin->second.first->SetBinContent(zposBin->second.second, zposValue);
	zposBin->second.first->SetBinError(zposBin->second.second, zposError);
      }
      // zpos is fixed for this fitter
    
      if (phizBin != m_phizBin.end()) {
	phizBin->second.first->SetBinContent(phizBin->second.second, phizValue);
	phizBin->second.first->SetBinError(phizBin->second.second, phizError);
      }
      // phiz is fixed for this fitter
    }
  
    // run all of the angle fitters and put their results into the histograms
    for (std::map<std::pair<TH1F*,int>,MuonResidualsTwoBin*>::const_iterator fitter = m_angleFitters.begin();  fitter != m_angleFitters.end();  ++fitter) {
      std::map<MuonResidualsTwoBin*,std::pair<TH1F*,int> >::const_iterator slopeBin = m_slopeBin.find((*fitter).second);
      std::map<MuonResidualsTwoBin*,std::pair<TH1F*,int> >::const_iterator slopeantisymBin = m_slopeantisymBin.find((*fitter).second);

      double slopeValue = 2e10;
      double slopeError = 1e10;
      double slopeAntisym = 2e10;

//       // the fit is verbose in std::cout anyway
//       std::cout << "=====================================================================================================" << std::endl;
//       std::cout << "Fitting " << slopeBin->second.first->GetTitle() << " bin " << slopeBin->second.second << " (" << fitter->second->numResidualsPos() << " + " << fitter->second->numResidualsNeg() << " super-residuals)" << std::endl;
      if (fitter->second->fit(NULL)) {
	slopeValue = fitter->second->value(MuonResidualsAngleFitter::kAngle) * 1000.;                     // convert from radians to mrad
	slopeError = fitter->second->error(MuonResidualsAngleFitter::kAngle) * 1000.;
	slopeAntisym = fitter->second->antisym(MuonResidualsAngleFitter::kAngle) * 1000.;
      }

      if (slopeBin != m_slopeBin.end()) {
	slopeBin->second.first->SetBinContent(slopeBin->second.second, slopeValue);
	slopeBin->second.first->SetBinError(slopeBin->second.second, slopeError);
      }
      else assert(false);

      if (slopeantisymBin != m_slopeantisymBin.end()) {
	slopeantisymBin->second.first->SetBinContent(slopeantisymBin->second.second, slopeAntisym);
	slopeantisymBin->second.first->SetBinError(slopeantisymBin->second.second, slopeError);
      }
      else assert(false);
    }
  } // end doFits

  // fill the profiles and simple histograms whether or not you actually do fitting
  for (std::map<MuonResidualsTwoBin*,std::pair<TProfile*,int> >::const_iterator fitter = m_offsetprofs.begin();  fitter != m_offsetprofs.end();  ++fitter) {
    TProfile* prof = fitter->second.first;
    int bin = fitter->second.second;
    double center = prof->GetBinCenter(bin);

    for (std::vector<double*>::const_iterator residiter = fitter->first->residualsPos_begin();  residiter != fitter->first->residualsPos_end();  ++residiter) {
      prof->Fill(center, (*residiter)[MuonResidualsPositionFitter::kResidual] * 10.);
    }
    for (std::vector<double*>::const_iterator residiter = fitter->first->residualsNeg_begin();  residiter != fitter->first->residualsNeg_end();  ++residiter) {
      prof->Fill(center, (*residiter)[MuonResidualsPositionFitter::kResidual] * 10.);
    }
  }
  for (std::map<MuonResidualsTwoBin*,std::pair<TProfile*,int> >::const_iterator fitter = m_slopeprofs.begin();  fitter != m_slopeprofs.end();  ++fitter) {
    TProfile* prof = fitter->second.first;
    int bin = fitter->second.second;
    double center = prof->GetBinCenter(bin);

    for (std::vector<double*>::const_iterator residiter = fitter->first->residualsPos_begin();  residiter != fitter->first->residualsPos_end();  ++residiter) {
      prof->Fill(center, (*residiter)[MuonResidualsAngleFitter::kResidual] * 1000.);
    }
    for (std::vector<double*>::const_iterator residiter = fitter->first->residualsNeg_begin();  residiter != fitter->first->residualsNeg_end();  ++residiter) {
      prof->Fill(center, (*residiter)[MuonResidualsAngleFitter::kResidual] * 1000.);
    }
  }
  for (std::map<MuonResidualsTwoBin*,TH1F*>::const_iterator fitter = m_offsethists.begin();  fitter != m_offsethists.end();  ++fitter) {
    TH1F* hist = fitter->second;
    for (std::vector<double*>::const_iterator residiter = fitter->first->residualsPos_begin();  residiter != fitter->first->residualsPos_end();  ++residiter) {
      hist->Fill((*residiter)[MuonResidualsPositionFitter::kResidual] * 10.);
    }
    for (std::vector<double*>::const_iterator residiter = fitter->first->residualsNeg_begin();  residiter != fitter->first->residualsNeg_end();  ++residiter) {
      hist->Fill((*residiter)[MuonResidualsPositionFitter::kResidual] * 10.);
    }
  }
  for (std::map<MuonResidualsTwoBin*,TH1F*>::const_iterator fitter = m_slopehists.begin();  fitter != m_slopehists.end();  ++fitter) {
    TH1F* hist = fitter->second;
    for (std::vector<double*>::const_iterator residiter = fitter->first->residualsPos_begin();  residiter != fitter->first->residualsPos_end();  ++residiter) {
      hist->Fill((*residiter)[MuonResidualsAngleFitter::kResidual] * 1000.);
    }
    for (std::vector<double*>::const_iterator residiter = fitter->first->residualsNeg_begin();  residiter != fitter->first->residualsNeg_end();  ++residiter) {
      hist->Fill((*residiter)[MuonResidualsAngleFitter::kResidual] * 1000.);
    }
  }

  // write out the pseudontuples for a later job to collect
  if (m_writeTemporaryFile != std::string("")) {
    FILE *file;
    file = fopen(m_writeTemporaryFile.c_str(), "w");
    int size = m_allFitters.size();
    fwrite(&size, sizeof(int), 1, file);

    std::vector<MuonResidualsTwoBin*>::const_iterator fitter = m_allFitters.begin();
    for (int i = 0;  i < size;  ++i, ++fitter) {
      (*fitter)->write(file, i);
    }

    fclose(file);
  }
}

//
// constructors and destructor
//

// AlignmentMonitorMuonSystemMap::AlignmentMonitorMuonSystemMap(const AlignmentMonitorMuonSystemMap& rhs)
// {
//    // do actual copying here;
// }

//
// assignment operators
//
// const AlignmentMonitorMuonSystemMap& AlignmentMonitorMuonSystemMap::operator=(const AlignmentMonitorMuonSystemMap& rhs)
// {
//   //An exception safe implementation is
//   AlignmentMonitorMuonSystemMap temp(rhs);
//   swap(rhs);
//
//   return *this;
// }

//
// const member functions
//

//
// static member functions
//

//
// SEAL definitions
//

DEFINE_EDM_PLUGIN(AlignmentMonitorPluginFactory, AlignmentMonitorMuonSystemMap, "AlignmentMonitorMuonSystemMap");
