# paint.c - Project Update
## Jeremy Ryan, Nate Sampo, Benjamin Ziemann

### Project Goal

For this project, we aim to create a basic image editing program in C. We will draw inspiration from the tools, functionality, and interfaces of other programs, such as Microsoft Paint, Paint.<span></span>NET, and GIMP.

As a lower bound, we hope to create an interface with a mouse-controlled brush that can draw black pixels on a white canvas and export to a common image format. As stretch goals, we would like to implement several additional tools and features:

- **Reading:** Loading images from an image file, which you can edit and save.

- **Colors:** Drawing in arbitrary colors, perhaps with variable opacity.

- **Fill tool:** Applying a color to all adjacent pixels with similar values.

- **Shape tool:** Drawing shapes onto the canvas.

- **Line tool:** Drawing lines onto the screen, perhaps with variable thickness or anti-aliasing.

- **Canvas tools:** Cropping, resizing, or otherwise manipulating the image as a whole.

- **More:** Other features found in image editing software.


### Learning Goals

**Nate:** I hope to write a number of algorithms and improve my knowledge of the C language. In class so far, we've done a lot of smaller scale programming; implementing one method in a class for example. So, in accordance with this, I also want to structure our code in a nice, scalable way, such that adding another feature or expanding upon an existing one is simple and easy.

**Jeremy:** I hope to improve my ability to make robust tools and data structures in a low-level language, which can be integrated well with other people's code. Additionally, I would like to explore the math and algorithms behind tools like the fill tool, image resizing, and anti-aliasing.

**Ben:** I would like to get a better creating structures and frameworks to better allow for scalability. This projects seems to work well with this goal as it will be important how we first set up our project so that as we expand to more and more tools it will be intuitive how they will interact with the larger application, and how they interact with each other. I'd also to get better at working with APIs, a necessity given we will be working with a graphics library.


### Where We've Been

Using the GTK library, we were able to hit our minimum viable product of a canvas that we can draw on, then save. A far as resources go, we've found a set we've consistently gone back to and feel confident in their ability to continue guide us as we dive more into our larger strech goals. Of the resources we found Stack Overflow was of course very helpful for specific examples but we also found the GTK API was surprisingly helpful and intuitively laid our for setting up the general structure and basics of our windows. We've also implemented our own file format ".pdc", more for our own convience, readablitiy, and debugging, which allows for saving and loading. Finally we implemented several basic tools, including line and color picker.

The color picker and layout of the toolbar windows and canvas were created through the use of the GTK library, specifically the window and grid types. For the implementation of the color picker, basic structs and conversion functions were written allowing us to define our color values in hex, 0 to 1 floating point, or traiditional 0 to 255 RGB. These were necessary as different parts of the program required different color formats.

The .pdc file format consists of a four dimensional array consisting of a pixel's x,y position, color channels red, green, blue, and alpha and finally their respective values. Initializing and saving these is just iterating over the entirety of these arrays and writing the data out to the appropriate location, an Image structure or .pdc file respectively.


### Where We're Going


There are a few specific tasks we are working on to build our application past our original MVP goal. These tasks range from minor tools to major GUI elements.

Nate is working on the paint bucket tool, where the user can replace all contiguous segments of a specific color with another color. While not a part of our MVP, the paint bucket tool is a staple in every image editor, and we hope to make paint.c feel more complete with its inclusion.

To help complete the basic aspects of our GUI, Ben is working with the GTK library to replace our current tool selection system with a more elegant one. As the project stands now, selecting a tool sets a global variable that is read by the program to determine what actions to take on specific user inputs. Ben is replacing this system with a system built into GTK, where functions can be attached to interactive UI elements to be called whenever the user interacts with those elements.

Currently, drawing on the canvas does not automatically update the screen, so changes can only be viewed by dragging another window across the canvas to load new pixel data. Previously, we were refreshing every pixel on the screen every time the user drew a point, resulting in very low FPS when drawing. Jeremy is remedying this issue by locating and only updating pixels that are changed. This change should find the optimal balance of performance and responsiveness.
