# paint.c - Project Update
## Jeremy Ryan, Nate Sampo, Benjamin Ziemann
Project code: https://github.com/natesampo/paint.c
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
 
### Resources
As far as resources go, we've found a set we've consistently gone back to and feel confident in their ability to continue guide us as we dive more into our larger stretch goals. Of the resources we found Stack Overflow was of course very helpful for specific examples but we also found the GTK API was surprisingly helpful and intuitively laid out for setting up the general structure and basics of our windows. Various examples of other GTK applications as a GUI for other people’s applications also proved helpful for niche or specific uses (color picker, signals).
 
//What were you able to get done?  Include in the report whatever evidence is appropriate to demonstrate the outcome of the project.  Consider including snippets of code with explanation; do not paste in large chunks of unexplained code.  Consider including links to relevant files.  And do include output from the program(s) you wrote.
## The Program
![alt text](https://github.com/natesampo/paint.c/blob/master/use.png "paint.c")
 
 
 
//Explain at least one design decision you made.  Were there several ways to do something?  Which did you choose and why?
## Design Decisions
 
//Reflect on the outcome of the project in terms of your learning goals.  Between the lower and upper bounds you identified in the proposal, where did your project end up?  Did you achieve your learning goals?
 
### Reflection
**Overall** 
We felt we met our minimum viable product fairly earlier on, making a simple canvas that individual pixels could be drawn on. We had planned to meet this goal fairly earlier on, allowing us more freedom, creativity, and flexibility with the rest of the project depending on our schedules, motivations, etc. From completion of the minimum viable product there we worked split our time between user features and improving the existing systems. User features included tools like line drawing and color picking. More internal updates included cleaner changes of states and better C practices in general, as well as optimization, especially when it came to drawing on the canvas.
**Jeremy:**
**Nate:**
**Ben:** I felt, given the parts of the project I worked on, I accomplished my learning goals, naming working with APIs. As the user interface is comprised entirely of GTK components, in addition to some of the backend like tool switching and starting new windows, working with existing functions, getting to understand GTK’s gobjects (gint, gchar, etc) and how to construct a larger framework from existing code definitely felt like I was meeting the goal I had in mind starting out this project. I would have liked to do a bit more with algorithms but still felt I got some use with them.
 
 
Submission Mechanics
1) In your project report, you should already have a folder called "reports" that contains a Markdown document called "update.md".  Make a copy of "update.md" called "report.md"
2) At the top of this document, give your project a meaningful name that at least suggests the topic of the project.  Do not call it "Software Systems Project 1".  List the complete names of all members of the team. 
3) Answer the questions in the Content section, above. Use typesetting features to indicate the organization of the document.  Do not include the questions as part of your document.
4) Complete your update, view it on GitHub, and copy the GitHub URL.  Then paste the URL in the submission space below.  You only need one proposal for each team, but everyone should submit it.
 
 


