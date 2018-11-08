/**
 * @file
 *
 * This example shows you how to create an application using the EZGL library.
 */

#include <ezgl/application.hpp>
#include <ezgl/camera.hpp>
#include <ezgl/canvas.hpp>
#include <ezgl/control.hpp>
#include <ezgl/graphics.hpp>

#include <iostream>

// Callback functions for event handling
void act_on_mouse_press(ezgl::application *application, GdkEventButton *event, double x, double y);
void act_on_mouse_move(ezgl::application *application, GdkEventButton *event, double x, double y);
void act_on_key_press(ezgl::application *application, GdkEventKey *event, char *key_name);

/**
 * Draw to the main canvas using the provided graphics object.
 *
 * The graphics object expects that x and y values will be in the main canvas' world coordinate system.
 */
void draw_main_canvas(ezgl::renderer &g);

static ezgl::rectangle initial_world{{0, 0}, 1100, 1150};

/**
 * The start point of the program.
 *
 * This function initializes an ezgl application and runs it.
 *
 * @param argc The number of arguments provided.
 * @param argv The arguments as an array of c-strings.
 *
 * @return the exit status of the application run.
 */
int main(int argc, char **argv)
{
  ezgl::application::settings settings;

  // Path to the resource that contains an XML description of the UI.
  // Note: this is not a file path, it is a resource path.
  settings.main_ui_resource = "/edu/toronto/eecg/ezgl/ece297/cd000/main.ui";

  // Note: the "main.ui" file has a GtkWindow called "MainWindow".
  settings.window_identifier = "MainWindow";

  // Note: the "main.ui" file has a GtkDrawingArea called "MainCanvas".
  settings.canvas_identifier = "MainCanvas";

  // Create our EZGL application.
  ezgl::application application(settings);

  application.add_canvas("MainCanvas", draw_main_canvas, initial_world);

  // Run the application until the user quits.
  // This hands over all control to the GTK runtime---after this point
  // you will only regain control based on callbacks you have setup.
  // Three callbacks can be provided to handle mouse button presses,
  // mouse movement and keyboard button presses in the graphics area,
  // respectively. Those callbacks are optional, so we can pass nullptr if
  // we don't need to take any action on those events
  return application.run(argc, argv, act_on_mouse_press, act_on_mouse_move, act_on_key_press);
}

