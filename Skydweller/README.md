# Detination Simulation

I'm working on the technical interview from the PDF provisioned by Skydweller.

# Assumptions

I'm told that the diameter of the wheel is 6 inches. Given [Skydweller's cover photo](https://media.licdn.com/dms/image/C4E1BAQHdEYOGf10wGg/company-background_10000/0/1612268565869/skydwelleraerocom_cover?e=1706040000&v=beta&t=rYHYCXR_ZLAVr88aV8ENv0BWff3oHhf96TuiVEVOdmE), that tire looks closer to a foot across. In my English, wheel is the metal part you mount the tire on but may also be the wheel and tire. I don't need to know the rim diameter, I need to the tire diameter. I imported the image of the wheel to GIMP to take measurements. The rim has a radius of 3 inches and 139 pixels in the horizontal direction. The tire has an additional radius of 125 pixels to the first line where the tire curves. I'll estimate an additional 22 pixels of radius in the curve. That gives a total radius of 286 pixels or 6.17 inches \(15.67 cm\).

I'm going to go with normal math conventions for angles: 0 degrees to the left, increasing anti-clockwise, in units of radians.

It would appear to me that the first sensor is at about 45 degrees or pi/4 radians and the second is at 135 degrees or 3/4*pi radians.

I will assume nothing about the wheel's position. The ferrous bolts were specified to be 90 degrees or pi/2 radians from each other. I currently assume pi/70 radians, 2.57 degrees, .135 inches, or 3.2 mm of accuracy in placement.

I will assume that the plane's location is defined by the main gear and that there is 5 cm of travel in the main gear where the wheel is in contact with the runway.

## Compile directions for g++

g++ Wheel_Sim.cpp Wheel.cpp Vector3.cpp -O3
