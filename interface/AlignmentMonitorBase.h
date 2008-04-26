#ifndef CommonAlignmentMonitor_AlignmentMonitorBase_h
#define CommonAlignmentMonitor_AlignmentMonitorBase_h
// -*- C++ -*-
//
// Package:     CommonAlignmentMonitor
// Class  :     AlignmentMonitorBase
// 
/**\class AlignmentMonitorBase AlignmentMonitorBase.h Alignment/CommonAlignmentMonitor/interface/AlignmentMonitorBase.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Original Author:  Jim Pivarski
//         Created:  Fri Mar 30 12:21:02 CDT 2007
// $Id: AlignmentMonitorBase.h,v 1.4 2007/12/04 23:29:26 ratnik Exp $
//

// system include files

#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "Alignment/CommonAlignment/interface/AlignableNavigator.h"
#include "Alignment/TrackerAlignment/interface/AlignableTracker.h"
#include "Alignment/MuonAlignment/interface/AlignableMuon.h"
#include <FWCore/Framework/interface/Frameworkfwd.h> 
#include "Alignment/CommonAlignmentAlgorithm/interface/AlignmentParameterStore.h"
#include "DataFormats/TrackReco/interface/Track.h"

// user include files
#include "PhysicsTools/UtilAlgos/interface/TFileDirectory.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TTree.h"

// forward declarations

class AlignmentMonitorBase {
   public:
      typedef std::pair<const Trajectory*, const reco::Track*> ConstTrajTrackPair; 
      typedef std::vector<ConstTrajTrackPair>  ConstTrajTrackPairCollection;

      /// Constructor
      AlignmentMonitorBase(const edm::ParameterSet &cfg, std::string name = "INVALID");
      
      /// Destructor
      virtual ~AlignmentMonitorBase() {}

      /// Called at beginning of job: don't reimplement
      void beginOfJob(AlignableTracker *pTracker, AlignableMuon *pMuon, AlignmentParameterStore *pStore);

      /// Called at beginning of loop: don't reimplement
      void startingNewLoop();

      /// Called for each event: don't reimplement
      void duringLoop(const edm::EventSetup &iSetup, const ConstTrajTrackPairCollection &iTrajTracks);

      /// Called at end of loop: don't reimplement
      void endOfLoop(const edm::EventSetup &iSetup);

      /// Called at end of processing: don't implement
      void endOfJob();

      /////////////////////////////////////////////////////

      /// Book or retrieve histograms; MUST be reimplemented
      virtual void book() = 0;

      /// Called for each event (by "run()"): may be reimplemented
      virtual void event(const edm::EventSetup &iSetup, const ConstTrajTrackPairCollection &iTrajTracks) { };

      /// Called after updating AlignableTracker and AlignableMuon (by
      /// "endOfLoop()"): may be reimplemented
      virtual void afterAlignment(const edm::EventSetup &iSetup) { };

   protected:
      /// Use this every time you book a histogram (so that
      /// AlignmentMonitorBase can find your histograms in a
      /// collector (parallel-processing) job)
      TH1F *book1D(std::string dir, std::string name, std::string title, int nchX, double lowX, double highX);
      TProfile *bookProfile(std::string dir, std::string name, std::string title, int nchX, double lowX, double highX, int nchY=1, double lowY=0., double highY=0., char *option="s");
      TTree *bookTree(std::string dir, std::string name, std::string title);

      int                     iteration()    { return m_iteration; };
      AlignableTracker        *pTracker()    { return mp_tracker; };
      AlignableMuon           *pMuon()       { return mp_muon; };
      AlignmentParameterStore *pStore()      { return mp_store; };
      AlignableNavigator      *pNavigator()  { return mp_navigator; };

   private:
      AlignmentMonitorBase(const AlignmentMonitorBase&); // stop default
      const AlignmentMonitorBase& operator=(const AlignmentMonitorBase&); // stop default

      TFileDirectory *directory(std::string dir);
      
      // ---------- member data --------------------------------

      int m_iteration;
      AlignableTracker         *mp_tracker;
      AlignableMuon            *mp_muon;
      AlignmentParameterStore  *mp_store;
      AlignableNavigator       *mp_navigator;

      std::map<std::vector<std::string>, TFileDirectory*> m_baseDirMap, m_iterDirMap;
};


#endif
