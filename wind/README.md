Below is a summary of how the wind data is smoothed over time. More details on the exponential weighting method can be found here: https://www.mathworks.com/help/dsp/ug/sliding-window-method-and-exponential-weighting-method.html  


## Exponential Weighting Method
The weighting method is recursive and therefore takes into account all past data without using lots of memory. The code contains a variable "memory" that is set between 0 and 1. If memory=0.7, 
when the weights settle (after 20 or so samples), 30% of the output will be the new data point, while 70% will be old data. As data ages, an additional factor of .7 is slapped on.  

First, the weight of a data point is defined with regards to the weight of the previous point (the very first weight ![equtaion](https://latex.codecogs.com/gif.latex?W_%7B0%7D%3D0))

![equation](https://latex.codecogs.com/gif.latex?W_%7Bn%7D%3Dmemory%5Ctimes%20W_%7Bn-1%7D&plus;1)  
![equation](https://latex.codecogs.com/gif.latex?W_%7Bn%7D) settles to 3.333 when memory = 0.7. To get the desired weight of 0.3 for new data, we need ![equation](https://latex.codecogs.com/gif.latex?%5Cfrac%7B1%7D%7BW_%7Bn%7D%7D). So the output so publish:  

![equation](https://latex.codecogs.com/gif.latex?OUT_%7Bn%7D%20%3D%20%281-%5Cfrac%7B1%7D%7BW_%7Bn%7D%7D%29%5Ctimes%20OUT_%7Bn-1%7D&plus;%28%5Cfrac%7B1%7D%7BW_%7Bn%7D%7D%29%5Ctimes%20NEWDATA)

## Sines and Cosines
The rotary encoder attached to the wind sensor returns values in [0,359.9]. The average of 5 and 357 is 181, but for degrees it should be 1. 
To fix this, the inputs to the exponential weighting are not [0,359.9], but instead sin([0,359.9]). Another identical exponential weighting is done with cos([0,359.9]) as its input. 
Notice that you can uniquely identify a value between [0,359.9] by its sin and cosine.  

They are recombined with the 2 input arctangent for C++ and converted from radians to degrees: 
```
atan2f(sinval, cosval)*57.2958  
```
this returns a value between [-180,180], which is published to ROS. 
