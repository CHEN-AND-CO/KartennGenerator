#!/bin/sh
cd data

# World Map
wget https://naciscdn.org/naturalearth/110m/cultural/ne_110m_admin_0_countries.zip
unzip ne_110m_admin_0_countries.zip -d world/


cd ..