#!/usr/bin/bash

#####
# Run G4 simulation 
#####

. ~/.bashrc

cd /work/
conda init
conda activate

SEED=123231
BASE_NAME="CaloX_run_0"
PARTICLE="pi-"
ENERGY="500 GeV"
NEVENTS=10000

while getopts "s:b:p:e:n:" name; do
    case $name in   
        s) 
            SEED=$OPTARG
            ;;
        b) 
            BASE_NAME=$OPTARG
            ;;
        p) 
            PARTICLE=$OPTARG
            ;;
        e) 
            ENERGY=$OPTARG
            ;;
        n) 
            NEVENTS=$OPTARG
            ;;
    esac
done

echo "SEED = $SEED"
echo "BASE_NAME = $BASE_NAME"
echo "PARTICLE = $PARTICLE"
echo "ENERGY = $ENERGY"
echo "NEVENTS = $NEVENTS"

MAC=${BASE_NAME}.mac

cat << EOF > $MAC
/trm/seed ${SEED}
/uui/name ${BASE_NAME}
/run/initialize

/gun/particle ${PARTICLE}
/gun/energy ${ENERGY}

/run/beamOn ${NEVENTS}
EOF

/opt/CaloX_G4/build/calox-hgcal -m $MAC -t 1
