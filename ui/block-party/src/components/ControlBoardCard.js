import React, { useState, useEffect, useRef } from 'react';

function ControlBoardCard() {
  const [topOre, setTopOre] = useState(true);
  const [groundOre, setGroundOre] = useState(true);
  const [dogBone, setDogBone] = useState(false);

  // Create a ref to hold the WebSocket
  const ws = useRef(null);
  // Ref to check if this is the component's first render
  const isFirstRender = useRef(true);

  useEffect(() => {
    // Open the WebSocket connection
    ws.current = new WebSocket('ws://localhost:8080');

    ws.current.onopen = () => {
      console.log('WebSocket connected');
    };

    ws.current.onerror = (error) => {
      console.error('WebSocket error:', error);
    };

    // Clean up on unmount
    return () => {
      if (ws.current) {
        ws.current.close();
      }
    };
  }, []);

  // useEffect to send state updates.
  useEffect(() => {
    // Skip sending on first render, if not desired.
    if (isFirstRender.current) {
      isFirstRender.current = false;
      return;
    }
    const payload = {
      top_ore_collector: topOre,
      ground_ore_collector: groundOre,
      dog_bone: dogBone,
    };
    if (ws.current && ws.current.readyState === WebSocket.OPEN) {
      ws.current.send(JSON.stringify(payload));
      console.log('Payload sent:', payload);
    } else {
      console.error('WebSocket is not connected.');
    }
  }, [topOre, groundOre, dogBone]);

  // Toggle functions simply update the local state.
  const toggleTopOre = () => {
    setTopOre(prev => !prev);
  };

  const toggleGroundOre = () => {
    setGroundOre(prev => !prev);
  };

  const toggleDogBone = () => {
    setDogBone(prev => !prev);
  };

  return (
    <div style={styles.card}>
      <h3 style={styles.heading}>Control Board</h3>
      <Toggle 
        label="Top Ore Collector"
        isOn={topOre}
        handleToggle={toggleTopOre}
      />
      <Toggle 
        label="Ground Ore Collector"
        isOn={groundOre}
        handleToggle={toggleGroundOre}
      />
      <Toggle 
        label="Dog Bone"
        isOn={dogBone}
        handleToggle={toggleDogBone}
      />
    </div>
  );
}

// Reusable toggle switch component
function Toggle({ label, isOn, handleToggle }) {
  return (
    <div style={styles.toggleRow}>
      <span>{label}</span>
      <div style={styles.toggleContainer}>
        <input
          style={styles.checkbox}
          checked={isOn}
          onChange={handleToggle}
          type="checkbox"
        />
      </div>
    </div>
  );
}

const styles = {
  card: {
    backgroundColor: '#fff',
    color: '#000',
    borderRadius: '7px',
    padding: '1.5rem',
  },
  heading: {
    margin: 0,
    marginBottom: '1rem',
  },
  toggleRow: {
    display: 'flex',
    justifyContent: 'space-between',
    alignItems: 'center',
    marginBottom: '1rem',
  },
  toggleContainer: {
    position: 'relative',
  },
  checkbox: {
    transform: 'scale(1.5)',
    cursor: 'pointer',
  },
};

export default ControlBoardCard;