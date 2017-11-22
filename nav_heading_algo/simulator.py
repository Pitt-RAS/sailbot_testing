#
# quickly hacked together
#

from math import pi, sqrt, sin, cos
from heading import heading

def distance(a, b):
    displacement = (a[0] - b[0], a[1] - b[1])
    return sqrt(displacement[0]**2 + displacement[1]**2)

position_boat = (0.0, 0.0)
heading_boat = 0.0*pi/180

position_target = (15.0, 15.0)

speed_wind = 2.0
heading_wind = (180+65)*pi/180

beating_parameter = 5.0

print('var start = [%f, %f];' % (100*position_boat[0], 100*position_boat[1]))
print('var end = [%f, %f];' % (100*position_target[0], 100*position_target[1]))

print('var heading_wind = %f;' % heading_wind)

print('var points = [')

while distance(position_boat, position_target) > 0.1:
    heading_boat = heading(position_boat, heading_boat,
                           position_target,
                           speed_wind, heading_wind,
                           beating_parameter)

    advancement = (0.1 * speed_wind * cos(heading_boat),
                   0.1 * speed_wind * sin(heading_boat))

    position_boat = (position_boat[0] + advancement[0],
                     position_boat[1] + advancement[1])

    print('  [%f, %f, %f],' % (100*position_boat[0],
                               100*position_boat[1], heading_boat))

print('];')
