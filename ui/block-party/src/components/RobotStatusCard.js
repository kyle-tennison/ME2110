import React, { useState, useEffect } from 'react';

function mapMode(num) {
    const map = {
        0: "BAG",
        1: "PWM",
        2: "BAG ANIMATE"
    };

    return map[num] || "Unknown"; // Return "Unknown" for unmapped integers
}

function RobotStatusCard() {
  // Initialize state for our status values.

  const defaultStatus = {
    switch1: 'Unknown',
    switch2: 'Unknown',
    potentiometer: 'Unknown',
    status: 'Disconnected',
    mode: 'Unknown',
  };

  const [robotStatus, setRobotStatus] = useState(defaultStatus);

  useEffect(() => {
    console.log("Attempting websocket connection...")
    const ws = new WebSocket('ws://localhost:8080');

    ws.onopen = () => {
      console.log('WebSocket connected');
    };

    ws.onclose = (() => {
      setRobotStatus(defaultStatus);
    });

    ws.onmessage = (evt) => {
      try {
        if (evt.data.startsWith("!")){
          return;
        }
        const data = JSON.parse(evt.data);
        console.log("Inbound websocket message: ", data)
        setRobotStatus({
          switch1: data.in1 ? 'ON' : 'OFF',
          switch2: data.in2 ? 'ON' : 'OFF',
          potentiometer: `${data.pot}%`, // e.g., "30%"
          status: "OK",
          mode: mapMode(data.op),
        });
      } catch (error) {
        console.error('Error parsing message data:', error);
      }
    };

    ws.onerror = (error) => {
      console.error('WebSocket error:', error);
      setRobotStatus(defaultStatus);
    };

    return () => {
      ws.close();
      setRobotStatus(defaultStatus);
    };
  }, []); // Empty dependency array means this useEffect runs only once on mount.

  return (
    <div style={styles.card}>
      <h3 style={styles.heading}>Robot Status</h3>
      <p>Switch 1: {robotStatus.switch1}</p>
      <p>Switch 2: {robotStatus.switch2}</p>
      <p>Potentiometer: {robotStatus.potentiometer}</p>
      <p>Status: {robotStatus.status}</p>
      <p>Mode: {robotStatus.mode}</p>
    </div>
  );
}

const styles = {
  card: {
    backgroundColor: '#fff',
    color: '#000',
    borderRadius: '7px',
    padding: '1.5rem',
    boxShadow: 'rgba(0, 0, 0, 0.99) 0px 0px 4px 5px',
  },
  heading: {
    margin: 0,
    marginBottom: '1rem',
  },
};

export default RobotStatusCard;