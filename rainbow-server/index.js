const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const chalk = require('chalk');

let prevColor = [-1, -1, -1];

async function getSerialPortForArduino () {
  const ports = await SerialPort.list();
  return ports.find(p => /arduino/ig.test(p.manufacturer))?.path;
}

async function start () {
  const path = await getSerialPortForArduino();
  const port = new SerialPort(path, {
    baudRate: 9600
  });
  const parser = port.pipe(new Readline({
    delimiter: '\n'
  }));

  port.on('open', () => {
    console.log('connected');
  });
  parser.on('data', data => {
    const parts = data.split('|');
    const on = parseInt(parts[0], 10);
    const color = parts[1]?.split(',').map(val => parseInt(val, 10));
    const colorChange = color?.reduce((acc, currValue, currIndex) => currValue !== prevColor[currIndex] || acc, false);
    prevColor = color || prevColor;
    if (on && colorChange) {
      process.stdout.write(chalk.rgb.apply(chalk, color)('■'));
    }
  });
}

start();
