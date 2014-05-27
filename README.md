# Graphene
###  A thin layer of types for graphic libraries

When creating graphic libraries you most likely end up dealing with points
and rectangles. If you're particularly unlucky, you may end up dealing
with affine matrices and 2D transformations. If you're writing a graphic
library with 3D transformations, though, you are going to hit the jackpot:
4x4 matrices, projections, transformations, vectors, and quaternions.

Most of this stuff exists, in various forms, in other libraries, but it
has the major drawback of coming along with the rest of those libraries,
which may or may not be what you want. Those libraries are also available
in various languages, as long as those languages are C++; again, it may or
may not be something you want.

For this reason, I decided to write the thinnest, smallest possible layer
needed to write a canvas library; given its relative size, and the
propensity for graphics libraries to have a pun in their name, I decided
to call it Graphene.

This library provides types and their relative API; it does not deal with
windowing system surfaces, drawing, scene graphs, or input. You're
supposed to do that yourself, in your own canvas implementation, which is
the whole point of writing the library in the first place.

### Dependencies

Graphene has little to no dependencies.

Graphene optionally uses the GObject type system to provide types that can
be used by GObject-based libraries for properties and marshalling of signal
arguments. The type symbols are compiled inside the same shared library as
all the other symbols. This dependency can be disabled by passing the
`--disable-gobject-types` command line switch to the `configure` script.
It is possible to check if Graphene has been compiled with GObject types by
checking if the `graphene-gobject-1.0` pkg-config module exists and it is
installed on your system.

Graphene depends on GLib for its test suite. It is possible to disable
building the test suite by passing the `--disable-tests` command line switch
to the `configure` script. Similarly, the micro-benchmark suite also depends
on GLib.

Graphene contains fast paths for speeding up vector operations; those
optimizations are optional, and used only if both Graphene was compiled
with support for them *and* if the system you're running on has them.
Currently, Graphene supports the following platform-specific fast paths:

 * Streaming SIMD Extensions (SSE) 2
 * ARM NEON (*experimental*)
 * GCC vector extensions

In the remote case in which none of these optimizations are available,
Graphene will fall back to a naive scalar implementation.

### Installation

In order to build and install Graphene you will need development tools and
the headers of the dependencies.

First of all, clone the Git repository:

    $ git clone git://github.com/ebassi/graphene
    $ cd graphene

Then run:

    $ ./autogen.sh
    $ make
    $ make check
    # make install

It is possible, when building Graphene, to disable specific optimizations by
passing options to the `configure` script:

 * `--disable-sse2` - will disable the SSE2 fast paths
 * `--disable-arm-neon` - will disable the ARM NEON fast paths
 * `--disable-gcc-vector` - will disable the GCC vector intrinsics

See the output of `configure --help` for more information.

### Contributing

If you found a bug (which I'm sure there will be plenty), or if you want
to add your own pet feature, then follow these steps:

 1. Fork the [ebassi/graphene](https://github.com/ebassi/graphene) repo
 2. Fix bugs or add new features and push them to your clone
 3. Open [a new issue](https://github.com/ebassi/graphene/issues/new)
 4. Open [a pull request](https://github.com/ebassi/graphene/pulls)
 5. Wait for me to give feedback on the pull request
 6. Celebrate when your code gets merged

That's pretty much it.

## Documentation

### Available types

Graphene provides common types needed to handle 3D transformations:

 * 2D points
 * 3D points
 * rectangles
 * quads
 * quaternions
 * vectors (2, 3, or 4-sized)
 * matrices

All types can be placed on the stack, but provide allocation/free functions
for working on the heap as well. The contents of all structure types, except
for points and rectangles, should be considered private, and should never be
accessed directly.

### Known issues

 * Segfault in the scalar code path for matrix interpolation with clang.
   It seems that compiling with clang and `-O2` causes a segmentation fault
   in the `graphene_matrix_t` interpolation code in an assignment between
   two SIMD4f types, when using the scalar implementation. Tracked in
   [issue 1](https://github.com/ebassi/graphene/issues/1).

### License

Graphene is released under the terms of the MIT/X11 license.

See the `LICENSE` file for more details.
