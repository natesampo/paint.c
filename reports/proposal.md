# paint.c - Project Proposal
## Jeremy Ryan, Nate Sampo, Benjamin Ziemann


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

**Nate:** I am Nate.

**Jeremy:** I hope to improve my ability to make robust tools and data structures in a low-level
language, which can be integrated well with other people's code. Additionally, I
would like to explore the math and algorithms behind tools like the fill tool, image resizing,
and anti-aliasing.

**Ben:** I would like to get a better creating structures and frameworks to better allow for scalability. This projects seems to work well with this goal as it will be important how we first set up our project so that as we expand to more and more tools it will be intuitive how they will interact with the larger application, and how they interact with each other. I'd also to get better at working with APIs, a necessity given we will be working with a grpahics library.

### Resources and Getting Started

*What do you need to get started?  Have you found the resources you need, do you have a plan to find them, or do you need help?*

To start off with we need to decide whether we will be building our own image editor from scratch or starting with another user's code and building on more advanced features from there. Each has it's benefits, with the former allowing us to really understand the what goes into C application building and making sure we are really thinking through the the underlying structure of the code so we can scale it later on. The latter means learning to better read, understand, and identify spots to improve in others' code, while allowing us to expand past the more basic implementation and really dwelve down into specific, technical challenges associated with each of the more advanced paint tools we would implement. Making this decision will require a bit of research and testing though we are leaning heavily towards building our own.

Either way we also need to identify what graphics libraries are out there and which we want to use. We've begun searching and found several which look promising (and a few that aren't) through general searches, coding forums, and past projects.

We believe this use of the graphics API will be the most removed from the NINJA's skill set as it the most specific part contextually to our project and it's unclear how much graphics experience they would have, though their understanding of how to strucutre a C program and setting up that workflow earlier rather than later and having to rework it. Past that though we are confident both more in our own experience with each of us having taken several software classes and the NINJA's ability to help as we tackle more implementation style problems associated with each tool as opposed to the challenges that come along with learning a specific new graphics library.

### First Steps

*What are your first steps?  Describe at least three concrete tasks that you can do immediately, and identify which member of the team will do them.  For each one, what is the "definition of done"; that is, what will you produce to demonstrate that the task is done?*
