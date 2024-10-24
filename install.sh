#!/bin/bash

echo "Building the project..."
make clean
make

echo "Installing the server..."
sudo make install

echo "Installation complete!"
