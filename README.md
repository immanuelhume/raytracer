This is yet another follow-along of Peter Shirley's [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) book.
By default, we use a quarter of the cores available on the host, and display the image in an SDL window.

```
Usage: rtc [OPTION...]
Another Peter Shirley Ray Tracer clone

  -m, --maxdepth=MAX_DEPTH   The maximum recursion depth when bouncing rays
  -p, --png=FILENAME         Save to PNG instead of creating a window
  -r, --res=RESOLUTION       Initial screen resolution, e.g. 1280x720
  -s, --samples=NUM_SAMPLES  Number of samples per pixel
  -?, --help                 Give this help list
      --usage                Give a short usage message
```

![Final render from book1](notes/checkered-floor.png)
