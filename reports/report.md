# paint.c - An Image Editor by Programmers for Programmers
## Jeremy Ryan, Nate Sampo, Benjamin Ziemann
Project code: https://github.com/natesampo/paint.c
## Project Goal
For this project, we aim to create a basic image editing program in C. We will draw inspiration from the tools, functionality, and interfaces of other programs, such as Microsoft Paint, Paint.<span></span>NET, and GIMP.
As a lower bound, we hope to create an interface with a mouse-controlled brush that can draw black pixels on a white canvas and export to a common image format. As stretch goals, we would like to implement several additional tools and features:
- **Reading:** Loading images from an image file, which you can edit and save.
- **Colors:** Drawing in arbitrary colors, perhaps with variable opacity.
- **Fill tool:** Applying a color to all adjacent pixels with similar values.
- **Shape tool:** Drawing shapes onto the canvas.
- **Line tool:** Drawing lines onto the screen, perhaps with variable thickness or anti-aliasing.
- **Canvas tools:** Cropping, resizing, or otherwise manipulating the image as a whole.
- **More:** Other features found in image editing software.
 
## Learning Goals
**Nate:** I hope to write a number of algorithms and improve my knowledge of the C language. In class so far, we've done a lot of smaller scale programming; implementing one method in a class for example. So, in accordance with this, I also want to structure our code in a nice, scalable way, such that adding another feature or expanding upon an existing one is simple and easy.

**Jeremy:** I hope to improve my ability to make robust tools and data structures in a low-level language, which can be integrated well with other people's code. Additionally, I would like to explore the math and algorithms behind tools like the fill tool, image resizing, and anti-aliasing.

**Ben:** I would like to get a better creating structures and frameworks to better allow for scalability. This projects seems to work well with this goal as it will be important how we first set up our project so that as we expand to more and more tools it will be intuitive how they will interact with the larger application, and how they interact with each other. I'd also to get better at working with APIs, a necessity given we will be working with a graphics library.
 
## Resources
As far as resources go, we've found a set we've consistently gone back to and feel confident in their ability to continue guide us as we dive more into our larger stretch goals. Of the resources we found Stack Overflow was of course very helpful for specific examples but we also found the GTK API was surprisingly helpful and intuitively laid out for setting up the general structure and basics of our windows. Various examples of other GTK applications as a GUI for other people’s applications also proved helpful for niche or specific uses (color picker, signals).
 


