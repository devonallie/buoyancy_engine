'''
File Name: runGUI.py
Author: Logan Crooks
Date Created: Oct 18, 2021

This script acts as the entry point for the GUI

'''

import logging
import argparse
import drawMainWindow

parser = argparse.ArgumentParser()
parser.add_argument('-l', '--loglevel', type=str, default='INFO', required=False)

args = parser.parse_args()
loglevel = args.loglevel

window = drawMainWindow.MainGUIWindow()
