# Condyles Features Extractor

## Description

Specific features are computed at each point of the 3D Model. Those features are added to the vtk file as scalar, vector or array.

Default features: 
  
  * Normals (Vector)
  * Positions (3 scalars)
  * Maximum curvature (1 scalar)
  * Minimum curvature (1 scalar)
  * Gaussian curvature (1 scalar)
  * Mean curvature (1 scalar)

Optional features:

  * landmarks (1 array)
  * distances to other mesh (as many scalar as distMesh provided)

## Build

#### Requirements 

Building ComputeMeanShape requires to have built previously:

* VTK version 7
* ITK 
* SlicerExecutionModel


###### Linux or MacOSX 

Consider this tree of repertories:
```
~/Project/CondylesFeaturesExtractor
         /CondylesFeaturesExtractor-build
```

Start a terminal.
First change your current working directory to the build directory ```CondylesFeaturesExtractor-build```
```
cd ~/Project/CondylesFeaturesExtractor-build
```

Generate the project using ```cmake```
```
cmake -DVTK_DIR:PATH=path/to/VTK -DITK_DIR:PATH=path/to/ITK -DSlicerExecutionModel:PATH=path/to/SlicerExecutionModel ../CondylesFeaturesExtractor
make
```


## Usage

```
./condylesfeaturesextractor inputModel outputModel 
[options: 
--distMeshOn --distMesh [<std::vector<std::string>> list of vtk files]
--landmarksOn --landmarks [<std::string> fcsv file] ]
```


## Licence

See LICENSE.txt for information on using and contributing.