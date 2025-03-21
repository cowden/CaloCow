#!/usr/bin/env bash

if [[ $# -ne 3 ]]; then
    echo "initialize_study.sh [study name] [batch size]"
    return 0
fi

# study name
STUDY_NAME=$1
# batch size
BATCH_SIZE=$2

# generate random seed
# echo $RANDOM

# create tables
psql postgres -c "create database $STUDY_NAME;"
psql $STUDY_NAME << EOF
CREATE TABLE DATA (
    ID integer PRIMARY KEY,
    SEED integer UNIQUE
);
CREATE TABLE PERF (
    ID INTEGER PRIMARY KEY,
    DATA_ID INTEGER,
    EPOCH INTEGER,
    BATCH INTEGER,
    LOSS REAL
    );
EOF

# populate seeds
count=0
while [[ $count -lt $BATCH_SIZE ]]; do
    sql="INSERT INTO DATA (ID, SEED) VALUES ($count, $RANDOM);"
    psql $STUDY_NAME -c "$sql"
    if [[ $? -eq 0 ]]; then
        ((count++))
    fi
done




