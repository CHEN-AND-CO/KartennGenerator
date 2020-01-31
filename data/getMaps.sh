#!/bin/sh
cd data

# World Map
wget -N https://naciscdn.org/naturalearth/110m/cultural/ne_110m_admin_0_countries.zip
unzip -n ne_110m_admin_0_countries.zip -d world/

# Finistere map
wget -N https://download.openstreetmap.fr/extracts/europe/france/bretagne/finistere.osm.pbf
FILE=finistere.osm
if [ -f "$FILE" ]; then
    echo "Not updating $FILE"
else 
    echo "$FILE does not exist"
    osmconvert finistere.osm.pbf --out-osm -o=finistere.osm
fi
ogr2ogr -update -progress --config OSM_USE_CUSTOM_INDEXING NO -skipfailures -f "ESRI Shapefile" finistere finistere.osm -overwrite

cd ..