# Closest Point on Mesh Query
A simple C++ implementation to find a closest  point on a mesh to a given external point P, within a specified search radius R.

## Requirements
* [Ply IO ](http://people.sc.fsu.edu/~jburkardt/data/ply/ply.html)
* [Boost Minimalist Test Library](http://www.boost.org/doc/libs/1_46_1/libs/test/doc/html/minimal.html)

### Building code
```
% make
% bin/ClosestPoint.out -h      #Displays help for running
```
### Sample run 
```
% bin/ClosestPoint.out data/happy.ply
```
The test cases read the query points from `data/queryPoints.txt`. Each line is a space separated value of the cordinates followed by the search radius value

### Data used for testing
Ply file |Vertices | Faces
------------ | -------------|-------------
bunny.ply | 35947 | 69451 
hand.ply | 327323 | 654666
happy.ply |  543652 | 1087716

### [Detailed Wiki](https://github.com/bronzelion/closest-point-on-mesh/wiki/Closest-Point-on-a-Mesh-:-Background-and-Approaches)