## The Program
![alt text](https://github.com/natesampo/paint.c/blob/master/use.png "paint.c")

Over the course of this project we succeeded in implementing several features both in the back end and user facing side of the application, meeting our minimum viable product and several of our stretch goals.

At a bare bones level on the user side we implemented interactable windows that allow a user to draw on a canvas and select a number of standard digital art tools. These windows were created using the GTK library.

Of these tools we implemented the standard pencil, line, paint bucket, and color picker tools. The color picker was built up using the GTK library, with the color selected being used by other functions by storing it in structures to make parsing easy in both hex and value formats (see design decisions).

We made a simple file format, ".pdc", to test our image creation, writing each pixel into a single large text file in hex format, allowing us easily parse it to read images back exactly as they were created. Because we had it in hex and many of the sliders and color pickers out put in value format, we wrote conversion functions, ```decimal_to_hex``` and ```hex_to_decimal```,  to go between the two formats.
 
 
## Design Decisions
 
### PDC Files
As one of our original "reach" goals for this project, we hoped to have the ability to read and write to files. This could either be implemented either by interfacing with common image formats, like PNG or JPEG, or by creating a custom file format for reads and writes, like Photoshop or Paint.<span></span>NET.

Although it's convenient to be able to read directly from image formats, we instead opted to create our own file format. In addition to making the implementation simpler, it also gave us the freedom to add other information, such as layers and settings, that don't exist in common image types.

### Hex, Decimal, and Pointers
Some interesting design differences came out of implementation of helper functions by different members of our team. Because our PDC files stored information as hexadecimal characters and our functions used 8-bit integers, we had functions ```decimal_to_hex``` and ```hex_to_decimal``` to convert between length-2 character arrays and integers.

**Hex to Decimal**
The first of these functions relied on the fact that numerals and lowercase letters have contiguous ASCII codes, so you can add an offset to the input integer to get the hex character. It additionally used ```malloc``` and returned a pointer to a character array.

```
/* Converts an integer value into its hexadecimal equivalent, and returns a
 * pointer to a memory-allocated char array. Must be freed by caller.
 */
char* decimal_to_hex(int n) {
	int i, digit;
	char *hex = (char*) malloc(sizeof(char) * (CHAR_DEPTH + 1));

	for (i=0; i<CHAR_DEPTH; i++) {
		digit = n % 16;

		if (digit < 10) {
			hex[CHAR_DEPTH - i - 1] = 48 + digit;
		} else {
			hex[CHAR_DEPTH - i - 1] = 87 + digit;
		}

		n /= 16;
	}

	return hex;
}
```

**Decimal to Hex**
The latter of these functions used a switch statement to identify cases, iterating to a maximum ```CHAR_DEPTH``` defined in a define statement at the top of the file.

```
/* Takes a char pointer to a 2-digit hexadecimal number and returns its integer value.
 *
 * Hex value must be exactly CHAR_DEPTH characters, and only contain numbers 0-9
 * and lowercase letters a-f.
 */
int hex_to_decimal(char* hex) {

	int total = 0;
	for (int i = 0; i < CHAR_DEPTH; i++) {

		switch (hex[i]) {

			// Increment total based on new digit
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				total *= 16;
				total += hex[i] - '0';
				break;

			case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
				total *= 16;
				total += hex[i] - 'a' + 10;
				break;

		}
	}
	
	return total;
}
```
Ultimately, both of these approaches work in the context of the project, and each have advantages and disadvantages. Switch statements, while perhaps more readable, take more lines of code and are a pain to write for a large number of possible states. Similarly, each has different requirements from the caller: ```hex_to_decimal``` requires a very specific array length, otherwise risking memory errors; ```decimal_to_hex``` requires the user to free any memory allocated.


 
## Reflection
### Overall 
We felt we met our minimum viable product fairly earlier on, making a simple canvas that individual pixels could be drawn on. We had planned to meet this goal fairly earlier on, allowing us more freedom, creativity, and flexibility with the rest of the project depending on our schedules, motivations, etc. From completion of the minimum viable product there we worked split our time between user features and improving the existing systems. User features included tools like line drawing and color picking. More internal updates included cleaner changes of states and better C practices in general, as well as optimization, especially when it came to drawing on the canvas.

As seen with our stretch goals, given more time there was many different ways we could have taken this project from additional tools, to canvas functions, to other functions that would draw it closer to more widely used programs like GIMP or Paint.<span></span>NET. Other additions could directly affect images like filters, bringing it closer still to industry standards like Photoshop.

**Jeremy:** I feel that I accomplished my learning goals reasonably well. I was able to work on some interesting optimization problems with the brush tool and canvas rendering. I would have liked to have had more time to implement extra features like shapes, distortions, and rotations, but it was beyond the scope of the project for our team in the amount of time we had. I did learn a lot about GTK and Cairo, which could be useful doing C projects in the future that require GUIs or graphics.

**Nate:** I definitely worked on some interesting algorithms, and overall am a fan of the project we've created. Perhaps the most interesting problem I had to solve was the paint bucket tool. I first wrote a recursive approach to the problem, which worked for filling small sections of the canvas. However, when using this tool on large sections it would hit a max recursion depth and segfault. Fixing this issue was an interesting bit of problem solving and helped expand my understanding of C, and serves as a good example of my goals being fulfilled. I did feel that my goal of nicely structured code was addressed at the beginning, with the creation of header files, a makefile, and generally good organization, but as the project came to a close the paint.c file grew large and unwieldy.

**Ben:** I felt, given the parts of the project I worked on, I accomplished my learning goals, namely working with APIs. As the user interface is comprised entirely of GTK components, in addition to some of the backend like tool switching and starting new windows, working with existing functions, getting to understand GTK’s gobjects (gint, gchar, etc) and how to construct a larger framework from existing code definitely felt like I was meeting the goal I had in mind starting out this project. I would have liked to do a bit more with algorithms but still felt I got some use with them tangentially to the tool creation.

