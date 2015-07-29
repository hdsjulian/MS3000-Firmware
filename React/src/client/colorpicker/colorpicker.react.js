import React from 'react';

import Component from '../components/component.react';
import * as actions from './actions';
import * as ledActions from '../leds/actions';

import {msg} from '../intl/store';

export default class ColorPicker extends Component {

  onChange(e) {
    e.preventDefault();
    const {id, active} = this.props;
    const props = {
      value: e.target.value,
      id,
      active,
    };
    ledActions.changeLed(props);
  }

  addColor(e) {
    e.preventDefault();
    actions.addColor(e.target.value);
  }

  render() {
    const styles = {
      container: {
        display: 'inline-block',
      },
    };

    const { active, value } = this.props;

    return (
      <div style={styles.container}>
        <input
          type='color'
          value={ value }
          onChange={(e) => this.onChange(e)}
        />
        <button
          value={ value }
          title={msg('colorPicker.add.title')}
          onClick={(e) => this.addColor(e)}
        >
          {msg('colorPicker.add.text')}
        </button>
      </div>
    );
  }
}
