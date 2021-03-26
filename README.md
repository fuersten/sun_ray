# The sun_ray Ray Tracer

The sun_ray ray tracer follows closely the book [The Ray Tracer Challenge](http://raytracerchallenge.com/). I can warmly recommend that book. Actually, one of the best programming related books I have ever read. As the approach to create a ray tracer is test driven, sun_ray has lots of tests and currently around 92.8% of unit test coverage.

I have not finished the book yet and have implemented the ray tracer including up to chapter 12 and parts of chapter 15. Nevertheless, you can create really nice images!

I diverged from the recommendation from the book to use a yaml like approach as scene description format for the images to render. As I am very fond of parsing and compilers, I choose to implement a small domain specific scripting language to generate the images. That actually was a project in itself, but a fun one.

## Platforms

The ray tracer builds and runs on
* Mac OS X
* Linux
* Windows

## Build Instructions

Consult the projects install.md file for detailed build instructions for the specific platforms.

## Usage

In general, you will use the sun_ray command line tool to create images from **wsl** (world scripting language) files. **wsl** is the extension for DSL script files. You can find a couple of samples in the samples project directory.

### Options

| Option | Description |
|--|--|
| help | show usage and options |
| dump | dump the stack machine instructions |
| format | output the formatted input files |
| FILE | files to process |

#### help

Outputs the usage and options of the tool.

	> ./sun_ray --help
	SunRay ray tracer 0.12.0
	(C)2021 Lars-Christian Fuerstenberg

	Usage: sun_ray [ --help ] | [ [-df] <FILE> [<FILE>]... ]
	help:
	  --help                              display this help and exit

	processing options:
	  -d, --dump                          dump instructions
	  -f, --format                        format the program
	  <FILE>                              script to execute

#### dump

Outputs the stack machine instructions for the given **wsl** files, one after the other. It will also execute each file in the order that has been given on the command line.

	> ./sun_ray --dump samples/hello_world.wsl
	SunRay ray tracer 0.12.0
	(C)2021 Lars-Christian Fuerstenberg
	
	================================================================================
	samples/hello_world.wsl:
	================================================================================
	
	PUSH Hello world!
	PUSH 1.00
	PUSH 24.00
	FUNC
	DROP
	
	================================================================================
	
	Hello world!

#### format

Formats the given **wsl** files, one after the other. It will also execute each file in the order that has been given on the command line.

	./sun_ray --format samples/hello_world.wsl
	SunRay ray tracer 0.12.0
	(C)2021 Lars-Christian Fuerstenberg
	
	================================================================================
	samples/hello_world.wsl:
	================================================================================
	
	println('Hello world!')
	
	================================================================================
	
	Hello world!

## WSL Scripting Language

The WSL scripting language is a dynamically typed imperative programming language. It has some build in classes to manipulate the world, but no user defined classes can be created. Currently, it lacks support for defining own functions. I might change that in the future.

Let's check out the most simple, aka 'Hello world!', example:

	# The obligatory 'Hello world!' example.
	println('Hello world!')

The `#` introduces a line comment. Everything after the hash is ignored by the compiler. In the next line, the `println` function is called. It will print the formatted string, more on that later, to stdout and add a new line. Thats it! There is no main function or similar. The execution will start directly with the first line in the file. The language has no return statement and the result of the execution will be the result of the last executed statement.

### Keywords

Valid keywords are:
* and
* end
* false
* if
* not
* or
* true
* while
* all build-in classes and functions

### Comments

The `#` introduces a line comment. Everything after the `#` is ignored. The `#` can be positioned anywhere on a column. There is only one kind of comment.

	# This is a comment
	n = 24              # This is also a valid comment

### Variables and Types

As **WSL** is a dynamically typed language, a variable can change the type by assigning it a value from another type. The type of a variable will also be inferred by the compiler, hence a variable has always to be initialized with some value.

	n = 42              # n is a real
	n = 'I love this!'  # n is a string
	n = 47.11           # n is a real
	n = true            # n is a boolean
	n = [1, 1.8, 0]     # n is a vector
	n = (1, 1.8, 0)     # n is a point
	n = Color(1, 1, 1)  # n is a Color

#### Identifier

An identifier always starts with a lower character (a-z) and can be composed of alphanumeric characters (a-z, A-Z, and 0-9) and `_`. Some examples of valid identifier:

	n
	n4711
	base_material
	world
	pUSCHEL

Invalid identifier:

	4n
	_test
	Cool
	n$4711

The set of identifiers has no intersection with the set of keywords. This means that an identifier is not allowed to be a keyword.

#### Types

**WSL** supports the following types

| Type | Description |
|--|--|
| boolean | is either `true` or `false` |
| number | is a floating point double number |
| string | a string enclosed in single quotes `'` |
| vector | a three element vector with x, y, and z component describing a direction |
| point | a three element point with x, y, and z component describing a location |
| Build-in Classes | classes for graphical elements, canvas, etc. |

#### Boolean

Booleans are mostly used for control flow like `if` or `while`. You cannot calculate with a boolean and it will not be converted automatically into a number. Nevertheless, the boolean type supports some operations:

| Operator | Kind | Resulting Type | Description |
|--|--|--|--|
| and | binary | boolean | Logical `and` between two operands |
| or | binary | boolean | Logical `or` between two operands |
| == | binary | boolean | Compares the two operands for equality  |
| <> | binary | boolean | Compares the two operands for inequality  |
| not | unary | boolean | Inverts the value of the operand |

In a boolean binary operation, all operands have to be of type boolean. A boolean can be either `true` or `false`.

#### Number

The only kind of number **WSL** supports is a double floating point number. It is also the only data type a calculation can be made with. The usual operations are supported for calculations.

Arithmetic operations:

| Operator | Kind | Resulting Type | Description |
|--|--|--|--|
| + | binary | number | Adds two operands |
| - | binary | number | Subtracts two operands |
| / | binary | number | Divides two operands |
| * | binary | number | Multiplies two operands |
| - | unary | number | Negates the operand |

Logical operations:

| Operator | Kind | Resulting Type | Description |
|--|--|--|--|
| == | binary | boolean | Compares the two operands for equality |
| <> | binary | boolean | Compares the two operands for inequality |
| > | binary | boolean | Compares if the left operand is greater than the right one |
| < | binary | boolean | Compares if the left operand is lesser than the right one |
| >= | binary | boolean | Compares if the left operand is greater or equal than the right one |
| <= | binary | boolean | Compares if the left operand is lesser or equal than the right one |

In a number binary operation, all operands have to be of type number.
A number can also be specified in scientific notation, i.e. `n = 2.00600e-002`. Examples for numbers:

	42
	47.11
	2.00600e-002
	-5002

#### String

A string has to be delimited by single quotes:

	'This is a string'
	"This is not a string!"   # will trigger a compile error

It can contain any ASCII character including escape sequences (`\n\r\b` etc.). Escape sequences have to be escaped with `\` in order to be displayed as raw text.

	println('\n')             # will add a new line to stdout
	println('\\n')            # will print \n on stdout

#### Vector

In general a Vector describes a direction. A detailed description of the Vector is given in the **Build-in Classes** section. In addition to the `Vector` keyword, a Vector can be defined with a sequence of three numbers in square brackets. This is just syntactic sugar.

	v = [1, 1.6, 2.0]
	v = Vector(1, 1.6, 2.0)   # this is equivalent

#### Point

In general a Point describes a location. A detailed description of the Point is given in the **Build-in Classes** section. In addition to the `Point` keyword, a Point can be defined with a sequence of three numbers in parenthesis. This is just syntactic sugar.

	p = (1, 1.6, 2.0)
	p = Point(1, 1.6, 2.0)   # this is equivalent

### Control Structures

In general, control structures control the flow of a program. Control structures can be nested and pare always with the most inner `end` keyword.

	if (n > 1)
	  count = 0
	  res = 1
	  while(count < n)
	    res = count * res
	    count = count + 1
	  end
	end

#### If

The `if` control structure is a simple *if* without *else* or *else if*. The expression has to be a logical expression that evaluates to a boolean. There is no automatic conversion from number to boolean. The *if* block has to be terminated by the `end` keyword.

	if (n == 42)
	  println('This is the number')
	end
	if (n <> 42)
	  println('Nah!')
	end

#### While

The `while` expression has to be a logical expression that evaluates to a boolean. There is no automatic conversion from number to boolean. The *while* block has to be terminated by the `end` keyword.

	n = 0
	while(n < 10)
	  n = n + 1               # there is no increment operator
	end

### Build-in Functions

Currently, there are no user defined functions. The only functions available are those provided by the language itself. Specifying wrong parameter types for a function will result in runtime errors. Parameters are immutable and will never be changed by the call.

#### Mathematical Functions

All mathematical functions work on numbers and return a number.

| Function | Arguments | Description |
|--|--|--|
| abs | n | Returns the absolute value of n |
| acos | n | Returns the arc cosine of n in radians |
| asin | n | Returns the arc sine of n in radians |
| atan | n | Returns the arc tangent of n in radians |
| ceil | n | Rounds n up to the nearest integer not smaller than n |
| cos | n | Returns the cosine of an angle of n radians |
| deg_to_rad | n| Returns the radians equivalent of an angle of n degrees |
| exp | n | Returns e raised to the power of n |
| floor | n | Rounds n down to the nearest integer not greater than n |
| log | n | Returns the natural logarithm of n |
| max | n, m | Returns the larger argument |
| min | n, m | Returns the smaller argument |
| mod | n, d | Returns the remainder of n/d rounded towards zero |
| pow | n, e | Returns n raised to the power of e |
| round | n | Rounds to the nearest integer value n, rounding halfway cases away from zero |
| sin | n | Returns the sine of an angle of n radians |
| sqrt | n | Returns the square root of n |
| tan | n | Returns the tangent of an angle of n radians |
| trunc | n | Rounds n towards zero |

#### Pseudo Random Number Generator (PRNG) Functions

All PRNG work on numbers and return a number.

| Function | Arguments | Description |
|--|--|--|
| random | - | Returns a random number between -1.0 and 1.0, depending on the initial seed value. The random number generator is a Mersenne-Twister PRNG. |
| seed | n | Seeds and re-initializes the random engine generator. The same seed will always generate the same number sequence. |

#### Time Functions

| Function | Arguments | Description |
|--|--|--|
| now | - | Returns the current UTC time as a string with the format YYYY-MM-ddThh:mm:ss |

#### Formatting and Printing

| Function | Arguments | Description |
|--|--|--|
| print | v | Prints the string representation of v to stdout |
| print | s, v [,v] | Prints the formatted string to stdout using the format s on the given values. The format is a string with {} as replacement tokens for the values in order. |
| println | v | As print, but add a new line to stdout |
| println | s, v [,v] | As print, but add a new line to stdout |
| str | v | Returns the string representation of v |

### Build-in Classes

The build-in classes form the backbone of the interaction with the actual rendering engine. With objects from these classes, the actual scenes to render are described. In order to render a scene, you need at least a Camera and a World object. It would also be good to add some graphical objects to the scene.

Each class has one or more constructors and some methods and/or properties. Constructors are used to create objects (instances) from a specific class. Methods and properties are used to manipulate an object or retrieve informations from an object. In essence, a property is syntactic sugar for setter and getter from a class. All methods and properties are accessed using a `.` operator on an object.

#### Camera

| Constructor | Arguments | Description |
|--|--|--|
| Camera | - | Creates a default camera object |

| Methods | Arguments | Description |
|--|--|--|
| render | w | Renders the given World w with the camera as view into the world. Returns a Canvas object with the rendered scene. |

| Property | Read/Write | Description |
|--|--|--|
| from | W | Specifies the point where the camera is located in the scene |
| to | W | Specifies the point where the camera looks into the scene |
| up | W | Specifies the vector of the cameras orientation in the scene |
| field_of_view | W | The cameras field of view in radians |
| horizontal | R/W | The width of the cameras view |
| vertical | W | The height of the cameras view |

Canvas
CheckerPattern
Color
Cube
Disk
GradientPattern
Light
Material
Measurement
Plane
Point
RingPattern
Sphere
StripePattern
Triangle
Vector
World

