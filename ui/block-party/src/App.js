import React from 'react';
import './App.css';
import RobotStatusCard from './components/RobotStatusCard';
import ControlBoardCard from './components/ControlBoardCard';
import VideoSection from './components/VideoSection';
import DesignOverview from './components/DesignOverview';

function App() {
  return (
    <div className="app-container">
      {/* LEFT PANEL */}
      <div className="left-panel">
        <div className="logo-block">
        <img className="block-party" src="block-party.svg" alt="Block Party Logo" />
        </div>
        
        {/* Robot Status Card */}
        <RobotStatusCard />
        
        {/* Control Board Card */}
        <ControlBoardCard />
      </div>

      {/* MAIN CONTENT AREA */}
      <div className="main-content">
        {/* Video Section */}
        <VideoSection />

        {/* Design Overview */}
        <DesignOverview />

        {/* Footer (Logos at the bottom, for example) */}
        <div className="footer-logos">
          <img className="block-party" src="block-party.svg" alt="Block Party Logo" />
          <img src="GeorgiaTech_White.svg" alt="Georgia Tech Logo" />
        </div>
      </div>
    </div>
  );
}

export default App;