# Software-Rasteriser
A Graphics rasteriser written in exclusively in C++ without the use of an external graphics library such as OpenGL.
Written by following the CSC3223: Graphics for Games course's Tutorial series.

## Overview
A C++ rasteriser mimicking the graphical rendering pipeline present in modern graphics APIs such as OpenGL.

Diagram of the rendering pipeline below:
![Rendering Pipeline](http://i64.tinypic.com/se5zww.jpg "Rendering Pipeline")

The rasteriser can currently draw three types of geometric primitives: Points, Lines, and Triangles.

### Points
To draw point primitives, the point's vertex coordinate is transformed from local space into world space by multiplying the model matrix by the view and projection matrices, then the vertex is colored in according to the point's mesh.

### Lines
To draw line primitives, firstly the line's vertex coordinates are transformed from local space into world space by multiplying the model matrix by the view and projection matrices, then the two vertices are colored in according to the line's mesh. The Cohen-Sutherland Line clipping algorithm (SoftwareRasteriser::CohenSutherlandLine) is applied to transform the vertices into clip-space and to determine if the line is actually within the screen space, and therefore if it has to be rendered or not in the current frame. If the clipping test passes, the perspective divide is applied on the line vertices, to ensure they get rasterised in the correct location, and the vertex data is passed onto the line drawing function, which determines the pixels on screen to be drawn according to Bresenham’s Line Algorithm (SoftwareRasteriser::RasteriseLine).

- *Drawing*: Bresenham’s Line Algorithm
- *Clipping*: Cohen-Sutherland Line Clipping
- *Interpolation*: Color influenced based off distance between the two vertices

### Triangles
To draw triangle primitives, firstly the triangle's model-view-projection matrix is calculated by multiplying its model matrix by the view and projection matrices. The Sutherland-Hodgman Polygon Clipping algorithm (SoftwareRasteriser::SutherlandHodgmanTri) is then applied to determine the triangle vertices to be drawn on screen. Once the clipping stage is over, the perspective divide is applied on the vertices, the triangle is ready to be drawn. The triangle drawing functions works by calculating a bounding-box that surrounds the triangle, and determining which points/pixels in the bounding-box actually fall inside the traingle. To calculate whether a point is inside a triangle triangle areas are used. To do so we need to calculate the area of four triangles: the triangle we are trying to draw, and the three sub-triangles formed by drawing a line from the point p to each vertex. If the sum of the three sub triangles equals the area of the original triangle, then p is inside the triangle, and should be shaded, but if the sum of areas is greater than the original triangle, then p lies outside the triangle. Once we know that a point is within the triangle we determine the color, or texture coordinate, of it by interpolating it using Barycentric Coordinates.

- *Drawing*: Bounding-box, then tested against triangle areas
- *Clipping*: Sutherland-Hodgman Polygon Clipping
- *Interpolation*: Barycentric Coordinates

### Other Rasteriser features

Rasteriser capabilities:
- Triangle Alpha Blending
- Textured Triangles
- Primitive Clipping and Back-face Culling
- Color Interpolation
- Depth Buffers
- Scene Projection (Orthographic and Perspective)

## See More
### Video
Video of running asteroid-field scene produced by the rasteriser can be found at: https://dzinzi.tinytake.com/sf/MTc5MTI1Nl81ODQ4MTE5
