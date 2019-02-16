# Paint.C Project Proposal

### Project Goal

For this project, we aim to create a basic image editing program in C. We will draw inspiration from the tools, functionality, and interfaces of other programs, such as Microsoft Paint, Paint.NET, and GIMP.

As a lower bound, we hope to create an interface with a mouse-controlled brush that can draw black pixels on a white canvas and export to a common image format. As stretch goals, we would like to implement several additional tools and features:

- **Reading:** Loading images from an image file, which you can edit and save.

- **Colors:** Drawing in arbitrary colors, perhaps with variable opacity.

- **Fill tool:** Applying a color to all adjacent pixels with similar values.

- **Shape tool:** Drawing shapes onto the canvas.

- **Line tool:** Drawing lines onto the screen, perhaps with variable thickness or anti-aliasing.

- **Canvas tools:** Cropping, resizing, or otherwise manipulating the image as a whole.

- **More:** Other features found in image editing software.

### Learning Goals

*What are your learning goals; that is, what do you intend to achieve by working on this project?*

**Nate:** I hope to write a number of algorithms and improve my knowledge of the C language. In class so far, we've done a lot of smaller scale programming; implementing one method in a class for example. So, in accordance with this, I also want to structure our code in a nice, scalable way, such that adding another feature or expanding upon an existing one is simple and easy.

**Jeremy:** I hope to improve my ability to make robust tools and data structures in a low-level
language, which can be integrated well with other people's code. Additionally, I
would like to explore the math and algorithms behind tools like the fill tool, image resizing,
and anti-aliasing.

**Ben:** I am Ben.

### Resources and Getting Started

*What do you need to get started?  Have you found the resources you need, do you have a plan to find them, or do you need help?*

### First Steps

*What are your first steps?  Describe at least three concrete tasks that you can do immediately, and identify which member of the team will do them.  For each one, what is the "definition of done"; that is, what will you produce to demonstrate that the task is done?*

Our very first step will be creating an application window, displaying figures on that window, and receiving keystrokes and mouse events. Nate will be tackling this task. This step will be considered "done" when we can successfully display pixel values, images, and text to an application window and can successfully catch key and mouse events. This step is integral to the application, as we will not be able to make our tool interactive without being able to display information and receive input from the user.

Another first step is creating a GUI to allow the user to select different colors and tools. In this step, Ben will create scalable implementations of buttons, sliders, color pickers, and other graphical interfaces needed to allow the user to interact successfully with our paint algorithms.

The canvas is another first step that requires much thought. The canvas is required to have many features, like zooming, scaling, cropping, and the ability to be drawn on. Jeremy will implement the canvas and ensure that the user is able to utilize any tool they select on the canvas.