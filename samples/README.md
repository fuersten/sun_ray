# Samples

## Dependencies

- image magick
- ffmpeg

## Installation on Mac

- use brew to install

```sh
  $ brew install imagemagick
  $ brew install ffmpeg
```

## Create Images from Scripts

- use the ```sun_ray``` command line tool to create images from scripts

```sh
  $ sun_ray /some/path/to/samples/sample_1.wsl
```

## Create a Video from Images

- use the ```make_movie.sh``` script to create a small mpeg movie from an image series

```sh
  $ ./make_movie.sh /some/path/to/samples/bouncing.wsl bounce
```

## Create Animated gif from Images

```sh
  $ convert -delay 20 -loop 0 bounce*.png bounce.gif
```

