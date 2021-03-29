![Supported Platforms](https://img.shields.io/badge/platform-macOS%20%7C%20Linux%20%7C%20Windows-blue.svg)
![License: BSD-3](https://img.shields.io/badge/License-BSD3-blue.svg)
[![GitHub Build Status](https://github.com/fuersten/sun_ray/workflows/CMake%20Build%20Matrix/badge.svg)](https://github.com/fuersten/sun_ray/actions)

# The sun_ray Ray Tracer

The sun_ray ray tracer follows closely the book [The Ray Tracer Challenge](http://raytracerchallenge.com/). I can highly recommend that book. Actually, one of the best programming related books I have ever read. As the approach to create a ray tracer is test driven, sun_ray has lots of tests and currently around 92.8% of unit test coverage.

I have not finished the book yet and have implemented the ray tracer including up to chapter 12 and parts of chapter 15. Nevertheless, you can create really nice images!

I diverged from the recommendation from the book to use a yaml like approach as scene description format for the images to render, as there is no good ready to use implementation available for C++. As I am very fond of parsing and compilers, I choose to implement a small domain specific scripting language to generate the images. That actually was a project in itself, but a fun one.

## Platforms

The ray tracer builds and runs on
* Mac OS X
* Linux
* Windows

## Samples

Images of all samples can be found [here](samples).

## Build Instructions

Consult the projects install.md file for detailed build instructions for the specific platforms.

## Usage

In general, you will use the sun_ray command line tool to create images from **wsl** (world scripting language) files. **wsl** is the extension for DSL script files. You can find a couple of samples in the samples project directory. It will return 1 on success and -1 on any failures such as compilation, runtime errors, or file not found. Upon a failure, the execution will be terminated immediately, even if there still are unprocessed script files.

### Options

| Option | Description |
|:--|:--|
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

	> ./sun_ray --format samples/hello_world.wsl
	SunRay ray tracer 0.12.0
	(C)2021 Lars-Christian Fuerstenberg
	
	================================================================================
	samples/hello_world.wsl:
	================================================================================
	
	println('Hello world!')
	
	================================================================================
	
	Hello world!

#### FILE

Executes the given scripts, one after the other in the order that has been given on the command line.

	> ./sun_ray --format samples/hello_world.wsl samples/fibonacci.wsl
	SunRay ray tracer 0.12.0
	(C)2021 Lars-Christian Fuerstenberg

	================================================================================
	../../../samples/hello_world.wsl:
	================================================================================

	Hello world!
	================================================================================
	../../../samples/fibonacci.wsl:
	================================================================================

	n => 90.00 = 2880067194370816000.00

## WSL Scripting Language

The WSL scripting language is a dynamically typed imperative programming language. It has some build in classes to manipulate the world, but no user defined classes can be created. Currently, it lacks support for defining own functions. I might change that in the future.

Let's check out the most simple, aka 'Hello world!', example:

	# The obligatory 'Hello world!' example.
	println('Hello world!')

The `#` introduces a line comment. Everything after the hash is ignored by the compiler. In the next line, the `println` function is called. It will print the formatted string (more on that later) to stdout and add a new line. Thats it! There is no main function or similar. The execution will start directly with the first line in the file. As the language has no return statement, the execution of code can only be controlled by the two control structures `if` and `while`.

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
	n = 24                              # This is also a valid comment

### Variables and Types

As **WSL** is a dynamically typed language, a variable can change the type by assigning it a value from another type. The type of a variable will also be inferred by the compiler, hence a variable has always to be initialized with some value and has no explicit type.

	n = 42                              # n is a number
	n = 'I love this!'                  # n is a string
	n = 47.11                           # n is a number
	n = true                            # n is a boolean
	n = [1, 1.8, 0]                     # n is a Vector
	n = (1, 1.8, 0)                     # n is a Point
	n = Color(1, 1, 1)                  # n is a Color

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
|:--|:--|
| Boolean | is either `true` or `false` |
| Number | is a floating point double number |
| String | a string enclosed in single quotes `'` |
| Vector | a three element vector with x, y, and z component describing a direction |
| Point | a three element point with x, y, and z component describing a location |
| Build-in Classes | classes for shapes, canvas, etc. |

#### Boolean

Booleans are mostly used for control flow like `if` or `while`. You cannot calculate with a boolean and it will not be converted automatically into a number. Nevertheless, the boolean type supports some operations:

| Operator | Kind | Resulting Type | Description |
|:--|:--|:--|:--|
| and | binary | Boolean | Logical `and` between two operands |
| or | binary | Boolean | Logical `or` between two operands |
| == | binary | Boolean | Compares the two operands for equality  |
| <> | binary | Boolean | Compares the two operands for inequality  |
| not | unary | Boolean | Inverts the value of the operand |

In a boolean binary operation, all operands have to be of type Boolean. A boolean can be either `true` or `false`.

#### Number

The only kind of number **WSL** supports is a double floating point number. It is also the only data type a calculation can be made with. The usual operations are supported for calculations.

Arithmetic operations:

| Operator | Kind | Resulting Type | Description |
|:--|:--|:--|:--|
| + | binary | Number | Adds two operands |
| - | binary | Number | Subtracts two operands |
| / | binary | Number | Divides two operands |
| * | binary | Number | Multiplies two operands |
| - | unary | Number | Negates the operand |

Logical operations:

| Operator | Kind | Resulting Type | Description |
|:--|:--|:--|:--|
| == | binary | Boolean | Compares the two operands for equality |
| <> | binary | Boolean | Compares the two operands for inequality |
| > | binary | Boolean | Compares if the left operand is greater than the right one |
| < | binary | Boolean | Compares if the left operand is lesser than the right one |
| >= | binary | Boolean | Compares if the left operand is greater or equal than the right one |
| <= | binary | Boolean | Compares if the left operand is lesser or equal than the right one |

In a number binary operation, all operands have to be of type Number.
A number can also be specified in scientific notation, i.e. `n = 2.00600e-002`. Examples for numbers:

	42
	47.11
	2.00600e-002
	-5002

#### String

A string has to be delimited by single quotes:

	'This is a string'
	"This is not a string!"             # will trigger a compile error

It can contain any ASCII character including escape sequences (`\n\r\t` etc.). Escape sequences have to be escaped with `\` in order to be displayed as raw text.

	println('\n')                       # will add a new line to stdout
	println('\\n')                      # will print \n on stdout

Strings can be concatenated with `+`

	s = 'I am ' + 'legend'
	s = s + ', you know'                # s = 'I am legend, you know'

#### Vector

In general a vector describes a direction. A detailed description of the vector is given in the **Build-in Classes** section. In addition to the `Vector` keyword, a vector can be defined with a sequence of three numbers in square brackets. This is just syntactic sugar.

	v = [1, 1.6, 2.0]
	v = Vector(1, 1.6, 2.0)             # this is equivalent

#### Point

In general a point describes a location. A detailed description of the point is given in the **Build-in Classes** section. In addition to the `Point` keyword, a point can be defined with a sequence of three numbers in parenthesis. This is just syntactic sugar.

	p = (1, 1.6, 2.0)
	p = Point(1, 1.6, 2.0)              # this is equivalent

### Control Structures

In general, control structures control the flow of a program. Control structures can be nested and pair always with the most inner `end` keyword.

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

The `while` expression has to be a logical expression that evaluates to a boolean. There is no automatic conversion from number to boolean. The *while* block has to be terminated by the `end` keyword. There is no for loop, as a it can be implemented easily with a while loop.

	n = 0
	while(n < 10)
	  n = n + 1                         # there is no increment operator
	end

### Build-in Functions

Currently, there are no user defined functions. The only functions available are those provided by the language itself. Specifying wrong parameter types for a function will result in runtime errors. Parameters are immutable and will never be changed by the call.

#### Mathematical Functions

All mathematical functions work on numbers and return a number.

| Function | Arguments | Description |
|:--|:--|:--|
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

Examples

	x = max(7, 11)                      # x is set to 11
	x = min(7, 11)                      # x is set to 7
	n = deg_to_rad(65.0)                # n is set to 1.13
	m = mod(47.00, 11.0)                # m is set to 3.0
	
#### Pseudo Random Number Generator (PRNG) Functions

All PRNG work on numbers and return a number.

| Function | Arguments | Description |
|:--|:--|:--|
| random | - | Returns a random number between -1.0 and 1.0, depending on the initial seed value. The random number generator is a Mersenne-Twister PRNG. |
| seed | n | Seeds and re-initializes the random engine generator. The same seed will always generate the same number sequence. |

Example

	seed(328827939)
	x = random()                        # x is 0.49
	x = random()                        # x is 0.57

#### Time Functions

| Function | Arguments | Description |
|:--|:--|:--|
| now | - | Returns the current UTC time as a string with the ISO 8601 date format YYYY-MM-DDThh:mm:ss.sss |

Example

	println(now())                      # prints '2021-03-27T11:24:05.310' to stdout

#### Formatting and Printing

| Function | Arguments | Description |
|:--|:--|:--|
| print | v | Prints the string representation of v to stdout |
| print | s, v [,v] | Prints the formatted string to stdout using the format s on the given values. The format is a string with {} as replacement tokens for the values in order. |
| println | v | As print, but add a new line to stdout |
| println | s, v [,v] | As print, but add a new line to stdout |
| str | v | Returns the string representation of v. v can be any type including objects. |

Examples:

	n = 42
	println('The number is {}', n)      # prints 'The number is 42.00' to stdout
	println('{} is {}', 'n', n)         # prints 'n is 42.00' to stdout
	
	println('The color is {}', Color(0.45, 1.0, 0.35)) 
	                                    # prints 'The color is 
	                                    #   <Color r: 0.45 g: 1 b: 0.35>' to stdout
	s = 'Number' + str(n)
	println(s)                          # prints 'Number42' to stdout

### Build-in Classes

The build-in classes form the backbone of the interaction with the actual rendering engine. With objects from these classes, the actual scenes to render are described. In order to render a scene, you need at least a Camera and a World object. It would also be good to add some graphical shapes to the scene.

Each class has one or more constructors and some methods and/or properties. Constructors are used to create objects (instances) from a specific class. Methods and properties are used to manipulate an object or retrieve informations from an object. In essence, a property is syntactic sugar for setter and getter from a class. A property can be readable, writeable, or both. All methods and properties are accessed using a `.` operator on an object. Most of the methods actually return the manipulated object in order to be able to chain calls for the same object resulting in more dense and readable code.

#### Camera

The camera defines the properties of your scene. It is the window into your scene and defines how "the picture is taken". A camera can be moved around, rotated, zoomed in and out, etc.

| Constructor | Description |
|:--|:--|
| `Camera()` | Creates a default camera object with `from` Point(0.0, 1.5, 0.7), `to` Point(0.0, 1.0, 0.0), `up` Vector(0.0, 1.0, 0.0), `field_of_view` PI/3.0, `horizontal` 500.0, and `vertical` 250.0 |

| Methods | Description |
|:--|:--|
| `Canvas render(World w)` | Renders the given World w with the camera as view into the world. Returns a Canvas object with the rendered scene. |

| Property | Read/Write | Type | Description |
|:--|:--|:--|:--|
| `from` | W | Point | Specifies the point where the camera is located in the scene |
| `to` | W | Point | Specifies the point to where the camera looks into the scene |
| `up` | W | Vector | Specifies the vector of the cameras orientation in the scene |
| `field_of_view` | W | Number | The cameras field of view angle in radians |
| `horizontal` | R/W | Number | The width of the cameras view in pixel |
| `vertical` | W | Number | The height of the cameras view in pixel |

Example:

	pi = 3.141592653589
	camera = Camera()
	camera.from = (0, 2.5, -7)
	camera.to = (0, 1, 0)
	camera.up = [0, 1, 0]
	camera.field_of_view = pi / 3
	camera.horizontal = 5000
	camera.vertical = camera.horizontal / 2

#### Canvas

The canvas is where the scene will be rendered on. Normally, you do not create a canvas yourself, but use the canvas that is returned by the render method of the camera. Nevertheless, you can use a canvas to draw arbitrary stuff on it.

| Constructor | Description |
|:--|:--|
| `Canvas(Number width, Number height)` | Creates a Canvas object with the given width and height in pixel. All pixels will be initialized to the color Color(0, 0, 0), aka black. |

| Methods | Description |
|:--|:--|
| `Number set_pixel(Number x, Number y, Color c)` | Sets the pixel at the location x, y to the color c. x and y have to fit into the canvas, otherwise a runtime error will be raised. x and y are zero based. Returns 0. |
| `Number write(String pathname)` | Saves the canvas to the given file. The extension will be set to '.png' if no extension is given. The image format is currently always PNG. Returns 0. |

| Property | Read/Write | Type | Description |
|:--|:--|:--|:--|
| `width` | R | Number | Specifies the width of the canvas in pixel |
| `height` | R | Number | Specifies the height of the canvas in pixel |

Example:

	canvas = Canvas(100, 100)
	n = 0
	while(n < 100)
	  canvas.set_pixel(n, n, Color(0.75, 0.5, 0.2))
	  n = n + 1
	end
	canvas.write('test_image')

#### CheckerPattern (is a Pattern)

Applying a checker pattern results in an effect like a three dimensional checker board.

| Constructor | Description |
|:--|:--|
| `CheckerPattern(Color a, Color b)` | Creates a CheckerPattern with the two given colors. The two colors will be used in an alternating form to create the pattern. |

Inherits all methods from the Pattern base class.

#### Color

Represents a color. 

| Constructor | Description |
|:--|:--|
| `Color()` | Creates a default Color object with all components set to 0.0. |
| `Color(Number red, Number green, Number blue)` | Creates a Color object with the given red, green, and blue values. The values should have a range between 0.0 and 1.0. |

| Property | Read/Write | Type | Description |
|:--|:--|:--|:--|
| `red` | R/W | Number | Specifies the value of the red color component |
| `green` | R/W | Number | Specifies the value of the green color component |
| `blue` | R/W | Number | Specifies the value of the blue color component |

Examples:

	black = Color(0, 0, 0)
	white = Color(1, 1, 1)
	grey = Color(0.5, 0.5, 0.5)
	light_grey = Color()
	light_grey.red = 0.7
	light_grey.green = 0.7
	light_grey.blue = 0.7

#### Cube (is a Shape)

A Cube is, in fact, an axis-aligned bounding box (AABB) with an edge length of 1. In order to create boxes, rectangles or other cube like structures, the cube can be transformed in various ways.

| Constructor | Description |
|:--|:--|
| `Cube(Material)` | Creates a Cube with the given Material m |

Inherits all methods from the Shape base class.

Examples:

	blue_material = Material()
	blue_material.color = Color(0.537, 0.831, 0.914)
	world.add(Cube(blue_material).scale(3.5, 3.5, 3.5).translate(8.5, 1.5, -0.5))

#### Disk (is a Shape)

The Disk is an infinitely thin disk with a radius of 1. An inner radius can be specified in order to create a disk with a hole in it. Nice for making some planet rings! The disk is actually not described in the book, but I thought it would be a good addition.

| Constructor | Description |
|:--|:--|
| `Disk(Material)` | Creates a Disk with the given Material m |

| Property | Read/Write | Type | Description |
|:--|:--|:--|:--|
| `inner_radius` | W | Number | Specifies the inner radius of the disk |

Inherits all methods from the Shape base class.

Examples:

	material = Material()
	material.ambient = 0.3
	material.diffuse = 0.5
	material.specular = 0.5
	material.shininess = 100
	material.reflective = 0.3
	material.pattern = CheckerPattern(Color(1, 0.75, 0.35), Color(0.35, 0.75, 1))

	disk = Disk(material)
	disk.inner_radius = 0.5

#### GradientPattern (is a Pattern)

Adds a gradient to a material blending smoothly between two colors.

| Constructor | Description |
|:--|:--|
| `GradientPattern(Color a, Color b)` | Creates a GradientPattern with the two given colors. The two colors represent the left and right color values of the gradient. The patterns colors will be lineary interpolated from these colors. |

Inherits all methods from the Pattern base class.

#### Light

Represents a point light illuminating the scene depending on the intensity and location. A point light does not have a size or direction.

| Constructor | Description |
|:--|:--|
| `Light()` | Creates a default Light object with the `location` Point(-10.0, 10.0, -10.0) and the `intensity` Color(1.0, 1.0, 1.0)  |
| `Light(Point location, Color intensity)` | Creates a Light object with the given location and intensity. |

| Property | Read/Write | Type | Description |
|:--|:--|:--|:--|
| `location` | W | Point | Specifies the location of the light |
| `intensity` | W | Color | Specifies the intensity of the light |

Examples:

	light = Light(Point(-10, 10, -10), Color(0.70, 0.70, 0.70))

#### Material

A material gives a shape its specific look. Along the color it also specifies other properties like illumination parameters, reflectiveness, transparency, pattern, etc.

| Constructor | Description |
|:--|:--|
| `Material()` | Creates a default Material object with the `color` Color(1.0, 1.0, 1.0), `ambient` of 0.1, `diffuse` of 0.9, `specular` of 0.9, `shininess` of 200.0, `reflective` 0.0, `transparency` 0.0, `refractive_index` of 1.0 and no `pattern` |
| `Material(Material m)` | Creates a Material object initialized by the given material m |

| Property | Read/Write | Type | Description |
|:--|:--|:--|:--|
| `color` | W | Color | Specifies the color of the material |
| `ambient` | W | Number | Specifies the ambient of the material |
| `diffuse` | W | Number | Specifies the diffuse of the material |
| `specular` | W | Number | Specifies the specular of the material |
| `shininess` | W | Number | Specifies the shininess of the material |
| `reflective` | W | Number | Specifies the reflectivity of the material |
| `transparency` | W | Number | Specifies the transparency of the material |
| `refractive_index` | W | Number | Specifies the refractive index of the material |
| `pattern` | W | Pattern | Specifies the pattern of the material |

Examples:

	material = Material()
	material.ambient = 0.3
	material.diffuse = 0.5
	material.specular = 0.5
	material.shininess = 100
	material.reflective = 0.3
	material.pattern = CheckerPattern(Color(1, 1, 1), Color(0.5, 0.5, 0.5))

#### Measurement

The measurement class is used to measure time durations between arbitrary events.

| Constructor | Description |
|:--|:--|
| `Measurement()` | Creates a Measurement object  |

| Methods | Description |
|:--|:--|
| `Number elapsed_time()` | Returns the milliseconds since either the construction of the measurement object or the last elapsed_time call. |

Examples:

	println('Render world')
	measure = Measurement()
	canvas = camera.render(world)
	println('World rendered in {} milliseconds', measure.elapsed_time())
	canvas.write('fresnel')
	println('World stored in {} milliseconds', measure.elapsed_time())

#### Shape

Is the base class of all graphical shapes like Sphere, Plain, etc. It cannot be instantiated, but provides a couple of methods and properties. All shapes will be created at the center of the scene (0, 0, 0) and have to be transformed (moved, sized, rotated, etc) in order to have the desired form and placement.

All the shapes, with the exception of the Disk, map directly to constructs from the book.

| Methods | Description |
|:--|:--|
| `Shape scale(Number x, Number y, Number z)` | Scales the object according to the given scaling factors. Returns the shape. |
| `Shape rotate_x(Number n)` | Rotates the object n radians around the x-axis. Returns the shape. |
| `Shape rotate_y(Number n)` | Rotates the object n radians around the y-axis. Returns the shape. |
| `Shape rotate_z(Number n)` | Rotates the object n radians around the z-axis. Returns the shape. |
| `Shape translate(Number x, Number y, Number z)` | Translates (aka moves) the object according to the given displacements. Returns the shape. |
| `Shape shear(Number xy, Number xz, Number yx, Number yz, Number zx, Number zy)` | Shears the object according to the given proportions. The first parameter describes the change of x in proportion to y. The other parameters work analogous. Returns the shape. |

| Property | Read/Write | Type | Description |
|:--|:--|:--|:--|
| `casts_shadow` | W | Boolean | Specifies if this shape casts a shadow |

#### Pattern

Is the base class for all patterns. It cannot be instantiated, but provides a couple of methods.

| Methods | Description |
|:--|:--|
| `Pattern scale(Number x, Number y, Number z)` | Scales the pattern according to the given scaling factors. Returns the pattern. |
| `Pattern rotate_x(Number n)` | Rotates the pattern n radians around the x-axis. Returns the pattern. |
| `Pattern rotate_y(Number n)` | Rotates the pattern n radians around the y-axis. Returns the pattern. |
| `Pattern rotate_z(Number n)` | Rotates the pattern n radians around the z-axis. Returns the pattern. |
| `Pattern translate(Number x, Number y, Number z)` | Translates (aka moves) the pattern according to the given displacements. Returns the pattern. |
| `Pattern shear(Number xy, Number xz, Number yx, Number yz, Number zx, Number zy)` | Shears the pattern according to the given proportions. The first parameter describes the change of x in proportion to y. The other parameters work analogous. Returns the pattern. 

#### Plane (is a Shape)

The Plane is a perfectly flat surface stretching to infinity in the xz dimensions. Using transformations, the plane can be rotated and pushed to whatever location is necessary. Great for creating backdrops.

| Constructor | Description |
|:--|:--|
| `Plane(Material)` | Creates a Plane with the given Material m |

Inherits all methods from the Shape base class.

Examples:

	backdrop_material = Material()
	backdrop_material.color = Color(1, 1, 1)
	backdrop_material.ambient = 1
	backdrop_material.diffuse = 0
	backdrop_material.specular = 0
	backdrop = Plane(backdrop_material).rotate_x(pi / 2).translate(0, 0, 500)
	world.add(backdrop)

#### Point

Points describe locations in a scene.

| Constructor | Description |
|:--|:--|
| `Point(Number x, Number y, Number z)` | Creates a Point object with the given coordinates |

| Methods | Description |
|:--|:--|
| `Point add(Vector v)` | Returns a newly created point initialized with the result of the addition of the point and the given vector. Does not modify the object itself. |

| Property | Read/Write | Type | Description |
|:--|:--|:--|:--|
| `x` | R | Number | Specifies the x component of the point |
| `y` | R | Number | Specifies the y component of the point |
| `z` | R | Number | Specifies the z component of the point |

Examples:

	projectile_position = Point(0, 1, 0)
	projectile_velocity = Vector(1, 1.8, 0).normalize().multiply(11.25)
	projectile_position = projectile_position.add(projectile_velocity)

#### RingPattern (is a Pattern)

Adds a ring pattern to a material. The pattern shows alternating colored rings.

| Constructor | Description |
|:--|:--|
| `RingPattern(Color a, Color b)` | Creates a RingPattern with the two given colors. The rings will have the two colors alternating. |

Inherits all methods from the Pattern base class.

#### Sphere (is a Shape)

The sphere is, in essence, the surface of a ball. It has a radius of 1.

| Constructor | Description |
|:--|:--|
| `Sphere(Material)` | Creates a Sphere with the given Material m |

Inherits all methods from the Shape base class.

Examples:

	sphere_material = Material()
	sphere_material.color = Color(0.373, 0.404, 0.550)
	sphere_material.diffuse = 0.2
	sphere_material.ambient = 0.0
	sphere_material.specular = 1.0
	sphere_material.shininess = 200
	sphere_material.reflective = 0.7
	sphere_material.transparency = 0.7
	sphere_material.refractive_index = 1.5
	sphere = Sphere(sphere_material).scale(3.5, 3.5, 3.5).translate(1, -1, 1)
	world.add(sphere)

#### StripePattern (is a Pattern)

Adds a stripe pattern to a material. The pattern shows alternating colored stripes.

| Constructor | Description |
|:--|:--|
| `StripePattern(Color a, Color b)` | Creates a StripePattern with the two given colors. The stripers will have the two colors alternating. |

Inherits all methods from the Pattern base class.

#### Triangle (is a Shape)

The Triangle is the base for all complex objects. In general, it is not an easy task to compose an object from triangles and one would usually use a tool to do that and import some file into the ray tracer. Unfortunately, I haven't completed that chapter yet.

| Constructor | Description |
|:--|:--|
| `Triangle(Material, Point v1, Point v2, Point v3)` | Creates a Triangle with the given Material m and using the three given points as the three vertices. |

Inherits all methods from the Shape base class.

Examples:

	material = Material()
	material.color = Color(0.84, 0.65, 0.40)
	v1 = Point(1, 0, 0)
	v2 = Point(-1, 0, 0)
	v3 = Point(0, 1, 0.5)
	world.add(Triangle(material, v1, v2, v3).rotate_y(deg_to_rad(40)).translate(-1, 0, 0))

	v1 = Point(-1, 0, 1)
	v2 = Point(-1, 0, 0)
	v3 = Point(0, 1, 0.5)
	world.add(Triangle(material, v1, v2, v3).rotate_y(deg_to_rad(40)).translate(-1, 0, 0))

	v1 = Point(1, 0, 1)
	v2 = Point(1, 0, 0)
	v3 = Point(0, 1, 0.5)
	world.add(Triangle(material, v1, v2, v3).rotate_y(deg_to_rad(40)).translate(-1, 0, 0))

	v1 = Point(-1, 0, 1)
	v2 = Point(1, 0, 1)
	v3 = Point(0, 1, 0.5)
	world.add(Triangle(material, v1, v2, v3).rotate_y(deg_to_rad(40)).translate(-1, 0, 0))

#### Vector

Vectors describe directions in the scene.

| Constructor | Description |
|:--|:--|
| `Vector(Number x, Number y, Number z)` | Creates a Vector object with the given coordinates |

| Methods | Description |
|:--|:--|
| `Vector add(Vector v)` | Returns a newly created vector initialized with the result of the addition of the vector and the given vector. Does not modify the object itself. |
| `Vector normalize()` | Returns a newly created vector initialized with the result of the normalized vector. Does not modify the object itself. |
| `Vector multiply(Number n)` | Returns a newly created vector initialized with the result of the multiplication of the vector with n. Does not modify the object itself. |

| Property | Read/Write | Type | Description |
|:--|:--|:--|:--|
| `x` | R | Number | Specifies the x component of the vector |
| `y` | R | Number | Specifies the y component of the vector |
| `z` | R | Number | Specifies the z component of the vector |

Examples:

	projectile_position = Point(0, 1, 0)
	projectile_velocity = Vector(1, 1.8, 0).normalize().multiply(11.25)
	projectile_position = projectile_position.add(projectile_velocity)

#### World

The world is the container of all scene objects and lights.

| Constructor | Description |
|:--|:--|
| `World()` | Creates a World object |

| Methods | Description |
|:--|:--|
| `World add(Shape s)` | Adds shape s to the world for rendering. Returns the world. The shape will actually be copied into the world and will be immutable in the world.  |
| `World add(Light l)` | Adds light l to the world for rendering. Returns the world. The light will actually be copied into the world and will be immutable in the world.  |

| Property | Read/Write | Type | Default | Description |
|:--|:--|:--|:--|:--|
| `shadows` | W | Boolean | true | Specifies if shadows shall be activated |
| `reflections` | W | Boolean | true | Specifies if reflections shall be activated |
| `refractions` | W | Boolean | true | Specifies if refractions shall be activated |

Examples:

	pi = 3.141592653589
	
	camera = Camera()
	camera.from = (0, 2.5, -7)
	camera.to = (0, 0, 0)
	camera.up = [0, 1, 0]
	camera.field_of_view = pi / 3
	camera.horizontal = 1500
	camera.vertical = camera.horizontal / 2
	
	world = World()
	world.add(Light(Point(-10, 10, -10), Color(0.70, 0.70, 0.70)))
	world.add(Sphere(Material()))
	world.shadows = true
	world.reflections = false
	
	canvas = camera.render(world)
	canvas.write('sphere')