void draw_main_canvas(ezgl::renderer &g)
{
  /* The redrawing function for still pictures */

  {
    /* Draw some rectangles using the indexed colors */
    const float rectangle_width = 50;
    const float rectangle_height = rectangle_width;
    const ezgl::point2d start_point(150, 30);
    ezgl::rectangle color_rectangle = {start_point, rectangle_width, rectangle_height};

    // Some of the available colors, a complete list is in ezgl/include/colour.hpp
    ezgl::colour colour_indicies[] = {
      ezgl::GREY_55,
      ezgl::GREY_75,
      ezgl::WHITE,
      ezgl::BLACK,
      ezgl::BLUE,
      ezgl::GREEN,
      ezgl::YELLOW,
      ezgl::CYAN,
      ezgl::RED,
      ezgl::DARK_GREEN,
      ezgl::MAGENTA
    };

    // format text font and color
    g.set_colour(ezgl::BLACK);
    g.format_font("monospace", ezgl::font_slant::normal, ezgl::font_weight::normal, 10);

    // draw text
    g.draw_text({110, color_rectangle.centre_y()}, "colors", 2 * (start_point.x - 110), rectangle_height);

    for (size_t i = 0; i < sizeof (colour_indicies) / sizeof (colour_indicies[0]); ++i) {
      // Change the color of next draw calls
      g.set_colour(colour_indicies[i]);

      // Draw filled in rectangles
      g.fill_rectangle(color_rectangle);

      // Increment the start point
      color_rectangle += {rectangle_width, 0};
    }

    // Draw text
    g.draw_text({400, color_rectangle.centre_y()}, "fill_rectangle", DBL_MAX, rectangle_height);

    /* Draw some rectangles with RGB triplet colours and alpha (transparency) */

    // Hack to make the colors change once per second
    std::srand(time(0));

    for (size_t i = 0; i < 3; ++i) {
      // Increment the start point
      color_rectangle += {rectangle_width, 0};

      // Change the next draw calls colour. rgb and alpha values range from 0 to 255
      g.set_colour(std::rand() % 256, std::rand() % 256, std::rand() % 256, 255);

      // Draw filled in rectangles
      g.fill_rectangle(color_rectangle);
    }

    /* Draw a black border rectangle */

    // Change the next draw calls color to black
    g.set_colour(ezgl::BLACK);

    // Change the next draw calls line width
    g.set_line_width(1);

    // Draw a rectangle bordering all the drawn rectangles
    g.draw_rectangle(start_point, color_rectangle.top_right());

  }

  {
    /* Draw some example lines, shapes, and arcs */
    float radius = 50;

    // Draw solid line
    g.set_colour(ezgl::BLACK);
    g.draw_text({250, 150}, "draw_line", 150.0, DBL_MAX);
    g.set_line_dash(ezgl::line_dash::none);
    g.draw_line({200, 120}, {200, 200});

    // Draw dashed line
    g.set_line_dash(ezgl::line_dash::asymmetric_5_3);
    g.draw_line({300, 120}, {300, 200});

    // Draw elliptic arc
    g.set_colour(ezgl::MAGENTA);
    g.draw_text({450, 160}, "draw_elliptic_arc", 150.0, DBL_MAX);
    g.draw_elliptic_arc({550, 160}, 30, 60, 90, 270);

    // Draw filled in elliptic arc
    g.draw_text({700, 160}, "fill_elliptic_arc", 150.0, DBL_MAX);
    g.fill_elliptic_arc({800, 160}, 30, 60, 90, 270);

    // Draw arcs
    g.set_colour(ezgl::BLUE);
    g.draw_text({190, 300}, "draw_arc", radius * 2, 150);
    g.draw_arc({190, 300}, radius, 0, 270);
    g.draw_arc({300, 300}, radius, 0, -180);

    // Draw filled in arcs
    g.fill_arc({410, 300}, radius, 90, -90);
    g.fill_arc({520, 300}, radius, 0, 360);
    g.set_colour(ezgl::BLACK);
    g.draw_text({520, 300}, "fill_arc", radius * 2, 150);
    g.set_colour(ezgl::BLUE);
    g.fill_arc({630, 300}, radius, 90, 180);
    g.fill_arc({740, 300}, radius, 90, 270);
    g.fill_arc({850, 300}, radius, 90, 30);

  }

  {
    /* Draw some rotated text */
    const float textsquare_width = 200;

    ezgl::rectangle textsquare = {{100, 400}, textsquare_width, textsquare_width};

    g.set_colour(ezgl::BLUE);
    g.draw_rectangle(textsquare);

    g.set_colour(ezgl::GREEN);
    g.draw_rectangle(textsquare.centre(), {textsquare.right(), textsquare.top()});
    g.draw_rectangle({textsquare.left(), textsquare.bottom()}, textsquare.centre());

    g.set_colour(ezgl::RED);
    g.draw_line({textsquare.left(), textsquare.bottom()}, {textsquare.right(), textsquare.top()});
    g.draw_line({textsquare.left(), textsquare.top()}, {textsquare.right(), textsquare.bottom()});

    g.set_colour(0, 0, 0, 100);
    g.set_font_size(14);
    g.draw_text({textsquare.centre_x(), textsquare.bottom()}, "0 degrees", textsquare.width(), textsquare.height());

    g.set_text_rotation(90);
    g.draw_text({textsquare.right(), textsquare.centre_y()}, "90 degrees", textsquare.width(), textsquare.height());

    g.set_text_rotation(180);
    g.draw_text({textsquare.centre_x(), textsquare.top()}, "180 degrees", textsquare.width(), textsquare.height());

    g.set_text_rotation(270);
    g.draw_text({textsquare.left(), textsquare.centre_y()}, "270 degrees", textsquare.width(), textsquare.height());

    g.set_text_rotation(45);
    g.draw_text(textsquare.centre(), "45 degrees", textsquare.width(), textsquare.height());

    g.set_text_rotation(135);
    g.draw_text(textsquare.centre(), "135 degrees", textsquare.width(), textsquare.height());

    // It is probably a good idea to set text rotation back to zero,
    g.set_text_rotation(0);

  }

  {
    /* Draw some Polygons */
    g.set_font_size(10);
    g.set_colour(ezgl::RED);

    // Draw a triangle
    g.fill_poly({{500, 400}, {440, 480}, {560, 480}});

    // Draw a 4-point polygon
    g.fill_poly({{700, 400}, {650, 480}, {750, 480}, {800, 400}});

    g.set_colour(ezgl::BLACK);
    g.draw_text({500, 450}, "fill_poly", 80.0, DBL_MAX);
    g.draw_text({725, 440}, "fill_poly", 100.0, DBL_MAX);

    g.set_colour(ezgl::DARK_GREEN);
    g.set_line_dash(ezgl::line_dash::none);
    ezgl::rectangle rect = {{350, 550}, {650, 670}};
    g.draw_text(rect.centre(), "draw_rectangle", rect);
    g.draw_rectangle(rect);

  }

  {
    /* Draw some semi-transparent primitives */
    g.set_font_size(10);

    g.set_colour(255, 0, 0, 255);
    g.fill_rectangle({1000, 400}, {1050, 800});

    g.set_colour(0, 0, 255, 255);
    g.fill_rectangle({1000+50, 400}, {1050+50, 800});

    g.set_colour(0, 255, 0, 255/2);  // 50% transparent
    g.fill_rectangle({1000+25, 400-100}, {1050+25, 800-200});

    g.set_colour(255, 100, 255, 255/2);
    g.fill_poly({{465, 380}, {400, 450}, {765, 450}, {850, 380}});

    g.set_colour(100, 100, 255, 255/3);
    g.fill_poly({{550, 420}, {475, 500}, {875, 500}});

    g.set_colour(ezgl::BLACK);
    g.set_text_rotation(90);
    g.draw_text({1000 - 50, 500}, "Partially transparent polys", 500, DBL_MAX);
    g.set_text_rotation(0);

  }

  {
    /* Draw wide lines with different end shapes */
    g.set_font_size(10);

    for (int i = 0; i <= 2; ++i)
    {
      double offsetY = 50*i;

      if (i == 0) {
        g.set_colour(ezgl::BLACK);
        g.set_line_cap(ezgl::line_cap::butt); // Butt ends
        g.set_line_dash(ezgl::line_dash::none); // Solid line
        g.draw_text({1100, 920+offsetY}, "Butt ends, opaque", 400, DBL_MAX);
      }

      else if (i == 1) {
        g.set_colour(ezgl::GREEN, 255*2/3); // Green line that is 33% transparent)
        g.set_line_cap(ezgl::line_cap::round); // Round ends
        g.set_line_dash(ezgl::line_dash::none); // Solid line
        g.draw_text({1100, 920+offsetY}, "Round ends, 33% transparent", 400, DBL_MAX);
      }

      else {
        g.set_colour(ezgl::RED, 255/3);  // Red line that is 67% transparent
        g.set_line_cap(ezgl::line_cap::butt); // butt ends
        g.set_line_dash(ezgl::line_dash::asymmetric_5_3); // Dashed line
        g.draw_text({1100, 920+offsetY}, "Butt ends, 67% transparent", 400, DBL_MAX);
      }

      g.draw_text({200, 900+offsetY}, "Thin line (width 1)", 200, DBL_MAX);
      g.set_line_width(1);
      g.draw_line({100, 920+offsetY}, {300, 920+offsetY});

      g.draw_text({500, 900+offsetY}, "Width 3 Line", 200, DBL_MAX);
      g.set_line_width(3);
      g.draw_line({400, 920+offsetY}, {600, 920+offsetY});

      g.draw_text({800, 900+offsetY}, "Width 6 Line", 200, DBL_MAX);
      g.set_line_width(6);
      g.draw_line({700, 920+offsetY}, {900, 920+offsetY});

      g.set_line_width(1);

    }

  }

  {
    /* Draw some example text, with the bounding box functions */
    const float text_example_width = 800;
    const int num_lines = 2;
    const int max_strings_per_line = 3;
    const int num_strings_per_line[num_lines] = {3, 2};

    const char* const line_text[num_lines][max_strings_per_line] = {
      {
        "8 Point Text",
        "12 Point Text",
        "18 Point Text"
      },
      {
        "24 Point Text",
        "32 Point Text"
      }
    };

    const int text_sizes[num_lines][max_strings_per_line] = {
      {8, 12, 15},
      {24, 32}
    };

    g.set_colour(ezgl::BLACK);
    g.set_line_dash(ezgl::line_dash::asymmetric_5_3);

    for (int i = 0; i < num_lines; ++i) {
      ezgl::rectangle text_bbox = {{100., 710. + i * 60.}, text_example_width / num_strings_per_line[i], 60.};

      for (int j = 0; j < num_strings_per_line[i]; ++j) {
        g.set_font_size(text_sizes[i][j]);
        g.draw_text(text_bbox.centre(), line_text[i][j], text_bbox);
        g.draw_rectangle(text_bbox);
        text_bbox = {{text_bbox.left() + text_example_width / num_strings_per_line[i], text_bbox.bottom()} , text_bbox.width(), text_bbox.height()};
      }
    }

  }

}

