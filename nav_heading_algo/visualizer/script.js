/*
 * quickly hacked together
 */

var clear = function() {
  context.clearRect(0, 0, context.canvas.width, context.canvas.height);
};

var drawPolygon = function(points) {
  context.beginPath();

  var first = true;
  var firstPoint;

  points.forEach(function(point) {
    /* reflect in y */
    point = [point[0], -point[1]];

    /* transform from origin at center to origin at bottom left */
    point = [point[0] + maxDisplacement,
             point[1] + maxDisplacement];

    if (first) {
      context.moveTo(point[0], point[1]);
      first = false;
      firstPoint = point;
    } else {
      context.lineTo(point[0], point[1]);
    }
  });

  context.lineTo(firstPoint[0], firstPoint[1]);

  context.stroke();
};

/* Draws a crude boat at position coordinates (x, y) and heading h.
 *
 * Position coordinates start from bottom left and increase up and right.
 * Heading starts at right and increases counter-clockwise.
 *
 * Parameters x and y are centimeters in (-10*100, 10*100).
 *
 * Parameter h is radians.
 */

var drawBoat = function(x, y, h) {
  /* vertices of a skinny triangle */
  var a = [-100.0, -50.0];
  var b = [-100.0,  50.0];
  var c = [ 100.0,   0.0];

  /* rotate to h */
  a = rotate(a, h);
  b = rotate(b, h);
  c = rotate(c, h);

  /* translate to x, y */
  a = [a[0] + x, a[1] + y];
  b = [b[0] + x, b[1] + y];
  c = [c[0] + x, c[1] + y];

  drawPolygon([a, b, c]);
};

var drawArrow = function(heading) {
  var points = [
    [ 150, -50.0],
    [ 200,   0.0],
    [ 150,  50.0],
    [ 200,   0.0],
    [-200,   0.0],
    [ 200,   0.0]
  ];

  points.forEach(function(point) {
    var x = rotate(point, heading);

    point[0] = x[0];
    point[1] = x[1];
  });

  drawPolygon(points);
};

var drawCircle = function(x, y)
{
  point = translate([x, -y], maxDisplacement, maxDisplacement);

  context.beginPath();

  context.arc(point[0], point[1], 50, 0, 2*Math.PI);
  context.stroke();
};

/* Returns a point rotated around the origin by some angle.
 *
 * Parameter point is (x, y).
 *
 * Parameter angle is radians.
 */

var rotate = function(point, angle) {
  return [ point[0] * Math.cos(angle) - point[1] * Math.sin(angle),
           point[1] * Math.cos(angle) + point[0] * Math.sin(angle) ]
};

var translate = function(point, x, y) {
  return [ point[0] + x, point[1] + y ];
};

var drawStatic = function() {
  /* start */
  context.strokeStyle = "#00ff00";
  drawCircle(start[0], start[1]);

  /* finish */
  context.strokeStyle = "#0000ff";
  drawCircle(end[0], end[1]);

  /* wind */
  context.strokeStyle = "#ff0000";
  drawArrow(heading_wind);

  context.strokeStyle = "#000000";
};

var drawNextPoint = function() {
  var point = points[0];

  clear();
  drawStatic();
  drawBoat(point[0], point[1], point[2]);

  points.shift();

  if (points.length != 0)
    setTimeout(drawNextPoint, 100);
};

var body = document.getElementById('body');
var bodyWidth  = body.getBoundingClientRect().width;
var bodyHeight = body.getBoundingClientRect().height;

var minLength = bodyWidth < bodyHeight ? bodyWidth : bodyHeight;

var canvas = document.getElementById('canvas');
var context = canvas.getContext('2d');

var maxDisplacement = 20*100;
context.canvas.width  = 2 * maxDisplacement;
context.canvas.height = 2 * maxDisplacement;

context.lineWidth = 25;

canvas.style.width  = minLength + 'px';
canvas.style.height = minLength + 'px';

drawStatic();
drawBoat(points[0][0], points[0][1], points[0][2]);

setTimeout(drawNextPoint, 1000);
