# Rasterization-in-2D-graphics-editor
Interactive Graphics program to demonstrate rasterization.
<br>
External libraries required: GLM, GLFW, GLEW.
<br><br>
To run : 
cmake ../ ; make ; ./Assignment2_bin
<br><br>
The output file name can be changed in CMakeLists.txt.

### Outputs

##### Insertion and Translation:
When 'I' key is pressed on the keyboard the program goes into Insertion mode. The first click gives us a point with the preview of the line through it. The second click makes a line with the preview of the possible triangle and the third and final click completes the triangle.
<br><br>
When 'O' key is pressed the program goes into traslation mode in which clicking on a triangle selects it and translation can be performed by holding the mouse and dragging the traingle. Clockwise and anti-clockwise rotation can be performed by pressing the 'H' and 'J' keys respectively. The triangle can be scaled and up and down with the 'K' and 'L' keys respectively.<br><br>

![Alt Text](https://github.com/msDikshaGarg/Rasterization-in-2D-graphics-editor/blob/main/outputs/insert%2Ctranslate.gif)

##### Panning the camera

The camera can be panned using the 'A,W,S,D' keys according to their standard directions, the camera can also zoom in and out using the '+' and '-' keys. <br><br>
![Alt Text](https://github.com/msDikshaGarg/Rasterization-in-2D-graphics-editor/blob/main/outputs/pan.gif)

##### Color Individual Vertices

Pressing the 'C' key takes the program into color mode. Each individual vertex of any triangle can be selected and colors can be assigned by any key from '1-9'.<br><br>
![Alt Text](https://github.com/msDikshaGarg/Rasterization-in-2D-graphics-editor/blob/main/outputs/color.gif)

##### Animation:
Pressing the ',' key allows us to create the first keyframe for animation. Triangles can be put into any orientation or position. Upon clicking the '.' key it allows us to create the second keyframe which allows us to translate or modify the triangles. Upon pressing the 'SPACE' key on the keyboard both the keyframes are alternated in a fixed interval of time giving the effect of an animation. <br><br>
![Alt Text](https://github.com/msDikshaGarg/Rasterization-in-2D-graphics-editor/blob/main/outputs/animation.gif)