/**
 * Function to handle mouse press event
 * The current mouse position in the main canvas' world coordinate system is returned
 * A pointer to the application and the entire GDK event are also returned
 */
void act_on_mouse_press(ezgl::application *application, GdkEventButton *event, double x, double y)
{
  std::cout << "User clicked the ";

  if (event->button == 1)
    std::cout << "left ";
  else if (event->button == 2)
    std::cout << "middle ";
  else if (event->button == 3)
    std::cout << "right ";

  std::cout << "mouse button at coordinates (" << x << "," << y << ") ";

  if ((event->state & GDK_CONTROL_MASK) && (event->state & GDK_SHIFT_MASK))
    std::cout << "with control and shift pressed ";
  else if (event->state & GDK_CONTROL_MASK)
    std::cout << "with control pressed ";
  else if (event->state & GDK_SHIFT_MASK)
    std::cout << "with shift pressed ";

  std::cout << std::endl;
}

/**
 * Function to handle mouse move event
 * The current mouse position in the main canvas' world coordinate system is returned
 * A pointer to the application and the entire GDK event are also returned
 */
void act_on_mouse_move(ezgl::application *application, GdkEventButton *event, double x, double y)
{
  std::cout << "Mouse move at coordinates (" << x << "," << y << ") "<< std::endl;
}

/**
 * Function to handle keyboard press event
 * The name of the key pressed is returned (0-9, a-z, A-Z, Up, Down, Left, Right, Shift_R, Control_L, space, Tab, ...)
 * A pointer to the application and the entire GDK event are also returned
 */
void act_on_key_press(ezgl::application *application, GdkEventKey *event, char *key_name)
{
  std::cout << key_name <<" key is pressed" << std::endl;
}
