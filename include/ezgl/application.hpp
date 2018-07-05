#ifndef EZGL_APPLICATION_HPP
#define EZGL_APPLICATION_HPP

#include <ezgl/settings.hpp>

#include <string>

#include <gtk/gtk.h>

/**
 * An easy to use library for creating a graphical user interface.
 */
namespace ezgl {

/**
 * Represents the core application.
 */
class application {
public:
  /**
   * Create an application.
   *
   * @param s The settings to use for this application.
   */
  explicit application(settings s);

  /**
   * Destructor.
   */
  ~application();

  /**
   * Copies are disabled - there should be only one application object.
   */
  application(application const &) = delete;

  /**
   * Copies are disabled - there should be only one application object.
   */
  application &operator=(application const &) = delete;

  /**
   * Ownership of an application is transferrable.
   */
  application(application &&) = default;

  /**
   * Ownership of an application is transferrable.
   */
  application &operator=(application &&) = default;

  /**
   * Run the application.
   *
   * @param argc The number of arguments.
   * @param argv An array of the arguments.
   *
   * @return The exit status.
   */
  int run(int argc, char **argv);

private:
  static void startup(GtkApplication *gtk_app, gpointer user_data);

  // called when GTK activates our application for the first time
  static void activate(GtkApplication *gtk_app, gpointer user_data);

  // called when m_canvas needs to be redrawn
  static gboolean draw_canvas(GtkWidget *widget, cairo_t *cairo, gpointer data);

  // called when a key was pressed
  static gboolean press_key(GtkWidget *widget, GdkEventKey *event, gpointer data);

  static gboolean move_mouse(GtkWidget *widget, GdkEventMotion *event, gpointer data);

  static gboolean click_mouse(GtkWidget *widget, GdkEventButton *event, gpointer data);

  // the configured settings
  settings m_settings;

  // the GTK application
  GtkApplication *m_application;

  // each ezgl application contains only one window
  GtkWidget *m_window;

  // each ezgl application has one drawable canvas
  GtkWidget *m_canvas;
};
}

#endif //EZGL_APPLICATION_HPP
