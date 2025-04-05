import React, { useState } from 'react';

function ControlBoardCard() {
  const [topOre, setTopOre] = useState(true);
  const [groundOre, setGroundOre] = useState(true);
  const [dogBone, setDogBone] = useState(false);

  return (
    <div style={styles.card}>
      <h3 style={styles.heading}>Control Board</h3>
      <Toggle 
        label="Top Ore Collector"
        isOn={topOre}
        handleToggle={() => setTopOre(!topOre)}
      />
      <Toggle 
        label="Ground Ore Collector"
        isOn={groundOre}
        handleToggle={() => setGroundOre(!groundOre)}
      />
      <Toggle 
        label="Dog Bone"
        isOn={dogBone}
        handleToggle={() => setDogBone(!dogBone)}
      />
    </div>
  );
}

// A small reusable toggle switch component
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