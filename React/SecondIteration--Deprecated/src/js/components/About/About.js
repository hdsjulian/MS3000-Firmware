// need to import react here
import React, {Component, PropTypes} from 'react';

import Radium from 'radium';

@Radium
export default class About extends Component {

  constructor(props) {
    super(props);
  }

  render() {

    return (
      <div className='about-page'>
        <h3>Welcome to the MagicShifter 3000 UserInterface</h3>
        <div>
          <p>This page will soon contain a description of features and usage instructions.</p>
        </div>
      </div>
    );
  }
}