import React from 'react';

function VideoSection() {
  return (
    <div className="video-section">
        <video width="100%" height="auto" controls>
        <source src="video.m4v" type="video/mp4" />
        Your browser does not support the video tag.
      </video>
    </div>
  );
}

export default VideoSection;