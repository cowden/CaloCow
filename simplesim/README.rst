Simple G4 Simulation Scenarios
==============================

Simple ECAL
-----------
Simulate PbWO4 (lead-tungstate) crsytal ecal composed of 22x22x23 cm^3
cells arrayed in a 5x5 transverse grid. Cells are indexed  from 0 to 24
starting in the upper right corner and proceeding left in row-major
fashion.  Cell 12 is the center of the 5x5 grid.  The segmentation is
applied in the sensitive detector element rather than geometrically in
detector construction.

The primary particle gun shoots particles into the calorimeter face.
The impact point is uniformly sampled within the central cell.  The
primary particle energy is sampled from a normal distribution centered
and the specified gun energy with a 10% energy fluctuations about the
mean.  Alternatively, a uniform sample over a broad energy range can be
selected by removing the comment in `PrimaryGeneractorAction.cc`.

Data is saved in HDF5 format.  Each event is represented by a single 28
deminsional vector.  The first element records the primary particle
energy, the next two elements record the x-y impact point of the primary
particle in the transverse face, and the remaining 25 elements
correspond to the total energy deposited in the calorimeter cells.
