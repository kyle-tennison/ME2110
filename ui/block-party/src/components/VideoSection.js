import React from 'react';

function VideoSection() {
  return (
    <div className="video-section">
      <video width="100%" height="auto" autoPlay loop muted playsInline>
        <source src="video.mp4" type="video/mp4" />
        Your browser does not support the video tag.
      </video>
    </div>
  );
}

export default VideoSection;