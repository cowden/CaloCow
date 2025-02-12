#!/usr/bin/bash

#####
# Run G4 simulation 
#####

cd /work/
conda init
conda activate

SEED=123231
BASE_NAME="CaloX_run_0"
PARTICLE="pi-"
ENERGY="500 GeV"
NEVENTS=10000

cat << EOF > run.mac
/trm/seed ${SEED}
/uui/name ${BASE_NAME}
/run/initialize

/gun/particle ${PARTICLE}
/gun/energy ${ENERGY}

/run/beamOn ${NEVENTS}
EOF

/opt/CaloX_G4/build/calox-hgcal -m run.mac -t 1
