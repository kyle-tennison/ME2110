import React from 'react';

function RobotStatusCard() {
  return (
    <div style={styles.card}>
      <h3 style={styles.heading}>Robot Status</h3>
      <p>Switch 1: ON</p>
      <p>Switch 2: ON</p>
      <p>Potentiometer: 30%</p>
      <p>Status: OK</p>
      <p>Mode: BAG</p>
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